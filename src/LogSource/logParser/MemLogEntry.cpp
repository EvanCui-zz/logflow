
#include <windows.h>
#include <stdio.h>
#include "types.h"
#include "logging.h"

//#include "interlocked.h"
//#include "InternalString.h"
//#include "circmemorybuffer.h"

#include "MemLogEntry.h"
#include "MemoryLog.h"
#include "MemLogSprintf.h"
#include "binarylogger.h"

extern UNICODE_STRING_NT __counted_unicode_nullstring;
extern ANSI_STRING_NT __counted_ansi_nullstring;

void BinaryLogProcessHeader::Init()
{
    m_marker = BINARY_LOG_PROCESS_HEADER_MARKER;
    m_byteOrderCheck = BINARY_LOG_BYTE_ORDER_CHECK;
    m_formatBlockVersion = FORMAT_PREPROCESS_BLOCK_VER;
    m_logEntryVersion = LOG_ENTRY_VERSION;
    m_paramDescVersion = FORMAT_PARAM_DESC_VER;
    m_paramDescSerializedLen = sizeof(format_param_desc);
    m_pid = GetCurrentProcessId();
}

bool BinaryLogProcessHeader::Verify()
{
    return
        (m_marker == BINARY_LOG_PROCESS_HEADER_MARKER) &&
        (m_byteOrderCheck == BINARY_LOG_BYTE_ORDER_CHECK) &&
        (m_formatBlockVersion <= FORMAT_PREPROCESS_BLOCK_VER) &&
        (m_logEntryVersion <= LOG_ENTRY_VERSION) &&
        (m_paramDescVersion <= FORMAT_PARAM_DESC_VER);
}
//
//int LogEntry::GetBytesNeeded (
//    __in format_preprocess_block *pPreprocessBlock,
//    __inout int *varArgSizes,
//    __in UInt16 varArgSizeArrayCount,
//    __in va_list argptr)
//{
//    // Determine va_list size.
//    int varArgsSize = 0;
//
//    if (pPreprocessBlock->m_uNumVarSizedArgs > 0)
//    {
//        varArgsSize = LogEntry::GetVarSizeArgsSize (pPreprocessBlock, varArgSizes, varArgSizeArrayCount, argptr);
//    }
//
//    // The bytes needed is the sum of the object size and the size of the
//    // variable argument list. Start with the object size.
//    return (sizeof(LogEntry) + pPreprocessBlock->m_uFixedArgsSize + varArgsSize);
//}
//
//int LogEntry::GetVarSizeArgsSize (
//    __in format_preprocess_block *pPreprocessBlock,
//    __inout int *varArgSizes,
//    __in UInt16 varArgSizeArrayCount,
//    __in va_list argptr)
//{
//    // This logentry has variable sized arguments, finc out their sizes.
//    // It is common that the variable sized arguments are at the beginning of a log entry, so exploit that as well.
//    int size = 0, argSize;
//    int varSizedArgsSeen = 0;
//
//    for (int i = 0; i < pPreprocessBlock->m_nDescInUse && varSizedArgsSeen < pPreprocessBlock->m_uNumVarSizedArgs; i++)
//    {
//        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
//
//        switch (pCurDesc->m_stackType)
//        {
//            case SE_PSTR://         0x01
//            case SE_PSTR_STATIC://  0x0E
//            {
//                char * s = (char *)get_ptr_arg(&argptr);
//                argSize = (s != NULL ? (strlen(s) + 1): 1);
//                size += argSize;
//                if (varSizedArgsSeen < varArgSizeArrayCount)
//                {
//                    varArgSizes[varSizedArgsSeen] = argSize;
//                }
//                varSizedArgsSeen++;
//                break;
//            }
//
//            case SE_PSTR_INTERNAL://  0x12
//            {
//                char * s = (char *)get_ptr_arg(&argptr);
//                argSize = (s != NULL ? (InternalizedStringHeader::GetInternalizedStringLength (s) + 1) : 1);
//                size += argSize;
//                if (varSizedArgsSeen < varArgSizeArrayCount)
//                {
//                    varArgSizes[varSizedArgsSeen] = argSize;
//                }
//                varSizedArgsSeen++;
//                break;
//            }
//
//            case SE_PWSTR://        0x02
//            {
//                wchar_t * ws = (wchar_t *)get_ptr_arg(&argptr);
//                argSize = (ws != NULL ? (wcslen(ws) + 1) * sizeof(wchar_t) : sizeof(wchar_t));
//                size += argSize;
//                if (varSizedArgsSeen < varArgSizeArrayCount)
//                {
//                    varArgSizes[varSizedArgsSeen] = argSize;
//                }
//                varSizedArgsSeen++;
//                break;
//            }
//
//            // In these paths, the var-arg always points to either a
//            // UNICODE_STRING or ANSI_STRING (i.e. never a counted_string
//            // type).  Note that we are cumputing the size required to
//            // store a serialized counted_string.
//            case SE_COUNTED_STR://  0x10
//            {
//                ANSI_STRING_NT * s = (ANSI_STRING_NT *)get_ptr_arg(&argptr);
//
//                if (!ValidAnsiString(s))
//                {
//                    s = &__counted_ansi_nullstring;
//                }
//
//                argSize = sizeof(counted_string) + s->Length;
//                size += argSize;
//                if (varSizedArgsSeen < varArgSizeArrayCount)
//                {
//                    varArgSizes[varSizedArgsSeen] = argSize;
//                }
//                varSizedArgsSeen++;
//                break;
//            }
//
//            case SE_COUNTED_WSTR:// 0x11
//            {
//                UNICODE_STRING_NT * s = (UNICODE_STRING_NT *)get_ptr_arg(&argptr);
//
//                if (!ValidUnicodeString(s))
//                {
//                    s = &__counted_unicode_nullstring;
//                }
//
//                argSize = sizeof(counted_string) + s->Length;
//                size += argSize;
//                if (varSizedArgsSeen < varArgSizeArrayCount)
//                {
//                    varArgSizes[varSizedArgsSeen] = argSize;
//                }
//                varSizedArgsSeen++;
//                break;
//            }
//
//            case SE_CHAR://         0x03
//
//                get_char_arg(&argptr);
//                break;
//
//            case SE_WCHAR://        0x04
//            case SE_INT16://        0x05
//
//                get_short_arg(&argptr);
//                break;
//
//            case SE_INT32://        0x06
//
//                get_int_arg(&argptr);
//                break;
//
//            case SE_INT64://        0x07
//            case SE_TIMESTAMP://    0x0C
//            case SE_TIMEINTERVAL:// 0x0D
//
//                get_long_long_arg(&argptr);
//                break;
//
//            case SE_DOUBLE://       0x08
//            {
//                va_arg(argptr, double);
//                break;
//            }
//
//            case SE_GUID_BRACE://  0x09
//            case SE_GUID://        0x0A
//            {
//                va_arg(argptr, GUID);
//                break;
//            }
//
//            case SE_SOCKADDR://    0x0B
//            {
//                va_arg(argptr, SOCKADDR);
//                break;
//            }
//        }
//    }
//
//    return size;
//}

void LogEntry::FixArgsOffsets()
{
    for (int i = 0; i < m_pPreprocessBlock->m_nDescInUse; i++)
    {
        format_param_desc *pCurDesc = &m_pPreprocessBlock->m_param_descs[i];

        if (pCurDesc->m_stackType == SE_PSTR ||
            pCurDesc->m_stackType == SE_PSTR_STATIC ||
            pCurDesc->m_stackType == SE_PSTR_INTERNAL ||
            pCurDesc->m_stackType == SE_PWSTR ||
            pCurDesc->m_stackType == SE_COUNTED_STR ||
            pCurDesc->m_stackType == SE_COUNTED_WSTR ||
            pCurDesc->m_stackType == SE_GUID_BRACE ||
            pCurDesc->m_stackType == SE_GUID ||
            pCurDesc->m_stackType == SE_SOCKADDR)
        {
            if (m_args[i].u64 != 0)
            {
                // Replace offset with actual pointer to data.
                m_args[i].pv = ((char *)m_args + m_args[i].u64);
            }
        }
    }
}

