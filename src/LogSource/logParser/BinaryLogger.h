/*++

Module Name:

    BinaryLogger.cpp

Synopsis:

    Support for parsing XStore binary log files.

--*/

#pragma once 

class LogEntry;
class LogDestinationFile;
struct format_preprocess_block;

#include <map>
#include <vector>

#define FILE_READ_CHUNK_SIZE (10 * 1024 * 1024)
#define MIN_BUFFER_BYTES (2 * 64 * 1024)


#ifdef LOGGER_DEBUG_MODE
#define DEBUG_BREAK \
    { \
        if (IsDebuggerPresent()) DebugBreak(); \
    }
#else
#define DEBUG_BREAK
#endif // DEBUG_BREAK



#ifdef MIN
#undef MIN
#endif // MIN

#define MIN(a,b) ((a) <= (b) ? (a) : (b))

typedef std::map<FpbUidType, format_preprocess_block *> FpbMapType;

class BinaryLogDecodeSession
{
public:
    //
    // Compressed bytes decoded.
    //
    ULONG m_bytesDecoded;

    //
    // Number of log entries parsed.
    //
    ULONG m_logEntriesDecoded;

    //
    // Number of format blocks parsed.
    //
    ULONG m_formatBlocksDecoded;

    //
    // Map of format preprocess blocks that have been encountered in
    // the input stream so far.
    //
    FpbMapType m_fpbMap;

    HANDLE m_inFile;

	//Mark the file stream has been to EOF,but do not close the file handler
	BOOL m_reachToEnd;

    PCHAR m_readBuffer;

    //
    // Offset to get the next byte from the input buffer.
    //
    ULONG m_readBufferOffset;

    //
    // Total size of the input buffer. Fixed after allocation.
    //
    ULONG m_readBufferSize;

    //
    // Count of unconsumed bytes in the read buffer. Does not count
    // data not yet read from the input file.
    //
    ULONG m_inputBytesRemaining;

    //
    // Count of bytes that could not be parsed correctly.
    //
    ULONG m_bytesSkipped;

    //
    // true if the read buffer was allocated by us and must be freed on
    // exit. false if it was allocated by the caller.
    //
    bool m_fFreeReadBuffer;

    //
    // Stores the PID of the log entry author. Updated when we encounter
    // a new process header in the input stream.
    //
    DWORD m_pid;

    //
    // Stores the version of the log entries. Updated when we encounter
    // a new process header in the input stream.
    //
    UINT16 m_logEntryVersion;

    //
    // Caches the activity ID of the last log entry. Used to fulfill back
    // references.
    //
    GUID m_lastActivityId;

    //
    // Caches the entry ID of the last log entry. Used to fulfill back
    // references.
    //
    GUID m_lastEntryId;

    //
    // Caches the timestamp of the last log entry. Used to fulfill back
    // references.
    //
    FILETIME m_lastTimestamp;

    //
    // Caches the thread ID of the last log entry. Used to fulfill back
    // references.
    //
    DWORD m_lastThreadId;

    //
    // A map of parameter strings in the current session. Stored to
    // fulfill string back references.
    //
    LogParameterReverseMap m_paramMap;

    BinaryLogDecodeSession();

    ~BinaryLogDecodeSession();

    void MoveDataToStartOfReadBuffer();

    bool AllocateReadBuffer();

    DWORD ReadInputFile(ULONG size);

    // Ensure we have at least one log entry in the read buffer. If not,
    // then read it from the input file. It is not an error if we hit
    // EOF.
    DWORD CheckAndPageInEntry();

    UINT8 GetCurrentMarker();

    void AdvanceReadPointer(ULONG size);

    // Get a pointer to the current read location.
    PCHAR GetReadPointer(PULONG size);

    VOID AddFormatBlock(format_preprocess_block *pBlock);

    format_preprocess_block *GetFormatBlock(FpbUidType uid);

    void SetEof();

	DWORD Refresh();
};

