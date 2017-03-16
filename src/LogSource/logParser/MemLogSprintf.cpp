///////////////////////////////////////////////////////////////////
//
// Copyright (c) Microsoft Corporation.
//
// SYNOPSIS
//
// sprintf style functionality for high speed binary logging.
//
// Breaks up sprintf into 2 steps:
//
//   1. preprocess the output string and determine what parameters
//      are on the stack, and their format modifiers, results are
//      stored in a preprocess_block.
//
//   2. do the output of the sprintf using the preprocess_block.
//
//  This technique allows the preprocessing of static format strings
//  to be done only once.  It also facilitates the copying of the
//  variable parameters into a block of memory for transfer to a
//  separate thread for formatting and output, so the caller need
//  not be blocked waiting for the formatting to complete.
//
// Supports additional output types for:
//
//    "%ta"  sockaddr, currently only IP addresses supported.
//    "%tg" "%tG", GUIDS without and with braces.
//    "%tT" "%tt", time stamps and time intervals
//
// sprintf code logic splurped from longhorn CRT
//
// see:
//     crts\crtw32\sdtio\output.c,maketab.c
//
//
///////////////////////////////////////////////////////////////////
#include <windows.h>
#include <stdio.h>
#include "types.h"
#include "logging.h"
//#include "InternalString.h"

#include "MemLogEntry.h"
#include "MemoryLog.h"
#include "MemLogSprintf.h"
#include "ActivityFactory.h"
#include "binarylogger.h"
#include "BinaryLogReader.h"


#pragma warning(disable:4995)

#ifdef _T
#undef _T
#endif // _T

#define _T(X) (X)

//returns number of bytes written
static const Size_t UInt32NetAddr(
    char *pDest,
    const UInt32 *pNetAddr
)
{
    static char *decimals[256] = { "0",  "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8",  "9",
                                 "10", "11", "12", "13", "14", "15", "16", "17", "18", "19",
                                 "20", "21", "22", "23", "24", "25", "26", "27", "28", "29",
                                 "30", "31", "32", "33", "34", "35", "36", "37", "38", "39",
                                 "40", "41", "42", "43", "44", "45", "46", "47", "48", "49",
                                 "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
                                 "60", "61", "62", "63", "64", "65", "66", "67", "68", "69",
                                 "70", "71", "72", "73", "74", "75", "76", "77", "78", "79",
                                 "80", "81", "82", "83", "84", "85", "86", "87", "88", "89",
                                 "90", "91", "92", "93", "94", "95", "96", "97", "98", "99",
                                "100","101","102","103","104","105","106","107","108","109",
                                "110","111","112","113","114","115","116","117","118","119",
                                "120","121","122","123","124","125","126","127","128","129",
                                "130","131","132","133","134","135","136","137","138","139",
                                "140","141","142","143","144","145","146","147","148","149",
                                "150","151","152","153","154","155","156","157","158","159",
                                "160","161","162","163","164","165","166","167","168","169",
                                "170","171","172","173","174","175","176","177","178","179",
                                "180","181","182","183","184","185","186","187","188","189",
                                "190","191","192","193","194","195","196","197","198","199",
                                "200","201","202","203","204","205","206","207","208","209",
                                "210","211","212","213","214","215","216","217","218","219",
                                "220","221","222","223","224","225","226","227","228","229",
                                "230","231","232","233","234","235","236","237","238","239",
                                "240","241","242","243","244","245","246","247","248","249",
                                "250","251","252","253","254","255" };


    UInt32 netaddr = *pNetAddr;
    char *pDst = pDest;

    UInt8 byte0 = (UInt8)(netaddr & 0xff);
    UInt8 byte1 = (UInt8)((netaddr & 0xff00) >> 8);

    char *str0 = decimals[byte0];
    char *str1 = decimals[byte1];

    while (*str0 != _T('\0'))
    {
        *pDst++ = *str0++;
    }

    UInt8 byte2 = (UInt8)((netaddr & 0xff0000) >> 16);
    char *str2 = decimals[byte2];

    *pDst++ = _T('.');


    while (*str1 != _T('\0'))
    {
        *pDst++ = *str1++;
    }

    UInt8 byte3 = (UInt8)((netaddr & 0xff000000) >> 24);
    char *str3 = decimals[byte3];

    *pDst++ = _T('.');

    while (*str2 != _T('\0'))
    {
        *pDst++ = *str2++;
    }

    *pDst++ = _T('.');

    while (*str3 != _T('\0'))
    {
        *pDst++ = *str3++;
    }

    *pDst = _T('\0');

    return pDst - pDest;
}


static
const UInt8 *
GuidWriteWord(
    char        *pDst,
    const UInt8 *pSrc,
    unsigned        uBytes
)
{
    static const char Hex[513] =
        "000102030405060708090A0B0C0D0E0F"
        "101112131415161718191A1B1C1D1E1F"
        "202122232425262728292A2B2C2D2E2F"
        "303132333435363738393A3B3C3D3E3F"
        "404142434445464748494A4B4C4D4E4F"
        "505152535455565758595A5B5C5D5E5F"
        "606162636465666768696A6B6C6D6E6F"
        "707172737475767778797A7B7C7D7E7F"
        "808182838485868788898A8B8C8D8E8F"
        "909192939495969798999A9B9C9D9E9F"
        "A0A1A2A3A4A5A6A7A8A9AAABACADAEAF"
        "B0B1B2B3B4B5B6B7B8B9BABBBCBDBEBF"
        "C0C1C2C3C4C5C6C7C8C9CACBCCCDCECF"
        "D0D1D2D3D4D5D6D7D8D9DADBDCDDDEDF"
        "E0E1E2E3E4E5E6E7E8E9EAEBECEDEEEF"
        "F0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF";

    unsigned  c;
    unsigned  c2;
    char  *pDst2;

    pDst += uBytes * 2;
    *pDst = '-';

    while (uBytes >= 2)
    {
        uBytes -= 2;
        c2 = *pSrc++;
        pDst2 = pDst - 2;
        c = *pSrc++;
        pDst = pDst2 - 2;
        *((wchar_t*)(&pDst2[0])) = *(wchar_t*)(&Hex[2 * c2]);
        *((wchar_t*)(&pDst[0])) = *(wchar_t*)(&Hex[2 * c]);
    }

    while (uBytes--)
    {
        c = *pSrc++;
        pDst -= 2;
        *((wchar_t*)(&pDst[0])) = *(wchar_t*)(&Hex[2 * c]);
    }

    return (pSrc);
}

// writes GUID to the szOut and optionally appends '\0' if there is extra space.
void
GuidWrite2(
    char *szOut,
    size_t cbOut,
    const GUID *pGuid,
    bool fBraces = true
)
{
    // make sure we have enough space to write GUID
    //LogAssert(cbOut >= (size_t)(fBraces ? 38 : 36));

    const UInt8 *p = (const UInt8 *)pGuid;
    char *pDst = szOut;

    if (fBraces) {
        *pDst++ = '{';
    }

    p = GuidWriteWord(pDst, p, 4);
    pDst += 9;

    p = GuidWriteWord(pDst, p, 2);
    pDst += 5;

    p = GuidWriteWord(pDst, p, 2);
    pDst += 5;

    GuidWriteWord(pDst, p, 1);
    GuidWriteWord(pDst + 2, p + 1, 1);
    pDst += 5;
    p += 2;

    GuidWriteWord(pDst + 0 * 2, p + 0, 1);
    GuidWriteWord(pDst + 1 * 2, p + 1, 1);
    GuidWriteWord(pDst + 2 * 2, p + 2, 1);
    GuidWriteWord(pDst + 3 * 2, p + 3, 1);
    GuidWriteWord(pDst + 4 * 2, p + 4, 1);
    GuidWriteWord(pDst + 5 * 2, p + 5, 1);

    pDst += 6 * 2;
    if (fBraces) {
        *pDst++ = '}';
    }

    // check if we have space for extra '\0'
    if ((size_t)(pDst - szOut) < cbOut)
    {
        *pDst = '\0';
    }
}

// Stuff from Cosmos we don't need when we integrate

typedef HRESULT CsError;
#define CsError_OK S_OK
#define CsError_Fail E_FAIL
#define CsError_StringTooLong 0x80000007

static const size_t k_CsTimeIntervalStringBufferSize = 64; // this size guarrantees successful completion of CsTimeIntervalToString

// Cached local time zone bias. These values may optionally be set  before multithreading begins, to speed up time-to-string conversions
// bool g_fFixedLocalTimeZoneBias = false;
// CsTimeInterval g_tiFixedLocalTimeZoneBias = CsTimeInterval_Zero;
// char g_szFixedLocalTimeZoneBiasSuffix[k_CsTimeIntervalStringBufferSize+2] = "";
// size_t g_nbFixedLocalTimeZoneBiasSuffix = 0;

#if defined (ELEMENTCOUNT)
#undef ELEMENTCOUNT
#endif
#define ELEMENTCOUNT(x) sizeof(x)/sizeof(x[0])

// converts a cosmos timeinterval to a human-readable string.
// The generated string may be fed back into CsStringToTimeInterval
static CsError CsTimeIntervalToString(CsTimeInterval timeInterval, char *pBuffer, size_t buffLen)
{
    char tempBuff[k_CsTimeIntervalStringBufferSize];
    char *pBuff = pBuffer;
    if (buffLen < k_CsTimeIntervalStringBufferSize) {
        // use a temporary buffer if we aren't sure it will fit
        pBuff = tempBuff;
    }
    CsError err = CsError_OK;

    if (timeInterval == CsTimeInterval_Infinite) {
        strcpy(pBuff, "infinite");
    }
    else if (timeInterval == CsTimeInterval_NegativeInfinite) {
        strcpy(pBuff, "negativeinfinite");
    }
    else {
        bool neg = (timeInterval < CsTimeInterval_Zero);
        UInt64 v;
        if (neg) {
            v = (UInt64)(-timeInterval);
        }
        else {
            v = (UInt64)timeInterval;
        }
        UInt32 frac100ns = (UInt32)(v % CsTimeInterval_Second);
        v = v / CsTimeInterval_Second;
        UInt32 sec = (UInt32)(v % (UInt32)60);
        v = v / 60;
        UInt32 min = (UInt32)(v % (UInt32)60);
        v = v / 60;
        UInt32 hr = (UInt32)(v % (UInt32)24);
        v = v / 24;
        // v now contains days


        int i = 0;
        if (neg) {
            pBuff[i++] = '-';
        }
        int ret = 0;
        bool fOutput = false;
        if (v != 0) {
            ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%I64ud", v);
            i += ret;
            fOutput = true;
        }
        if (hr != 0 || (fOutput && (min != 0 || sec != 0 || frac100ns != 0))) {
            fOutput = true;
            ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%uh", hr);
            i += ret;
        }
        if (min != 0 || (fOutput && (sec != 0 || frac100ns != 0))) {
            fOutput = true;
            ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%um", min);
            i += ret;
        }
        if (frac100ns == 0) {
            // whole number of seconds
            if (sec != 0 || !fOutput) {
                fOutput = true;
                ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%us", sec);
                i += ret;
            }
        }
        else if (hr != 0 || min != 0 || sec != 0)
        {
            // fractional seconds
            fOutput = true;
            ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%u.%07u", sec, frac100ns);
            i += ret;

            // remove traling "0" characters
            while (i > 0 && pBuff[i - 1] == '0') {
                --i;
            }

            pBuff[i++] = 's';
            pBuff[i] = '\0';
        }
        else
        {
            // log as ms
            fOutput = true;
            UInt32 ms = frac100ns / CsTimeInterval_Millisecond;
            frac100ns = frac100ns % CsTimeInterval_Millisecond;

            if (frac100ns == 0)
            {
                ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%ums", ms);
                i += ret;
            }
            else
            {
                ret = _snprintf(pBuff + i, k_CsTimeIntervalStringBufferSize - i - 1, "%u.%04u", ms, frac100ns);
                i += ret;

                // remove traling "0" characters
                while (i > 0 && pBuff[i - 1] == '0')
                {
                    --i;
                }

                pBuff[i++] = 'm';
                pBuff[i++] = 's';
                pBuff[i] = '\0';
            }
        }
    }

    if (err == CsError_OK && pBuff == tempBuff) {
        size_t n = strlen(tempBuff) + 1;
        if (n <= buffLen) {
            memcpy(pBuffer, tempBuff, n);
        }
        else {
            err = CsError_StringTooLong;
            pBuffer[0] = '\0';
        }
    }

    return err;
}

static CsError CsGenerateTimeZoneBiasSuffix(CsTimeInterval bias, char *szBuff, size_t nbBuff)
{

    if (bias == CsTimeInterval_Zero) {
        if (nbBuff < 2) {
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }

        szBuff[0] = 'Z';
        szBuff[1] = '\0';
        return CsError_OK;
    }
    else {
        if (nbBuff < 4) {
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }
        *(szBuff++) = 'L';
        nbBuff--;
        char s = '+';
        if (bias < CsTimeInterval_Zero) {
            s = '-';
            bias = -bias;
        }
        *(szBuff++) = s;
        nbBuff--;
        *szBuff = '\0';
        return CsTimeIntervalToString(bias, szBuff, nbBuff);
    }
}

static CsError CsTimeStampToSystemTime(UINT64 timeStamp, SYSTEMTIME* pSystemTime, BOOL fToLocalTimeZone)
{
    UINT64      ts;

    if (fToLocalTimeZone)
    {
        if (!FileTimeToLocalFileTime((const FILETIME *)&timeStamp, (FILETIME*)&ts))
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }
    }
    else
    {
        ts = timeStamp;
    }

    if (!FileTimeToSystemTime((const FILETIME *)&ts, pSystemTime))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return CsError_OK;
}

static CsError CsTimeStampToString(CsTimeStamp timeStamp, char *pBuffer, int buffLen, CsTimeInterval bias = CsTimeInterval_Zero, Int32 nFracDig = -1);

static CsError CsTimeStampToString(CsTimeStamp timeStamp, char *pBuffer, int buffLen, CsTimeInterval bias, Int32 nFracDig)
{
    SYSTEMTIME st;

    if (timeStamp == CsTimeStamp_Never) {
        if (buffLen < 6) {
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }
        memcpy(pBuffer, "never", 6);
        return CsError_OK;
    }
    else if (timeStamp == CsTimeStamp_LongAgo) {
        if (buffLen < 9) {
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }
        memcpy(pBuffer, "longago", 9);
        return CsError_OK;
    }

    if (buffLen < 20) {
        // Minimum possible timestamp is e.g. "2008-07-04T12:47:00Z"
        return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
    }

    CsTimeStamp oldTimeStamp = timeStamp;
    timeStamp -= bias;
    if ((bias > 0 && timeStamp > oldTimeStamp) || (bias < 0 && timeStamp < oldTimeStamp)) {
        // Bias caused an overflow of the timestamp.
        return HRESULT_FROM_WIN32(ERROR_INVALID_TIME);
    }

    CsError err = CsTimeStampToSystemTime(timeStamp, &st, false);
    if (err != CsError_OK) {
        return err;
    }

    if (nFracDig < 0) {
        if (st.wMilliseconds == 0) {
            nFracDig = 0;
        }
        else {
            nFracDig = 3;
        }
    }

    char szFrac[16];
    if (nFracDig == 0) {
        szFrac[0] = '\0';
    }
    else {
        szFrac[0] = '.';
        //LogAssert(st.wMilliseconds < 1000);
        sprintf(szFrac + 1, "%03u", st.wMilliseconds);
        if (nFracDig > 3) {
            if (nFracDig > 7) {
                // CsTimeStamp has no meaningful precision beyond 10^(-7) seconds
                nFracDig = 7;
            }
            // Compute quantum ticks in excess of 1ms boundary
            UInt32 remainder = (UInt32)(timeStamp % 10000);
            sprintf(szFrac + 4, "%04u", remainder);
        }
        szFrac[nFracDig + 1] = '\0';
    }


    if (bias == CsTimeInterval_Zero) {
        // No local time bias -- Zulu time
        int nb = _snprintf(pBuffer, (size_t)buffLen, "%04u-%02u-%02uT%02u:%02u:%02u%sZ",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            szFrac);
        if (nb <= 0) {
            pBuffer[0] = '\0';
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }
    }
    else {
        // Local time zone bias
        char szSuffix[k_CsTimeIntervalStringBufferSize + 2];
        const char *suffix;

        err = CsGenerateTimeZoneBiasSuffix(bias, szSuffix, ELEMENTCOUNT(szSuffix));
        //LogAssert(err == CsError_OK);
        suffix = szSuffix;

        int nb = _snprintf(pBuffer, (size_t)buffLen, "%04u-%02u-%02uT%02u:%02u:%02u%s%s",
            st.wYear,
            st.wMonth,
            st.wDay,
            st.wHour,
            st.wMinute,
            st.wSecond,
            szFrac,
            suffix);
        if (nb <= 0) {
            pBuffer[0] = '\0';
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }
    }

    return CsError_OK;
}


// The state table.  This table is actually two tables combined into one.
// The lower nybble of each byte gives the character class of any
// character; while the uper nybble of the byte gives the next state
// to enter.  See the macros below the table for details.
//
// The table is generated by maketabc.c -- use this program to make
// changes.

static const unsigned char __lookuptable_s[] = {
    /* ' ' */  0x06,
    /* '!' */  0x00,
    /* '"' */  0x00,
    /* '#' */  0x06,
    /* '$' */  0x00,
    /* '%' */  0x01,
    /* '&' */  0x00,
    /* ''' */  0x00,
    /* '(' */  0x10,
    /* ')' */  0x00,
    /* '*' */  0x03,
    /* '+' */  0x06,
    /* ',' */  0x00,
    /* '-' */  0x06,
    /* '.' */  0x02,
    /* '/' */  0x10,
    /* '0' */  0x04,
    /* '1' */  0x45,
    /* '2' */  0x45,
    /* '3' */  0x45,
    /* '4' */  0x05,
    /* '5' */  0x05,
    /* '6' */  0x05,
    /* '7' */  0x05,
    /* '8' */  0x05,
    /* '9' */  0x35,
    /* ':' */  0x30,
    /* ';' */  0x00,
    /* '<' */  0x50,
    /* '=' */  0x00,
    /* '>' */  0x00,
    /* '?' */  0x00,
    /* '@' */  0x00,
   #if defined(_NTSUBSET_) || defined(_SAFECRT_IMPL)
    /* 'A' */  0x20,       // Disable %A format
   #else
    /* 'A' */  0x28,
   #endif
    /* 'B' */  0x20,
    /* 'C' */  0x38,
    /* 'D' */  0x50,
   #if defined(_NTSUBSET_)
    /* 'E' */  0x50,       // Disable %E format
   #else
    /* 'E' */  0x58,
   #endif
    /* 'F' */  0x07,
   #if defined(_NTSUBSET_)
    /* 'G' */  0x00,       // Disable %G format
   #else
    /* 'G' */  0x08,
   #endif
    /* 'H' */  0x00,
    /* 'I' */  0x37,
    /* 'J' */  0x30,
    /* 'K' */  0x30,
    /* 'L' */  0x57,
    /* 'M' */  0x50,
    /* 'N' */  0x07,
    /* 'O' */  0x00,
    /* 'P' */  0x00,
    /* 'Q' */  0x20,
    /* 'R' */  0x20,
    /* 'S' */  0x08,
    /* 'T' */  0x00,
    /* 'U' */  0x00,
    /* 'V' */  0x00,
    /* 'W' */  0x00,
    /* 'X' */  0x08,
    /* 'Y' */  0x60,
    /* 'Z' */  0x68,
    /* '[' */  0x60,
    /* '\' */  0x60,
    /* ']' */  0x60,
    /* '^' */  0x60,
    /* '_' */  0x00,
    /* '`' */  0x00,
   #if defined(_NTSUBSET_) || defined(_SAFECRT_IMPL)
    /* 'a' */  0x70,       // Disable %a format
   #else
    /* 'a' */  0x78,
   #endif
    /* 'b' */  0x70,
    /* 'c' */  0x78,
    /* 'd' */  0x78,
   #if defined(_NTSUBSET_)
    /* 'e' */  0x70,       // Disable %e format
   #else
    /* 'e' */  0x78,
   #endif
   #if defined(_NTSUBSET_)
    /* 'f' */  0x70,       // Disable %f format
   #else
    /* 'f' */  0x78,
   #endif
   #if defined(_NTSUBSET_)
    /* 'g' */  0x00,       // Disable %g format
   #else
    /* 'g' */  0x08,
   #endif
    /* 'h' */  0x07,
    /* 'i' */  0x08,
    /* 'j' */  0x00,
    /* 'k' */  0x00,
    /* 'l' */  0x07,
    /* 'm' */  0x00,
   #if defined(_NTSUBSET_) || defined(_SAFECRT_IMPL)
    /* 'n' */  0x00,       // Disable %n format
   #else
    /* 'n' */  0x08,
   #endif
    /* 'o' */  0x08,
    /* 'p' */  0x08,
    /* 'q' */  0x00,
    /* 'r' */  0x00,
    /* 's' */  0x08,
    /* 't' */  0x08,
    /* 'u' */  0x08,
    /* 'v' */  0x00,
    /* 'w' */  0x07,
    /* 'x' */  0x08
};

#define FIND_CHAR_CLASS(lookuptbl, c)      \
        ((c) < _T(' ') || (c) > _T('x') ? \
            CH_OTHER            \
            :               \
        (enum CHARTYPE)(lookuptbl[(c)-_T(' ')] & 0xF))

#define FIND_NEXT_STATE(lookuptbl, class, state)   \
        (enum STATE)(lookuptbl[(class) * NUMSTATES + (state)] >> 4)

#define __CRTDECL_STDCALL __stdcall

// flag definitions
#define FL_SIGN       0x00001   /* put plus or minus in front */
#define FL_SIGNSP     0x00002   /* put space or minus in front */
#define FL_LEFT       0x00004   /* left justify */
#define FL_LEADZERO   0x00008   /* pad with leading zeros */
#define FL_LONG       0x00010   /* long value given */
#define FL_SHORT      0x00020   /* short value given */
#define FL_SIGNED     0x00040   /* signed data given */
#define FL_ALTERNATE  0x00080   /* alternate form requested */
#define FL_NEGATIVE   0x00100   /* value is negative */
#define FL_FORCEOCTAL 0x00200   /* force leading '0' for octals */
#define FL_LONGDOUBLE 0x00400   /* long double value given */
#define FL_WIDECHAR   0x00800   /* wide characters */
#define FL_LONGLONG   0x01000   /* long long value given */
#define FL_I64        0x08000   /* __int64 value given */

// state definitions

enum STATE {
    ST_NORMAL,          // normal state; outputting literal chars
    ST_PERCENT,         // just read '%'
    ST_FLAG,            // just read flag character
    ST_WIDTH,           // just read width specifier
    ST_DOT,             // just read '.'
    ST_PRECIS,          // just read precision specifier
    ST_SIZE,            // just read size specifier
    ST_TYPE             // just read type specifier
};

#define NUMSTATES (ST_TYPE + 1)

// character type values

enum CHARTYPE {
    CH_OTHER,           // character with no special meaning
    CH_PERCENT,         // '%'
    CH_DOT,             // '.'
    CH_STAR,            // '*'
    CH_ZERO,            // '0'
    CH_DIGIT,           // '1'..'9'
    CH_FLAG,            // ' ', '+', '-', '#'
    CH_SIZE,            // 'h', 'l', 'L', 'N', 'F', 'w'
    CH_TYPE             // type specifying character
};

__declspec(selectany) UNICODE_STRING_NT __counted_unicode_nullstring = { sizeof(L"(null)") - sizeof(UNICODE_NULL),
                                                                         sizeof(L"(null)"),
                                                                         __wnullstring };

__declspec(selectany) ANSI_STRING_NT __counted_ansi_nullstring = { sizeof("(null)") - sizeof(ANSI_NULL),
                                                                   sizeof("(null)"),
                                                                   __nullstring };

//
//// allocates a correctly sized preprocess block from the heap, and fills it in from the source preprocess block
//format_preprocess_block *alloc_preprocess_block(format_preprocess_block *src)
//{
//    UInt16 nDescInUse = src->m_nDescInUse;
//
//    UInt32 size = offsetof(format_preprocess_block,m_param_descs)+sizeof(format_param_desc)*nDescInUse;
//    format_preprocess_block *dest = (format_preprocess_block *) new char[size];
//
//    memcpy(dest,src,size);
//
//    dest->m_nDesc = nDescInUse;
//
//    return dest;
//}
//
////
//// preprocess_format
////
//// Description:
////
////  preprocesses the printf sytle format string, and determines the type and position
////  of each format specifier within the string.
////
////  The logic was largely lifted from the CRT.
////
////
//// Arguments:
////
////  char * format - printf style format string.
////  format_preprocess_block - structure to receive information about how to process the
////                            variable args for the format string.
////
//// Return Value:
////
////  bool - success - true, failure - false
////
////  m_nDescInUse - number of parameters filled is filled in the format_preprocess_block,
////                 if there was not enough space m_nDescInUse will be greater than m_nDesc.
////
//format_preprocess_block * __CRTDECL preprocess_format(
//    const char *format,
//    format_preprocess_block *pFormatPreprocessBlock
//    )
//{
//    format_preprocess_block PreprocessBlock;
//    format_preprocess_block *pPreprocessBlock;
//
//    if(pFormatPreprocessBlock == NULL)
//    {
//        pPreprocessBlock = &PreprocessBlock;
//    }
//    else
//    {
//        pPreprocessBlock = pFormatPreprocessBlock;
//    }
//
//
//    // remember the starting value of format;
//    const char        *startingFormat = format;
//
//    char               ch;        // character just read
//    int                nDesc=0;   // number of descritpors in the provided preprocess_block
//
//    enum STATE         state;     // current state
//    enum CHARTYPE      chclass;   // class of current character
//
//    format_param_desc *pCurDesc = NULL;
//
//    pPreprocessBlock->m_format = format;
//
//    nDesc = pPreprocessBlock->m_nDesc;
//    pPreprocessBlock->m_nDescInUse=0;
//
//    state = ST_NORMAL;
//
//    // main loop - loop while formatting
//    while((ch = *format++) != _T('\0'))
//    {
//        chclass = FIND_CHAR_CLASS(__lookuptable_s, ch);             // find character class
//        state   = FIND_NEXT_STATE(__lookuptable_s, chclass, state); // find next state
//
//        switch(state)
//        {
//            case ST_NORMAL:
//            {
//
//            NORMAL_STATE:       // target
//
//                // normal state -- just skip over character
//
//                break;
//            }
//
//            case ST_PERCENT:
//            {
//                // found a format specifier
//
//                pCurDesc = &pPreprocessBlock->m_param_descs[pPreprocessBlock->m_nDescInUse];
//
//                pPreprocessBlock->m_nDescInUse++;
//
//                if(pPreprocessBlock->m_nDescInUse > pPreprocessBlock->m_nDesc)
//                {
//                    Log(LogID_Logging,
//                        LogLevel_Error,
//                        "Format String has too many type specifiers",
//                        "Over %d specifiers in format string %s",
//                        PreprocessBlock.m_nDesc, format);
//
//                    goto error_exit;
//                }
//
//                pCurDesc->m_offset    = (UInt16)(format-startingFormat-1);
//                pCurDesc->m_prefixlen = 0;
//                pCurDesc->m_fldwidth  = 0;
//                pCurDesc->m_flags     = 0;
//                pCurDesc->m_precision = -1;
//                pCurDesc->m_capex     = 0;
//                pCurDesc->m_hexadd    = 0;
//                pCurDesc->m_stackType = SE_PERCENT;
//                pCurDesc->m_len       = 1;
//                break;
//            }
//
//            case ST_FLAG:
//            {
//                // set flag based on which flag character
//                switch (ch)
//                {
//                    case _T('-'):
//                        pCurDesc->m_flags |= FL_LEFT;   /* '-' => left justify */
//                        break;
//                    case _T('+'):
//                        pCurDesc->m_flags |= FL_SIGN;   /* '+' => force sign indicator */
//                        break;
//                    case _T(' '):
//                        pCurDesc->m_flags |= FL_SIGNSP; /* ' ' => force sign or space */
//                        break;
//                    case _T('#'):
//                        pCurDesc->m_flags |= FL_ALTERNATE;  /* '#' => alternate form */
//                        break;
//                    case _T('0'):
//                        pCurDesc->m_flags |= FL_LEADZERO;   /* '0' => pad with leading zeros */
//                        break;
//                }
//                break;
//            }
//
//            case ST_WIDTH:
//            {
//                // update width value
//                if (ch == _T('*')) {
//                    // width on stack not supported.
//                    return false;
//                }
//                else
//                {
//                    if(pCurDesc->m_fldwidth <= 24)
//                    {
//                        // add digit to current field width
//                        pCurDesc->m_fldwidth = pCurDesc->m_fldwidth * 10 + (ch - _T('0'));
//                    }
//                    else
//                    {
//                        // we only have 8 bits for field width, cap it.
//                        pCurDesc->m_fldwidth=255;
//                    }
//                }
//                break;
//            }
//
//            case ST_DOT:
//            {
//                // zero the precision, since dot with no number means 0
//                // not default, according to ANSI
//                pCurDesc->m_precision = 0;
//                break;
//            }
//
//            case ST_PRECIS:
//            {
//               if (ch == _T('*'))
//               {
//                    // precision on stack not supported
//                    return false;
//                }
//                else
//                {
//                    if(pCurDesc->m_precision <= 24)
//                    {
//                        // add digit to current precision
//                        pCurDesc->m_precision = pCurDesc->m_precision * 10 + (ch - _T('0'));
//                    }
//                    else
//                    {
//                        // we only have 7 bits for precision, cap it.
//                        pCurDesc->m_precision=127;
//                    }
//                }
//                break;
//            }
//
//            case ST_SIZE:
//            {
//                /* just read a size specifier, set the flags based on it */
//                switch (ch)
//                {
//                    case _T('l'):
//                    {
//                        /*
//                         * In order to handle the ll case, we depart from the
//                         * simple deterministic state machine.
//                         */
//                        if (*format == _T('l'))
//                        {
//                            ++format;
//                            pCurDesc->m_flags |= FL_LONGLONG;   /* 'll' => long long */
//                        }
//                        else
//                        {
//                            pCurDesc->m_flags |= FL_LONG;   /* 'l' => long int or wchar_t */
//                        }
//                        break;
//                    }
//
//                    case _T('I'):
//                    {
//                        /*
//                         * In order to handle the I, I32, and I64 size modifiers, we
//                         * depart from the simple deterministic state machine. The
//                         * code below scans for characters following the 'I',
//                         * and defaults to 64 bit on WIN64 and 32 bit on WIN32
//                         */
//#if PTR_IS_INT64
//                        pCurDesc->m_flags |= FL_I64;    /* 'I' => __int64 on WIN64 systems */
//#endif
//                        if ( (*format == _T('6')) && (*(format + 1) == _T('4')) )
//                        {
//                            format += 2;
//                            pCurDesc->m_flags |= FL_I64;    /* I64 => __int64 */
//                        }
//                        else if ( (*format == _T('3')) && (*(format + 1) == _T('2')) )
//                        {
//                            format += 2;
//                            pCurDesc->m_flags &= ~FL_I64;   /* I32 => __int32 */
//                        }
//                        else if ( (*format == _T('d')) ||
//                                  (*format == _T('i')) ||
//                                  (*format == _T('o')) ||
//                                  (*format == _T('u')) ||
//                                  (*format == _T('x')) ||
//                                  (*format == _T('X')) )
//                        {
//                           /*
//                            * Nothing further needed.  %Id (et al) is
//                            * handled just like %d, except that it defaults to 64 bits
//                            * on WIN64.  Fall through to the next iteration.
//                            */
//                        }
//                        else
//                        {
//                            state = ST_NORMAL;
//                            goto NORMAL_STATE;
//                        }
//                        break;
//                    }
//
//                    case _T('h'):
//                    {
//                        pCurDesc->m_flags |= FL_SHORT;  /* 'h' => short int or char */
//                        break;
//                    }
//
//                    case _T('w'):
//                    {
//                        pCurDesc->m_flags |= FL_WIDECHAR;  /* 'w' => wide character */
//                        break;
//                    }
//
//
//                }
//                break;
//
//                case ST_TYPE:
//                {
//                    // we have finally read the actual type character
//                    pCurDesc->m_len = (UInt8)(format - (startingFormat+pCurDesc->m_offset));
//
//                    switch(ch)
//                    {
//                        case _T('C'): // ISO wide character
//                        {
//                            pCurDesc->m_stackType = SE_WCHAR;
//                            break;
//                        }
//
//                        case _T('c'): // single character specified by an int argument
//                        {
//                            pCurDesc->m_stackType = SE_CHAR;
//                            break;
//                        }
//
//                        case _T('Z'):
//                        {
//                            // unsupport NT extension for counted string
//                            Log(LogID_Logging,
//                                LogLevel_Error,
//                                "Format String has unsupported type specifier %Z",
//                                "format string %s has invalid type specifier",
//                                format);
//
//                            goto error_exit;
//                            break;
//                        }
//
//                        case _T('S'): // ISO wide character string
//                        {
//                            pCurDesc->m_stackType = SE_PWSTR;
//                            break;
//                        }
//
//                        case _T('s'):
//                        {
//                            /* print a string --                            */
//                            /* ANSI rules on how much of string to print:   */
//                            /*   all if precision is default,               */
//                            /*   min(precision, length) if precision given. */
//                            /* prints '(null)' if a null string is passed   */
//                            if(!(pCurDesc->m_flags & (FL_LONG|FL_WIDECHAR)))
//                            {
//                                pCurDesc->m_stackType = SE_PSTR;
//                            }
//                            else
//                            {
//                                pCurDesc->m_stackType = SE_PWSTR;
//                            }
//                            break;
//                        }
//
//                        case _T('n'): // write count of characters so far into specified target
//                        {
//                            // not supported requires full processing...
//                            return false;
//                            break;
//                        }
//
//                        //
//                        // floating point.
//                        //
//
//                        case _T('E'):
//                        case _T('G'):
//                        case _T('A'):
//                        {
//                            pCurDesc->m_capex = 1;      // capitalize exponent */
//                            // fall through...
//                        }
//                        case _T('e'):
//                        case _T('f'):
//                        case _T('g'):
//                        case _T('a'):
//                        {
//                            pCurDesc->m_stackType = SE_DOUBLE;
//
//                            // floating point conversion -- we call cfltcvt routines
//                            // to do the work for us.
//                            pCurDesc->m_flags |= FL_SIGNED;         // floating point is signed conversion
//
//                            // compute the precision value
//
//                            if (pCurDesc->m_precision < 0)
//                            {
//                                pCurDesc->m_precision = 6;          // default precision: 6
//                            }
//                            else if (pCurDesc->m_precision == 0 && ch == _T('g'))
//                            {
//                                pCurDesc->m_precision = 1;          /* ANSI specified */
//                            }
//                            break;
//                        }
//
//                        //
//                        // Integers
//                        //
//                        case _T('d'):
//                        case _T('i'):
//                        {
//                            pCurDesc->m_flags |= FL_SIGNED;
//                            pCurDesc->m_radix = 10;
//
//                            if(pCurDesc->m_flags & (FL_I64|FL_LONGLONG))
//                            {
//                                pCurDesc->m_stackType = SE_INT64;
//                            }
//                            else if (pCurDesc->m_flags & FL_SHORT)
//                            {
//                                pCurDesc->m_stackType = SE_INT16;
//                            }
//                            else
//                            {
//                                pCurDesc->m_stackType = SE_INT32;
//                            }
//                            break;
//                        }
//
//                       case _T('u'):
//                       {
//                            pCurDesc->m_radix = 10;
//                            if(pCurDesc->m_flags & (FL_I64|FL_LONGLONG))
//                            {
//                                pCurDesc->m_stackType = SE_INT64;
//                            }
//                            else if (pCurDesc->m_flags & FL_SHORT)
//                            {
//                                pCurDesc->m_stackType = SE_INT16;
//                            }
//                            else
//                            {
//                                pCurDesc->m_stackType = SE_INT32;
//                            }
//                            break;
//                       }
//                       case _T('p'):
//                       {
//                           // write a pointer -- this is like an integer or long
//                           // except we force precision to pad with zeros and
//                           // output in big hex.
//                           pCurDesc->m_precision= 2 * sizeof(void *); // number of hex digits we need
//#if     PTR_IS_INT64
//                           pCurDesc->m_flags |= FL_I64;                    /* assume we're converting an int64 */
//#elif   !PTR_IS_INT
//                           pCurDesc->m_flags |= FL_LONG;                   /* assume we're converting a long */
//#endif
//                           // fallthrough to hex formatting...
//                        }
//
//                        case _T('X'):
//                            pCurDesc->m_hexadd = _T('A') - _T('9') - 1;     /* set hexadd for uppercase hex */
//                            goto COMMON_HEX;
//
//                        case _T('x'):
//                        {
//                            pCurDesc->m_hexadd = _T('a') - _T('9') - 1;     /* set hexadd for lowercase hex */
//                        COMMON_HEX:
//                            if(pCurDesc->m_flags & FL_I64)
//                            {
//                                pCurDesc->m_stackType = SE_INT64;
//                            }
//                            else
//                            {
//                                pCurDesc->m_stackType = SE_INT32;
//                            }
//
//                            pCurDesc->m_radix = 16;
//                            if (pCurDesc->m_flags & FL_ALTERNATE) {
//                                /* alternate form means '0x' prefix */
//                                pCurDesc->m_prefix[0] = _T('0');
//                                pCurDesc->m_prefix[1] = (_T('x') - _T('a') + _T('9') + 1 + pCurDesc->m_hexadd);  /* 'x' or 'X' */
//                                pCurDesc->m_prefixlen = 2;
//                            }
//                            break;
//                        }
//
//                        case _T('o'):
//                        {
//                            // unsigned octal output */
//                            pCurDesc->m_radix = 8;
//                            if(pCurDesc->m_flags & (FL_I64|FL_LONGLONG))
//                            {
//                                pCurDesc->m_stackType = SE_INT64;
//                            }
//                            else
//                            {
//                                pCurDesc->m_stackType = SE_INT32;
//                            }
//                            if(pCurDesc->m_flags & FL_ALTERNATE)
//                            {
//                                pCurDesc->m_flags |= FL_FORCEOCTAL;
//                            }
//                            break;
//
//                        }
//
//                        case _T('t'):
//                        {
//                            // extended logging types
//
//                            switch(*format)
//                            {
//
//                                // extended types:
//                                // %ta - socket address
//                                // %tG - GUID with braces
//                                // %tg - guid with no braces
//                                // %tT - TimeStamp
//                                // %tt - TimeInterval
//                                // %ts - static string
//
//                                case _T('a'):
//                                {
//                                    pCurDesc->m_stackType = SE_SOCKADDR;
//                                    break;
//                                }
//
//                                case _T('G'):
//                                {
//                                    pCurDesc->m_stackType = SE_GUID_BRACE;
//                                    break;
//                                }
//
//                                case _T('g'):
//                                {
//                                    pCurDesc->m_stackType = SE_GUID;
//                                    break;
//                                }
//
//                                case _T('T'):
//                                {
//                                    pCurDesc->m_stackType = SE_TIMESTAMP;
//                                    break;
//                                }
//
//                                case _T('t'):
//                                {
//                                    pCurDesc->m_stackType = SE_TIMEINTERVAL;
//                                    break;
//                                }
//
//                                case _T('s'):
//                                {
//                                    pCurDesc->m_stackType = SE_PSTR_STATIC;
//                                    break;
//                                }
//
//                                default:
//                                {
//                                    // skip the fixups below.
//                                    state = ST_NORMAL;
//                                    goto NORMAL_STATE;
//                                }
//                            }
//
//                            // extra character for extended format specifier
//                            pCurDesc->m_len += 1;
//                            format += 1;
//
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    pPreprocessBlock->m_formatLen = (UInt16)(format-startingFormat);
//
//    if(pPreprocessBlock != pFormatPreprocessBlock)
//    {
//        //if the caller did not provide the preprocess block,
//        //allocate a correctly sized block from the heap, and copy
//        //from the full sized one constructed on the stack.
//        pPreprocessBlock = alloc_preprocess_block(&PreprocessBlock);
//    }
//
//exit:
//    return pPreprocessBlock;
//
//error_exit:
//    pPreprocessBlock=NULL;
//    goto exit;
//
//}
//
//LogEntry * __CRTDECL WriteLogEntry(
//    PVOID pBuffer,
//    int bufferSize,
//    format_preprocess_block *pPreprocessBlock,
//    const char *title,
//    int tid,
//    FILETIME ft,
//    GUID activityId,
//    GUID entryPointId,
//    va_list argptr)
//{
//    // That's the minimum we'll have starting at m_args.
//    int fixedArgsSize = pPreprocessBlock->m_nDescInUse * sizeof(param);
//
//    bufferSize -= ((sizeof(LogEntry) + fixedArgsSize));
//
//    if (bufferSize < 0)
//    {
//        // buffer too small
//        return NULL;
//    }
//
//    LogEntry * pLogEntry = (LogEntry *)pBuffer;
//
//    pLogEntry->m_pPreprocessBlock   = pPreprocessBlock;
//    pLogEntry->m_preprocessBlockUid = pPreprocessBlock->m_blockUid;
//    pLogEntry->m_title              = title;
//    pLogEntry->m_ThreadId           = tid;
//    pLogEntry->m_timestamp          = ft;
//    pLogEntry->m_ActivityId         = activityId;
//    pLogEntry->m_EntryPointId       = entryPointId;
//
//    pPreprocessBlock->CacheTitle(title);
//
//    // Now we pack the argument list starting at m_Buffer.
//    // There are pPreprocessBlock->m_nDescsInUse arguments and each will have
//    // a dedicated 64 bit entry in m_Buffer. This is the fixed arguments block.
//    // Arguments that fit within 64 bits will be copied in that dedicated entry.
//    // Arguments that don't fit (like strings, GUIDs) will be packed after the
//    // fixed argument block. Their dedicated entry will store their offset,
//    // relative to m_Buffer. That scheme will allow the memory logger to
//    // memcpy the argument list to the target file without any other processing.
//
//    // Initialize the offset to indicate past the fixed arguments block. Offset
//    // is based at the begining of the fixed arguments block.
//    int offset = fixedArgsSize;
//    char * base = (char *)pLogEntry->m_args;
//
//    ZeroMemory(base, fixedArgsSize);
//
//    for(int i = 0; i<pPreprocessBlock->m_nDescInUse; i++)
//    {
//        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
//
//        switch(pCurDesc->m_stackType)
//        {
//            case SE_PSTR://         0x01
//            case SE_PSTR_STATIC://  0x0E
//            {
//                char * s = (char *)get_ptr_arg(&argptr);
//
//                if (s != NULL && bufferSize > 0)
//                {
//                    pLogEntry->m_args[i].u64 = offset;
//
//                    // Copy/truncate the string, always add terminating NULL.
//                    char * target = base + offset;
//                    char c;
//
//                    do
//                    {
//                        c = *target++ = *s++;
//                        bufferSize--;
//                    }
//                    while (c && bufferSize > 0);
//
//                    offset = (int)(target - base);
//
//                    if (c)
//                    {
//                        // We ran out of buffer. Add \0.
//                        *(target - 1) = '\0';
//                    }
//                }
//                break;
//            }
//
//            case SE_PWSTR://        0x02
//            {
//                wchar_t * s = (wchar_t *)get_ptr_arg(&argptr);
//
//                if (s != NULL && bufferSize >= sizeof(wchar_t))
//                {
//                    pLogEntry->m_args[i].u64 = offset;
//
//                    // Copy/truncate the string, always add terminating NULL.
//                    wchar_t * target = (wchar_t *)(base + offset);
//                    wchar_t wc;
//
//                    do
//                    {
//                        wc = *target++ = *s++;
//                        bufferSize -= sizeof(wchar_t);
//                    }
//                    while (wc && bufferSize >= sizeof(wchar_t));
//
//                    offset = (int)((char *)target - base);
//
//                    if (wc)
//                    {
//                        // We ran out of buffer. Add \0.
//                        *(target - 1) = L'\0';
//                    }
//                }
//                break;
//            }
//
//            case SE_CHAR://         0x03
//            {
//                pLogEntry->m_args[i].c = get_char_arg(&argptr);
//                break;
//            }
//
//            case SE_WCHAR://        0x04
//            case SE_INT16://        0x05
//            {
//                pLogEntry->m_args[i].wc = get_short_arg(&argptr);
//                break;
//            }
//
//            case SE_INT32://        0x06
//            {
//                pLogEntry->m_args[i].u = get_int_arg(&argptr);
//                break;
//            }
//
//            case SE_INT64://        0x07
//            case SE_TIMESTAMP://    0x0C
//            case SE_TIMEINTERVAL:// 0x0D
//            {
//                pLogEntry->m_args[i].u64 = get_long_long_arg(&argptr);
//                break;
//            }
//            case SE_DOUBLE://       0x08
//            {
//                pLogEntry->m_args[i].d = va_arg(argptr, double);
//                break;
//            }
//
//            case SE_GUID_BRACE://  0x09
//            case SE_GUID://        0x0A
//            {
//                GUID localGuid = va_arg(argptr, GUID);
//
//                if (bufferSize >= sizeof(GUID))
//                {
//                    pLogEntry->m_args[i].u64 = offset;
//                    memcpy(base + offset, &localGuid, sizeof(GUID));
//                    offset += sizeof(GUID);
//                    bufferSize -= sizeof(GUID);
//                }
//                break;
//            }
//
//            case SE_SOCKADDR://    0x0B
//            {
//                SOCKADDR localSockAddr = va_arg(argptr, SOCKADDR);
//
//                if (bufferSize >= sizeof(SOCKADDR))
//                {
//                    pLogEntry->m_args[i].u64 = offset;
//                    memcpy(base + offset, &localSockAddr, sizeof(SOCKADDR));
//                    offset += sizeof(SOCKADDR);
//                    bufferSize -= sizeof(SOCKADDR);
//                }
//                break;
//            }
//
//            case SE_PERCENT: // 0x0F
//            {
//                // we encountered a percent sign followed by some other character,
//                // just skip over the percent sign as per spec.
//                break;
//            }
//        }
//    }
//
//    pLogEntry->m_argsSize = offset;
//
//    return pLogEntry;
//}
//
//LogEntry * __CRTDECL WriteLogEntryStatic(
//    PVOID pBuffer,
//    int bufferSize,
//    format_preprocess_block *pPreprocessBlock,
//    va_list argptr)
//{
//    // That's the minimum we'll have starting at m_args.
//    int fixedArgsSize = pPreprocessBlock->m_nDescInUse * sizeof(param);
//
//    bufferSize -= ((sizeof(LogEntry) + fixedArgsSize));
//
//    if (bufferSize < 0)
//    {
//        // buffer too small
//        return NULL;
//    }
//
//    LogEntry * pLogEntry = (LogEntry *)pBuffer;
//
//    pLogEntry->m_pPreprocessBlock   = pPreprocessBlock;
//    pLogEntry->m_preprocessBlockUid = pPreprocessBlock->m_blockUid;
//
//    // See WriteLogEntry for how we pack arguments.
//    int offset = fixedArgsSize;
//    char * base = (char *)pLogEntry->m_args;
//
//    ZeroMemory(base, fixedArgsSize);
//
//    for(int i = 0; i<pPreprocessBlock->m_nDescInUse; i++)
//    {
//        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
//
//        switch(pCurDesc->m_stackType)
//        {
//            case SE_PSTR://         0x01
//            case SE_PWSTR://        0x02
//            case SE_PSTR_STATIC://  0x0E
//            {
//                pLogEntry->m_args[i].pv = get_ptr_arg(&argptr);
//                break;
//            }
//
//            case SE_CHAR://         0x03
//            {
//                pLogEntry->m_args[i].c = get_char_arg(&argptr);
//                break;
//            }
//
//            case SE_WCHAR://        0x04
//            case SE_INT16://        0x05
//            {
//                pLogEntry->m_args[i].s = get_short_arg(&argptr);
//                break;
//            }
//
//            case SE_INT32://        0x06
//            {
//                pLogEntry->m_args[i].u = get_int_arg(&argptr);
//                break;
//            }
//
//            case SE_INT64://        0x07
//            case SE_TIMESTAMP://    0x0C
//            case SE_TIMEINTERVAL:// 0x0D
//            {
//                pLogEntry->m_args[i].u64 = get_long_long_arg(&argptr);
//                break;
//            }
//            case SE_DOUBLE://       0x08
//            {
//                pLogEntry->m_args[i].d = va_arg(argptr, double);
//                break;
//            }
//
//            case SE_GUID_BRACE://  0x09
//            case SE_GUID://        0x0A
//            {
//                GUID localGuid = va_arg(argptr, GUID);
//
//                if (bufferSize >= sizeof(GUID))
//                {
//                    pLogEntry->m_args[i].pv = base + offset;
//                    memcpy(pLogEntry->m_args[i].pv, &localGuid, sizeof(GUID));
//                    offset += sizeof(GUID);
//                    bufferSize -= sizeof(GUID);
//                }
//                break;
//            }
//
//            case SE_SOCKADDR://    0x0B
//            {
//                SOCKADDR localSockAddr = va_arg(argptr, SOCKADDR);
//
//                if (bufferSize >= sizeof(SOCKADDR))
//                {
//                    pLogEntry->m_args[i].pv = base + offset;
//                    memcpy(pLogEntry->m_args[i].pv, &localSockAddr, sizeof(SOCKADDR));
//                    offset += sizeof(SOCKADDR);
//                    bufferSize -= sizeof(SOCKADDR);
//                }
//                break;
//            }
//
//            case SE_PERCENT: // 0x0F
//            {
//                // we encountered a percent sign followed by some other character,
//                // just skip over the percent sign as per spec.
//                break;
//            }
//        }
//    }
//
//    pLogEntry->m_argsSize = offset;
//
//    return pLogEntry;
//}

// returns bytes copied, not including NULL
// also returns zero when no bytes are copied
size_t WCharToUTF8(char *szOut, size_t cbOut, wchar_t const *szIn, size_t iSize)
{
    size_t i = 0;
    size_t iOut = 0;
    size_t cbRemaining = cbOut;
    BOOL isMultiByte = false;

    if (cbOut == 0)
    {
        return 0;
    }

    // requires target string to have some extra space, but makes this much more effecient
    for (; i < iSize && szIn[i] != L'\0' && cbRemaining >= 4; i++)
    {
        wchar_t wch = szIn[i];

        if (wch < 0x80)
        {
            cbRemaining -= 1;
            szOut[iOut++] = (UInt8)(wch);
        }
        else
        {
            // we use WideCharToMultiByte to convert wchar to utf8 if we find a wide char having a value more than 128 
            isMultiByte = true;
            iOut = 0;
            break;
        }
    }

    if (isMultiByte && cbOut > 1)
    {
        // needs to leave space for NULL terminator otherwise the epilogue may break the character
        // similar to cbRemaining >= 4 above that there could still be buffer left for rendering other args
        iOut = WideCharToMultiByte(CP_UTF8, 0, szIn, (Int32)iSize, szOut, (Int32)cbOut - 1, NULL, NULL);

        if (iOut == 0)
        {
            if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            {
                // Caller's caller assumes we will print something and return >= 1.  If there are
                // no characters that can be converted, just stick a something in so that we aren't broken.
                szOut[iOut++] = '?';
            }
            else
            {
                // convert char by char and truncate the rest
                size_t ret = 0;
                while (iOut < cbOut - 1 && iSize > 0)
                {
                    ret = WideCharToMultiByte(CP_UTF8, 0, szIn, 1, szOut + iOut, (Int32)(cbOut - iOut) - 1, NULL, NULL);
                    if (ret == 0)
                    {
                        // not enough space for the current char, errors other than insufficient buffer have been handled above
                        break;
                    }
                    else
                    {
                        szIn++;
                        iSize--;
                        iOut += ret;
                    }
                }
            }
        }
    }

    szOut[iOut] = '\0';

    return iOut;

}

//jwesker
Size_t __CRTDECL log_entry_sprintf(
    char *out,
    Size_t  cbOut,
    LogEntry *pLogEntry
)
{

    format_preprocess_block *pPreprocessBlock = pLogEntry->m_pPreprocessBlock;

    const char *format = pPreprocessBlock->m_format;
    Size_t   iformat = 0;

    Size_t cbWritten = 0;

    for (int i = 0; i < pPreprocessBlock->m_nDescInUse; i++)
    {

        // copy up to the first format specifier into the output buffer.

        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];

        UInt32 Ordinal = pCurDesc->m_stackType;

        // copy non-format specifiers bytes since the last format specifier

        Size_t BytesToCopy = pCurDesc->m_offset - iformat;

        if (BytesToCopy + cbWritten > cbOut)
        {
            // early exit, out of target buffer.
            goto exit;
        }

        memcpy(out + cbWritten, &format[iformat], BytesToCopy);

        // update our position in the format string to be past this format specifier
        iformat += BytesToCopy + pCurDesc->m_len;
        cbWritten += BytesToCopy;

        if (cbWritten == cbOut)
        {
            // out of target buffer
            goto exit;
        }

        //  NB: 'p' in this case may either contain a pointer provided by the
        //      original caller when we're in direct mode, or point into the
        //      memory beyond pLogEntry->m_args, but still within pLogEntry
        //      if LogEntry::FixArgsOffsets() has been called (memory logging
        //      mode). This complicates SE_COUNTED_STR and SE_COUNTED_WSTR
        //      since we point to different structures in these two cases.  This
        //      juggling is due to the NT string types containing a memory
        //      pointer within the buffer provided to the azure log APIs.
        param p = pLogEntry->m_args[i];

        switch (Ordinal)
        {
        case SE_PSTR://         0x01
        case SE_PSTR_STATIC: // 0x0E
        case SE_PSTR_INTERNAL: // 0x12
        {
            if (p.pstr == NULL)
            {
                p.pstr = __nullstring;
            }

            Size_t len = strlen(p.pstr); // length without NULL termination

            if (pCurDesc->m_precision != -1 && (Int32)len > pCurDesc->m_precision)
            {
                len = pCurDesc->m_precision;
            }

            if (len + cbWritten > cbOut)
            {
                len = cbOut - cbWritten;
            }

            // copy all the bytes, without the NULL termination, see epilogue
            memcpy(out + cbWritten, p.pstr, len);
            cbWritten += len;
            break;
        }

        case SE_PWSTR://        0x02
        {
            // we convert UNICODE strings to UTF8

            if (p.pwstr == NULL)
            {
                p.pwstr = __wnullstring;
            }

            Size_t WCharsToCopy = wcslen(p.pwstr); // length without NULL termination.

            if (pCurDesc->m_precision != -1 && (Int32)WCharsToCopy > pCurDesc->m_precision)
            {
                WCharsToCopy = pCurDesc->m_precision;
            }

            Size_t len;

            // copies the bytes with NULL termination, returns length without NULL termination                
            len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, p.pwstr, WCharsToCopy);

            // only count the bytes, not the NULL termination, see epilogue
            cbWritten += len;

            break;
        }

        case SE_COUNTED_STR://  0x10
        {
            Size_t len;
            char *s;

            bool bStringWithinLogEntry =
                (((PUCHAR)p.pCountedStr > (PUCHAR)&pLogEntry->m_args[0]) &&
                ((PUCHAR)p.pCountedStr < (PUCHAR)&pLogEntry->m_args[0] + pLogEntry->m_argsSize));

            //  See comment above declaration of 'p'.  %Z handling was added after
            //  the high performance logging was implemented, so it's kinda a hack.
            if (bStringWithinLogEntry)
            {
                len = p.pCountedStr->usLengthInBytes;   // length without NULL termination
                s = &p.pCountedStr->cBuffer[0];
            }
            else
            {
                if (ValidAnsiString(p.pAnsiStr))
                {
                    len = p.pAnsiStr->Length;
                    s = &p.pAnsiStr->Buffer[0];
                }
                else
                {
                    len = __counted_ansi_nullstring.Length;
                    s = &__counted_ansi_nullstring.Buffer[0];
                }
            }

            if (pCurDesc->m_precision != -1 && (Int32)len > pCurDesc->m_precision)
            {
                len = pCurDesc->m_precision;
            }

            if (len + cbWritten > cbOut)
            {
                len = cbOut - cbWritten;
            }

            memcpy(out + cbWritten, s, len);
            cbWritten += len;

            break;
        }

        case SE_COUNTED_WSTR:// 0x11
        {
            Size_t WCharsToCopy;
            wchar_t *s;

            bool bStringWithinLogEntry =
                (((PUCHAR)p.pCountedStr > (PUCHAR)&pLogEntry->m_args[0]) &&
                ((PUCHAR)p.pCountedStr < (PUCHAR)&pLogEntry->m_args[0] + pLogEntry->m_argsSize));

            //  See comment above declaration of 'p'.  %Z handling was added after
            //  the high performance logging was implemented, so it's kinda a hack.
            if (bStringWithinLogEntry)
            {
                WCharsToCopy = p.pCountedStr->usLengthInBytes / sizeof(wchar_t);
                s = &p.pCountedStr->wcBuffer[0];
            }
            else
            {
                if (ValidUnicodeString(p.pUnicodeStr))
                {
                    WCharsToCopy = p.pUnicodeStr->Length / sizeof(wchar_t);
                    s = &p.pUnicodeStr->Buffer[0];
                }
                else
                {
                    WCharsToCopy = __counted_unicode_nullstring.Length / sizeof(wchar_t);
                    s = &__counted_unicode_nullstring.Buffer[0];
                }
            }

            if (pCurDesc->m_precision != -1 && (Int32)WCharsToCopy > pCurDesc->m_precision)
            {
                WCharsToCopy = pCurDesc->m_precision;
            }

            Size_t len;

            // Convert at most WCharsToCopy chars.  Returns length without NULL termination.
            len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, s, WCharsToCopy);

            // only count the bytes, not the NULL termination, see epilogue
            cbWritten += len;

            break;
        }

        case SE_CHAR://         0x03
        {
            out[cbWritten++] = p.c;
            break;
        }

        case SE_WCHAR://        0x04
        {
            if (p.wc == L'\0')
            {
                // Same behavior as for SE_CHAR.
                out[cbWritten++] = '\0';
            }
            else
            {
                // convert UNICODE characters to UTF8

                Size_t len;

                // copies the bytes with NULL termination, returns length without NULL termination
                len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, &p.wc, 1);

                cbWritten += len;
            }

            break;
        }

        case SE_INT16://        0x05
        case SE_INT32://        0x06
        case SE_INT64://        0x07
        {
            // in deference to all those nice output formatting params,
            // we roll all the formatting together here.  If we get complaints
            // we can always call _snprintf directly or port the formatting code.

            // copy the prefix into the target buffer.

            if (pCurDesc->m_prefixlen != 0)
            {
                BytesToCopy = pCurDesc->m_prefixlen;

                if (BytesToCopy + cbWritten > cbOut)
                {
                    goto exit;
                }

                out[cbWritten++] = pCurDesc->m_prefix[0];

                if (BytesToCopy > 1)
                {
                    out[cbWritten++] = pCurDesc->m_prefix[1];
                }
            }

            Size_t BytesLeft = cbOut - cbWritten;
            int radix = pCurDesc->m_radix;
            errno_t result = 1; // init to error
            Size_t cbIntWritten = 0;
            char *target = &out[cbWritten];

            // NULL terminate in case we can't fit the value in.
            *target = '\0';

            // we use a lot of the formatting options of printf here, by using "itoa" style
            // rendering of the output string.  That is probably a good thing performance wise.
            // if we get too many complaints, change to sprintf with the format string.

            if (Ordinal == SE_INT32 || Ordinal == SE_INT16)
            {
                // maximum length for a signed 32 bit integer is 13 bytes including NULL termination
                if (BytesLeft >= 13)
                {
                    if (pCurDesc->m_flags & FL_SIGNED)
                    {
                        // signed
                        result = _itoa_s(p.n, target, BytesLeft, radix);
                    }
                    else
                    {
                        // unsigned
                        result = _ui64toa_s(p.u, target, BytesLeft, radix);
                    }
                }
            }
            else if (Ordinal == SE_INT64)
            {
                // maximum length for a signed 64 bit integer is 24 bytes including NULL termination
                if (BytesLeft >= 24)
                {
                    if (pCurDesc->m_flags & FL_SIGNED)
                    {
                        //signed
                        result = _i64toa_s(p.i64, target, BytesLeft, radix);
                    }
                    else
                    {
                        // unsigned
                        result = _ui64toa_s(p.u64, target, BytesLeft, radix);
                    }
                }
            }

            cbIntWritten = strlen(target);

            if (result == 0 && cbIntWritten < pCurDesc->m_fldwidth)
            {
                if ((radix == 16) || (pCurDesc->m_flags | FL_LEADZERO))
                {
                    // add leading 0's for hex values.
                    Size_t leadZeros = pCurDesc->m_fldwidth - cbIntWritten;

                    if (leadZeros + cbIntWritten + 1 < BytesLeft)
                    {
                        // shift the current value to the right (including the NULL termination)
                        memmove(target + leadZeros, target, cbIntWritten + 1);

                        // lead pad the result with 0's
                        memset(target, '0', leadZeros);

                        cbIntWritten += leadZeros;
                    }
                }
            }

            cbWritten += cbIntWritten;

            break;
        }

        case SE_DOUBLE://       0x08
        {
            // nothing is better at converting floats to strings
            // that the C sprintf.  The operation takes so long the
            // extra calling overhead is negligible.

            char fformat[40];

            Size_t BytesLeft = cbOut - cbWritten;
            Size_t BytesCopied;

            // copy the format string out of the format_param_desc
            memcpy(fformat, &format[pCurDesc->m_offset], pCurDesc->m_len);
            // NULL terminate the format string
            fformat[pCurDesc->m_len] = 0;

            BytesCopied = _snprintf_s(&out[cbWritten], BytesLeft, _TRUNCATE, fformat, p.d);

            if (BytesCopied == -1)
            {
                BytesCopied = BytesLeft;
            }

            cbWritten += BytesCopied;
            break;
        }

        // This is why we re-implemented sprintf style functionality so we can get
        // in the path and parse our own types...

        case SE_GUID_BRACE: // 0x09
        case SE_GUID://        0x0A
        {

            bool fBraces = false;
            int BytesToCopy;

            GUID *pGUID = (GUID *)p.pv;

            if (pGUID == NULL)
            {
                pGUID = &__nullguid;
            }

            if (Ordinal == SE_GUID_BRACE)
            {
                fBraces = true;
                BytesToCopy = 38;
            }
            else
            {
                BytesToCopy = 36;
            }

            if (BytesToCopy + cbWritten > cbOut)
            {
                goto exit;
            }

            GuidWrite2(&out[cbWritten], cbOut - cbWritten, pGUID, fBraces);

            cbWritten += BytesToCopy;
            break;
        }

        case SE_SOCKADDR://    0x0B
        {
            SOCKADDR *pSockAddr = (SOCKADDR *)p.pv;

            if (pSockAddr == NULL)
            {
                pSockAddr = &__nullsockaddr;
            }

            if (pSockAddr->sa_family == AF_INET)
            {
                Size_t cbIpWritten;
                sockaddr_in *pinaddr = (sockaddr_in*)pSockAddr;
                Size_t BytesLeft = cbOut - cbWritten;
                char *target = &out[cbWritten];

                if (BytesLeft < sizeof("255.255.255.255:65535"))
                {
                    //maybe not enough room, bail...
                    goto exit;
                }

                // we have enough room guaranteed
                cbIpWritten = UInt32NetAddr(target, (UInt32*)&pinaddr->sin_addr);

                target += cbIpWritten;

                int cbPortWritten = sprintf(target, ":%d", htons(pinaddr->sin_port));

                if (cbPortWritten != -1)
                {
                    cbIpWritten += cbPortWritten;
                }

                cbWritten += cbIpWritten;

            }

            break;
        }

        case SE_TIMESTAMP://    0x0C
        {
            CsTimeStamp TimeStamp = (CsTimeStamp)p.u64;

            Size_t cbTimeWritten;
            Size_t BytesLeft = cbOut - cbWritten;
            char *target = &out[cbWritten];

            HRESULT err = CsTimeStampToString(TimeStamp, target, (int)BytesLeft);

            if (err != CsError_OK)
            {
                goto exit;
            }

            //ick, maybe copying out of printf would be better. BUBUG:
            cbTimeWritten = strlen(target);

            cbWritten += cbTimeWritten;

            break;
        }

        case SE_TIMEINTERVAL:// 0x0D
        {
            CsTimeInterval TimeInterval = (CsTimeInterval)p.i64;

            Size_t cbTimeWritten;
            Size_t BytesLeft = cbOut - cbWritten;
            char *target = &out[cbWritten];

            HRESULT err = CsTimeIntervalToString(TimeInterval, target, BytesLeft);

            if (err != CsError_OK)
            {
                goto exit;
            }

            //ick, maybe copying out of printf would be better. BUBUG:
            cbTimeWritten = strlen(target);

            cbWritten += cbTimeWritten;

            break;
        }

        case SE_PERCENT: // 0x0E
        {
            // just skip over the percent if not a valid format specifier
            break;
        }
        }
    }

    // copy the tail of the format string to the output buffer.
    {
        Size_t BytesLeft = cbOut - cbWritten;
        Size_t BytesCopied;
        errno_t err;

        if (BytesLeft)
        {
            out[cbWritten] = _T('\0');

            err = strncpy_s(&out[cbWritten], BytesLeft, &format[iformat], _TRUNCATE);

            BytesCopied = strlen(&out[cbWritten]);

            cbWritten += BytesCopied;
        }
    }

exit:

    // epilogue:
    // NULL terminate the output
    if (cbWritten == cbOut)
    {
        cbWritten -= 1;
    }
    out[cbWritten++] = '\0';

    return cbWritten;
}

Size_t CopyCSharpFormat(PCHAR formatOut, Size_t size, PCSTR source, Size_t len)
{
    Size_t written = 0;
    for (int j = 0; j < len && source[j] != '\0'; j++)
    {
        if (written >= size) return written;
        char c = source[j];
        formatOut[written++] = c;

        if (written >= size) return written;
        if (c == '{') formatOut[written++] = '{';
        else if (c == '}') formatOut[written++] = '}';
    }

    return written;
}

Size_t __CRTDECL log_entry_sprintf_csformat(
    PCHAR formatOut,
    Size_t cbFormatOut,
    PCHAR out,
    Size_t cbOut,
    Size_t indexWidthLength[],
    Size_t parameterMaxCount,
    LogEntry* pLogEntry)
{
    format_preprocess_block *pPreprocessBlock = pLogEntry->m_pPreprocessBlock;
    const char *format = pPreprocessBlock->m_format;

    Size_t   iformat = 0;
    Size_t cbFormatWritten = 0;
    Size_t cbWritten = 0;
    Size_t cbLastWritten = 0;

    Size_t count = MIN(parameterMaxCount, pPreprocessBlock->m_nDescInUse);

    for (int i = 0; i < count; i++)
    {
        // copy up to the first format specifier into the output buffer.
        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
        UInt32 Ordinal = pCurDesc->m_stackType;

        // copy non-format specifiers bytes since the last format specifier
        Size_t BytesToCopy = pCurDesc->m_offset - iformat;

        if (BytesToCopy + cbFormatWritten > cbFormatOut)
        {
            BytesToCopy = cbFormatOut - cbFormatWritten;
        }

        indexWidthLength[i * 3] = pCurDesc->m_offset;
        indexWidthLength[i * 3 + 1] = pCurDesc->m_len;

        cbFormatWritten += CopyCSharpFormat(formatOut + cbFormatWritten, cbFormatOut - cbFormatWritten, &format[iformat], (int)BytesToCopy);
        iformat += BytesToCopy + pCurDesc->m_len;

        cbFormatWritten += sprintf_s(formatOut + cbFormatWritten, cbFormatOut - cbFormatWritten, "{%d}", i);

        if (cbWritten >= cbOut || cbFormatWritten >= cbFormatOut)
        {
            // out of target buffer
            goto exit;
        }

        //  NB: 'p' in this case may either contain a pointer provided by the
        //      original caller when we're in direct mode, or point into the
        //      memory beyond pLogEntry->m_args, but still within pLogEntry
        //      if LogEntry::FixArgsOffsets() has been called (memory logging
        //      mode). This complicates SE_COUNTED_STR and SE_COUNTED_WSTR
        //      since we point to different structures in these two cases.  This
        //      juggling is due to the NT string types containing a memory
        //      pointer within the buffer provided to the azure log APIs.
        param p = pLogEntry->m_args[i];
        switch (Ordinal)
        {
        case SE_PSTR://         0x01
        case SE_PSTR_STATIC: // 0x0E
        case SE_PSTR_INTERNAL: // 0x12
        {
            if (p.pstr == NULL)
            {
                p.pstr = __nullstring;
            }

            Size_t len = strlen(p.pstr); // length without NULL termination

            if (pCurDesc->m_precision != -1 && (Int32)len > pCurDesc->m_precision)
            {
                len = pCurDesc->m_precision;
            }

            if (len + cbWritten > cbOut)
            {
                len = cbOut - cbWritten;
            }

            // copy all the bytes, without the NULL termination, see epilogue
            memcpy(out + cbWritten, p.pstr, len);
            cbWritten += len;
            break;
        }

        case SE_PWSTR://        0x02
        {
            // we convert UNICODE strings to UTF8

            if (p.pwstr == NULL)
            {
                p.pwstr = __wnullstring;
            }

            Size_t WCharsToCopy = wcslen(p.pwstr); // length without NULL termination.

            if (pCurDesc->m_precision != -1 && (Int32)WCharsToCopy > pCurDesc->m_precision)
            {
                WCharsToCopy = pCurDesc->m_precision;
            }

            // copies the bytes with NULL termination, returns length without NULL termination                
            Size_t len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, p.pwstr, WCharsToCopy);

            // only count the bytes, not the NULL termination, see epilogue
            cbWritten += len;
            break;
        }

        case SE_COUNTED_STR://  0x10
        {
            Size_t len;
            char *s;

            bool bStringWithinLogEntry =
                (((PUCHAR)p.pCountedStr > (PUCHAR)&pLogEntry->m_args[0]) &&
                ((PUCHAR)p.pCountedStr < (PUCHAR)&pLogEntry->m_args[0] + pLogEntry->m_argsSize));

            //  See comment above declaration of 'p'.  %Z handling was added after
            //  the high performance logging was implemented, so it's kinda a hack.
            if (bStringWithinLogEntry)
            {
                len = p.pCountedStr->usLengthInBytes;   // length without NULL termination
                s = &p.pCountedStr->cBuffer[0];
            }
            else
            {
                if (ValidAnsiString(p.pAnsiStr))
                {
                    len = p.pAnsiStr->Length;
                    s = &p.pAnsiStr->Buffer[0];
                }
                else
                {
                    len = __counted_ansi_nullstring.Length;
                    s = &__counted_ansi_nullstring.Buffer[0];
                }
            }

            if (pCurDesc->m_precision != -1 && (Int32)len > pCurDesc->m_precision)
            {
                len = pCurDesc->m_precision;
            }

            if (len + cbWritten > cbOut)
            {
                len = cbOut - cbWritten;
            }

            memcpy(out + cbWritten, s, len);
            cbWritten += len;

            break;
        }

        case SE_COUNTED_WSTR:// 0x11
        {
            Size_t WCharsToCopy;
            wchar_t *s;

            bool bStringWithinLogEntry =
                (((PUCHAR)p.pCountedStr > (PUCHAR)&pLogEntry->m_args[0]) &&
                ((PUCHAR)p.pCountedStr < (PUCHAR)&pLogEntry->m_args[0] + pLogEntry->m_argsSize));

            //  See comment above declaration of 'p'.  %Z handling was added after
            //  the high performance logging was implemented, so it's kinda a hack.
            if (bStringWithinLogEntry)
            {
                WCharsToCopy = p.pCountedStr->usLengthInBytes / sizeof(wchar_t);
                s = &p.pCountedStr->wcBuffer[0];
            }
            else
            {
                if (ValidUnicodeString(p.pUnicodeStr))
                {
                    WCharsToCopy = p.pUnicodeStr->Length / sizeof(wchar_t);
                    s = &p.pUnicodeStr->Buffer[0];
                }
                else
                {
                    WCharsToCopy = __counted_unicode_nullstring.Length / sizeof(wchar_t);
                    s = &__counted_unicode_nullstring.Buffer[0];
                }
            }

            if (pCurDesc->m_precision != -1 && (Int32)WCharsToCopy > pCurDesc->m_precision)
            {
                WCharsToCopy = pCurDesc->m_precision;
            }

            Size_t len;

            // Convert at most WCharsToCopy chars.  Returns length without NULL termination.
            len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, s, WCharsToCopy);

            // only count the bytes, not the NULL termination, see epilogue
            cbWritten += len;

            break;
        }

        case SE_CHAR://         0x03
        {
            out[cbWritten++] = p.c;
            break;
        }

        case SE_WCHAR://        0x04
        {
            if (p.wc == L'\0')
            {
                // Same behavior as for SE_CHAR.
                out[cbWritten++] = '\0';
            }
            else
            {
                // convert UNICODE characters to UTF8
                // copies the bytes with NULL termination, returns length without NULL termination
                Size_t len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, &p.wc, 1);

                cbWritten += len;
            }

            break;
        }

        case SE_INT16://        0x05
        case SE_INT32://        0x06
        case SE_INT64://        0x07
        {
            // in deference to all those nice output formatting params,
            // we roll all the formatting together here.  If we get complaints
            // we can always call _snprintf directly or port the formatting code.

            // copy the prefix into the target buffer.

            if (pCurDesc->m_prefixlen != 0)
            {
                BytesToCopy = pCurDesc->m_prefixlen;

                if (BytesToCopy + cbWritten > cbOut)
                {
                    goto exit;
                }

                out[cbWritten++] = pCurDesc->m_prefix[0];
                if (BytesToCopy > 1)
                {
                    out[cbWritten++] = pCurDesc->m_prefix[1];
                }
            }

            Size_t BytesLeft = cbOut - cbWritten;
            int radix = pCurDesc->m_radix;
            errno_t result = 1; // init to error
            Size_t cbIntWritten = 0;
            char *target = &out[cbWritten];

            // NULL terminate in case we can't fit the value in.
            *target = '\0';

            // we use a lot of the formatting options of printf here, by using "itoa" style
            // rendering of the output string.  That is probably a good thing performance wise.
            // if we get too many complaints, change to sprintf with the format string.

            if (Ordinal == SE_INT32 || Ordinal == SE_INT16)
            {
                // maximum length for a signed 32 bit integer is 13 bytes including NULL termination
                if (BytesLeft >= 13)
                {
                    if (pCurDesc->m_flags & FL_SIGNED)
                    {
                        // signed
                        result = _itoa_s(p.n, target, BytesLeft, radix);
                    }
                    else
                    {
                        // unsigned
                        result = _ui64toa_s(p.u, target, BytesLeft, radix);
                    }
                }
            }
            else if (Ordinal == SE_INT64)
            {
                // maximum length for a signed 64 bit integer is 24 bytes including NULL termination
                if (BytesLeft >= 24)
                {
                    if (pCurDesc->m_flags & FL_SIGNED)
                    {
                        //signed
                        result = _i64toa_s(p.i64, target, BytesLeft, radix);
                    }
                    else
                    {
                        // unsigned
                        result = _ui64toa_s(p.u64, target, BytesLeft, radix);
                    }
                }
            }

            cbIntWritten = strlen(target);

            if (result == 0 && cbIntWritten < pCurDesc->m_fldwidth)
            {
                if ((radix == 16) || (pCurDesc->m_flags | FL_LEADZERO))
                {
                    // add leading 0's for hex values.
                    Size_t leadZeros = pCurDesc->m_fldwidth - cbIntWritten;

                    if (leadZeros + cbIntWritten + 1 < BytesLeft)
                    {
                        // shift the current value to the right (including the NULL termination)
                        memmove(target + leadZeros, target, cbIntWritten + 1);

                        // lead pad the result with 0's
                        memset(target, '0', leadZeros);

                        cbIntWritten += leadZeros;
                    }
                }
            }

            cbWritten += cbIntWritten;

            break;
        }

        case SE_DOUBLE://       0x08
        {
            // nothing is better at converting floats to strings
            // that the C sprintf.  The operation takes so long the
            // extra calling overhead is negligible.

            char fformat[40];

            Size_t BytesLeft = cbOut - cbWritten;
            Size_t BytesCopied;

            // copy the format string out of the format_param_desc
            memcpy(fformat, &format[pCurDesc->m_offset], pCurDesc->m_len);
            // NULL terminate the format string
            fformat[pCurDesc->m_len] = 0;

            BytesCopied = _snprintf_s(&out[cbWritten], BytesLeft, _TRUNCATE, fformat, p.d);

            if (BytesCopied == -1)
            {
                BytesCopied = BytesLeft;
            }

            cbWritten += BytesCopied;
            break;
        }

        // This is why we re-implemented sprintf style functionality so we can get
        // in the path and parse our own types...

        case SE_GUID_BRACE: // 0x09
        case SE_GUID://        0x0A
        {

            bool fBraces = false;
            int BytesToCopy;

            GUID *pGUID = (GUID *)p.pv;

            if (pGUID == NULL)
            {
                pGUID = &__nullguid;
            }

            if (Ordinal == SE_GUID_BRACE)
            {
                fBraces = true;
                BytesToCopy = 38;
            }
            else
            {
                BytesToCopy = 36;
            }

            if (BytesToCopy + cbWritten > cbOut)
            {
                goto exit;
            }

            GuidWrite2(&out[cbWritten], cbOut - cbWritten, pGUID, fBraces);

            cbWritten += BytesToCopy;
            break;
        }

        case SE_SOCKADDR://    0x0B
        {
            SOCKADDR *pSockAddr = (SOCKADDR *)p.pv;

            if (pSockAddr == NULL)
            {
                pSockAddr = &__nullsockaddr;
            }

            if (pSockAddr->sa_family == AF_INET)
            {
                Size_t cbIpWritten;
                sockaddr_in *pinaddr = (sockaddr_in*)pSockAddr;
                Size_t BytesLeft = cbOut - cbWritten;
                char *target = &out[cbWritten];

                if (BytesLeft < sizeof("255.255.255.255:65535"))
                {
                    //maybe not enough room, bail...
                    goto exit;
                }

                // we have enough room guaranteed
                cbIpWritten = UInt32NetAddr(target, (UInt32*)&pinaddr->sin_addr);

                target += cbIpWritten;

                int cbPortWritten = sprintf(target, ":%d", htons(pinaddr->sin_port));

                if (cbPortWritten != -1)
                {
                    cbIpWritten += cbPortWritten;
                }

                cbWritten += cbIpWritten;

            }

            break;
        }

        case SE_TIMESTAMP://    0x0C
        {
            CsTimeStamp TimeStamp = (CsTimeStamp)p.u64;

            Size_t cbTimeWritten;
            Size_t BytesLeft = cbOut - cbWritten;
            char *target = &out[cbWritten];

            HRESULT err = CsTimeStampToString(TimeStamp, target, (int)BytesLeft);

            if (err != CsError_OK)
            {
                goto exit;
            }

            //ick, maybe copying out of printf would be better. BUBUG:
            cbTimeWritten = strlen(target);

            cbWritten += cbTimeWritten;

            break;
        }

        case SE_TIMEINTERVAL:// 0x0D
        {
            CsTimeInterval TimeInterval = (CsTimeInterval)p.i64;

            Size_t cbTimeWritten;
            Size_t BytesLeft = cbOut - cbWritten;
            char *target = &out[cbWritten];

            HRESULT err = CsTimeIntervalToString(TimeInterval, target, BytesLeft);

            if (err != CsError_OK)
            {
                goto exit;
            }

            //ick, maybe copying out of printf would be better. BUBUG:
            cbTimeWritten = strlen(target);

            cbWritten += cbTimeWritten;

            break;
        }

        case SE_PERCENT: // 0x0E
        {
            // just skip over the percent if not a valid format specifier
            break;
        }
        }

        indexWidthLength[i * 3 + 2] = cbWritten - cbLastWritten;
        cbLastWritten = cbWritten;
    }

    // copy the tail of the format string to the output buffer.
    {
        cbFormatWritten += CopyCSharpFormat(formatOut + cbFormatWritten, cbFormatOut - cbFormatWritten, &format[iformat], _TRUNCATE);
        formatOut[cbFormatWritten] = _T('\0');
    }

exit:

    // epilogue:
    // NULL terminate the output
    if (cbFormatWritten == cbFormatOut)
    {
        cbFormatWritten -= 1;
    }
    formatOut[cbFormatWritten++] = '\0';

    if (cbWritten == cbOut)
    {
        cbWritten -= 1;
    }
    out[cbWritten++] = '\0';

    return count;
}

Size_t __CRTDECL log_entry_sprintf_cformat(
    PCHAR out,
    Size_t cbOut,
    StringToken tokens[],
    Size_t maxTokenCount,
    LogEntry* pLogEntry)
{
    memset(tokens, 0, sizeof(tokens));
    format_preprocess_block *pPreprocessBlock = pLogEntry->m_pPreprocessBlock;
    PCSTR format = pPreprocessBlock->m_format;

    Size_t tokenCount = 0;
    if (tokenCount >= maxTokenCount) return tokenCount;

    Size_t   iformat = 0;
    Size_t cbWritten = 0;
    Size_t cbLastWritten = 0;
    int count = pPreprocessBlock->m_nDescInUse;
    for (int i = 0; i < count; i++)
    {
        // copy up to the first format specifier into the output buffer.
        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
        UInt32 Ordinal = pCurDesc->m_stackType;

        // copy non-format specifiers bytes since the last format specifier
        Size_t BytesToCopy = pCurDesc->m_offset - iformat;

        tokens[tokenCount].Pointer.Single = &format[iformat];
        tokens[tokenCount].Length = BytesToCopy;
        tokenCount++;

        if (tokenCount >= maxTokenCount) return tokenCount;

        tokens[tokenCount].Pointer.Single = out + cbWritten;
        tokens[tokenCount].IsParameter = 1;

        //        indexWidthLength[i * 3] = pCurDesc->m_offset;
          //      indexWidthLength[i * 3 + 1] = pCurDesc->m_len;

                //memcpy(out+cbWritten, &format[iformat],BytesToCopy);

                // update our position in the format string to be past this format specifier
        iformat += BytesToCopy + pCurDesc->m_len;
        //  cbWritten += BytesToCopy;

        param p = pLogEntry->m_args[i];
        switch (Ordinal)
        {
        case SE_PSTR://         0x01
        case SE_PSTR_STATIC: // 0x0E
        {
            if (p.pstr == NULL)
            {
                p.pstr = __nullstring;
            }


            Size_t len = strlen(p.pstr); // length without NULL termination
            /*
            if (pCurDesc->m_precision != -1 && (Int32)len > pCurDesc->m_precision)
            {
                len = pCurDesc->m_precision;
            }

            if (len + cbWritten > cbOut)
            {
                len = cbOut - cbWritten;
            }

            memcpy(out + cbWritten, p.pstr, len);
            */

            // copy all the bytes, without the NULL termination, see epilogue
           // Size_t len = sprintf_s(out + cbWritten, cbOut - cbWritten, "%ld", p.pstr);
           // cbWritten += len;

            tokens[tokenCount].Pointer.Single = p.pstr;
            tokens[tokenCount].Length = len;
            break;
        }

        case SE_PWSTR://        0x02
        {
            // we convert UNICODE strings to UTF8

            if (p.pwstr == NULL)
            {
                p.pwstr = __wnullstring;
            }


            Size_t WCharsToCopy = wcslen(p.pwstr); // length without NULL termination.

            if (pCurDesc->m_precision != -1 && (Int32)WCharsToCopy > pCurDesc->m_precision)
            {
                WCharsToCopy = pCurDesc->m_precision;
            }

            // copies the bytes with NULL termination, returns length without NULL termination
            Size_t len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, p.pwstr, WCharsToCopy);

            // only count the bytes, not the NULL termination, see epilogue
            cbWritten += len;

            tokens[tokenCount].IsUnicode = 1;
            tokens[tokenCount].Length = WCharsToCopy;
            tokens[tokenCount].Pointer.Wide = p.pwstr;
            break;
        }

        case SE_CHAR://         0x03
        {
            out[cbWritten++] = p.c;
            break;
        }

        case SE_WCHAR://        0x04
        {
            if (p.wc == L'\0')
            {
                // Same behavior as for SE_CHAR.
                out[cbWritten++] = '\0';
            }
            else
            {
                // convert UNICODE characters to UTF8
                // copies the bytes with NULL termination, returns length without NULL termination
                Size_t len = WCharToUTF8(out + cbWritten, cbOut - cbWritten, &p.wc, 1);

                cbWritten += len;
            }

            break;
        }

        case SE_INT16://        0x05
        case SE_INT32://        0x06
        case SE_INT64://        0x07
        {
            // in deference to all those nice output formatting params,
            // we roll all the formatting together here.  If we get complaints
            // we can always call _snprintf directly or port the formatting code.

            // copy the prefix into the target buffer.

            if (pCurDesc->m_prefixlen != 0)
            {
                BytesToCopy = pCurDesc->m_prefixlen;

                if (BytesToCopy + cbWritten > cbOut)
                {
                    goto exit;
                }

                out[cbWritten++] = pCurDesc->m_prefix[0];
                if (BytesToCopy > 1)
                {
                    out[cbWritten++] = pCurDesc->m_prefix[1];
                }
            }

            Size_t BytesLeft = cbOut - cbWritten;
            int radix = pCurDesc->m_radix;
            errno_t result = 1; // init to error
            Size_t cbIntWritten = 0;
            char *target = &out[cbWritten];

            // NULL terminate in case we can't fit the value in.
            *target = '\0';

            // we use a lot of the formatting options of printf here, by using "itoa" style
            // rendering of the output string.  That is probably a good thing performance wise.
            // if we get too many complaints, change to sprintf with the format string.

            if (Ordinal == SE_INT32 || Ordinal == SE_INT16)
            {
                // maximum length for a signed 32 bit integer is 13 bytes including NULL termination
                if (BytesLeft >= 13)
                {
                    if (pCurDesc->m_flags & FL_SIGNED)
                    {
                        // signed
                        result = _itoa_s(p.n, target, BytesLeft, radix);
                    }
                    else
                    {
                        // unsigned
                        result = _ui64toa_s(p.u, target, BytesLeft, radix);
                    }
                }
            }
            else if (Ordinal == SE_INT64)
            {
                // maximum length for a signed 64 bit integer is 24 bytes including NULL termination
                if (BytesLeft >= 24)
                {
                    if (pCurDesc->m_flags & FL_SIGNED)
                    {
                        //signed
                        result = _i64toa_s(p.i64, target, BytesLeft, radix);
                    }
                    else
                    {
                        // unsigned
                        result = _ui64toa_s(p.u64, target, BytesLeft, radix);
                    }
                }
            }

            cbIntWritten = strlen(target);

            if (result == 0 && cbIntWritten < pCurDesc->m_fldwidth)
            {
                if (radix == 16)
                {
                    // add leading 0's for hex values.
                    Size_t leadZeros = pCurDesc->m_fldwidth - cbIntWritten;

                    if (leadZeros + cbIntWritten + 1 < BytesLeft)
                    {
                        // shift the current value to the right (including the NULL termination)
                        memmove(target + leadZeros, target, cbIntWritten + 1);

                        // lead pad the result with 0's
                        memset(target, '0', leadZeros);

                        cbIntWritten += leadZeros;
                    }
                }
            }

            cbWritten += cbIntWritten;

            break;
        }

        case SE_DOUBLE://       0x08
        {
            // nothing is better at converting floats to strings
            // that the C sprintf.  The operation takes so long the
            // extra calling overhead is negligible.

            char fformat[40];

            Size_t BytesLeft = cbOut - cbWritten;
            Size_t BytesCopied;

            // copy the format string out of the format_param_desc
            memcpy(fformat, &format[pCurDesc->m_offset], pCurDesc->m_len);
            // NULL terminate the format string
            fformat[pCurDesc->m_len] = 0;

            BytesCopied = _snprintf_s(&out[cbWritten], BytesLeft, _TRUNCATE, fformat, p.d);

            if (BytesCopied == -1)
            {
                BytesCopied = BytesLeft;
            }

            cbWritten += BytesCopied;
            break;
        }

        // This is why we re-implemented sprintf style functionality so we can get
        // in the path and parse our own types...

        case SE_GUID_BRACE: // 0x09
        case SE_GUID://        0x0A
        {

            bool fBraces = false;
            int BytesToCopy;

            GUID *pGUID = (GUID *)p.pv;

            if (pGUID == NULL)
            {
                pGUID = &__nullguid;
            }

            if (Ordinal == SE_GUID_BRACE)
            {
                fBraces = true;
                BytesToCopy = 38;
            }
            else
            {
                BytesToCopy = 36;
            }

            if (BytesToCopy + cbWritten > cbOut)
            {
                goto exit;
            }

            GuidWrite2(&out[cbWritten], cbOut - cbWritten, pGUID, fBraces);

            cbWritten += BytesToCopy;
            break;
        }

        case SE_SOCKADDR://    0x0B
        {
            SOCKADDR *pSockAddr = (SOCKADDR *)p.pv;

            if (pSockAddr == NULL)
            {
                pSockAddr = &__nullsockaddr;
            }

            if (pSockAddr->sa_family == AF_INET)
            {
                Size_t cbIpWritten;
                sockaddr_in *pinaddr = (sockaddr_in*)pSockAddr;
                Size_t BytesLeft = cbOut - cbWritten;
                char *target = &out[cbWritten];

                if (BytesLeft < sizeof("255.255.255.255:65535"))
                {
                    //maybe not enough room, bail...
                    goto exit;
                }

                // we have enough room guaranteed
                cbIpWritten = UInt32NetAddr(target, (UInt32*)&pinaddr->sin_addr);

                target += cbIpWritten;

                int cbPortWritten = sprintf(target, ":%d", htons(pinaddr->sin_port));

                if (cbPortWritten != -1)
                {
                    cbIpWritten += cbPortWritten;
                }

                cbWritten += cbIpWritten;

            }

            break;
        }

        case SE_TIMESTAMP://    0x0C
        {
            CsTimeStamp TimeStamp = (CsTimeStamp)p.u64;

            Size_t cbTimeWritten;
            Size_t BytesLeft = cbOut - cbWritten;
            char *target = &out[cbWritten];

            HRESULT err = CsTimeStampToString(TimeStamp, target, (int)BytesLeft);

            if (err != CsError_OK)
            {
                goto exit;
            }

            //ick, maybe copying out of printf would be better. BUBUG:
            cbTimeWritten = strlen(target);

            cbWritten += cbTimeWritten;

            break;
        }

        case SE_TIMEINTERVAL:// 0x0D
        {
            CsTimeInterval TimeInterval = (CsTimeInterval)p.i64;

            Size_t cbTimeWritten;
            Size_t BytesLeft = cbOut - cbWritten;
            char *target = &out[cbWritten];

            HRESULT err = CsTimeIntervalToString(TimeInterval, target, BytesLeft);

            if (err != CsError_OK)
            {
                goto exit;
            }

            //ick, maybe copying out of printf would be better. BUBUG:
            cbTimeWritten = strlen(target);

            cbWritten += cbTimeWritten;

            break;
        }

        case SE_PERCENT: // 0x0E
        {
            // just skip over the percent if not a valid format specifier
            break;
        }
        }

        if (cbWritten > cbLastWritten) tokens[tokenCount].Length = cbWritten - cbLastWritten;

        tokenCount++;
        if (tokenCount >= maxTokenCount) return tokenCount;
    }

exit:

    // epilogue:
    // NULL terminate the output
    if (cbWritten == cbOut)
    {
        cbWritten -= 1;
    }
    out[cbWritten++] = '\0';

    return tokenCount;
}

/***
*int get_int_arg(va_list *pargptr)
*
*Purpose:
*   Gets an int argument off the given argument list and updates *pargptr.
*
*Entry:
*   va_list *pargptr - pointer to argument list; updated by function
*
*Exit:
*   Returns the integer argument read from the argument list.
*
*Exceptions:
*
*******************************************************************************/

__inline int __CRTDECL get_int_arg(
    va_list *pargptr
)
{
    return va_arg(*pargptr, int);
}

/***
*long get_long_arg(va_list *pargptr)
*
*Purpose:
*   Gets an long argument off the given argument list and updates *pargptr.
*
*Entry:
*   va_list *pargptr - pointer to argument list; updated by function
*
*Exit:
*   Returns the long argument read from the argument list.
*
*Exceptions:
*
*******************************************************************************/

#if     !LONG_IS_INT
__inline long __CRTDECL get_long_arg(
    va_list *pargptr
)
{
    return va_arg(*pargptr, long);
}
#endif

#if     !LONGLONG_IS_INT64
__inline long long __CRTDECL get_long_long_arg(
    va_list *pargptr
)
{
    return va_arg(*pargptr, long long);
}
#endif

#if     _INTEGRAL_MAX_BITS >= 64    /*IFSTRIP=IGN*/

__inline __int64 __CRTDECL get_int64_arg(
    va_list *pargptr
)
{
    return va_arg(*pargptr, __int64);
}

#endif

/***
*short get_short_arg(va_list *pargptr)
*
*Purpose:
*   Gets a short argument off the given argument list and updates *pargptr.
*   *** CURRENTLY ONLY USED TO GET A WCHAR_T, IFDEF _INTL ***
*
*Entry:
*   va_list *pargptr - pointer to argument list; updated by function
*
*Exit:
*   Returns the short argument read from the argument list.
*
*Exceptions:
*
*******************************************************************************/


#if     !SHORT_IS_INT
__inline short __CRTDECL get_short_arg(
    va_list *pargptr
)
{
    return va_arg(*pargptr, short);
}

#endif

__inline char __CRTDECL get_char_arg(
    va_list *pargptr
)
{
    return va_arg(*pargptr, char);
}

//
////
//// Serialize a format_preprocess_block entry.
////
////
//// The serialized representation looks as follows:
////
//// [version] [serialized len] [structure fields] [parameter blocks] [format string] [null] [source file] [null] [function] [null]
////
//// pBufferIn - Supplies the output buffer. This buffer can go unused if a
////             pointer to a cached serialized representation is returned. This
////             buffer must be allocated and freed by the caller.
////
//// bufferLen - Supplies the length of the output buffer.
////
//// pBufferOut - Returns a pointer to the serialized representation. The caller
////              must not attempt to free this buffer.
////
//// Returns the length of the serialized representation. Returns zero if the
//// block was too long to serialize in the supplied buffer.
////
//int format_preprocess_block::Serialize(
//    __in_bcount(bufferLen) PCHAR pBufferIn,
//    __in int bufferLen,
//    __in PCHAR *pBufferOut
//    )
//{
//    // Not doing a fenced read for now. If the block gets updated after
//    // we sample it as NULL, we'll handle it later.
//    if (m_pSerializedBlock != NULL)
//    {
//        *pBufferOut = m_pSerializedBlock;
//        return m_serializedLen;
//    }
//
//    PCHAR pTarget;
//    int titleLen = 0;
//    int formatLen = (int) strlen(m_format) + 1;
//    int fileNameLen = (int) strlen(m_filename) + 1;
//    int functionLen = (int) strlen(m_function) + 1;
//
//    if (m_cachedTitle != NULL)
//    {
//        m_fSerializedTitle = true;
//        titleLen = (int) strlen(m_cachedTitle) + 1;
//    }
//
//    m_serializedLen =
//        (UInt16) (FPB_HEADER_SIZE +
//                  (sizeof(format_param_desc) * m_nDescInUse) +
//                  functionLen +
//                  fileNameLen +
//                  formatLen +
//                  titleLen);
//
//    if (bufferLen < m_serializedLen)
//    {
//        // The provided buffer is too small.
//        return 0;
//    }
//
//    if (!m_bTemporary)
//    {
//        pTarget = new char[m_serializedLen];
//
//        if (pTarget == NULL)
//        {
//            // Failed to allocate a buffer for the cachenew so just use
//            // the caller supplied buffer.
//            pTarget = pBufferIn;
//        }
//    }
//    else
//    {
//        // It's a temporary entry so we don't cache the serialized
//        // representation.
//        pTarget = pBufferIn;
//    }
//
//    int paramDescStart = FIELD_OFFSET(format_preprocess_block, m_formatLen);
//    int nextWriteOffset = paramDescStart;
//
//    //
//    // First write the updated block header (we just updated the length above).
//    //
//    memcpy(pTarget, this, paramDescStart);
//
//    //
//    // Next, serialize all the parameter descriptor blocks.
//    //
//    memcpy(
//        pTarget + nextWriteOffset,
//        &m_param_descs[0],
//        sizeof(m_param_descs[0]) * m_nDescInUse);
//
//    nextWriteOffset += (int) (sizeof(m_param_descs[0]) * m_nDescInUse);
//
//    //
//    // Finally, write the embedded strings.
//    //
//    memcpy(pTarget + nextWriteOffset, m_format, formatLen);
//    nextWriteOffset += formatLen;
//
//    memcpy(pTarget + nextWriteOffset, m_filename, fileNameLen);
//    nextWriteOffset += fileNameLen;
//
//    memcpy(pTarget + nextWriteOffset, m_function, functionLen);
//    nextWriteOffset += functionLen;
//
//    if (titleLen > 0)
//    {
//        memcpy(pTarget + nextWriteOffset, m_cachedTitle, titleLen);
//        nextWriteOffset += titleLen;
//    }
//
//    //
//    // Try to cache the serialized representation.
//    //
//    if (pTarget != pBufferIn)
//    {
//        if(InterlockedCompareExchangePointer((PVOID *) &m_pSerializedBlock, pTarget, 0) != 0)
//        {
//            //
//            // Someone else updated the cached representation just before
//            // us so use it instead.
//            //
//            delete [] pTarget;
//            pTarget = m_pSerializedBlock;
//        }
//    }
//
//    *pBufferOut = pTarget;
//    return m_serializedLen;
//}


//
// Unserialize the format block from its binary representation.
//
// pBufferIn [in] - Supplies the serialized representation of the format
//                  block.
//
// bufferSize [in] - Supplies the number of bytes available in the buffer.
//
// Returns:
//      The number of bytes consumed from the input buffer on success.
//      Returns zero on failure.
//
ULONG format_preprocess_block::Unserialize(
    __in_bcount(bufferSize) const char *pBufferIn,
    __in ULONG bufferSize
)
{
    // Copy the header.
    CopyMemory(this, pBufferIn, FPB_HEADER_SIZE);

    if ((m_fpbMarker != FORMAT_PREPROCESS_BLOCK_MARKER) ||
        (bufferSize < m_serializedLen))
    {
        DEBUG_BREAK;
        return 0;
    }

    int descLen = (sizeof(format_param_desc) * m_nDescInUse);
    int stringsLen = m_serializedLen - (FPB_HEADER_SIZE + descLen);

    if (stringsLen < 0)
    {
        DEBUG_BREAK;
        return 0;
    }

    // Allocate storage for the strings. We'll reuse the serialized block
    // field which gets freed on deletion.
    m_pSerializedBlock = new char[stringsLen];

    if (m_pSerializedBlock == NULL)
    {
        DEBUG_BREAK;
        return 0;
    }

    // Copy the descriptors which are serialized just after the header.
    CopyMemory(&m_param_descs[0], pBufferIn + FPB_HEADER_SIZE, descLen);

    // Copy the embedded strings just after the block fields.
    CopyMemory(
        m_pSerializedBlock,
        pBufferIn + FPB_HEADER_SIZE + descLen,
        stringsLen);

    // Initialize the remaining fields, including the embedded string
    // pointers.
    m_format = (PCHAR)(m_pSerializedBlock);
    m_formatLen = (UINT16)strlen(m_format);

    m_filename = m_format + m_formatLen + 1;
    UINT16 filenameLen = (UINT16)strlen(m_filename);

    m_function = m_filename + (filenameLen + 1);

    if (m_fSerializedTitle)
    {
        UINT16 functionLen = (UINT16)strlen(m_function);
        m_cachedTitle = m_function + (functionLen + 1);
    }

    m_freeCachedTitle = false;

    return m_serializedLen;
}

//
////
//// Serialize a LogEntry as a byte stream.
////
//// pBufferIn [in] - Supplies the output buffer.
////
//// bufferLen [in] - Supplies the length of the output buffer.
////
//// pLastActivityId [in] - Supplies the last activity ID that was output to
////                        the current log file. If it is the same as the
////                        activity ID of this entry we can avoid emitting it
////                        again and just insert a back reference to the
////                        "previous activity ID".
////
//// pLastEntryId [in] - Supplies the last entry ID that was output to
////                     the current log file. If it is the same as the
////                     entry ID of this entry we can avoid emitting it
////                     again and just insert a back reference to the "previous
////                     entry ID".
////
//// Returns the length of the serialized representation. Returns zero if the
//// block was too long to serialize in the supplied buffer.
////
//int LogEntry::Serialize(
//    __in_bcount(bufferLen) PCHAR pBufferIn,
//    __in int bufferLen,
//    __in const GUID *pLastActivityId,
//    __in const GUID *pLastEntryId,
//    __in FILETIME lastTimestamp,
//    __in DWORD lastThreadId
//    )
//{
//    // First, leave space for the LogEntry fields.
//    int nextWriteOffset = LOG_ENTRY_HEADER_SIZE;
//
//    if (m_pPreprocessBlock == NULL)
//    {
//        // Can't serialize without a preprocess block.
//        return 0;
//    }
//
//    m_backReferenceMask = 0;
//
//    bool haveActivityId = false;
//    bool haveEntryId = false;
//    bool haveTitle = false;
//    bool reuseLastActivityId = false;
//    bool reuseLastEntryId = false;
//    bool titleInPreprocessBlock = false;
//    bool reuseLastTimestamp = false;
//    bool reuseLastThreadId = false;
//
//    if ((bufferLen - nextWriteOffset) < (2 * sizeof(GUID)))
//    {
//        // The buffer is too short.
//        return 0;
//    }
//
//    // TODO: Avoid writing this if the previous log entry had the
//    // same activity or entry ID.
//    // Write the activity ID if one is present.
//    if (memcmp(&m_ActivityId, &__nullguid, sizeof(GUID)) != 0)
//    {
//        if (memcmp(pLastActivityId, &m_ActivityId, sizeof(GUID)) == 0)
//        {
//            reuseLastActivityId = true;
//        }
//        else
//        {
//            haveActivityId = true;
//            memcpy(pBufferIn + nextWriteOffset, &m_ActivityId, sizeof(GUID));
//            nextWriteOffset += sizeof(GUID);
//        }
//    }
//
//    // Write the entrypointID if one is present.
//    if (memcmp(&m_EntryPointId, &__nullguid, sizeof(GUID)) != 0)
//    {
//        if (memcmp(pLastEntryId, &m_EntryPointId, sizeof(GUID)) == 0)
//        {
//            reuseLastEntryId = true;
//        }
//        else
//        {
//            haveEntryId = true;
//            memcpy(pBufferIn + nextWriteOffset, &m_EntryPointId, sizeof(GUID));
//            nextWriteOffset += sizeof(GUID);
//        }
//    }
//
//    // Write the timestamp if it has changed since the last entry was output.
//    if (memcmp(&lastTimestamp, &m_timestamp, sizeof(lastTimestamp)) == 0)
//    {
//        reuseLastTimestamp = true;
//    }
//    else
//    {
//        memcpy(pBufferIn + nextWriteOffset, &m_timestamp, sizeof(m_timestamp));
//        nextWriteOffset += sizeof(m_timestamp);
//    }
//
//    // Write the thread ID if it has changed since the last entry was output.
//    if (lastThreadId == m_ThreadId)
//    {
//        reuseLastThreadId = true;
//    }
//    else
//    {
//        memcpy(pBufferIn + nextWriteOffset, &m_ThreadId, sizeof(m_ThreadId));
//        nextWriteOffset += sizeof(m_ThreadId);
//    }
//
//
//    // Write the title string if it is non-empty.
//    if ((m_title != NULL) && (m_title[0] != '\0'))
//    {
//        if ((m_pPreprocessBlock->m_fSerializedTitle) &&
//            (strcmp(m_title, m_pPreprocessBlock->m_cachedTitle) == 0))
//        {
//            // The title was output in the preprocess block and it
//            // hasn't changed, so there is no need to output it again.
//            titleInPreprocessBlock = true;
//        }
//        else
//        {
//            int titleLen = (int) (strlen(m_title) + 1);
//
//            if ((bufferLen - nextWriteOffset) < titleLen)
//            {
//                return 0;
//            }
//
//            haveTitle = true;
//            memcpy(pBufferIn + nextWriteOffset, m_title, titleLen);
//            nextWriteOffset += titleLen;
//        }
//    }
//
//
//    // Make sure we have enough to copy the arguments block
//    if ((bufferLen - nextWriteOffset) < (int)(sizeof(m_argsSize) + m_argsSize))
//    {
//        return 0;
//    }
//
//    // Copy the size of the arguments block
//    memcpy(pBufferIn + nextWriteOffset, &m_argsSize, sizeof(m_argsSize));
//    nextWriteOffset += sizeof(m_argsSize);
//
//    // Copy the arguments block
//    memcpy(pBufferIn + nextWriteOffset, m_args, m_argsSize);
//    nextWriteOffset += m_argsSize;
//
//    //
//    // Update and write the entry header.
//    // The marker identifies the start of the log entry and also whether
//    // entry has embedded activityID/entryPointID GUIDs, timestamp and
//    // thread ID.
//    //
//    LogEntryMarker marker;
//    marker.m_asInt16 = 0;
//    marker.s1.m_haveActivityId = haveActivityId;
//    marker.s1.m_haveEntryPointId = haveEntryId;
//    marker.s1.m_haveTitle = haveTitle;
//    marker.s1.m_titleInPreprocessBlock = titleInPreprocessBlock;
//    marker.s1.m_useLastActivityId = reuseLastActivityId;
//    marker.s1.m_useLastEntryPointId = reuseLastEntryId;
//    marker.s1.m_useLastTimestamp = reuseLastTimestamp;
//    marker.s1.m_useLastThreadId = reuseLastThreadId;
//    marker.s1.m_pad = LOG_ENTRY_MARKER_BYTE;
//
//    m_logEntryMarker = marker;
//    m_serializedLen = (UInt16) nextWriteOffset;
//
//    memcpy(pBufferIn, this, LOG_ENTRY_HEADER_SIZE);
//
//    return m_serializedLen;
//}

//
// Unserialize the LogEntry fields from the given input stream.
// This does not unserialize the deep-copied structures that follow
// the LogEntry fields. We need a two step approach to unserializing
// so that we can locate the correct format block after parsing the
// base entry.
//
// pSourceBuffer [in] - Supplies the buffer holding the serialized representation
//                      of the log entry.
//
// sourceBufferSize [in] - Supplies the number of bytes in the serialized buffer.
//
// pLastActivityId [in] - Activity ID of the last log entry that was parsed
//                        from the same binary stream. If the current entry
//                        header makes a back reference to the "previous
//                        activity ID" then this value is used.
//
// pLastEntryID [in] - Entry ID of the last log entry that was parsed
//                     from the same binary stream. If the current entry
//                     header makes a back reference to the "previous
//                     entry ID" then this value is used.
//
// pBytesRead [out] - Returns the number of bytes consumed from the serialized
//                    buffer.
//
// pTargetBuffer [in] - Supplies the buffer where the log entry will be unpacked.
//
// targetBufferSize [in] -  Supplies the total size of the log entry that was
//                          allocated by the caller, including space for deep
//                          copied parameters.
//
// targetBufferUsed [in] - Returns the size of the log entry that was consumed.
//
// Return Value:
//     Pointer to half-filled log entry. The log entry has the header, title
//     and source info but the parameters have not yet been deserialized.
//     UnserializeParameters must be invoked to fully initialize the log
//     entry.
//
LogEntry *LogEntry::UnserializeEntryBase(
    __in UINT16 logEntryVersion,
    __in const char *pSourceBuffer,
    __in ULONG sourceBufferSize,
    __in const GUID *pLastActivityId,
    __in const GUID *pLastEntryId,
    __in FILETIME lastTimestamp,
    __in DWORD lastThreadId,
    __out PULONG pBytesRead,
    __in_bcount(targetBufferSize) char *pTargetBuffer,
    __in INT targetBufferSize
)
{
    (*pBytesRead) = 0;

    if (sourceBufferSize < LOG_ENTRY_HEADER_SIZE)
    {
        DEBUG_BREAK;
        return NULL;
    }

    LogEntry UNALIGNED *pHeader = (LogEntry *)pSourceBuffer;
    LogEntryMarker marker = pHeader->m_logEntryMarker;

    if (marker.s1.m_pad != LOG_ENTRY_MARKER_BYTE)
    {
        DEBUG_BREAK;
        return NULL;
    }

    bool haveActivityId = !!(marker.s1.m_haveActivityId);
    bool haveEntrypointId = !!(marker.s1.m_haveEntryPointId);
    bool useLastActivityId = !!(marker.s1.m_useLastActivityId);
    bool useLastEntryId = !!(marker.s1.m_useLastEntryPointId);
    bool useLastTimestamp = !!(marker.s1.m_useLastTimestamp);
    bool useLastThreadId = !!(marker.s1.m_useLastThreadId);

    ULONG baseLength =
        LOG_ENTRY_HEADER_SIZE +
        (sizeof(GUID) * haveActivityId) +
        (sizeof(GUID) * haveEntrypointId);

    if ((sourceBufferSize < baseLength) ||
        (pHeader->m_serializedLen < LOG_ENTRY_HEADER_SIZE))
    {
        DEBUG_BREAK;
        return NULL;
    }

    ZeroMemory(pTargetBuffer, targetBufferSize);
    LogEntry *pLogEntry = (LogEntry *)pTargetBuffer;

    // Copy the header.
    CopyMemory(pTargetBuffer, pHeader, LOG_ENTRY_HEADER_SIZE);

    ULONG nextReadOffset = LOG_ENTRY_HEADER_SIZE;

    //
    // Copy the activity and entry point ID guids if they are present.
    //
    if (haveActivityId)
    {
        CopyMemory(&pLogEntry->m_ActivityId, pSourceBuffer + nextReadOffset, sizeof(GUID));
        nextReadOffset += sizeof(GUID);
    }
    else if (useLastActivityId)
    {
        pLogEntry->m_ActivityId = *pLastActivityId;
    }

    if (logEntryVersion >= 3)
    {
        // In V3, EntryId is a subset of ActivityId.
        LOGGER_ACTIVITY_ID * pLoggerActivityId = (LOGGER_ACTIVITY_ID *)&pLogEntry->m_ActivityId;

        if (pLoggerActivityId->ChildSourceId != 0)
        {
            LOGGER_ACTIVITY_ID * pLoggerEntryId = (LOGGER_ACTIVITY_ID *)&pLogEntry->m_EntryPointId;

            *pLoggerEntryId = *pLoggerActivityId;
            pLoggerEntryId->ChildSourceId = 0;
            pLoggerEntryId->ChildSequenceNumber = 0;
        }
    }
    else
    {
        if (haveEntrypointId)
        {
            CopyMemory(&pLogEntry->m_EntryPointId, pSourceBuffer + nextReadOffset, sizeof(GUID));
            nextReadOffset += sizeof(GUID);
        }
        else if (useLastEntryId)
        {
            pLogEntry->m_EntryPointId = *pLastEntryId;
        }
    }
    if (useLastTimestamp)
    {
        pLogEntry->m_timestamp = lastTimestamp;
    }
    else
    {
        CopyMemory(&pLogEntry->m_timestamp, pSourceBuffer + nextReadOffset, sizeof(pLogEntry->m_timestamp));
        nextReadOffset += sizeof(pLogEntry->m_timestamp);
    }

    if (useLastThreadId)
    {
        pLogEntry->m_ThreadId = lastThreadId;
    }
    else
    {
        CopyMemory(&pLogEntry->m_ThreadId, pSourceBuffer + nextReadOffset, sizeof(pLogEntry->m_ThreadId));
        nextReadOffset += sizeof(pLogEntry->m_ThreadId);
    }

    *pBytesRead = nextReadOffset;

    return pLogEntry;
}

//
// Unserialize the deep copied parameter list for the log entry
//
// pBufferIn [in] - Supplies the buffer holding the serialized representation
//                  of the log entry.
//
// bufferSize [in] - Supplies the number of bytes in the serialized buffer.
//
// baseFileOffset [in] - Supplies the starting offset in the file where
//                       this log entry was serialized. V1 only.
//
// entryLen [in] - The total number of bytes available in the entry. This
//                 minus entryBytesUsed is what is left for the deep copied
//                 parameters.
//
// pBytesRead [out] - Returns the number of bytes consumed from the serialized
//                    buffer.
//
// logEntryVersion [in] - Supplies the log entry version (1 or 2).
//
// paramMap [in] - A map to store the offset to string mapping of parameter
//                 strings that were deserialized. Also used to look up
//                 subsequent backreferences to the same strings. V1 only.
//
// Return Value:
//      Returns NO_ERROR if the deep copied parameter list was successfully
//      initialized. Returns appropriate Win32 error code on failure.
//
DWORD LogEntry::UnserializeParameters(
    __in_bcount(bufferSize) const char *pReadBuffer,
    __in ULONG bufferSize,
    __in ULONG baseFileOffset,
    __in ULONG entryLen,
    __out PULONG pBytesRead,
    __in UINT16 logEntryVersion,
    __in LogParameterReverseMap& paramMap
)
{
    *pBytesRead = 0;
    const char * title = NULL;
    size_t titleLen = 0;

    bool haveTitle = !!(m_logEntryMarker.s1.m_haveTitle);

    ULONG nextReadOffset = 0;

    // Update the title from the preprocess block if necessary.
    if ((m_logEntryMarker.s1.m_titleInPreprocessBlock) &&
        (m_pPreprocessBlock->m_cachedTitle != NULL))
    {
        m_title = m_pPreprocessBlock->m_cachedTitle;
    }
    else if (haveTitle)
    {
        // The title has been serialized with the entry. Get a pointer to it but
        // don't copy till after we deserialize the arguments.
        size_t remainder = (bufferSize - nextReadOffset);

        titleLen = strnlen(pReadBuffer + nextReadOffset, remainder);
        if (titleLen == remainder)
        {
            // The Title is not NULL terminated. Read buffer overflow.
            DEBUG_BREAK;
            return ERROR_INVALID_DATA;
        }

        title = pReadBuffer + nextReadOffset;

        nextReadOffset += (ULONG)(titleLen + 1);
    }
    else
    {
        m_title = "";
    }

    if (logEntryVersion == 1)
    {
        //  This is a dead code path.  See common in UnserializeParametersV1().
        ULONG fixedArgsSize = m_pPreprocessBlock->m_nDescInUse * sizeof(m_args[0]);
        ULONG nextWriteOffset = sizeof(LogEntry) + fixedArgsSize;

        if (title != NULL)
        {
            if ((titleLen + 1 + nextWriteOffset) > entryLen)
            {
                // The write buffer is not large enough. The serializedLen
                // was wrong.
                DEBUG_BREAK;
                return ERROR_INVALID_DATA;
            }

            m_title = (char *)this + nextWriteOffset;

            CopyMemory((PVOID)m_title, title, titleLen + 1);

            nextWriteOffset += (ULONG)(titleLen + 1);
        }


        DWORD bytesReadV1;

        DWORD err = UnserializeParametersV1(
            pReadBuffer + nextReadOffset,
            bufferSize - nextReadOffset,
            baseFileOffset + nextReadOffset,
            nextWriteOffset,
            entryLen,
            &bytesReadV1,
            paramMap);
        if (err != NO_ERROR)
        {
            DEBUG_BREAK;
            return err;
        }

        nextReadOffset += bytesReadV1;
    }
    else
    {
        // Coming next is the args block size
        if (bufferSize - nextReadOffset < sizeof(DWORD))
        {
            DEBUG_BREAK;
            return ERROR_INVALID_DATA;
        }

        m_argsSize = *((DWORD *)(pReadBuffer + nextReadOffset));
        nextReadOffset += sizeof(DWORD);

        // Check source
        if (bufferSize - nextReadOffset < m_argsSize)
        {
            DEBUG_BREAK;
            return ERROR_INVALID_DATA;
        }

        CopyMemory(m_args, pReadBuffer + nextReadOffset, m_argsSize);
        nextReadOffset += m_argsSize;

        FixArgsOffsets();

        // If there was a title copy it at the end.
        if (title != NULL)
        {
            ULONG nextWriteOffset = sizeof(LogEntry) + m_argsSize;

            if ((titleLen + 1 + nextWriteOffset) > entryLen)
            {
                // The write buffer is not large enough. The serializedLen
                // was wrong.
                DEBUG_BREAK;
                return ERROR_INVALID_DATA;
            }

            m_title = (char *)this + nextWriteOffset;

            CopyMemory((PVOID)m_title, title, titleLen + 1);
        }
    }

    *pBytesRead = nextReadOffset;

    return NO_ERROR;
}

//
// Unserialize the deep copied parameter list for the V1 log entry
//
// pBufferIn [in] - Supplies the buffer holding the serialized representation
//                  of the log entry.
//
// bufferSize [in] - Supplies the number of bytes in the serialized buffer.
//
// baseFileOffset [in] - Supplies the starting offset in the file where
//                       this log entry was serialized.
//
//
// entryBufferUsed [in] - Supplies the number of bytes in the entry that have
//                       been used. This is the offset where we will start
//                       placing the deep-copied parameters.
//
// entryLen [in] - The total number of bytes available in the entry. This
//                 minus entryBytesUsed is what is left for the deep copied
//                 parameters.
//
// pBytesRead [out] - Returns the number of bytes consumed from the serialized
//                    buffer.
//
// paramMap [in] - A map to store the offset to string mapping of parameter
//                 strings that were deserialized. Also used to look up
//                 subsequent backreferences to the same strings.
//
// Return Value:
//      Returns NO_ERROR if the deep copied parameter list was successfully
//      initialized. Returns appropriate Win32 error code on failure.
//
DWORD LogEntry::UnserializeParametersV1(
    __in_bcount(bufferSize) const char *pReadBuffer,
    __in ULONG bufferSize,
    __in ULONG baseFileOffset,
    __in ULONG entryBufferUsed,
    __in ULONG entryLen,
    __out PULONG pBytesRead,
    __in LogParameterReverseMap& paramMap
)
{
    *pBytesRead = 0;

    size_t bytesRead = 0;

    CHAR *pWriteBuffer = (CHAR *) this;

    if (m_pPreprocessBlock == NULL)
    {
        return ERROR_INVALID_PARAMETER;
    }

    //
    // If the log entry is truncated it may be missing a null character.
    // If so then we must explicitly add one.
    //
    bool addAnsiNull = false;
    bool addWideNull = false;

    // Now start copying the stack parameters one at a time.
    for (UInt32 i = 0; i < m_pPreprocessBlock->m_nDescInUse; ++i)
    {
        param p = { 0 };

        // Bytes to copy from either the input stream or the parameter map.
        size_t bytesToCopy = 0;

        // Bytes to advance the read pointer. This is the same as bytesToCopy
        // unless we are reading from the map and not the input stream.
        size_t bytesToAdvance = 0;

        // Target location to copy the parameter.
        void *pDestAddress = NULL;

        // Push a pointer to the deep copied parameter on the stack.
        // true for non-integral types.
        bool pushPtr = false;

        // Offset at which a back referenced string was copied in the file.
        LogParameterOffsetType backReferenceOffset;

        // true if the parameter value is being populated from the map and
        // not from the input stream.
        bool readingFromMap = false;

        // true if we run out of write buffer
        bool writeBufferExceeded = false;

        // Source address to copy the parameter value. Can be a pointer to
        // a location in the source stream or in the parameter map.
        const char *pReadLocation = pReadBuffer + bytesRead;

        format_param_desc *pCurDesc = &m_pPreprocessBlock->m_param_descs[i];
        UInt32 Ordinal = pCurDesc->m_stackType;

        switch (Ordinal)
        {
        case SE_PSTR://         0x01
        case SE_PSTR_STATIC: // 0x0E
        {
            if (((1ULL << i) & m_backReferenceMask) != 0)
            {
                //
                // Back-reference to previous string.
                //
                if ((bufferSize - bytesRead) < sizeof(backReferenceOffset))
                {
                    // We don't have enough bytes to parse the back
                    // reference. This can happen if the log entry
                    // was truncated while it was being written. Just
                    // add a marker string in place of the actual
                    // string, which is now lost.
                    pReadLocation = __truncatedstring;
                    bytesToCopy = strlen(__truncatedstring) + 1;
                    bytesToAdvance = (bufferSize - bytesRead);
                    readingFromMap = true;
                    pushPtr = true;
                    break;
                }

                memcpy(&backReferenceOffset, pReadLocation, sizeof(backReferenceOffset));

                LogParameterReverseMap::const_iterator it = paramMap.find(backReferenceOffset);

                if (it == paramMap.end())
                {
                    // Back reference to a non-existent string likely
                    // due to the log entry being terminated. Fill in
                    // a marker string instead.
                    pReadLocation = __truncatedstring;
                    bytesToCopy = strlen(__truncatedstring) + 1;
                }
                else
                {
                    pReadLocation = it->second.c_str();
                    bytesToCopy = it->second.length() + 1;
                }

                bytesToAdvance = sizeof(backReferenceOffset);
                pushPtr = true;
                readingFromMap = true;
            }
            else
            {
                //
                // The string was copied inline.
                //
                pushPtr = true;
                bytesToCopy =
                    strnlen(pReadLocation, bufferSize - bytesRead);

                if (bytesToCopy == (bufferSize - bytesRead))
                {
                    addAnsiNull = true;
                }

                if (bytesToCopy > sizeof(UINT32))
                {
                    //
                    // Cache the string in the given map so we can
                    // satisfy any future back-references.
                    //
                    backReferenceOffset = (ULONG)(baseFileOffset + bytesRead);
                    std::string s(pReadLocation);
                    paramMap[backReferenceOffset] = s;
                }

                ++bytesToCopy; // For the NULL
            }

            break;
        }

        case SE_PWSTR://        0x02
        {
            pushPtr = true;
            size_t maxLen = (((bufferSize - bytesRead) / 2) * 2);

            bytesToCopy =
                wcsnlen((PWCHAR)pReadLocation, maxLen) * sizeof(WCHAR);

            if (bytesToCopy == maxLen)
            {
                addWideNull = true;
            }

            bytesToCopy += sizeof(WCHAR); // NULL
            break;
        }

        case SE_CHAR://         0x03
        {
            pDestAddress = &p.c;
            bytesToCopy = sizeof(p.c);
            break;
        }

        case SE_WCHAR://        0x04
        {
            pDestAddress = &p.wc;
            bytesToCopy = sizeof(p.wc);
            break;
        }

        case SE_INT16://        0x05
        {
            pDestAddress = &p.s;
            bytesToCopy = sizeof(p.s);
            break;
        }

        case SE_INT32://        0x06
        {
            pDestAddress = &p.n;
            bytesToCopy = sizeof(p.n);
            break;
        }

        case SE_TIMEINTERVAL:// 0x0D
        case SE_TIMESTAMP://    0x0C
        case SE_INT64://        0x07
        {
            pDestAddress = &p.i64;
            bytesToCopy = sizeof(p.i64);
            break;
        }

        case SE_DOUBLE://       0x08
        {
            pDestAddress = &p.d;
            bytesToCopy = sizeof(p.d);
            break;
        }

        case SE_GUID_BRACE: // 0x09
        case SE_GUID://        0x0A
        {
            pushPtr = true;
            bytesToCopy = sizeof(GUID);
            break;

        }

        case SE_SOCKADDR://    0x0B
        {
            pushPtr = true;
            bytesToCopy = sizeof(GUID);
            break;
        }

        case SE_PERCENT: // 0x0E
        {
            // just skip over the percent if not a valid format specifier
            // make sure we don't consume a stack entry
            continue;
        }
        }

        if ((!readingFromMap) && ((bufferSize - bytesRead) < bytesToCopy))
        {
            // The log entry looks truncated. Just copy what we have.
            bytesToCopy = (bufferSize - bytesRead);
        }

        if (!pushPtr)
        {
            // Copy directly on the stack without using the deep copy
            // buffer space.
            CopyMemory(pDestAddress, pReadLocation, bytesToCopy);
        }
        else
        {
            if ((entryLen - entryBufferUsed) < bytesToCopy)
            {
                // Out of space in the write buffer. Limit what we copy.

                if (!readingFromMap)
                {
                    bytesToAdvance = bytesToCopy;
                }

                bytesToCopy = (entryLen - entryBufferUsed);
                writeBufferExceeded = true;
            }

            if (bytesToCopy > 0)
            {
                PCHAR writeLocation = pWriteBuffer + entryBufferUsed;

                // Copy beyond the fixed arguments block and push a pointer to the location.
                CopyMemory(writeLocation, pReadLocation, bytesToCopy);
                p.pv = writeLocation;
                entryBufferUsed += (ULONG)bytesToCopy;
            }
        }

        m_args[i] = p;

        if (readingFromMap || writeBufferExceeded)
        {
            // The parameter was not copied from the input stream, so
            // bytesRead is not the same as bytesToCopy.
            bytesRead += bytesToAdvance;
        }
        else
        {
            bytesRead += bytesToCopy;
        }
    }

    if (bytesRead > 0)
    {
        if (addAnsiNull)
        {
            pWriteBuffer[entryBufferUsed - 1] = '\0';
        }
        else if (addWideNull)
        {
            *((PWCHAR)(&pWriteBuffer[entryBufferUsed - sizeof(WCHAR)])) = L'\0';
        }
    }

    *pBytesRead = (int)bytesRead;

    return NO_ERROR;
}

