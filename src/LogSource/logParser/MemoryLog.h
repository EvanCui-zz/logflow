#pragma once 
#include "xpresswrapper.h"

// a big chunk of memory for logging

class LogEntry;
class InlineCompressor;


#define COMPRESSED_LOG_BLOCK_MARKER     0xCC

#define CLB_FLAG_RESET                  0x01        // Reset decompressor.
#define CLB_FLAG_COMPACT_RENDERING      0x02        // Compact entry rendering (no logid, title, ...).

// Invalid binary epoch number.
#define INVALID_EPOCH                   -1LL

typedef struct _COMPRESSED_LOG_BLOCK_HEADER
{
    BYTE m_marker;
    BYTE m_compressionLevel;
    UINT32 m_flags;
    UINT32 m_originalSize;
    UINT32 m_compressedSize;
} COMPRESSED_LOG_BLOCK_HEADER;

class MemoryLog
{
public:
        static const Int32 c_cbOutputBuffer = 8192;
        static const Int32 c_cbBinOutputBuffer = 9 * 1024; // extra 1K for the preprocess block

        MemoryLog::MemoryLog()
        {
            m_hThreadLogOutput = INVALID_HANDLE_VALUE;
            m_LogOutputThreadId = 0;
            m_flushRequestedOffset = 0;
            m_logBuffer = NULL;
            m_logBufferIndexMask = 0;
            m_logBufferAvailableBytes = 0;
            m_logBufferWriteOffset = 0;
            m_logBufferReadOffset = 0;
            m_bufferFullLostCount = 0;
            m_invalidEntryLostCount = 0;
            m_destinationFullLostCount = 0;
            m_pid = 0;
        }

        bool Init(UInt32 bufferSize);

        void Flush();

        inline bool IsOnLogOutputThread()
        {
            return (::GetCurrentThreadId () == m_LogOutputThreadId);
        }

        void AddLostEntries(
            __in int bufferFullCount,
            __in int invalidEntryCount,
            __in int destinationFullCount);

        PVOID ReserveMemLogBuffer(
            __in UINT32 bytesNeeded,
            __out UINT32 * pBytesReserved);

        __inline void CommitMemLogBuffer(
            __in PVOID pAddress,
            __in UINT64 size)
        {
            InterlockedExchange64((INT64 *)pAddress, size);
        }

public:

        // Buffer management
        static const int c_LogBufferBlockSize = 1 << 7; // 128 bytes
        static const int c_MaxReserveBytes = 1 << 13; // 8K, should be multiple of block size

        BYTE * m_logBuffer;
        UINT64 m_logBufferIndexMask;
        INT64 m_logBufferAvailableBytes;
        UINT64 m_logBufferWriteOffset;
        UINT64 m_logBufferReadOffset;

        UINT64 m_bufferFullLostCount;
        UINT64 m_invalidEntryLostCount;
        UINT64 m_destinationFullLostCount;

        UINT64 m_flushRequestedOffset;

        CRITICAL_SECTION m_cs;
        HANDLE m_hFlushDoneEvent;

        char     *m_pOutputBuffer;

        HANDLE    m_hThreadLogOutput;
        DWORD     m_LogOutputThreadId;
        DWORD     m_pid;

public:
        void LogOutputThreadInternal();

private:

        static  DWORD WINAPI LogOutputThread(void *pvMemoryLog);
        bool StartLogOutputThread();

        __inline PVOID GetLogBufferPtr(
            __in UINT64 offset)
        {
            return (BYTE *)m_logBuffer + (offset & m_logBufferIndexMask);
        }

        void ReleaseCurrentMemLogBuffer();

        void DumpMemoryLogCounters(UINT64 qpc);
};


class InlineCompressor
{
private:

        static const int c_bufferSize = XPRESS_MAX_BLOCK;

public:

        InlineCompressor(MemoryLog * memoryLog, int compressionLevel)
        {
            m_memoryLog = memoryLog;
            m_compressionLevel = compressionLevel;
            m_blocksCompressed = 0;
            m_buffer = NULL;
            m_compressedBuffer = NULL;
            m_bufferPos = 0;
            m_isBufferReserved = false;
            m_reserveEpoch = -1;
            m_entryCount = 0;
            m_minTimestamp = 0;
            m_maxTimestamp = 0;
        }

        void Init();

        void Reset();

        bool ReserveBytes(
            __in UINT destinationIndex,
            __in UINT32 bytesNeeded,
            __out INT64 * pEpoch);

        void Flush();

        bool LogBinaryInEpoch(
            __in_bcount(formatBlockLen) const PVOID pFormatBlockBuffer,
            __in INT formatBlockLen,
            __in_bcount(entryLen) const PVOID pEntryBuffer,
            __in INT entryLen,
            __in INT64 epoch,
            __in UINT64 minTimestamp,
            __in UINT64 maxTimestamp);

private:
        // The parent memory log object
        MemoryLog * m_memoryLog;

        // XPRESS compression level to use.
        int         m_compressionLevel;

        // XPRESS object wrapper.
        XPressWrapper m_xpress;

        // Indicates how many blocks we have compressed with the current encoder
        // context.
        UINT        m_blocksCompressed;

        // Buffer where log entries are accumulated. When the buffer is full,
        // its content is compressed and written to the underlying log destination.
        BYTE *      m_buffer;

        // The current position within the buffer, indicating how much it's been
        // filled so far.
        UINT        m_bufferPos;

        // Tracks the number of entries currently in the buffer.
        INT         m_entryCount;

        // Buffer where we store compressed data. The data is immediately written
        // to the underlying log destination.
        BYTE *      m_compressedBuffer;

        // Indicates whether we have space reserved in the underlying log
        // destination and the epoch that space is reserved into. Unless we can
        // reserve space downstream we can't take any data. That is because the
        // caller needs to know the epoch information in order to serialize correctly.
        bool        m_isBufferReserved;
        INT64       m_reserveEpoch;

        // The undelying log destination index where we write the compressed entries.
        UINT        m_destinationIndex;

        // Minimum and maximum timestamps across entries in m_buffer.
        UINT64      m_minTimestamp;
        UINT64      m_maxTimestamp;
};

//
// Class that maintains the binary state associated with a binary log file.
//
class BinarySerializationContext
{
public:

    BinarySerializationContext(
        __in int logDestinationIndex,
        __in char * preprocessOutputBuffer,
        __in char * binaryOutputBuffer,
        __in InlineCompressor * inlineCompressor)
    {
        m_logDestinationIndex = logDestinationIndex;
        m_pPreprocessOutputBuffer = preprocessOutputBuffer;
        m_pBinaryOutputBuffer = binaryOutputBuffer;
        m_inlineCompressor = inlineCompressor;

        m_currentEpoch = INVALID_EPOCH;
        ZeroMemory(&m_lastActivityId, sizeof(m_lastActivityId));
        ZeroMemory(&m_lastEntryId, sizeof(m_lastEntryId));
        ZeroMemory(&m_lastTimestamp, sizeof(m_lastTimestamp));
        m_lastThreadId = 0;
    }

public:

    // The index that identifies this file within the set of all log destinations.
    int                 m_logDestinationIndex;

    // Incremental number that identifies the current binary serialization epoch. A new epoch
    // corresponds to a new file.
    INT64               m_currentEpoch;

    // The ActivityId, EntryId, Timestamp and ThreadId of the last entry written to the binary
    // file. If unchanged, the new entry will just set a corresponding flag instead of duplicating.
    // These values are reset when the epoch changes (i.e. a new file is started).
    GUID                m_lastActivityId;
    GUID                m_lastEntryId;
    FILETIME            m_lastTimestamp;
    DWORD               m_lastThreadId;

    // Intermediate buffers.
    char *              m_pBinaryOutputBuffer;
    char *              m_pPreprocessOutputBuffer;

    // XPress compressor wrapper to optionally compress log entries.
    InlineCompressor *  m_inlineCompressor;
};
