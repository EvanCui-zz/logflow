
#pragma once

// types of stack entries
#define SE_PSTR         0x01
#define SE_PWSTR        0x02
#define SE_CHAR         0x03
#define SE_WCHAR        0x04
#define SE_INT16        0x05
#define SE_INT32        0x06
#define SE_INT64        0x07
#define SE_DOUBLE       0x08
#define SE_GUID_BRACE   0x09
#define SE_GUID         0x0A
#define SE_SOCKADDR     0x0B
#define SE_TIMESTAMP    0x0C
#define SE_TIMEINTERVAL 0x0D
#define SE_PSTR_STATIC  0x0E
#define SE_PERCENT      0x0F
#define SE_COUNTED_STR      0x10
#define SE_COUNTED_WSTR     0x11
#define SE_PSTR_INTERNAL    0x12

__inline int __CRTDECL get_int_arg(va_list *pargptr);

#if !SHORT_IS_INT
__inline short __CRTDECL get_short_arg(va_list *pargptr);

__inline char __CRTDECL get_char_arg (va_list *pargptr);
#endif

#if     !LONG_IS_INT
__inline long __CRTDECL get_long_arg(va_list *pargptr);
#endif

#if     !LONGLONG_IS_INT64
__inline long long __CRTDECL get_long_long_arg(va_list *pargptr);
#endif

#if     _INTEGRAL_MAX_BITS >= 64    /*IFSTRIP=IGN*/
__inline __int64 __CRTDECL get_int64_arg(va_list *pargptr);
#endif

#ifndef _INTEGRAL_MAX_BITS
#define _INTEGRAL_MAX_BITS  64
#endif

// the following should be set depending on the sizes of various types
#define LONG_IS_INT      1      // 1 means long is same size as int
#define SHORT_IS_INT     0      // 1 means short is same size as int
#define LONGDOUBLE_IS_DOUBLE 1  // 1 means long double is same as double
#define LONGLONG_IS_INT64 1     // 1 means long long is same as int64
#if     defined (_WIN64)
#define PTR_IS_INT       0      // 1 means ptr is same size as int
#define PTR_IS_LONG      0      // 1 means ptr is same size as long
#define PTR_IS_INT64     1      // 1 means ptr is same size as int64
#else
#define PTR_IS_INT       1      // 1 means ptr is same size as int
#define PTR_IS_LONG      1      // 1 means ptr is same size as long
#define PTR_IS_INT64     0      // 1 means ptr is same size as int64
#endif

#if     LONGLONG_IS_INT64
    #define get_long_long_arg(x) (long long)get_int64_arg(x)
#endif

#if     LONG_IS_INT
    #define get_long_arg(x) (long)get_int_arg(x)
#endif

#ifndef _UNICODE
#if SHORT_IS_INT
    #define get_short_arg(x) (short)get_int_arg(x)
#endif
#endif

#if     PTR_IS_INT
    #define get_ptr_arg(x) (void *)(intptr_t)get_int_arg(x)
#elif   PTR_IS_LONG
    #define get_ptr_arg(x) (void *)(intptr_t)get_long_arg(x)
#elif   PTR_IS_INT64
    #define get_ptr_arg(x) (void *)get_int64_arg(x)
#else
    #error Size of pointer must be same as size of int or long
#endif

#define _MAX_PARAMS 64

#define FORMAT_PARAM_DESC_VER                       1
#define FORMAT_PREPROCESS_BLOCK_VER                 1
#define FORMAT_PREPROCESS_BLOCK_MARKER              0xAA

__declspec(selectany) char *__truncatedstring = "(truncated)";  /* string to print when a parameter cannot be output */
__declspec(selectany) wchar_t *__wtruncatedstring = L"(truncated)";  /* string to print when a parameter cannot be output */
__declspec(selectany) char *__nullstring = "(null)";  /* string to print on null ptr */
__declspec(selectany) wchar_t *__wnullstring = L"(null)";  /* string to print on null ptr */
__declspec(selectany) GUID __nullguid = {0};
__declspec(selectany) SOCKADDR __nullsockaddr = {0};

//
// Describes a format descriptor.
// Serialized to disk and hence versioned. The version is in the containing
// format_preprocess_block.
//
struct format_param_desc
{
    UInt16  m_flags;            // flags see FL_... in MemLogSprintf.cpp
    UInt16  m_offset;           // offset in format string of this parameter descriptor
    UInt8   m_len;              // length of this parameter descriptor including %
    UInt8   m_stackType;        // type of stack parameter (SE_PSTR, SE_INT32, ...)
    Int8    m_precision;        // format precision
    UInt8   m_radix;            // for integer types, the radix of the output format
    UInt8   m_fldwidth;         // field width
    char    m_prefix[2];        // prefix, like ("0x,x")
    UInt8   m_prefixlen;        // length of prefix
    UInt8   m_capex;            // if floating point and set, capital "E" exponent, else "e"
    UInt8   m_hexadd;           // value to add to hex digits
};


//
// Describes the format of a log entry. The structure is serialized to
// disk.
//
struct format_preprocess_block
{
    const UINT8 m_fpbMarker;        // This must be the first field. When parsing the log it indicates the start of a new FPB.
    UINT16      m_serializedLen;    // The size of the structure when serialized to disk. Variable.

    FpbUidType  m_blockUid;         // A unique identifier assigned to each new block.

    int         m_line;
    UINT16      m_logID;
    UINT16      m_level;            // This is a 'LogLevel' enum type defined in logging.h, and NOT the XLOG_LEVEL passed
                                    // into the Xlog() APIs and defined in XLog.h (see XLogpStreamMap[]).

    UInt16      m_nDescInUse;       // number of parameter descriptors in use
    bool        m_fSerializedTitle; // Whether the cached title was serialized to disk. A Race condition makes it possible that the cached
                                    // title is updated after the block has been serialized.
    //
    // Add any fields that must be serialized to disk before this point.
    //


    // Any fields defined after m_param_descs will not be serialized to disk directly.

    UInt16       m_formatLen;       // length of format string, excluding NULL
    bool         m_bTemporary;      // this preprocess block is a temporary allocation, free after logging
    INT64        m_lastEmitEpoch[c_logMaxBinaryLogDestinations];   // The logging epoch in which this block was last output.
    PCHAR        m_pSerializedBlock;// A cached serialized representation of the block.
    PCSTR        m_format;          // pointer to the format string
    PCSTR        m_filename;
    PCSTR        m_function;
    PCSTR        m_cachedTitle;     // Optimization to avoid emitting the title with each entry if it doesn't change.
    bool         m_freeCachedTitle; // Whether the cached title must be explicitly freed.
    UInt16       m_nDesc;           // number of parameter descriptors in the structure

    //
    // Add any fields that must not be serialized to disk after this point.
    //
    UInt16      m_uNumVarSizedArgs; // number of variable sized arguments (strings)
    UInt16      m_uFixedArgsSize;   // total size of fixed size arguments (that can be determined before hand)

    //
    // This must be the last data field.
    //
    __declspec(align(8)) format_param_desc m_param_descs[_MAX_PARAMS];

public:
    format_preprocess_block() :
        m_fpbMarker(FORMAT_PREPROCESS_BLOCK_MARKER)
    {

        m_filename   = NULL;
        m_function   = NULL;
        m_line       = NULL;
        m_logID      = (UINT16) LogID_Logging;
        m_level      = (UINT16) LogLevel_Debug;
        m_formatLen  = 0;
        m_nDesc      = _MAX_PARAMS;
        m_nDescInUse = 0;
        m_bTemporary = false;
        m_pSerializedBlock = NULL;
        m_serializedLen    = 0;
        m_cachedTitle      = NULL;
        m_fSerializedTitle = false;
        m_freeCachedTitle  = false;

        memset(m_lastEmitEpoch, 0, sizeof(m_lastEmitEpoch));
    }

    void Init(const char *filename, const char *function, const int line,LogID logID, LogLevel level, bool bTemporary, FpbUidType blockUid)
    {
        m_filename   = filename;
        m_function   = function;
        m_line       = line;
        m_logID      = (UINT16) logID;
        m_level      = (UINT16) level;
        m_bTemporary = bTemporary;
        m_blockUid   = blockUid;
    }

    ////
    //// One time to decide the total size of fixed arguments and the number of
    //// variable sized arguments
    ////
    //void PrecomputeArgsSize (void)
    //{
    //    int fixedArgsSize = 0;
    //    int numVarSizedArgs = 0;
    //
    //    // By default each argument gets 64 bits but some types will need more.
    //    fixedArgsSize = m_nDescInUse * sizeof(param);
    //
    //    for (int i = 0; i < m_nDescInUse; i++)
    //    {
    //        format_param_desc *pCurDesc = &(m_param_descs[i]);
    //
    //        switch (pCurDesc->m_stackType)
    //        {
    //            case SE_PSTR://         0x01
    //            case SE_PSTR_STATIC://  0x0E
    //            case SE_PWSTR://        0x02
    //            case SE_COUNTED_STR://  0x10
    //            case SE_COUNTED_WSTR:// 0x11
    //            case SE_PSTR_INTERNAL:// 0x12
    //
    //                numVarSizedArgs++;
    //                break;
    //         
    //            case SE_GUID_BRACE://  0x09
    //            case SE_GUID://        0x0A

    //                fixedArgsSize += sizeof(GUID);
    //                break;

    //            case SE_SOCKADDR://    0x0B

    //                fixedArgsSize += sizeof(SOCKADDR);
    //                break;
    //        }
    //    }

    //    m_uFixedArgsSize = (UInt16) fixedArgsSize;
    //    m_uNumVarSizedArgs = (UInt16) numVarSizedArgs;
    //}

    //
    // Cache the log entry title in the format block. This is an optimization
    // for the common case when the log entry title is a static string and can
    // be emitted once with the format block instead of repeatedly with each
    // entry.
    //
    VOID CacheTitle(PCSTR pTitle)
    {
        if (!m_bTemporary && (m_cachedTitle == NULL) && (pTitle != NULL) && (pTitle[0] != '\0'))
        {
            PCHAR pCached = _strdup(pTitle);

            if (pCached != NULL)
            {
                if (InterlockedCompareExchangePointer((PVOID *) &m_cachedTitle, pCached, NULL) != NULL)
                {
                    free(pCached);
                }
                else
                {
                    m_freeCachedTitle = true;
                }
            }
        }
    }

    ~format_preprocess_block()
    {
        if (m_pSerializedBlock != NULL)
        {
            delete [] m_pSerializedBlock;
        }

        if ((m_cachedTitle != NULL) && (m_freeCachedTitle))
        {
            free((PVOID)m_cachedTitle);
        }
    }

    int Serialize(__in_bcount(bufferLen) PCHAR pBufferIn, int bufferLen, PCHAR *pBufferOut);
    ULONG Unserialize(__in_bcount(bufferSize) const char *pBufferIn, ULONG bufferSize);
    bool IsTemporary()  { return m_bTemporary; }

private:
    // Deny copying and assignment.
    format_preprocess_block(const format_preprocess_block&);
    format_preprocess_block& operator=(const format_preprocess_block&);

};

C_ASSERT(FIELD_OFFSET(format_preprocess_block, m_fpbMarker) == 0);
C_ASSERT(FIELD_OFFSET(format_preprocess_block, m_blockUid) == 4);

#define FPB_HEADER_SIZE (FIELD_OFFSET(format_preprocess_block, m_formatLen))

Size_t __CRTDECL log_entry_sprintf(
    char *out,
    Size_t  cbOut,
    LogEntry *pLogEntry
    );

struct StringToken;
Size_t __CRTDECL log_entry_sprintf_cformat(
    PCHAR buffer, 
    Size_t bufferSize, 
    StringToken tokens[], 
    Size_t maxTokens,
	LogEntry* pLogEntry);

Size_t __CRTDECL log_entry_sprintf_csformat(
	PCHAR format,
	Size_t cbFormatOut,
	PCHAR out,
	Size_t cbOut,
	Size_t indexWidthLength[],
	Size_t parameterMaxCount,
	LogEntry* pLogEntry);

format_preprocess_block * __CRTDECL preprocess_format(
    const char *format,
    format_preprocess_block *pFormatPreprocessBlock=NULL
    );

LogEntry * __CRTDECL WriteLogEntry(
    PVOID pBuffer,
    int bufferSize,
    format_preprocess_block *pPreprocessBlock,
    const char *title,
    const GUID * activityId,
    FILETIME ft,
    int *varArgSizes,
    UInt16 varArgSizeArrayCount,
    va_list argptr);

// fill in the log entry without copying the strings, can only be used if log_entry_sprintf is called immediately after while
// the string pointers are guaranteed to continue to be valid.
LogEntry * __CRTDECL WriteLogEntryStatic(
    PVOID pBuffer,
    int bufferSize,
    format_preprocess_block *pPreprocessBlock,
    va_list argptr);

