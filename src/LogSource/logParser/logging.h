#pragma once

// For stdout and fflush
#include <stdio.h>

// For critical section
#include "shared.h"

// For LogID's
#include "logids.h"

// For FileChangeNotify
#include "changenotify.h"

// For ConfigChangeNotify Cookie
#include "config.h"

// Forward declaration.
class CounterEnumerator;
class AppAlertClient;
struct format_preprocess_block;

namespace Microsoft {
namespace Cis {
namespace Monitoring {
namespace Events {
class DiagnosticsLogger;
}}}};


#if defined( _PREFAST_ )
#define PREFAST_NO_EXIT __declspec(noreturn)
#else
#define PREFAST_NO_EXIT
#endif

// Logging
//
// To log something, call the Log(logid, level, title, ...) macro.
//
// The "LogID" enumeration is a shared enumeration across all components.   Items which
// use the same LogID should be in a related activity (e.g. everything which happens
// during an URL merge, or everything at the TCP Transport layer).  You shouldn't be
// creating a new LogID for each log statement, that is just excessive.
//
// The "title" parameter can be anything you want.  You can consider "title" and
// "fmt, ..." to be related, separating them is just a way to keep the data and non-data
// parts of a log statement separate, for log parsing purposes.  The data associated
// with a particular log statement should be in the "fmt, ..." part.
//
// For the log level, there are 3 levels; informational, warning, and error.  Error
// is something that will get propagated to operations for someone to look at, as soon
// as that infrastructure is in place.
//
// In the config.ini file, you can use rules to state what goes where (to stdout, to
// certain logging files), and whether you want source file/function/line info
// to be output.
//
// The logging config can be updated at any time and the logging system will pick up
// the changes.
//
// To see/edit the logging enumeration, see logid.h and logid.cpp
//
// Sample config file:
//

/*
[Counters]
Filename=counters.prf

[LogRules]
Rule1=*,*,*,stdout
Rule2=*,*,*,dumpster
Rule3=Crawler,*,*,crawlerstuff

[stdout]
LogSourceInfo=1

[dumpster]
FileNameBase=everything
LogSourceInfo=1
MaxFiles=10
MaxFileSize=100000
BufferSize=60000

[crawlerstuff]
FileNameBase=c:\logs\crawlerstuff
LogSourceInfo=0
MaxFiles=10
MaxFileSize=100000
BufferSize=60000
*/

typedef enum
{
    LogLevel_Debug,
    LogLevel_Info,
    LogLevel_Status,
    LogLevel_Warning,
    LogLevel_Error,
    LogLevel_AppAlert,
    LogLevel_Assert,
    LogLevel_Max
} LogLevel;


// These are the data types supported by log tags
// The data type is actually stored in the high order bits of the log tag ID
typedef enum
{
    LogTagType_None = 0,
    LogTagType_Int32,
    LogTagType_Int64,
    LogTagType_String,
    LogTagType_VarString,
    LogTagType_Float,
    LogTagType_WideString,
    LogTagType_VarWideString,
    LogTagType_Hex32,
    LogTagType_Hex64,
    LogTagType_UInt32,
    LogTagType_UInt64,
    LogTagType_Time
} LogTagType;

// True if millisecond timestamp format should be used. Defaults to second resolution.
extern bool g_fMillisecondTimestamps;

// The log level mask to be used for spewing to stderr before initialization. Defaults to everything.
extern int g_preInitializeLogLevelMask;

// Macros for dealing with log tags

// Given a log tag, retrieve the LogTagType
#define LogTagToType(tag) ((LogTagType) ((tag) >> 24))

// Given a log tag, retrieve just the index
#define LogTagToIndex(tag) ((tag) & 0x00FFFFFF)

// Given a log tag type and log tag index, create the tag value
#define MakeLogTag(index,type) (((type)<<24)+(index))

// Now we declare the log tag enumeration, by including logtagids.h, which happens to
// also be included in logging.cpp but with a different #define for DeclareTag()
#define DeclareTag(index, id, name, type) id=MakeLogTag(index,type)

// Declare the log tag enumeration
typedef enum
{
#include "logtagids.h"
} LogTag;

// Max size of a single log entry
// Anything larger than this will get truncated when written
const int c_logMaxEntrySize = 8192;

// Max number of rules
const int c_logMaxRules = 128;

// Max number of application callbacks
const int c_logMaxCallbackHandlers = 128;

// Max size for the title used in a rule
const int c_logMaxRuleTitleLength = 128;

// Can't be any higher than this, since we are using a bitmap
const int c_logMaxLogDestinations = 32;

// Max name size for a log destination (e.g. "stdout")
const int c_logMaxLogDestinationNameLength = MAX_PATH;

// Max number of binary log destinations
const int c_logMaxBinaryLogDestinations = 4;

#ifdef MAX
#undef MAX
#endif

#define MAX(a,b) (((a) >= (b)) ? (a) : (b))

// The function template for a callback an application can register as
// a handler. logString is a NULL-terminated string.
typedef void LoggerApplicationCallback(void* cookie, const char* logString);

struct LoggerApplicationCallbackInfo
{
    LoggerApplicationCallback*   m_handler;
    void*                        m_cookie;
};

// The activity state.
// This does not own the pointer.
// The ActivityState has a format defined by the class XsActivityTransferData.
// Currently, it is a count (BYTE) followed by a list of GUIDs
// We use primitive types instead of XsActivityTransferData since
// we use this in a TLS object.
typedef struct _XS_ACTIVITY_STATE
{
    BYTE* ActivityState;
    size_t ActivityStateLength;
} XS_ACTIVITY_STATE;


//
// Defines the interface to the logging system. This is the only way to invoke the
// logger. To obtain a pointer to this interface you should call GetBaseLogger().
//
class IBaseLogger
{
public:

    virtual BOOL Init(
        __in WCHAR * customSectionName) = 0;

    virtual void InitCounters() = 0;

    virtual void LogMVar(
        __in_opt void ** ppPreprocessBlock, 
        __in const char * file, 
        __in const char * function, 
        __in const int line, 
        __in LogID logID, 
        __in LogLevel level, 
        __in const char * title, 
        __in const char * fmt, 
        __in va_list args) = 0;

    virtual void LogMVar(
        __in const char * file, 
        __in const char * function, 
        __in const int line, 
        __in LogID logID, 
        __in LogLevel level, 
        __in const char * title, 
        __in LogTag tag, 
        __in va_list args) = 0;

    virtual void Flush(
        __in bool flushBuffers) = 0;

    virtual BOOL AddApplicationLogCallback(
        __in LoggerApplicationCallback * handler, 
        __in void * cookie) = 0;

    virtual BOOL EnableApplicationLogCallback() = 0;

    // Removed unused virtual function. Keep dummy entry to preserve binary compatibility.
    virtual void Dummy_1() {}

    virtual bool KillHungMiniDumpWritingProcess() = 0;

    virtual format_preprocess_block * CreatePreprocessFormat(
        __in LogID logID,
        __in LogLevel level,
        __in const char * file,
        __in const char * function,
        __in const int line,
        __in const char * title,
        __in const char * fmt) = 0;

    virtual XS_ACTIVITY_STATE GetCurrentActivityState() = 0;

    virtual void SetCurrentActivityState(
        __in_bcount_opt(activityStateLength) BYTE* activityState,
        __in size_t activityStateLength) = 0;

    virtual void SetCrashing() = 0;

    virtual bool IsCrashing() = 0;

    virtual void SetNoLostLogEntries(bool fNoLostLogEntries) = 0;

    virtual bool CanLostLogEntries() = 0;

    virtual bool SetDumpOrTerminateProcess() = 0;

    virtual bool IsNetlibDebugLogging() = 0;

    // LogM() overloads are invoked from the Log() macro
    void LogM(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title)
    {
        LogM(ppPreprocessBlock, file, function, line, logID, level, title, "");
    }

    void LogM(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);

        LogMVar(ppPreprocessBlock, file, function, line, logID, level, title, fmt, args);

        va_end(args);
    }

    void LogM(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, LogTag tag, ...)
    {
        UNREFERENCED_PARAMETER(ppPreprocessBlock);

        va_list args;
        va_start(args, tag);

        LogMVar(file, function, line, logID, level, title, tag, args);

        va_end(args);
    }
};

// Returns a default logger object. Whoever needs logging must call this method
// to get the logger.
extern IBaseLogger * g_baseLogger;

static __forceinline IBaseLogger * GetBaseLogger()
{
    return g_baseLogger;
}

// This is the macro used to log things
// Usage: Log(logarea, level, title, fmtstring, ...)
#define Log(...) \
{\
    static void *_pPreprocessBlock = NULL; \
    GetBaseLogger()->LogM(&_pPreprocessBlock, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__); \
}

inline
bool
LogIsProcessCrashing()
{
    return GetBaseLogger()->IsCrashing();
}

// An abstract class indicating a destination for log text
// This could be stdout, debug output string, a file, etc.
class LogDestination
{
public:
    // Initialize
    virtual BOOL     Init(ConfigParser *config, char *name);

    // Read items which can be changed after initialization
    virtual void     ReadChangeableConfig(ConfigParser *config);

    // Disable unreferenced parameter warning

    // Read items which cannot be changed after initialization
    virtual void     ReadUnchangeableConfig(ConfigParser* )
    {
    }

    // Flush the log file
    virtual void     Flush(bool)
    {
    }

    // Whether the destination accepts binary (unformatted) output.
    virtual bool IsBinary() const
    {
        return false;
    }

    // Append data to the log destination.  buffers are NOT guaranteed
    // to be null terminated.
    // Does NOT add a \r\n for you
    // You must have the lock to call this function
    // prefix contains the "LogLevel,date,LogID,Title"
    // srcInfo contains "SrcFile,srcFunc,SrcLine"
    // desc constains "Pid,Tid,User format string"
    virtual void AppendData(char *prefix, int prefixCount,
                            char* srcInfo, int srcInfoCount,
                            char* desc, int descCount) = NULL;

    virtual bool AppendBinaryDataInEpoch(
        __in_bcount_opt(ancillaryLen) const PVOID pAncillaryData, __in int ancillaryLen,
        __in_bcount_opt(formatLen) const PVOID pFormat, __in int formatLen,
        __in_bcount(dataLen) const PVOID pData, __in int dataLen,
        __in INT64 epoch,
        __in UINT64 minTimestamp, __in UINT64 maxTimestamp)
    {
        UNREFERENCED_PARAMETER(pAncillaryData);
        UNREFERENCED_PARAMETER(ancillaryLen);
        UNREFERENCED_PARAMETER(formatLen);
        UNREFERENCED_PARAMETER(pFormat);
        UNREFERENCED_PARAMETER(formatLen);
        UNREFERENCED_PARAMETER(pData);
        UNREFERENCED_PARAMETER(dataLen);
        UNREFERENCED_PARAMETER(epoch);
        UNREFERENCED_PARAMETER(minTimestamp);
        UNREFERENCED_PARAMETER(maxTimestamp);

        // Stub implementation.
        return false;
    }

    virtual bool ReserveBytes(
        __in INT bytesNeeded,
        __out INT64 * pEpoch)
    {
        UNREFERENCED_PARAMETER(bytesNeeded);
        UNREFERENCED_PARAMETER(pEpoch);

        return false;
    }

    virtual void AppendStructuredData(
            char *prefix,
            char *file, char *function, int line,
            char *component,
            char *title, int titleCount,
            char *message, int messageCount
            )
    {
        UNREFERENCED_PARAMETER(prefix);
        UNREFERENCED_PARAMETER(file);
        UNREFERENCED_PARAMETER(function);
        UNREFERENCED_PARAMETER(line);
        UNREFERENCED_PARAMETER(component);
        UNREFERENCED_PARAMETER(title);
        UNREFERENCED_PARAMETER(titleCount);
        UNREFERENCED_PARAMETER(message);
        UNREFERENCED_PARAMETER(messageCount);
        return;
    }

    virtual bool FullDumpEnabled() const { return false; }

    virtual BOOL AcceptStructuredData() const
    {
        return FALSE;
    }

    void             LockBuffer();
    void             UnlockBuffer();

    void             MakeNullTerminatedString(char *dest, int destSize,
                                              const char *source, int sourceCount,
                                              char** end, int* remaining);

    bool             MakeNullTerminatedWString(WCHAR *dest, int destSize,
                                              const char *source, int sourceCount,
                                              WCHAR** end, int* remaining);

    virtual ~LogDestination() {}

public:
    // Section name of this destination in the .INI file
    char             m_name[c_logMaxLogDestinationNameLength];

    // A flag indicating whether we should log file/function/line numbers
    BOOL             m_logSourceInfo;

    // A flag indicating if this is a file destination or not
    BOOL             m_isFileDestination;

protected:
    CRITSEC          m_criticalSection;
};


// Implements a log destination that is stdout
class LogDestinationStdout : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
    virtual void Flush(bool) { fflush(stdout); }
};

// Implements a log destination that is stderr
class LogDestinationStderr : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
    virtual void Flush(bool) { fflush(stderr); }
};

// Implements a log destination that simply saves up records until
class LogDestinationDeferredInit : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
    virtual void Flush(bool) { fflush(stderr); }
private:

};


// Implements a log destination that is OutputDebugString()
class LogDestinationDebugString : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
};


// Implements a log destination that is DebugBreak()
class LogDestinationDebugBreak : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
};


// Implements a log destination that asserts with a popup
class LogDestinationPopup : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
};

// Implements a log destination that calls an application-defined handler
class LogDestinationApplicationCallback : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
};



// Implements a log destination that terminates the process
class LogDestinationTerminate : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);
};


// Implements a lot destination that writes a full dump
class LogDestinationFullDump : public LogDestination
{
public:
    // Implements base class function
    void AppendData(char* prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);

    virtual bool FullDumpEnabled() const { return true; }
};

typedef std::map<int, int> LogFilesMapType;

enum LogFileType
{
    LogFileTypeMin,
    LogFileTypeText = LogFileTypeMin,
    LogFileTypeBinary,
    LogFileTypeMax
};

typedef struct _LOG_BUFFER_DESCRIPTOR
{
    // Buffer into which log data is copied as it is logged.
    // For memory mapped files this is the buffer that we get from MapViewOfFile.
    // For non memory mapped files this is a buffer that we allocate.
    BYTE * m_writeBuffer;

    // The size of the buffer we are writing to.
    // For memory mapped files it is equal to the map size.
    // For non memory mapped files it is the value of [BufferSize] config setting.
    int m_writeBufferSize;

    // The current write position within the buffer.
    int m_writeBufferPosition;

    // The minimum timestamp across all entries in this buffer.
    UINT64 m_minTimestamp;

    // The maximum timestamp across all entries in this buffer.
    UINT64 m_maxTimestamp;

    // Handle to the memory mapped file. This is NULL for non memory mapped files.
    HANDLE m_fileHandle;
    HANDLE m_tsStreamHandle;

    // Pointer to the next entry in the linked list of descriptors.
    struct _LOG_BUFFER_DESCRIPTOR * m_next;
} LOG_BUFFER_DESCRIPTOR;

#pragma warning(push)

#pragma warning(disable:4481)   // non-standard extension used: "override" keyword.

class BinarySerializationContext;

// Implements a log destination that is a set of files; e.g. crawler0.log, crawler1.log, etc.
class LogDestinationFile : public LogDestination
{
    // The time interval at which the worker thread retires failed log file operations.
    static const int c_WorkerRetryIntervalMs = 1000;

public:
    LogDestinationFile();
    virtual ~LogDestinationFile() {}

    BOOL    Init(ConfigParser *config, char *name);

    // Implements base class function
    virtual void AppendData(
        char *prefix, int prefixCount,
        char* srcInfo, int srcInfoCount,
        char* desc, int descCount) override;

    virtual bool AppendBinaryDataInEpoch(
        __in_bcount_opt(ancillaryLen) const PVOID pAncillaryData, __in int ancillaryLen,
        __in_bcount_opt(formatLen) const PVOID pFormat, __in int formatLen,
        __in_bcount(dataLen) const PVOID pData, __in int dataLen,
        __in INT64 epoch,
        __in UINT64 minTimestamp, __in UINT64 maxTimestamp) override;

    virtual bool ReserveBytes(
        __in INT bytesNeeded,
        __out INT64 * pEpoch) override;

    virtual bool IsBinary() const override;

    void    Flush(bool flushBuffers);

private:

    //
    // Log file creation/closing.
    //
    static UINT _stdcall WorkerThreadStarter(__in void * context);
    VOID    WorkerRun();

    VOID CheckSystemError(__in DWORD error);
    LogLevel CheckIfAlertOrError();

    HANDLE  WorkerCreateLogFile(
        __out int * pUsedSize,
        __out_opt HANDLE * pTsStreamHandle,
        __out_opt BYTE ** pMapAddress,
        __out_opt INT * pMapSize);

    bool    WorkerFlushLogBuffer(__in LOG_BUFFER_DESCRIPTOR * lbDesc);

    void    SwitchBuffers();

    bool    OpenFile(PCSTR pFileName, __out HANDLE * pFileHandle, __out bool *pFCreated);
    void    SetFileLastUpdateTime(__in HANDLE fileHandle);
    void    WriteTimestamps(__in HANDLE streamHandle, __in UINT64 minTimestamp, __in UINT64 maxTimestamp);

    VOID    Enqueue(LOG_BUFFER_DESCRIPTOR ** pQueue, LOG_BUFFER_DESCRIPTOR * lbDesc);
    LOG_BUFFER_DESCRIPTOR *
            Dequeue(LOG_BUFFER_DESCRIPTOR ** pQueue);
    VOID    Push(LOG_BUFFER_DESCRIPTOR ** pQueue, LOG_BUFFER_DESCRIPTOR * lbDesc);

    VOID    SpinWaitUntilFlushCompleted(DWORD timeoutMs);

    //
    // Configuration routines
    //
    void     ReadChangeableConfig(ConfigParser *config);
    void     ReadUnchangeableConfig(ConfigParser *config);

private:
    //
    // File and map management routines.
    //
    void              BuildFileMapFromDisk();
    void              MakeFilenamePrefix(__in_ecount(MAX_PATH) char *pFilename) const;
    void              MakeFilename(__in int fileNumber, __inout_ecount(MAX_PATH) char *pFilename, __in LogFileType fileType) const;
    bool              DeleteLogFileAndUpdateStats(__in int fileNumber, __in LogFileType fileType);
    void              DeleteOldLogFiles();
    void              AdjustLoggingQuota();

    void              RemoveFileFromMap(__in int fileNumber, __in LogFileType fileType);
    void              AddFileToMap(__in int fileNumber, __in int size, __in LogFileType fileType);
    int               ScanAndAddFilesToMap(__in LogFileType fileType);
    bool              CheckEmptyFile(__in int fileNumber, __in LogFileType fileType);
    int               QueryFileSize(PCSTR pszFileName) const;
    static int        GetNumberFromFileName(__in PCSTR pszName);
    int               GetOldestFileNumber(LogFileType fileType) const;

    LogFilesMapType&  GetMapForFileType(const LogFileType fileType);
    const LogFilesMapType&  GetMapForFileType(const LogFileType fileType) const;
    static PCSTR      GetExtensionForFileType(const LogFileType fileType);
    UINT64            TotalSizeOfFilesOnDisk() const;
    void              DiskFileUsageAdd(const UINT64 bytes, const LogFileType fileType);
    void              DiskFileUsageSubtract(const UINT64 bytes, const LogFileType fileType);

    void              CompressOrDeleteFiles();
    int               GetOldestNonCompressedFileNumber(LogFileType * fileType);

public:

    //
    // Configuration
    //

    // Maximum size of a log file (text or binary).
    int               m_maxTextFileSize;

    // Whether to use memory mapped IO or SyncIO
    bool              m_fMemoryMapped;

    // Whether to compress log files or not.
    bool              m_fCompressOldLogFiles;

    // Whether to emit log entries in binary format.
    bool              m_fBinaryMode;

    // A zero-based index that uniquely identifies this binary destination within the binary 
    // destinations set. The value is undefined for non binary destinations.
    int               m_binaryIndex;

    // Indicates the current binary serialization epoch. Incremented when the current binary file is
    // full and the destination must switch to a new binary file. As a result, the 
    // format_preprocess_block is written again in the new epoch (file) to make sure that every 
    // binary file has all the blocks needed to render itself.
    INT64             m_binaryEpoch;

    // State information generated and used by the binary compression module while compressing this
    // destination. The value is NULL for non binary destinations.
    BinarySerializationContext * m_binarySerializationContext;

    // The inline compression level to use to compress binary data. Zero indicates
    // no compression.
    int               m_inlineCompressionLevel;

    // The number of spare log files to pre-create. Zero if no spares needed.
    // For non memory mapped logging we pre-create a spare buffer instead of a file
    // on disk.
    int               m_spareCount;

    // The size of the write buffer. Does NOT apply to memory mapped logging.
    int               m_writeBufferSize;

    // The maximum disk space that can be used by all log files created
    // by this destination.
    UINT64            m_maxAllowedDiskUsage;


    // The amount of disk space that is currently allowed to be used by this destination. This
    // value is equal to m_maxAllowedDiskUsage but it may be reduced under space pressure.
    UINT64            m_allowedDiskUsage;

    // File name prefix for log files (ex: localCosmosLog_en.exe). It does not
    // have the sequence number part (_xxxxxx.log)
    char              m_filenameBase[MAX_PATH];


    //
    // Log file delete/compression.
    //

    // The total space occupied by text log files on the disk.
    UINT64            m_textFileDiskUsage;

    // The total space occupied by binary log files on the disk.
    UINT64            m_binaryFileDiskUsage;

    // Maps that are built on startup and describe all the log files
    // on disk. We do this to avoid scanning the disk repeatedly.
    LogFilesMapType   m_textFilesMap;
    LogFilesMapType   m_compressedFilesMap;
    LogFilesMapType   m_binaryFilesMap;

    //
    // Log buffer creation and management
    //

    // The log buffer we are currently logging to.
    LOG_BUFFER_DESCRIPTOR *   m_currentLogBuffer;

    // The sequence number of the latest log file that we created. It includes
    // the files that we pre create.
    int                       m_currentFileNumber;

    // Handle to the current log file. Does NOT apply to memory mapped logging.
    HANDLE                    m_fileHandle;

    // The current log file size. Does NOT apply to memory mapped logging.
    int                       m_fileSize;

    // The number of buffers that have been posted to the flush queue but did not complete the 
    // flush yet.
    volatile LONG             m_pendingFlushCount;

    // Queue of create log buffer requests.
    LOG_BUFFER_DESCRIPTOR *   m_createQueue;

    // Queue of close log buffer requests.
    LOG_BUFFER_DESCRIPTOR *   m_flushQueue;

    // Queue of available (pre-created) log buffers.
    LOG_BUFFER_DESCRIPTOR *   m_availableQueue;

    // Critical section to protect access to the above queues.
    CRITSEC                   m_queueCritSec;


    //
    // Misc
    //

    // We alert if something is wrong with the file destination (ex: can't create, can't write to 
    // it, etc.). This field stores the timestamp of the last alert. We use this field to make sure
    // we don't fire more that one alert per hour.
    UINT64            m_lastAlertTickCount;

    // The number of file system errors associated with the current log file. The logger alerts only
    // if this number exceeds a certain threshold.
    UINT64            m_fileErrorCount;

    // Worker thread handle and wakeup event
    HANDLE            m_hWorkerThread;
    HANDLE            m_hWorkerWakeupEvent;

    // Sticky error code that is set if the logging module hits
    // an unrecoverable error.
    DWORD             m_status;
};

#pragma warning(pop)

// This was previously used for invoking RDRTL.DLL exported API.
// It is now used to target the new monitoring agent instead.

class LogDestinationRDLog : public LogDestination
{
public:
    BOOL    Init(ConfigParser *config, char *name);

    void    AppendData(char *prefix, int prefixCount,
                    char* srcInfo, int srcInfoCount,
                    char* desc, int descCount);

    virtual void AppendStructuredData(
                char *prefix,
                char *file, char *function, int line,
                char *component,
                char *title, int titleCount,
                char *message, int messageCount
                );

    virtual BOOL AcceptStructuredData() const { return TRUE; }

    LogDestinationRDLog()
    {
        m_monitoringEvents = NULL;
    }

    virtual ~LogDestinationRDLog();

private:

    Microsoft::Cis::Monitoring::Events::DiagnosticsLogger *m_monitoringEvents;
};

// A rule for determining to which destinations an entry goes, given (area,level,title)
class LogRule
{
public:
    // Mask of levels affected (e.g. warning, informational, error)
    int     m_levelMask;

    // Mask of areas affected
    BYTE    m_areaMask[(LogID_Count/8) + 1];

    // If empty, means ALL titles
    char    m_title[c_logMaxRuleTitleLength];

    // Bitmap indicating which destinations
    unsigned int m_logDestinationMask;

    // Returns whether a particular area/level/title to be logged, matches this rule
    BOOL Matches(LogID area, LogLevel level, const char *title);

    void SetAreaID( LogID logID )
    {
        // Set bit coresponding to logID
        m_areaMask[logID/8] |= (1 << (logID & 7));
    }
};

class CrashdumpFileInfo {

public:

    std::string fileName;
    ULONGLONG fileSize;
};

// Forward declaration.
class MemoryLog;


// This class encapsulates all of logging
class Logger
{
    friend class MemoryLog;
    friend class InlineCompressor;
    friend class LogDestinationFile;
    friend class LogDestinationPopup;
    friend class LogDestinationTerminate;
    friend class LogDestinationFullDump;
    friend class BinaryLogReader;
    friend class LoggerProxy;

public:

    static BOOL Init(WCHAR *customSectionName = 0, const char * logsDir = NULL, BOOL slimMode = FALSE);
    static void  LogMVar(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, const char *fmt, va_list args);

    // Flush the contents to disk
    // You shouldn't really be calling this, but it's there because there is
    // intentionally no "clean shutdown" code in the system.
    static void  Flush(bool flushBuffers = false);

    static void SetNoLostLogEntries(bool fNoLostLogEntries);

    static bool SetDumpOrTerminateProcess();

private:

    // Initializes logging
    static void InitCounters();
    static void InitAppAlertClient();

    // Helper function called by Log() macro
    static void  LogV(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, LogTag tag, ...);

    static void LogVEx(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title,
                      FILETIME *ft, DWORD tid, DWORD pid, GUID *pActivityId, GUID *pEntryPointId, LogTag tag, ... );

    static void  LogV(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, const char *fmt, va_list args);


    static void  LogV(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, LogTag tag, va_list args,
                        FILETIME *ft=NULL, DWORD tid=0, DWORD pid=0, GUID *pActivityId=NULL, GUID *pEntryPointId=NULL);

    static char *MakeLogEntry(
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
        __in LogTag tag,
        __in va_list args,
        __in FILETIME *ft_in,
        __in DWORD tid,
        __in DWORD pid,
        __in const GUID *pActivityId,
        __in const GUID *pEntryPointId,
        __in bool fCompact,
        __out int *pSrcLen,
        __out PCHAR *titleStart,
        __out PCHAR *titleEnd,
        __out PCHAR *messageStart
    );

    static bool FormatTag(
        __in char ** bufferPtr,
        __in char * bufferEnd,
        __in LogTag tag,
        __in va_list * args);

    // Binary logging support.

    static bool LogBinaryInEpoch(
        __in_bcount(formatBufferLen) const PVOID pFormatBlockBuffer,
        __in INT formatBlockLen,
        __in_bcount(entryLength) const PVOID pEntryBuffer,
        __in INT entryLen,
        __in INT binaryLogDestinationIndex,
        __in INT64 epoch,
        __in UINT64 minTimestamp,
        __in UINT64 maxTimestamp
        );

    static bool ReserveBytes(
        __in INT binaryLogDestinationIndex,
        __in INT bytesNeeded,
        __out INT64 * pEpoch
        );

    // Determines whether the caller should generate formatted output, binary output or both.
    static void GetOutputModes(
        __in LogID logID,
        __in LogLevel level,
        __in_opt PCSTR title,
        __out bool * pfNeedFormatted,
        __out INT * pBinaryLogDestinationCount,
        __out_ecount(*pBinaryLogDestinationCount) INT * pBinaryLogDestinationIndex);

    // Memory logging support.
    static void  LogM(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title);
    static void  LogM(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, LogTag tag, ...);
    static void  LogM(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, const char *fmt, ...);

    static format_preprocess_block * CreatePreprocessFormat(
        __in LogID logID,
        __in LogLevel level,
        __in const char *file,
        __in const char *function,
        __in const int line,
        __in const char *title,
        __in const char *fmt);

    static void SetCrashing();

    static bool IsCrashing();

    static bool CanLostLogEntries();

    // This function handles all unhandled exceptions. It is hooked up
    // to the global exception handler in CommonInit.
    static LONG LogAndExitProcess(
        __in EXCEPTION_POINTERS*);

    __declspec(noreturn) 
    static void LogAndExitProcess(
        __in EXCEPTION_POINTERS* exceptionPointers, 
        __in unsigned int timeoutMilliSeconds, 
        __in bool fullDump);

    // Crash the current process by generating an unhandled exception.
    __declspec(noreturn) static void Crash();

    // Write out a mini dump of the running thread.
    // Timeout == 0 means never timeout.
    static bool WriteMiniDump (unsigned int timeoutMilliSeconds, bool fullDump);

    // Exception filter for the exception generated in WriteMiniDump.
    static LONG WriteMiniDumpExceptionFilter(
        __in EXCEPTION_POINTERS* exceptionPointers, 
        __in unsigned int timeoutMilliSeconds, 
        __in bool fullDump);

    // Write out a crash dump when exception pointers are know. This function should be called from
    // an exception handling code block.
    __declspec(noreturn)
    static void WriteExceptionMiniDump (EXCEPTION_POINTERS* exceptionPointers, unsigned int timeoutMilliSeconds, bool fullDump);

    // Notify the external process to take a dump and then terminate the current process.
    static VOID NotifyCrashDumpProcess(__in EXCEPTION_POINTERS *exceptionPointers, __in PCSTR szFilePath, __in bool fullDump, bool* pRet);

    // Return the full path of the directory where minidumps and crash logs will be written to.
    // This function is exposed to allow other programs to find out where the minidumps will be stored.
    static bool GetMiniDumpDataDir (char * szMiniDumpDataDir, size_t cbMiniDumpDataDir);

    // Enumerate all crash dumps that are prefixed with the EXE
    // name of the currently executing module and order them
    // by timestamp, with the oldest first. Deletes both the
    // .dmp files and the corresponding .crash files. Returns the
    // total disk usage of all matching crash dumps in the given
    // directory. Returns zero on failure.
    static ULONGLONG EnumerateAllCrashDumpsInDirectory(
        __in PCSTR szMiniDumpDataDir,
        __out std::map<ULONGLONG, CrashdumpFileInfo>& dumpMap
        );

    // Delete crash dumps in the given directory that were generated
    // by the current module name until disk usage falls below the
    // threshold at which we can write new crash dumps.
    // Returns the amount of disk space reclaimed. Returns zero on
    // failure.
    static ULONGLONG DeleteOldCrashDumps(
        __in PCSTR szMiniDumpDataDir,
        __in ULONGLONG totalDumpUsage,
        __out std::map<ULONGLONG, CrashdumpFileInfo>& dumpMap
        );

    // Check whether we can write a new crash dump into the supplied
    // directory. This function will attempt to reclaim disk space
    // by deleting old modules.
    static bool CheckMinidumpDirectory(
        __in PCSTR szMiniDumpDataDir
        );


    // For every minidump written, we will write out a crash log file. It contains information about
    // the process that was writing the minidump and when the process expected to be considered as hung.
    // This function will go through the minidump data dir and make sure those hunging process
    // are killed.
    // Note: the expire time is normally the current system time. We expose the parameter to make it easier
    // for testing. I don't want to use any date time data structure we implemented (such as DateTime) because
    // I want to avoid circluar includes.
    static bool KillPossibleHungMiniDumpWritingProcess (const SYSTEMTIME * stExpire);

    // Add a callback to the list of functions which will be called
    // when a log entry is sent to the applicationcallback
    // destination. These will be called sequentially in the order
    // they are added, and cannot be removed.
    static BOOL AddApplicationLogCallback(LoggerApplicationCallback* handler,
                                          void* cookie);

    // Turn on the application log callback when you don't have an .ini file
    static BOOL EnableApplicationLogCallback();

    // Get/set the activity state of the current thread.
    static XS_ACTIVITY_STATE GetCurrentActivityState();

    static void SetCurrentActivityState(
        __in_bcount_opt(activityStateLength) BYTE* activityState,
        __in size_t activityStateLength);

    static bool ReadBoolFromEnvironmentOrConfig(
        __in const ConfigParser *config,
        __in PCSTR sectionName,
        __in PCSTR paramName,
        __in bool defaultValue
        );

    static int ReadIntFromEnvironmentOrConfig(
        __in const ConfigParser *config,
        __in PCSTR sectionName,
        __in PCSTR paramName,
        __in int defaultValue);

    static __int64 ReadInt64FromEnvironmentOrConfig(
        __in const ConfigParser *config,
        __in PCSTR sectionName,
        __in PCSTR paramName,
        __in __int64 defaultValue);

private:
    static BOOL  PreInit();
    static BOOL  DefaultInit();
    static BOOL  SetDirectory(const char * logsDir);
    static void  ReadChangeableConfig(ConfigParser *config, WCHAR *customSectionName = 0);
    static void  RereadLogDestinationConfig(ConfigParser *config);
    static BOOL  ReadConfig(ConfigParser *config, WCHAR *customSectionName = 0);
    static void  ReadRules(ConfigParser *config, WCHAR *customSectionName = 0);
    static void  ReadAndApplyFineGrainedTimerSetting(__in ConfigParser *config, __in PCSTR sectionName);
    static void  ReadMemoryLogSettings(__in ConfigParser *config, __in PCSTR sectionName);
    static void  CreateMemoryLogger(__in ConfigParser *config, __in PCSTR sectionName);

    static void  ClearRules();
    static void  CreateDefaultRules(ConfigParser *config);
    static BOOL  AddRule(BYTE *areaMask, int levelMask, char *title, int logDestinationMask);
    static int   FindLogDestination(char *outputDestinationName);
    static int   FindOrCreateLogDestination(ConfigParser *config, char *name);
    static int   CreateLogDestinationHelper(ConfigParser *config, LogDestination *destination, char *name);
    static int   CreateFileLogDestination(ConfigParser *config, char *name);
    static int   CreateStdoutLogDestination(ConfigParser *config, char *name);
    static int   CreateStderrLogDestination(ConfigParser *config, char *name);
    static int   CreateDebugStringLogDestination(ConfigParser *config, char *name);
    static int   CreateApplicationCallbackLogDestination(ConfigParser *config, char *name);
    static int   CreateTerminateLogDestination(ConfigParser *config, char *name);
    static int   CreatePopupLogDestination(ConfigParser *config, char *name);
    static int   CreateDebugBreakLogDestination(ConfigParser *config, char *name);
    static int   CreateRDLoggerLogDestination(ConfigParser *config, char *name);
    static int   CreateFullDumpLogDestination(ConfigParser *config, char *name);

    static int   AreaToLogID(char *name);
    static void  _cdecl Stop();
    static void  _cdecl ShutdownAppAlertClient();
    static void  CALLBACK StopThread(ULONG_PTR dwParam);
    static BOOL  SpinLogFlushThread();
    static unsigned int  LogFlushThread(void *);
    static void  SendEntryToApplicationHandlers(const char* logEntry);
    static bool  FullDumpEnabled();

    // For AppAlert
    static bool  m_bAppAlertClientInitialized;
    static AppAlertClient *m_AppAlertClient;
    static unsigned int m_NumAlertsSent;
    static CRITSEC m_AppAlertInitCritSec;
    static bool SendAppAlert (const char *buffer);

    static volatile bool     m_preInitDone;
    static CRITSEC* m_pPreInitCritSec; // on heap so never destructed and we control construct time

    // chain exception handler
    static LPTOP_LEVEL_EXCEPTION_FILTER m_nextExceptionFilterChain;
    static bool m_bUnhandledExceptionFilterSet;

    friend class LogDestinationApplicationCallback;

    // config info read from cosmos.ini
    static ULONGLONG m_maxCrashdumpUsage;

    // minimum RAM that must be available in order to take a dump
    static INT m_skipCrashDumpRamThresholdPercent;

public:
    static void ChainUnhandledExceptionFilter();
    static const char* GetBuildString();

    // Forces logging initialization
    const static BOOL s_DefaultInitOk;

    // Directory in which to create the log files
    static char              m_directoryName[MAX_PATH];

    // The rules
    static volatile int      m_numRules;
    static LogRule           m_rules[c_logMaxRules];

    // The application callback handlers
    static volatile int      m_numCallbackHandlers;
    static LoggerApplicationCallbackInfo m_callbackHandlers[c_logMaxCallbackHandlers];

    // The log destinations; note, because this is all done without locks, log destinations
    // are NEVER deleted!
    static volatile int      m_numLogDestinations;
    static LogDestination *  m_logDestinations[c_logMaxLogDestinations];
    static HANDLE            m_hThread;
    static DWORD             m_dwThreadID;
    static volatile bool     m_bStopped;
    static bool              m_bUseFineGrainedTimer;
    static std::string       m_buildName;

    // The number of binary log destinations and their log destination index.
    static int               m_numBinaryLogDestinations;
    static int               m_binaryLogDestinations[c_logMaxBinaryLogDestinations];

    static bool              m_bUseMemoryLog;
    static MemoryLog        *m_pMemoryLog;

    // Tag as part of assert error log so ACB process would not file TFS bug.
    static const char       *m_noTfsBugTag;

    // Indicates that the process is crashing, either as a result of a failed assert or an unhandled
    // exception. Threads can query this flag and suspend their actions. For example, the heartbeats
    // should not be acknowledged once this flag is true.
    static bool             m_isCrashing;

    // If false (default), log entries can be lost due to logging system not able to keep up with the application.
    // If true, logging subsystem will block the application threads until buffer is available.
    static bool             m_fNoLostLogEntries;

    // Indicates whether the TerminateProcess or GeneratingDump is already in progress.
    static LONG             m_DumpOrTerminateInProgress;

    // Indicates whether the Netlib debug logging is enabled. Higher layers will query this flag 
    // and adjust the Netlib logging settings accordingly.
    static bool             m_bNetlibDebugLogging;

    // Indicates that the logger operates in slim mode. Slim mode disables some features like change
    // notifications, crash dumps.
    static bool             m_slimMode;
};

size_t VsprintfEx(char *buffer, size_t length, const char *fmt, va_list args);

size_t VsprintfEx(format_preprocess_block *pPreprocessBlock , char *buffer, size_t length, va_list args);

BOOL LogRarelyCheckTime(__int64 volatile *pNextReportTime, size_t uReportingPeriodSec);

// calls waitforsingleobject on the handle. If the handle does not
// get signalled within some timeout, exits the process
void WaitForThreadOrExit(HANDLE handle, DWORD threadID);

// Terminates the process using ::TerminateProcess API
__declspec(noreturn) void Terminate(UINT ec);

// Get/set the activity state of the current thread.
XS_ACTIVITY_STATE GetCurrentActivityState();
void SetCurrentActivityState(
    __in_bcount_opt(activityStateLength) BYTE* activityState,
    __in size_t activityStateLength
    );

// Get the activity id of the current thread.
GUID GetCurrentActivityId();

// Named pipe message used to notify the crash dump process to take a dump of the current process.
typedef struct _XS_DUMP_MESSAGE
{
    EXCEPTION_RECORD ExceptionRecord;
    CONTEXT ContextRecord;
    ULONG DumpFlags;
    ULONG ProcessId;
    ULONG ThreadId;
    char DumpFilePath[MAX_PATH];
    PVOID ExcludedMemoryRegionBaseAddress;
    size_t ExcludedMemoryRegionSizeInBytes;
} XS_DUMP_MESSAGE, *PXS_DUMP_MESSAGE;

// Helper function that builds a named pipe name based on the role's data directory.
bool BuildPipeNameFromRoleDataDir(
    __in const char* dataDir,
    __in size_t charCount,
    __out_ecount(charCount) char* pipeName);

bool WriteCrashLogFile(
    __in HANDLE hProcess, 
    __in DWORD dwProcessId, 
    __in const char * cszCrashLogFileFullPath, 
    __in unsigned int timeoutMilliSeconds);

// This function excludes a memory region from the dump file. This is used to reduce the dump file size.
void ExcludeMemoryRegionFromDump(
    __in PVOID baseAddress,
    __in size_t regionSizeInBytes);

#include "logassert.h"
