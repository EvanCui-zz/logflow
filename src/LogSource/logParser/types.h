#pragma once
#include "indexutil.h"
#include "float.h"

// Autopilot code that should be deprecated because it has dangerous ambigous overloads
#define APDEPRECATED_UNTYPED __declspec(deprecated)

// Cosmos/XStore code that should be deprecated because it has dangerous ambigous overloads
#define CSDEPRECATED_UNTYPED __declspec(deprecated)

typedef HRESULT CsError;

__forceinline CsError CsGetLastError()
{
    return HRESULT_FROM_WIN32( GetLastError() );
}

__forceinline CsError CsWSAGetLastError()
{
    return HRESULT_FROM_WIN32( WSAGetLastError() );
}

__forceinline CsError CsErrorFromWin32(DWORD err)
{
    return HRESULT_FROM_WIN32(err);
}

#if defined (ELEMENTCOUNT)
#undef ELEMENTCOUNT
#endif
#define ELEMENTCOUNT(x) (sizeof(x)/sizeof(x[0]))

typedef __int8 Int8;
typedef unsigned __int8 UInt8;
typedef __int16 Int16;
typedef unsigned __int16 UInt16;
typedef __int32 Int32;
typedef unsigned __int32 UInt32;
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
typedef size_t Size_t;

#define MAX_UINT8  ((UInt8)-1)
#define MAX_UINT16 ((UInt16)-1)
#define MAX_UINT32 ((UInt32)-1)
#define MIN_INT32  ((Int32)0x80000000)
#define MAX_INT32  ((Int32)0x7FFFFFFF)      // 2147483647
#define MAX_UINT64 ((UInt64)-1)
#define MAX_INT64  0x7FFFFFFFFFFFFFFFi64
#define MAX_FLOAT  (3.402823466e+38F)
#define MAX_SIZE_T ((size_t)((size_t)0 - (size_t)1))

#define PF_I64D "%I64d"
#define PF_I64X "%016I64x"
#define PF_I64O "%022I64o"

// A structure to represent a fixed-size chunk of
// memory
struct SIZED_STRING
{
    union
    {
        const UInt8 *pbData;
        const char  *pcData;
    };
    size_t cbData;
};

// A helper macro for defining a SIZED_STRING as part of a constant
#define INLINE_SIZED_STRING(str) { (const UInt8 *)str, sizeof(str) - 1 }

//  Structure to store a wchar_t version of a user dictionary word in memory
struct WCHAR_SIZED_STRING
{
    wchar_t* pData;
    size_t cchData;
};

// A helper macro for defining a SIZED_STRING as part of a constant
#define INLINE_WCHAR_SIZED_STRING(wstr) { wstr, sizeof(wstr)/sizeof(wstr[0]) - 1 }

// A utility class for creating temporary SIZED_STRINGs
class CStackSizedString : public SIZED_STRING
{
public:
    // Null-terminated input
    CStackSizedString(const char *szValue)
    {
        pbData = (const UInt8 *)szValue;
        cbData = strlen(szValue);
    }

    // Name/size pair
    CStackSizedString(
        const UInt8 *pbValue,
        size_t cbValue)
    {
        pbData = pbValue;
        cbData = cbValue;
    }
private:
    // prevent heap allocation
    void *operator new(size_t);
};


// A structure designed to be a self-contained index word
// The index is currently constrained to words less than
// 256 bytes in length; this makes it so clients do not
// have to ensure buffer lengths for calls to function that
// generate index words
struct INDEX_WORD_STRING
{
    UInt8 cbWord;
    UInt8 rgbWord[256];
};

///////////////////////////////////
#pragma warning(push)
#pragma warning(disable:4201)

typedef union tagFAInt64
{
	UInt64  n64;
	LONG64  i64;
    UInt8   nBytes[8];
    FILETIME  ft;

	struct
	{
		UInt32 nData;
		UInt32 nCount;
	};
	struct
	{
		Int32  i32_low;
		Int32  i32_hi;
	};
	struct
	{
		UInt32  n32_low;
		UInt32  n32_hi;
	};

	tagFAInt64():n64(0){};
    tagFAInt64( UInt64 nVal ):n64(nVal){};
    tagFAInt64( Int64  iVal ):i64(iVal){};
    tagFAInt64( UInt32 nHigh, UInt32 nLow ): n32_low(nLow), n32_hi(nHigh){};
    tagFAInt64( Int32  iHigh, Int32  iLow ): i32_low(iLow), i32_hi(iHigh){};
    tagFAInt64( DWORD  nHigh, DWORD  nLow ): n32_low(nLow), n32_hi(nHigh){};
    tagFAInt64( const FILETIME&  rftSrc ): ft(rftSrc){};

} FAInt64;  // flexible access 64 bit integer

typedef union tagFAInt32
{
	UInt32  n32;
	Int32   i32;
    UInt8   nBytes[4];
	struct
	{
		Int16  i16_low;
		Int16  i16_hi;
	};
	struct
	{
		UInt16  n16_low;
		UInt16  n16_hi;
	};
	tagFAInt32():n32(0){};
    tagFAInt32( UInt32 nVal ): n32(nVal){};
    tagFAInt32( Int32  iVal ): i32(iVal){};

} FAInt32;  // flexible access 32 bit integer
/* 
   A cosmos timestamp is defined as the number of 100-nanosecond intervals that have elapsed
   since 12:00 A.M. January 1, 1601 (UTC). It is the representation of choice whenever an
   absolute date/time must be used.
*/
typedef unsigned __int64 CsTimeStamp;

static const CsTimeStamp CsTimeStamp_Never = (CsTimeStamp)_UI64_MAX;
static const CsTimeStamp CsTimeStamp_LongAgo = (CsTimeStamp)0;

/*
   A cosmos elapsed time is defined as the number of 100-nanosecond intervals between two
   points in time. It may be negative. It is what you get when you subtract two CsTimestamp values, and is
   the representation of choice whenever a time interval needs to be represented persistently
   or in a network protocol.
*/
typedef __int64 CsTimeInterval;

static const CsTimeInterval CsTimeInterval_Infinite = (CsTimeInterval)_I64_MAX;
static const CsTimeInterval CsTimeInterval_NegativeInfinite = (CsTimeInterval)_I64_MIN;
static const CsTimeInterval CsTimeInterval_Zero = (CsTimeInterval)0;
static const CsTimeInterval CsTimeInterval_Quantum = (CsTimeInterval)1;
static const CsTimeInterval CsTimeInterval_100ns = CsTimeInterval_Quantum;
static const CsTimeInterval CsTimeInterval_Microsecond = CsTimeInterval_100ns * 10;
static const CsTimeInterval CsTimeInterval_Millisecond = CsTimeInterval_Microsecond * 1000;
static const CsTimeInterval CsTimeInterval_Second = CsTimeInterval_Millisecond * 1000;
static const CsTimeInterval CsTimeInterval_Minute = CsTimeInterval_Second * 60;
static const CsTimeInterval CsTimeInterval_Hour = CsTimeInterval_Minute * 60;
static const CsTimeInterval CsTimeInterval_Day = CsTimeInterval_Hour * 24;
static const CsTimeInterval CsTimeInterval_Week = CsTimeInterval_Day * 7;

// A CsTimeInterval_Year is defined as 52 weeks. It is for convenience, not for computing exact years.
static const CsTimeInterval CsTimeInterval_Year = CsTimeInterval_Week * 52;

/* An IPV4 IP address in host byte order */
typedef UInt32 CsIpAddress;

static const CsIpAddress CsAnyIpAddress = 0;
static const CsIpAddress CsInvalidIpAddress = 0;
static const CsIpAddress CsLocalIpAddress = 0x7F000001;  // 127.0.0.1
static const CsIpAddress CsLocalIpAddressEnd = 0x7FFAFFFF;  // 127.250.255.255


/* An IP port number in host byte order */
typedef UInt16 CsPortNumber;

static const CsPortNumber CsInvalidPortNumber = 0xFFFF;
static const CsPortNumber CsAnyPortNumber = 0;


typedef UInt16 XsUpgradeDomain;
static XsUpgradeDomain XsUpgradeDomain_Invalid = MAX_UINT16;

typedef XsUpgradeDomain CsUpgradeDomain;
static CsUpgradeDomain CsUpgradeDomain_Invalid = XsUpgradeDomain_Invalid;

typedef UInt16 XsFaultDomain;
static XsFaultDomain XsFaultDomain_Invalid = MAX_UINT16;

// TEMPLATE remove_reference
template<class _Ty>
struct remove_reference
{	// remove reference
    typedef _Ty type;
};

template<class _Ty>
struct remove_reference<_Ty&>
{	// remove reference
    typedef _Ty type;
};

template<class _Ty>
struct remove_reference<_Ty&&>
{	// remove rvalue reference
    typedef _Ty type;
};

// TEMPLATE FUNCTION move
template<class _Ty> inline
    typename remove_reference<_Ty>::type&&
    move(_Ty&& _Arg) throw ()
{	// forward _Arg as movable
    return ((typename remove_reference<_Ty>::type&&)_Arg);
}

#pragma warning(pop)
