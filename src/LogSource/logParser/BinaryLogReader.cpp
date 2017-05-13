/*++

Module Name:

    BinaryLogReader.cpp

Synopsis:

    Reader over binary log files.

--*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "MemLogEntry.h"
#include "MemoryLog.h"
#include "MemLogSprintf.h"
#include "binarylogger.h"
#include "binarylogreader.h"

#include <iostream>

BinaryLogReader::BinaryLogReader()
{
    m_session = NULL;
    m_currentLogEntry = NULL;
    m_currentLine[0] = 0;
    m_currentEntryBuffer[0] = NULL;

    m_xpress = NULL;
    m_uncompressedBufferPos = 0;
    m_uncompressedBufferSize = 0;

    m_compactRendering = FALSE;

}


BinaryLogReader::~BinaryLogReader()
{
    CloseReader();
}


DWORD BinaryLogReader::OpenReader(
    __in LPCWSTR logPath)
{
    DWORD error = NO_ERROR;

    BinaryLogDecodeSession * session = new BinaryLogDecodeSession();

    //
    // Open the source file.
    //
    wcscpy(this->m_logPath, logPath);
    session->m_inFile =
        ::CreateFileW(
            logPath,
            GENERIC_READ,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);

    if (session->m_inFile == INVALID_HANDLE_VALUE)
    {
        error = GetLastError();
        goto Failed;
    }

    if (!session->AllocateReadBuffer())
    {
        error = ERROR_NOT_ENOUGH_MEMORY;
        goto Failed;
    }

    error = session->CheckAndPageInEntry();
    if (error != NO_ERROR)
    {
        if (error != ERROR_HANDLE_EOF)
        {
            goto Failed;
        }

        // Empty file, unlikely but not an error.
        error = NO_ERROR;
    }

    m_session = session;
    session = NULL;
    LocateLast();

Failed:

    delete session;

    return error;
}

void BinaryLogReader::LocateLast()
{
    int size = GetFileSize(m_session->m_inFile, NULL);
    DWORD bytesRead;
    char *p = new char[size];
    memset(p, 0, size);
    LPOVERLAPPED overlapped = new OVERLAPPED;
    memset(overlapped, 0, sizeof(OVERLAPPED));
    overlapped->Offset = 0;
    BOOL result =
        ReadFile(
            m_session->m_inFile,
            p,
            size,
            &bytesRead,
            overlapped);
    while (p[--size] == 0);
    m_firstSize = size++;
    delete p;
    delete overlapped;
}

VOID BinaryLogReader::CloseReader()
{
    delete m_session;
    m_session = NULL;

    m_currentLogEntry = NULL;
    m_currentLine[0] = 0;
    m_currentEntryBuffer[0] = 0;

    if (m_xpress != NULL)
    {
        delete m_xpress;
        m_xpress = NULL;
    }
}

DWORD BinaryLogReader::ReFresh()
{
    DWORD error = NO_ERROR;

    if (m_session->m_inFile == INVALID_HANDLE_VALUE)
    {
        std::cout << "ttttttttttttttttttttttttttt\n";
    }
    
    error = m_session->Refresh();

    return error;
}

double BinaryLogReader::getPersentage()
{
    return m_session->m_bytesDecoded*100.0 / m_firstSize;
}

DWORD BinaryLogReader::MoveNext()
{
    DWORD error = NO_ERROR;
    BOOLEAN entryDecoded = FALSE;
    int counter = -1;

    // Reset current line information
    m_currentLogEntry = NULL;
    *m_currentLine = 0;

    while (!entryDecoded)
    {
        ULONG len;
        ULONG bytesConsumed = 0;
        PCHAR pBuffer;
        UINT8 marker;
        ULONG baseFileOffset = m_session->m_bytesDecoded;
        BOOL advanceBuffer = FALSE;

        if (m_uncompressedBufferPos < m_uncompressedBufferSize)
        {
#ifdef MYDEBUG
            std::cout << "using way 1" << "\tm_uncompressedBufferPos:" << m_uncompressedBufferPos << "\tm_uncompressedBufferSize:" << m_uncompressedBufferSize << std::endl;
#endif
            // We have data in the uncompressed buffer. Attempt to read from
            // there.
            advanceBuffer = TRUE;
            pBuffer = m_uncompressedBuffer + m_uncompressedBufferPos;
            len = m_uncompressedBufferSize - m_uncompressedBufferPos;
            marker = *pBuffer;
        }
        else
        {
#ifdef MYDEBUG
            std::cout << "using way 2" << std::endl;
#endif
            error = m_session->CheckAndPageInEntry();

            if (error == ERROR_HANDLE_EOF)
            {
                error = ERROR_NO_MORE_ITEMS;
                break;
            }
            else if (error != NO_ERROR)
            {
                break;
            }

            // Decode either a format preprocess block or log entry
            // at the current read location.
            pBuffer = m_session->GetReadPointer(&len);
            marker = m_session->GetCurrentMarker();
            baseFileOffset = m_session->m_bytesDecoded;

            if (len == 0)
            {
                // EOF.
                error = ERROR_NO_MORE_ITEMS;
                break;
            }
        }

        counter++;
        if (marker == FORMAT_PREPROCESS_BLOCK_MARKER)
        {
            //this is the log format string 
            //may be override
#ifdef MYDEBUG
            std::cout << counter << "\t" << "FORMAT_PREPROCESS_BLOCK_MARKER" << std::endl;
#endif
            error = DecodeAndSaveFormatBlock(pBuffer, len, &bytesConsumed);
        }
        else if (marker == BINARY_LOG_PROCESS_HEADER_MARKER)
        {
            //there will be only one entry in a bin file
#ifdef MYDEBUG
            std::cout << counter << "\t" << "BINARY_LOG_PROCESS_HEADER_MARKER" << std::endl;
#endif
            bytesConsumed = VerifyBinaryLogFileHeader(pBuffer, len);

            if (bytesConsumed == 0)
            {
                DEBUG_BREAK;
                error = ERROR_INVALID_DATA;
            }
            else
            {
                error = NO_ERROR;
            }
        }
        else if (marker == COMPRESSED_LOG_BLOCK_MARKER)
        {
            //compress as a huge block
#ifdef MYDEBUG
            std::cout << counter << "\t" << "COMPRESSED_LOG_BLOCK_MARKER" << std::endl;
#endif
            error = DecompressBlock(pBuffer, len, &bytesConsumed);
        }
        else if (marker == LOG_ENTRY_MARKER_BYTE)
        {
#ifdef MYDEBUG
            std::cout << counter << "\t" << "DecodeLogEntry" << std::endl;
#endif
            error = DecodeLogEntry(pBuffer, len, baseFileOffset, &bytesConsumed);

            if (error == NO_ERROR)
            {
                entryDecoded = TRUE;
            }
        }
        else
        {
            return ERROR_NO_MORE_ITEMS;
        }

        if (error == ERROR_NOT_READY)
        {
            //compress block must be some error
            //we already have bytes_decoded
            //next time we process at the bytes_decoded ptr
            return error;
#ifdef MYDEBUG
            std::cout << "[warnning]Detect a file Appending, Please wait." << std::endl;
#endif
        }
        else if (error != NO_ERROR)
        {
            // The stream cannot be parsed. This can occur if the process
            // crashed while writing a log entry. Scan forward looking for
            // a process header marker and try to reinterpret the logs
            // from there.
            // It can also happen if the entry is corrupt most likely because of
            // a software bug. In that case we do a best effort to find the next
            // entry and resume there.
#if 0
            ULONG i;

            for (i = 1; i < len; ++i)
            {
                UCHAR c = pBuffer[i];

                if (c == BINARY_LOG_PROCESS_HEADER_MARKER ||
                    c == COMPRESSED_LOG_BLOCK_MARKER ||
                    (advanceBuffer && c == FORMAT_PREPROCESS_BLOCK_MARKER) ||
                    (advanceBuffer && c == LOG_ENTRY_MARKER_BYTE))
                {
                    // Found a potential process header, compressed block, format block or entry.
                    // Format block or entry are accepted only if we are reading from the
                    // decompressed buffer (advanceBuffer == TRUE) since they are illegal otherwise.
                    // Try to parse the stream again.
                    break;
                }
            }

            m_session->m_bytesSkipped += i;
            AdvanceReadPointer(advanceBuffer, i);
#endif
        }
        else
        {
            AdvanceReadPointer(advanceBuffer, bytesConsumed);
        }
    }

    return error;
}

ULONGLONG BinaryLogReader::GetEntryLineNumber()
{
    return m_session->m_logEntriesDecoded;
}

GUID BinaryLogReader::GetEntryActivityId()
{
    return m_currentLogEntry->m_ActivityId;
}

VOID BinaryLogReader::GetEntryTitle(
    __out_ecount(titleLen) const char ** title,
    __out ULONG * titleLen)
{
    *title = (const char *)m_currentLogEntry->m_title;
    *titleLen = (ULONG)strlen(*title);
}

ULONGLONG BinaryLogReader::GetEntryTimestamp()
{
    ULARGE_INTEGER ui;

    ui.LowPart = m_currentLogEntry->m_timestamp.dwLowDateTime;
    ui.HighPart = m_currentLogEntry->m_timestamp.dwHighDateTime;

    return ui.QuadPart;
}

// Maps log levels to characterid
// Debug,Info,Status,Warning,Error,AppAlert,Assert
static char s_logLevelMap[] = "diswexa";

char BinaryLogReader::GetLoggingLevel()
{
    return s_logLevelMap[m_currentLogEntry->m_pPreprocessBlock->m_level];
}

int BinaryLogReader::GetLoggingLevelI()
{
    return m_currentLogEntry->m_pPreprocessBlock->m_level;
}

Size_t BinaryLogReader::GetFormatDataCSharpStyle(PCHAR format, Size_t formatSize, PCHAR parameters, Size_t parametersSize, Size_t indexWidthLength[], Size_t count)
{
    count = log_entry_sprintf_csformat(format, formatSize, parameters, parametersSize, indexWidthLength, count, m_currentLogEntry);
    parameters[parametersSize - 1] = '\0';
    format[formatSize - 1] = '\0';

    return count;
}

Size_t BinaryLogReader::GetFormatDataCStyle(PCHAR buffer, Size_t bufferSize, StringToken tokens[], Size_t maxTokens)
{
    return log_entry_sprintf_cformat(buffer, bufferSize, tokens, maxTokens, m_currentLogEntry);
}

void BinaryLogReader::getFormatedText(PCHAR formattedEntry)
{
    log_entry_sprintf(formattedEntry, MAX_LOG_ENTRY_SIZE, m_currentLogEntry);
    formattedEntry[MAX_LOG_ENTRY_SIZE - 1] = '\0';
}

LPCSTR 
BinaryLogReader::GetEntrySourceFile()
{
    return m_currentLogEntry->m_pPreprocessBlock->m_filename;
}

PCSTR BinaryLogReader::getEntryFunction()
{
    return m_currentLogEntry->m_pPreprocessBlock->m_function;
}

PCSTR BinaryLogReader::getEntryTitle()
{
    return m_currentLogEntry->m_title;
}

DWORD 
BinaryLogReader::GetEntrySourceLine()
{
    return m_currentLogEntry->m_pPreprocessBlock->m_line;
}

int BinaryLogReader::getEntryTid()
{
    return m_currentLogEntry->m_ThreadId;
}

int BinaryLogReader::getEntryPid()
{
    return m_session->m_pid;
}

long long BinaryLogReader::getEntryTime()
{
    return *(long long *)&m_currentLogEntry->m_timestamp;
}

void BinaryLogReader::getEntryTS(PCHAR p)
{
    sprintf(p, "%#llx", *(unsigned long long *)&m_currentLogEntry->m_timestamp);
}

LPCSTR BinaryLogReader::GetEntryFormatted()
{
    if (*m_currentLine != 0)
    {
        // We already rendered.
        return m_currentLine;
    }

    CHAR formattedEntry[MAX_LOG_ENTRY_SIZE];
    CHAR buffer[MAX_LOG_ENTRY_SIZE];
    CHAR sourceInfo[MAX_SOURCE_INFO_SIZE];

    //
    // Render the log entry.
    //
    log_entry_sprintf(formattedEntry, sizeof(formattedEntry), m_currentLogEntry);
    formattedEntry[sizeof(formattedEntry) - 1] = '\0';

    int srcLen;
    PCHAR pTitleStart;
    PCHAR pTitleEnd;
    PCHAR pMessageStart;

    //
    // Format the entry as expected by XStore, adding source info,
    // timestamps and activity IDs.
    //
    MakeLogEntryWrapper(
        buffer, sizeof(buffer),
        sourceInfo, sizeof(sourceInfo), true,
        m_currentLogEntry->m_pPreprocessBlock->m_filename,
        m_currentLogEntry->m_pPreprocessBlock->m_function,
        m_currentLogEntry->m_pPreprocessBlock->m_line,
        (LogID)m_currentLogEntry->m_pPreprocessBlock->m_logID,
        (LogLevel)m_currentLogEntry->m_pPreprocessBlock->m_level,
        m_currentLogEntry->m_title,
        &m_currentLogEntry->m_timestamp,
        m_currentLogEntry->m_ThreadId,
        m_session->m_pid,
        &m_currentLogEntry->m_ActivityId,
        &m_currentLogEntry->m_EntryPointId,
        &srcLen, &pTitleStart, &pTitleEnd, &pMessageStart,
        LogTag_String1,
        formattedEntry,
        LogTag_End);

    // Copy the formatted line to the current line. The source file information
    // comes separate hence we need to do this extra copying.
    PCHAR currentLine = m_currentLine;
    ULONG currentLineCch = _countof(m_currentLine);

    ULONG copyLen = MIN((ULONG)(pTitleEnd - buffer), currentLineCch - 1);
    strncpy_s(m_currentLine, currentLineCch, buffer, copyLen);

    currentLine += copyLen;
    currentLineCch -= copyLen;

    copyLen = MIN((ULONG)srcLen, currentLineCch - 1);
    strncpy_s(currentLine, currentLineCch, sourceInfo, copyLen);

    currentLine += copyLen;
    currentLineCch -= copyLen;

    strncpy_s(currentLine, currentLineCch, pTitleEnd, _TRUNCATE);

    return m_currentLine;
}

void BinaryLogReader::QueryStats(
    __out UINT64 * bytesSkipped,
    __out UINT64 * bytesDecoded)
{
    *bytesSkipped = (m_session != NULL ? m_session->m_bytesSkipped : 0);
    *bytesDecoded = (m_session != NULL ? m_session->m_bytesDecoded : 0);
}

void BinaryLogReader::AdvanceReadPointer(
    __in BOOL advanceBuffer,
    __in ULONG size)
{
    if (advanceBuffer)
    {
        m_uncompressedBufferPos += size;
    }
    else
    {
        m_session->AdvanceReadPointer(size);
    }
}

//
// Parses a format block at the given memory location. If successful,
// the format block is saved in the given session.
//
// pBuffer [in] - Supplies the input buffer where the format block is located.
//
// bufferLen [in] - Supplies the number of bytes in the input buffer.
//
// pSession [in] - Supplies a pointer to the parsing session.
//
// pBytesConsumed [out] - Returns the number of bytes consumed from the
//                        input stream.
//
// Return Value:
//      Returns NO_ERROR on success, failure code otherwise.
//
DWORD
BinaryLogReader::DecodeAndSaveFormatBlock(
    __in const CHAR *pBuffer,
    __in ULONG bufferLen,
    __out PULONG pBytesConsumed
)
{
    *pBytesConsumed = 0;

    DWORD error = 1;

    format_preprocess_block *pBlock = new format_preprocess_block;

    if (pBlock == NULL)
    {
        DEBUG_BREAK;
        error = ERROR_NOT_ENOUGH_MEMORY;
        goto Failed;
    }

    *pBytesConsumed = pBlock->Unserialize(pBuffer, bufferLen);

    if (*pBytesConsumed == 0)
    {
        DEBUG_BREAK;
        delete pBlock;
        error = ERROR_INVALID_DATA;
        goto Failed;
    }

    m_session->AddFormatBlock(pBlock);

    error = NO_ERROR;

Failed:

    return error;
}


//
// Verifies whether the binary log process header at the given offset
// is well formed.
//
// pSession [in] - Supplies a pointer to the parsing session.
//
// pBuffer [in] - Supplies the input buffer where the header is located.
//
// len [in] - Supplies the number of bytes in the input buffer.
//
// Return Value:
//     Returns the number of header bytes consumed.
//     Returns zero if the header was not well formed.
//
DWORD
BinaryLogReader::VerifyBinaryLogFileHeader(
    __in_bcount(len) PCHAR pBuffer,
    __in ULONG len
)
{
    BinaryLogProcessHeader hdr;
    memcpy(&hdr, pBuffer, MIN(len, sizeof(hdr)));

    if ((len < sizeof(hdr)) || (!hdr.Verify()))
    {
        goto Failed;
    }

    m_session->m_pid = hdr.m_pid;
    m_session->m_logEntryVersion = hdr.m_logEntryVersion;

    return sizeof(hdr);

Failed:
    return 0;
}


//
// Parse a log entry at the given memory location and render it into
// text. Write the rendered text to the output stream for the current
// session. The caller must ensure that the supplied buffer is large
// enough to store a log entry. If the buffer is not large enough the
// parse will fail.
//
// pBuffer [in] - Supplies a pointer to the input stream where the entry
// is located.
//
// bufferLen [in] - Supplies the length of the input buffer.
//
// baseFileOffset [in] - Supplies the offset in the input file where the
//                       log entry starts. This is used to generate
//                       back reference offsets.
//
// pSession [in] - Supplies a pointer to the parse session.
//
// pBytesConsumed [out] - Returns the number of bytes consumed from the
//                        input buffer.
//
// Returns:
//      NO_ERROR on success, failure code otherwise.
//
DWORD
BinaryLogReader::DecodeLogEntry(
    __in const CHAR *pBuffer,
    __in ULONG bufferLen,
    __in ULONG baseFileOffset,
    __out PULONG pBytesConsumed
)
{
    *pBytesConsumed = 0;

    DWORD error = 1;
    ULONG bytesConsumed1 = 0, bytesConsumed2 = 0;

    LogEntry *pEntry =
        LogEntry::UnserializeEntryBase(
            m_session->m_logEntryVersion,
            pBuffer,
            bufferLen,
            &m_session->m_lastActivityId,
            &m_session->m_lastEntryId,
            m_session->m_lastTimestamp,
            m_session->m_lastThreadId,
            &bytesConsumed1,
            m_currentEntryBuffer,
            sizeof(m_currentEntryBuffer));

    if (pEntry == NULL)
    {
        DEBUG_BREAK;
        error = ERROR_INVALID_DATA;
        goto Failed;
    }

    m_session->m_lastActivityId = pEntry->m_ActivityId;
    m_session->m_lastEntryId = pEntry->m_EntryPointId;
    m_session->m_lastTimestamp = pEntry->m_timestamp;
    m_session->m_lastThreadId = pEntry->m_ThreadId;

    pEntry->m_pPreprocessBlock =
        m_session->GetFormatBlock(pEntry->m_preprocessBlockUid);

    if (pEntry->m_pPreprocessBlock == NULL)
    {
        DEBUG_BREAK;
        error = ERROR_INVALID_DATA;
        goto Failed;
    }

    //
    // Now that we have located the preprocess block, unserialize
    // the LogEntry parameters that were deep-copied off the call
    // stack.
    //
    //JWesker impossible to use baseFileOffset
    error =
        pEntry->UnserializeParameters(
            pBuffer + bytesConsumed1,
            pEntry->m_serializedLen - bytesConsumed1,
            baseFileOffset + bytesConsumed1,
            sizeof(m_currentEntryBuffer),
            &bytesConsumed2,
            m_session->m_logEntryVersion,
            m_session->m_paramMap);

    if (error != NO_ERROR)
    {
        DEBUG_BREAK;
        goto Failed;
    }

    ++m_session->m_logEntriesDecoded;
#ifdef MYDEBUG
    std::cout << *pBytesConsumed << "\t" << bytesConsumed1 << "\t" << bytesConsumed2 << std::endl;
#endif
    *pBytesConsumed = (bytesConsumed1 + bytesConsumed2);

    m_currentLogEntry = pEntry;

Failed:
    return error;
}

//
// Wrapper function to setup the parameters for the call to MakeLogEntry
// in a va_arg structure and invoke MakeLogEntry to format the log entry.
//
// Return value:
//      Returns a pointer to the last byte of the formatted log entry.
//
PCHAR
BinaryLogReader::MakeLogEntryWrapper(
    __in_ecount(bufferLen) char *buffer,
    __in const int bufferLen,
    __in_ecount(srcBufferLen) char *sourceInfo,
    __in const int srcBufferLen,
    __in bool fNeedSourceInfo,
    __in PCSTR file,
    __in PCSTR function,
    __in const int line,
    __in LogID logID,
    __in LogLevel level,
    __in PCSTR title,
    __in FILETIME *ft_in,
    __in DWORD tid,
    __in DWORD pid,
    __in const GUID *pActivityId,
    __in const GUID *pEntryPointId,
    __out int *pSrcLen,
    __out PCHAR *ppTitleStart,
    __out PCHAR *ppTitleEnd,
    __out PCHAR *ppMessageStart,
    __in LogTag tag,
    ...
)
{
    va_list args;
    va_start(args, tag);

    char *pBufferEnd =
        Logger::MakeLogEntry(
            buffer, bufferLen,
            sourceInfo, srcBufferLen, true,
            file,
            function,
            line,
            logID,
            level,
            title,
            tag,
            args,
            ft_in,
            tid,
            pid,
            pActivityId,
            pEntryPointId,
            !!m_compactRendering,
            pSrcLen, ppTitleStart, ppTitleEnd, ppMessageStart);

    va_end(args);
    return pBufferEnd;
}


UINT XPressDecompress(
    __in XPressWrapper * xpress,
    __in_bcount(compressedSize) BYTE * compressed,
    __in INT compressedSize,
    __in_bcount(maxOriginalSize) BYTE * original,
    __in INT maxOriginalSize)
{
    __try
    {
        // XPress stops with INT 3 if it encounters an error. That behavior can be changed at
        // build time but not at runtime. Since there are many users of XPRESS we prefer
        // not to change the lib but do this hack instead.

        // Invoke decompressor
        return xpress->Decompress(compressed, compressedSize, original, maxOriginalSize);
    }
    __except (GetExceptionCode() == EXCEPTION_BREAKPOINT ?
        EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
        return 0;
    }
    return 0;
}


DWORD
BinaryLogReader::DecompressBlock(
    __in const CHAR * pBuffer,
    __in ULONG bufferLen,
    __out PULONG pBytesConsumed)
{
    DWORD error = NO_ERROR;
    UINT originalSize;


    COMPRESSED_LOG_BLOCK_HEADER UNALIGNED * header =
        (COMPRESSED_LOG_BLOCK_HEADER *)pBuffer;

    *pBytesConsumed = 0;
    if (bufferLen < sizeof(*header))
    {
        error = ERROR_INVALID_DATA;
        goto exit;
    }

    if (header->m_compressionLevel < 1 || header->m_compressionLevel > 9 ||
        header->m_originalSize < header->m_compressedSize)
    {
        error = ERROR_INVALID_DATA;
        goto exit;
    }

    // 	//JWesker search for 30¸öÈ«0
    // #define ZEROLEN 30
    // 	UINT idx = sizeof(*header);
    // 	UINT counter;
    // 	while(idx<bufferLen&&idx<header->m_compressedSize)
    // 	{
    // 		if(pBuffer[idx]!=0x0)
    // 			idx++;
    // 		else
    // 		{
    // 			counter=1;
    // 			idx++;
    // 			while (counter<ZEROLEN&&pBuffer[idx]==0x0)
    // 			{
    // 				counter++;
    // 				idx++;
    // 			}
    // 			if (counter>=ZEROLEN)
    // 			{
    // 				error = ERROR_NOT_READY;
    // 				goto exit;
    // 			}
    // 		}
    // 	}
    // #undef ZEROLEN

    if (m_xpress == NULL)
    {
        m_xpress = new XPressWrapper();
    }

    if (header->m_flags & CLB_FLAG_RESET)
    {
        m_xpress->Terminate();
        m_xpress->Initialize(FALSE, TRUE, header->m_compressionLevel);
    }

    m_compactRendering = (header->m_flags & CLB_FLAG_COMPACT_RENDERING);

    if (header->m_originalSize == header->m_compressedSize)
    {
#ifdef MYDEBUG
        std::cout << "+++++ this is a uncompressed context" << std::endl;
#endif
        // The block was not compressed.
        memcpy(m_uncompressedBuffer, pBuffer + sizeof(*header), header->m_originalSize);
        originalSize = header->m_originalSize;
    }
    else
    {
        // Invoke decompressor.
        originalSize = XPressDecompress(
            m_xpress,
            (BYTE *)pBuffer + sizeof(*header),
            header->m_compressedSize,
            (BYTE *)m_uncompressedBuffer,
            min(header->m_originalSize, c_maxBufferSize));
    }

    if (originalSize == 0 || originalSize != header->m_originalSize)
    {
        error = ERROR_INVALID_DATA;
        goto exit;
    }

    m_uncompressedBufferSize = originalSize;
    m_uncompressedBufferPos = 0;
    *pBytesConsumed = sizeof(*header) + header->m_compressedSize;

exit:

    return error;
}
