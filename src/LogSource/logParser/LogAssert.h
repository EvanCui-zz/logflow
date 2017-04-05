#pragma once

#include "logging.h"

#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>

//Disable conditional expr constant warning
#pragma warning(disable : 4127)

#undef assert
#undef Assert
#undef ASSERT

#define assert DONT_USE_BARE_ASSERT_USE_LogAssert
#define Assert assert
#define ASSERT assert


static char title[256] = "Assert failed:";

// LogAssertV() invoked from the LogAssert macro. We need to define these methods
// explicitly to tell prefast they never return.
PREFAST_NO_EXIT
void LogAssertV(const char *file, const char *function, const int line, LogID logID, const char *title);

PREFAST_NO_EXIT
void LogAssertV(const char *file, const char *function, const int line, LogID logID, const char *title, const char *fmt, ...);

// Write the assert to the verbose log explicitly, otherwise the assert will not make it in the verbose log.
#define LogAssert(exp, ...) \
    //do { \
    //    if (!(exp)) { \
    //        GetBaseLogger()->SetCrashing(); \
    //        strcat_s((char *)title, sizeof(title), #exp); \
    //        GetBaseLogger()->LogM(NULL, __FILE__, __FUNCTION__, __LINE__, LogID_Common, LogLevel_Debug, title, __VA_ARGS__); \
    //        LogAssertV(__FILE__, __FUNCTION__, __LINE__, LogID_Common, title, __VA_ARGS__); \
    //    } \
    //} while (0)


// Assert without taking a dump.
#define LogAssertNoDump(exp, ...) \
    //do { \
    //    if (!(exp)) { \
    //        if (IsDebuggerPresent()) \
    //        { \
    //            DebugBreak(); \
    //        } \
    //        GetBaseLogger()->SetCrashing(); \
    //        strcat_s((char *)title, sizeof(title), #exp); \
    //        GetBaseLogger()->LogM(NULL, __FILE__, __FUNCTION__, __LINE__, LogID_Common, LogLevel_Error, title, __VA_ARGS__); \
    //        GetBaseLogger()->LogM(NULL, __FILE__, __FUNCTION__, __LINE__, LogID_Common, LogLevel_Status, title, __VA_ARGS__); \
    //        GetBaseLogger()->Flush(true); \
    //        TerminateProcess(GetCurrentProcess(), 1); \
    //    } \
    //} while (0)


// we let prefast think that DebugLogAssert is always enabled
#if ( defined( DEBUG ) && !(defined( _PREFAST_ ) ) )

// NO-OP in RETAIL!
#define DebugLogAssert LogAssert

#else


// NO-OP in RETAIL!
#define DebugLogAssert(exp)

#endif

