#include <windows.h>
#include <stdio.h>
#include "types.h"
#include "logging.h"
#ifdef t-xijia
#include "interlocked.h"

#include "circmemorybuffer.h"
#endif
#include "MemLogEntry.h"
#include "MemoryLog.h"
#include "MemLogSprintf.h"
#include "binarylogger.h"

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

//int LogEntry::GetBytesNeeded(
//    __in format_preprocess_block *pPreprocessBlock,
//    __in va_list argptr)
//{
//    // The bytes needed is the sum of the object size and the size of the
//    // variable argument list. Start with the object size.
//    int objectSize = sizeof(LogEntry);
//
//    // Determine va_list size.
//    int argsSize = 0;
//    BOOL hasVarSizeArgs = false;
//
//    // By default each argument gets 64 bits but some types will need more.
//    argsSize = pPreprocessBlock->m_nDescInUse * sizeof(param);
//
//    for(int i = 0; i < pPreprocessBlock->m_nDescInUse; i++)
//    {
//        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
//
//        switch (pCurDesc->m_stackType)
//        {
//            case SE_PSTR://         0x01
//            case SE_PSTR_STATIC://  0x0E
//            case SE_PWSTR://        0x02
//
//                hasVarSizeArgs = true;
//                break;
//
//            case SE_GUID_BRACE://  0x09
//            case SE_GUID://        0x0A
//
//                argsSize += sizeof(GUID);
//                break;
//
//            case SE_SOCKADDR://    0x0B
//
//                argsSize += sizeof(SOCKADDR);
//                break;
//        }
//    }
//
//    if (hasVarSizeArgs)
//    {
//        argsSize += LogEntry::GetVarSizeArgsSize(pPreprocessBlock, argptr);
//    }
//
//    return (objectSize + argsSize);
//}
//
//int LogEntry::GetVarSizeArgsSize(
//    __in format_preprocess_block *pPreprocessBlock,
//    __in va_list argptr)
//{
//    int size = 0;
//
//    for(int i = 0; i < pPreprocessBlock->m_nDescInUse; i++)
//    {
//        format_param_desc *pCurDesc = &pPreprocessBlock->m_param_descs[i];
//
//        switch (pCurDesc->m_stackType)
//        {
//            case SE_PSTR://         0x01
//            case SE_PSTR_STATIC://  0x0E
//            {
//                char * s = (char *)get_ptr_arg(&argptr);
//                size += (s != NULL ? (strlen(s) + 1): 0);
//                break;
//            }
//
//            case SE_PWSTR://        0x02
//            {
//                wchar_t * ws = (wchar_t *)get_ptr_arg(&argptr);
//                size += (ws != NULL ? (wcslen(ws) + 1) * sizeof(wchar_t) : 0);
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
            pCurDesc->m_stackType == SE_PWSTR ||
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

