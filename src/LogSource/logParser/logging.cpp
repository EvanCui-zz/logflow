#define STRSAFE_NO_DEPRECATE

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <strsafe.h>
#include <stddef.h>
#include "shared.h"
#include "changenotify.h"
#include "logids.h"
#include "types.h"
#include "logging.h"
#include <crtdbg.h>
//#include "sysinfo.h"
#include <io.h>
#include <DbgHelp.h>
#include <time.h>
#include <shlobj.h>
#include <evntprov.h>
#include <psapi.h>

#pragma warning(push)

//
// C4201: nonstandard extension used : nameless struct/union.
//
#pragma warning( disable : 4201 )
#include <mmsystem.h>

#pragma warning(pop)

//JWesker
//#include "bldver.h"
//#include "DirUtils.h"
//#include "scopeguard.h"
//#include "datetime.h"

//#include "AppAlertClient.h"
//#include "interlocked.h"

#include "MemLogEntry.h"
#include "MemoryLog.h"
#include "MemLogSprintf.h"
#include "binarylogger.h"

// Include the log tags
#ifdef DeclareTag
#undef DeclareTag
#endif

#define DeclareTag(index, id, name, type) name

// True if millisecond timestamp format should be used. Defaults to second resolution.
bool g_fMillisecondTimestamps = true;

// The log level mask to be used for spewing to stderr before initialization. Defaults to everything.
int g_preInitializeLogLevelMask = -1;

// The log level mask to be used for spewing to rdlogger before initialization.
int g_preInitializeRDLoggerLogLevelMask = (1 << LogLevel_Assert) | (1 << LogLevel_AppAlert);

// These are the names of the tags, which are used for formatting Name="Value" pairs for outputting
const char *g_logTagNames[] =
{
#include "logtagids.h"
};

//jwesker
char *g_LogIDNames[] =
{
	"Logging",
	"Crawler",
	"CrawlerReceiver",
	"CrawlerFetcher",
	"CrawlerScheduler",
	"CrawlerDownloadLog",
	"Counters",
	"Parser",
	"XFERequestTracking",
	"IndexGeneration",
	"QueryProcessor",
	"IndexMerge",
	"IndexGeneral",
	"IndexQueryLog",
	"IndexServer",
	"MCPClient",
	"FileSync",
	"GetAndSyncChunkManifest",
	"ServiceManager",
	"DynamicRank",
	"Caption",
	"Federation",
	"APRunService",
	"GetAndRunServices",
	"Categorization",
	"StaticRanker",
	"CSClient",
	"CSServer",
	"CSLib",
	"Common",
	"Aggregator",
	"Netlib",
	"CatCFSpecific",
	"SpamDetection",
	"CrawlerUrlTracker",
	"StaticRankerDetail",
	"FixPIO",
	"CreateCounter",
	"CRCHashComputation",
	"FEX",
	"Moonshot",
	"Fcslite",
	"FcsliteCache",
	"QualityOfPage",
	"UrlSubmissionTool",
	"Search",
	"FSServer",
	"FSClient",
	"Encarta",
	"Music",
	"LeaseVote",
	"Test",
	"CreateSocEvent",
	"SyncFiles",
	"Doodad",
	"DoodadFCtoLC",
	"DoodadLCtoFC",
	"DoodadWatchdog",
	"DOSDog",
	"StaticRankerTester",
	"Collector",
	"Convertor",
	"Uploader",
	"LocalWD",
	"MachineWatchdog",
	"IPBlock",
	"DisableServerHeader",
	"IAdmin",
	"GenericAP",
	"PSGateway",
	"HttpProxy",
	"CacheInfo",
	"FeedBuilder",
	"FeedBuilderData",
	"SysWatchdog",
	"AlertService",
	"FexRLog",
	"FexCLog",
	"FexHLog",
	"FexPreLog",
	"FexPostDataLog",
	"PsTFTP",
	"PsGatewayWatchdog",
	"CacheManager",
	"RSL",
	"BurninTest",
	"QpsWatchdog",
	"ClusterBuilder",
	"MSRATermExtractor",
	"RealTimeCrawl",
	"RealTimeBuild",
	"ProfileStore",
	"PerfCollector",
	"PerfAggregator",
	"ReplicatedDataSet",
	"WatchdogsWatcher",
	"DigiWatchdog",
	"OutBandMappingWatchdog",
	"PhantomWatchdog",
	"DigiProvisioning",
	"MiniSwitchProvisioning",
	"TLALoadBalancer",
	"Shopping",
	"CacheLogsProcessor",
	"DatasetSync",
	"DeviceManager",
	"DeviceManagerIncomingWatchdog",
	"PhraseTokenExtractor",
	"CQAClient",
	"CQABuilder",
	"CQAOQI",
	"CQAUserStore",
	"CQATagStore",
	"CQAQAStore",
	"TermExtractor",
	"EMS",
	"EMSStat",
	"Macro",
	"CSM",
	"EN",
	"EnWd",
	"ENTest",
	"Cosmos",
	"CsClient",
	"CsNameServer",
	"PN",
	"Dryad",
	"CosmosTest",
	"CosmosTestJournal",
	"CosmosClient",
	"CockpitServer",
	"APWebServer",
	"QueryService",
	"MultiEnv",
	"Election",
	"APMPClient",
	"APProxy",
	"APProxyCommand",
	"FexGLog",
	"FexLiveLog",
	"wssynclog",
	"WmiLib",
	"PhantomLib",
	"AssetTool",
	"DeviceScanner",
	"DeadMachineMacScanner",
	"ServerTools",
	"BOOTP",
	"DHCP",
	"PXE",
	"BOOTPServer",
	"Duff",
	"IdCollect",
	"CosmosWebServer",
	"CosmosLogLoader",
	"CsmReplication",
	"CQACloseQuestions",
	"CQAPresser",
	"CQAPoller",
	"CQAStaticRanker",
	"CQATagTextProcessor",
	"CQAAlert",
	"CQAAlertStore",
	"FexCQA",
	"CQAWatchdogs",
	"MultiMedia",
	"ImageFetcher",
	"ImageFetcherCrawler",
	"ImageFetcherThumbnailer",
	"LiveSearchPane",
	"PsServer",
	"PsClient",
	"PsAgent",
	"RMAUtils",
	"LCDService",
	"CSV",
	"CSVParser",
	"CreateImage",
	"CreateImageFiles",
	"PhantomPowerStateWatchdog",
	"WebFeedDiscovery",
	"Answers",
	"Newsgroup",
	"TLARemoting",
	"HttpClient",
	"DiskTest",
	"DiskTestSequential",
	"DiskTestRandom",
	"MemoryTest",
	"WebFeedDiscoveryReader",
	"WebFeedDiscoveryProcessor",
	"WebFeedDiscoveryCrawlfileGenerator",
	"WebFeedDiscoveryRSSFetcher",
	"WebFeedDiscoveryProvider",
	"FileSyncIgnoreCRC",
	"FileSyncWrongCRC",
	"DSSlaveSync",
	"ReliableRebootService",
	"GenericRepair",
	"NetlibCorruptPacket",
	"QuerySuggestion",
	"ISNManager",
	"ChunkLocator",
	"ISAgent",
	"CDGDominantImage",
	"CosmosJobManager",
	"CosmosJobManagerWd",
	"CachePropagator",
	"CachePropagatorClient",
	"MlToHosts",
	"DryadMD",
	"UrlTracker",
	"DNSWatchdog",
	"PsWatchdog",
	"WebFeedDiscoveryPinger",
	"MacroSuggestion",
	"AppAlertService",
	"RowStatus",
	"Sputnik",
	"GeneralClient",
	"QueryAugmenter",
	"QueryStatistics",
	"QueryISNStatistics",
	"AnswersRLog",
	"AnswersALog",
	"DNSService",
	"FexMissingSnippetLog",
	"XifBuilder",
	"SharedModules",
	"DryadProfiler",    
	"CosmosProxy",
	"RTIndexCoverage",
	"Speller",
	"FCS",
	"MT_HttpServer",
	"MT_Distributor",
	"MT_Cache",
	"MT_Translator",
	"MT_ResearchSdk",
	"MT_ModelServer",
	"MT_DB",
	"Localization",
	"XcPs",
	"XcApi",
	"XcDataProvider",
	"XcBlob",
	"Journal",

	// Last entry must be a NULL (for sanity checking)
	NULL
};



// This is the count of valid tag indices
// There is no log tag index higher than this one
const int LogTagCountIndex = LogTagToIndex(LogTag_End);

// Max size of file, function, line text when added to the log file
const int c_logMaxSourceInfoSize = 256;

// This is used in the data structure below
const int c_maxLogTagNameSize = 32;

static const int c_TimeStringLength = 20;

// Name of file co-located with role containing the build name
char * c_buildNameFileName = "buildname.txt";


// This data structure is created on initialization, and is used for quick formatting of Name="Value" pairs
// It stores the Name=" header and its length, so we don't have to strlen() or concatenate ="
struct
{
    char  m_header[c_maxLogTagNameSize]; // includes Name="
    int   m_length;                      // length of the above
} g_logTagNameHeaders[LogTagCountIndex];

// Name of the log rules section
char *c_logRulesSectionName = "LogRules";

// Standard name of the destination which means to stdout
char *c_logDestinationStdout = "stdout";

// Standard name of the destination which means to stderr
char *c_logDestinationStderr = "stderr";

// Standard name of the destination which means to OutputDebugString
char *c_logDestinationDebugString = "debugstring";

// Standard name of the destination which means to call an application-specific handler
char *c_logDestinationApplicationCallback = "applicationcallback";

// Standard name of the destination which means to exit() if we see this
char *c_logDestinationTerminate = "terminate";

// Standard name of the destination which means to debug break if we see this
char *c_logDestinationDebugBreak = "debugbreak";

// Standard name of the destination which means to pop up an assert dialog if we see this
char *c_logDestinationPopup = "popup";

// Standard name of the RD Logger
char * c_logDestinationRDLogger = "rdlogger";

// Standard name of the full dump Logger
char * c_logDestinationFullDumpLogger = "fulldump";



// Parameter name of the file name base in a particular section
// If this parameter doesn't exist, it will use the section name as the filename base
char *c_logFileBaseParameterName = "FileNameBase";

// Asserts are handled in a special way.
// No further action is taken If the config file
// specifies the destination to be one of the following
// "popup", "debugbreak", "terminate"
// If none of these destinations are specified then
// popup is the default action for debug builds
// and terminate is the default action for retail builds
#ifndef DEBUG
char* c_defaultAssertDestination = c_logDestinationTerminate;


// Allow the process 15 minutes to write a full crash dump to disk.
static const int c_minidumpTimeout = (15*60*1000);      // 15 minutes in milliseconds
#else
char* c_defaultAssertDestination = c_logDestinationPopup;
const int c_minidumpTimeout = 0;
#endif

// Default values if they are not present in the config file, or the config file is not present
const int c_defaultLoggingWriteBufferSize = 16384;
const int c_defaultLoggingLogSourceInfo = TRUE;
const int c_defaultMaxAllowedDiskUsage = 50000000;     // ~50 MB.
const int c_defaultLogFileSpareCount = 1;
const int c_defaultLogBufferSpareCount = 5;

// The size of a text file on disk.
const int c_maxTextFileSize = 10000000;
const int c_maxTextFileSizeLimit = (256 * 1024 * 1024); // 256 MB.

static PCSTR c_textFileExtension       = "log";
static PCSTR c_binaryFileExtension     = "bin";

static FpbUidType g_nextBlockUid = 0;

// Compress the oldest text files if disk usage by text files is > 70%
// Stop compressing when the usage falls below 65%. The 5% delta is for
// hysterisis.
const int c_startCompressionHighWatermarkPct = 70;
const int c_stopCompressionLowWatermarkPct = 65;

// Memory mapping and compression must be turned on from the ini file.
const bool c_fDefaultMemoryMapped = 0;
const bool c_fBinaryMode = 0;

// By default we do inline compression for binary files. Note that inline
// compression only applies to binary logging.
const int c_inlineCompressionLevel = 6;

//
// The default amount of disk space to use for
// minidumps expressed in MB.
//
static const ULONGLONG c_defaultMaxCrashdumpUsage = 40000000000;    // ~ 40GB.

// Max allowed length for a rule string
const int c_maxRuleLength = 512;

// Length of a GUID in string format, no braces
const int c_guidStringLen = _countof("00000000-0000-0000-0000-000000000000") - 1;

// Null GUID
static GUID nullGUID={0};

// Declare class members
char              Logger::m_directoryName[MAX_PATH];

volatile int      Logger::m_numRules;
LogRule           Logger::m_rules[c_logMaxRules];

volatile int      Logger::m_numCallbackHandlers = 0;
LoggerApplicationCallbackInfo Logger::m_callbackHandlers[c_logMaxCallbackHandlers];

volatile int      Logger::m_numLogDestinations;
LogDestination *  Logger::m_logDestinations[c_logMaxLogDestinations];
HANDLE            Logger::m_hThread;
DWORD             Logger::m_dwThreadID;
volatile bool     Logger::m_bStopped;

time_t            g_nextWarningErrorsReset;
volatile long     g_totalWarnings;
volatile long     g_totalErrors;

bool              g_prefixAppName = false;

bool              Logger::m_bAppAlertClientInitialized = false;
AppAlertClient*   Logger::m_AppAlertClient = NULL;
unsigned int      Logger::m_NumAlertsSent = 0;
CRITSEC           Logger::m_AppAlertInitCritSec;

volatile bool     Logger::m_preInitDone = false;
CRITSEC* Logger::m_pPreInitCritSec = NULL; // on heap so never destructed and we control construct time

LPTOP_LEVEL_EXCEPTION_FILTER Logger::m_nextExceptionFilterChain=NULL;
bool              Logger::m_bUnhandledExceptionFilterSet=false;

ULONGLONG Logger::m_maxCrashdumpUsage = 0;
bool Logger::m_bUseFineGrainedTimer = FALSE;
std::string Logger::m_buildName;

// The minimum percentage of available RAM in order to take a crash dump on process crash. If 
// the amount of physical RAM is lower than this value then the dump is not taken and the process
// terminates immediately.
// A value of zero indicates no minimum (i.e. always take a dump).
// This setting is intended to make sure the process does not hang indefinitely waiting for the dump 
// to take place. Note that this is something that could happen to managed roles (RdRuntimeLoader) 
// if the logagent crashes after it suspended the crashed process. Native roles do not have this 
// issue because startcosmos restarts the log agent and the log agent kills the frozen process on 
// its restart. For managed roles, since the log agent is started by RdRuntimeLoader, the re-start
// never happens.
INT Logger::m_skipCrashDumpRamThresholdPercent = 0;

MemoryLog *Logger::m_pMemoryLog = NULL;
bool       Logger::m_bUseMemoryLog = false;
const char *Logger::m_noTfsBugTag = "DontFileTFSBug";

// The number of binary log destinations and their log destination indexes.
int         Logger::m_numBinaryLogDestinations = 0;
int         Logger::m_binaryLogDestinations[c_logMaxBinaryLogDestinations];

// Indicates whether the netlib debug logging is enabled.
bool       Logger::m_bNetlibDebugLogging = false;

// Indicates that the process is crashing, either as a result of a failed assert or an unhandled
// exception.
bool       Logger::m_isCrashing = false;

// If false (default), log entries can be lost due to logging system not able to keep up with the application.
// If true, logging subsystem will block the application threads until buffer is available.
bool        Logger::m_fNoLostLogEntries = false;

// Indicate whether the process is generating crash dump or being terminated.
LONG        Logger::m_DumpOrTerminateInProgress = 0;

// Indicates that the logger operates in slim mode. Slim mode disables some features like change
// notifications, crash dumps.
bool       Logger::m_slimMode = false;

//
// Memory region to exclude from the dump file. This is used to reduce the size of the dump file.
//

PVOID g_ExcludedMemoryRegionBaseAddress = NULL;
size_t g_ExcludedMemoryRegionSizeInBytes = 0;

// Store the current activity state in TLS. Each module (DLL/EXE) has a copy of this static 
// variable. The logger proxy makes sure that everybody uses the same static.
// The activity state is currently a list of GUIDs
static __declspec(thread) XS_ACTIVITY_STATE s_activityState = { NULL, 0};

GUID GetCurrentActivityId()
{
    GUID activityId;
    DWORD error = EventActivityIdControl(EVENT_ACTIVITY_CTRL_GET_ID, &activityId);

    return activityId;
}


// TODO: (NiranjNi): Remove this when the log search tool
// can do correlated searches.
// Replace all calls to this with calls to GetCurrentActivityId
static GUID GetCurrentEntryPointId()
{
    if (s_activityState.ActivityState != NULL)
    {
        GUID entryPointId;
        memcpy(&entryPointId, s_activityState.ActivityState + 1, sizeof(GUID));

        return entryPointId;
    }
    else
    {
        return nullGUID;
    }
}

// Initialize the values of the log tag name headers
// Initialize each header to Name="
// Store the length of the above string
// Returns FALSE if there wasn't enough room to initialize a particular parameter
BOOL InitializeLogTagHeaders()
{
    for (int i = 0; i < LogTagCountIndex; i++)
    {
        // Need space for <space>name="
        // +5 is just extra slop
        if ((strlen(g_logTagNames[i]) + 5) >= c_maxLogTagNameSize)
        {
            fprintf(stderr, "Log tag name was too long: %s\n", g_logTagNames[i]);
            return FALSE;
        }

        sprintf(g_logTagNameHeaders[i].m_header, "%s=\"", g_logTagNames[i]);
        g_logTagNameHeaders[i].m_length = (int) strlen(g_logTagNameHeaders[i].m_header);
    }

    return TRUE;
}

//// ExtractFileNameFromPath()
//// Extract the file name string from the full path
//static const char* ExtractFileNameFromPath(
//        __in LPCSTR filePath
//        )
//{
//    LPCSTR fileName = strrchr(filePath, '\\');
//    if (fileName)
//    {
//        fileName ++;
//    }
//    else
//    {
//        fileName = filePath;
//    }
//    return fileName;
//}

//
// Helper function that builds a valid named pipe name based on the role's data directory.
//
// In order for the pipe name building scheme to work with xcompute processes, which are launched by PN and
// their data directories are created under ENPN role's data directory, a back scan of the string "COSMOSDIR"
// is performed.
//
// Example 1: SDS process
//   dataDir   = "C:\Resources\Directory\Tenant.StorageDiagnostics.COSMOSDIR\data"
//   pipe name = "\\.\pipe\Resources_Directory_Tenant.StorageDiagnostics.COSMOSDIR"
//
// Example 2: a xcompute process launched by PN.
//   dataDir   = "C:\Resources\directory\Tenant.EnPn.COSMOSDIR\data\pn\job\0x18\user\0x19\Data
//   pipe name = ""\\.\pipe\Resources_Directory_Tenant.EnPn.COSMOSDIR
//

// The suffix string of the cosmos role's data directory.
const char* c_CosmosDataDirSuffix = "COSMOSDIR";

//bool BuildPipeNameFromRoleDataDir(
//    __in const char* dataDir,
//    __in size_t charCount,
//    __out_ecount(charCount) char* pipeName)
//{
//    bool nameBuilt = false;
//    size_t sourcePosition;
//    size_t sourceEndPosition;
//    size_t destinationPosition;
//    size_t remainingCharCount;
//    size_t suffixCharCount;
//
//    if (dataDir == NULL)
//    {
//        goto Cleanup;
//    }
//
//    //
//    // Skip the drive part of the path.
//    //
//
//    sourcePosition = 0;
//    while ((dataDir[sourcePosition] != 0) && (dataDir[sourcePosition] != '\\'))
//    {
//        sourcePosition++;
//    }
//
//    if (dataDir[sourcePosition] == '\\')
//    {
//        sourcePosition++;
//    }
//
//    if (dataDir[sourcePosition] == 0)
//    {
//        goto Cleanup;
//    }
//
//    //
//    // Locate the last occurance of "COSMOSDIR" in the path.
//    //
//
//    sourceEndPosition = strlen(dataDir);
//    suffixCharCount = strlen(c_CosmosDataDirSuffix);
//    if (sourceEndPosition <= suffixCharCount)
//    {
//        goto Cleanup;
//    }
//
//    sourceEndPosition -= suffixCharCount;
//    while (sourceEndPosition > sourcePosition)
//    {
//        if (_strnicmp(&(dataDir[sourceEndPosition]), c_CosmosDataDirSuffix, suffixCharCount) == 0)
//        {
//            break;
//        }
//        else
//        {
//            sourceEndPosition--;
//        }
//    }
//
//    if (sourceEndPosition == sourcePosition)
//    {
//        goto Cleanup;
//    }
//
//    sourceEndPosition += suffixCharCount;
//
//    //
//    // Construct the fixed part of the piple name.
//    //
//
//    HRESULT hr = StringCchCopyExA(
//        pipeName,
//        charCount,
//        "\\\\.\\pipe\\",
//        NULL,
//        &remainingCharCount,
//        0);
//
//    if (FAILED(hr))
//    {
//        goto Cleanup;
//    }
//
//    //
//    // Copy the remaining path into pipe name and convert '\' to '_' since '\' is not a valid character as
//    // pipe name.
//    //
//
//    destinationPosition = charCount - remainingCharCount;
//    while ((sourcePosition < sourceEndPosition) && (destinationPosition < charCount))
//    {
//        if (dataDir[sourcePosition] == '\\')
//        {
//            pipeName[destinationPosition] = '_';
//        }
//        else
//        {
//            pipeName[destinationPosition] = dataDir[sourcePosition];
//        }
//
//        sourcePosition++;
//        destinationPosition++;
//    }
//
//    if (destinationPosition < charCount)
//    {
//        pipeName[destinationPosition] = '\0';
//        nameBuilt = true;
//    }
//
//Cleanup:
//
//    return nameBuilt;
//}
//
////
//// This function registers an excluded memory range from the dump file. This is used to reduce the size of
//// the dump file and speed up the dump process.
////
//// N.B. There is no lock taken when updating the global variables of the excluded memory region. In order to
////      safely register an excluded region, call this function during the initialization where crashing is
////      unlikely.
////
////      The excluded memory region is only honored if MiniDumpWithFullMemory flag is specified, i.e. the
////      dump will include all the memory.
////
//// Arguments:
////
////      baseAddress - Supplies the starting address of the excluded memory region. The address must be
////                    page aligned.
////
////      regionSizeInBytes - Supplies the size of the excluded memory region. The size must be a multiple
////                          of page size.
////
//void ExcludeMemoryRegionFromDump(
//    __in PVOID baseAddress,
//    __in size_t regionSizeInBytes)
//{
//    g_ExcludedMemoryRegionBaseAddress = baseAddress;
//    g_ExcludedMemoryRegionSizeInBytes = regionSizeInBytes;
//}
//
//BOOL
//LogRarelyCheckTime (
//    __int64 volatile   *pNextReportTime,
//    size_t              uReportingPeriodSec
//    )
//{
//    __int64 iSystemTime;
//    __int64 iNextReportTime;
//    __int64 iNextReportTimeOld;
//
//    //
//    // compare current time with next report time and, if it's time to report,
//    // try to update next report time atomically; do the report if update succeeded
//    //
//    GetSystemTimeAsFileTime ((FILETIME *) &iSystemTime);
//    iNextReportTime = *pNextReportTime;
//    do
//    {
//        iNextReportTimeOld = iNextReportTime;
//        iNextReportTime = iSystemTime + ((__int64) uReportingPeriodSec) * 10000000;
//        if ((UInt64) iSystemTime < (UInt64) iNextReportTimeOld && (UInt64) iNextReportTime >= (UInt64) iNextReportTimeOld)
//            return (FALSE);
//        iNextReportTime = Interlocked::CompareExchange64 (pNextReportTime, iNextReportTime, iNextReportTimeOld);
//    }
//    while (iNextReportTime != iNextReportTimeOld);
//
//    return (TRUE);
//}
//
//void WaitForThreadOrExit(HANDLE handle, DWORD threadID)
//{
//    if (threadID != GetCurrentThreadId())
//    {
//        // wait for a maximum of 5 seconds. If the thread does not
//        // exit within 5 seconds, Exit the process.
//        DWORD ret = WaitForSingleObject(handle, 5*1000);
//        if (ret != WAIT_OBJECT_0)
//        {
//            ::Terminate(1);
//        }
//    }
//}
//
//void Terminate(UINT ec)
//{
//    ::TerminateProcess(GetCurrentProcess(), ec);
//}
//
//size_t VsprintfEx(char *buffer, size_t length, const char *fmt, va_list args)
//{
//    format_preprocess_block preprocessBlock;
//
//    if(preprocess_format(fmt, &preprocessBlock) == 0)
//    {
//        return 0;
//    }
//
//    return ::VsprintfEx(&preprocessBlock, buffer, length, args);
//
//}
//
//size_t VsprintfEx(format_preprocess_block *pPreprocessBlock , char *buffer, size_t length, va_list args)
//{
//    if (NULL == pPreprocessBlock)
//    {
//        return 0;
//    }
//
//    // Allocate to hold the entry. This should be enough since we do not deep
//    // copy the strings.
//    char logEntryBuffer[1024];
//
//    LogEntry * pLogEntry = WriteLogEntryStatic(
//                                                    logEntryBuffer,
//                                                    sizeof(logEntryBuffer),
//                                                    pPreprocessBlock,
//                                                    args);
//
//    if (pLogEntry == NULL)
//    {
//        return 0;
//    }
//
//    __try
//    {
//        // Avoid crashing if there is a mismatch between the format specifiers and the actual 
//        // parameter list e.g. Log("Account: %s, BlobName:%s", accountName)
//        // We have seen roles cycling because of this relatively minor defect.
//        // Instead of crashing, log the location of the offending log statement. The log maintains
//        // the logging level of the offending log statement.
//        // Note that WriteLogEntryStatic did NOT deep copy the strings and other pointer parameters. 
//        // The call below is where the pointer parameters are dereferenced and therefore it is the
//        // call that should be guarded.
//        return log_entry_sprintf(buffer, length, pLogEntry);
//    }
//    __except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
//                                            EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
//    {
//        Log(LogID_Logging, (LogLevel)pPreprocessBlock->m_level,
//            "Invalid parameter list", 
//            "Mismatch between the format specifiers and the parameter list. Calling File = %s Function = %s, Calling Line = %d, Format spec = %s", 
//            pPreprocessBlock->m_filename, 
//            pPreprocessBlock->m_function, 
//            pPreprocessBlock->m_line, 
//            pPreprocessBlock->m_format);
//
//            return 0;
//    }
//}


//void CALLBACK Logger::StopThread(ULONG_PTR dwParam)
//{
//    UNREFERENCED_PARAMETER(dwParam);
//}
//
//// Given an area name, return its LogID
//// Returns -1 if not found
//int Logger::AreaToLogID(char *name)
//{
//    for (int i = 0; i < LogID_Count; i++)
//    {
//        if (_stricmp(g_LogIDNames[i], name) == 0)
//            return i;
//    }
//
//    return -1;
//}
//
//
//// Create default rules for before we have initialized logging
//void Logger::CreateDefaultRules(ConfigParser *config)
//{
//    ClearRules();
//
//    // Default rule is log everything to stderr
//    int destinationIndex = FindOrCreateLogDestination(config, c_logDestinationStderr);
//    if (destinationIndex >= 0)
//    {
//        AddRule(NULL, g_preInitializeLogLevelMask, "*", (1<<destinationIndex));
//    }
//
//    // Also enable the rdlogger destination, which is currently used only for asserts
//    // and alerts. This way, even when there are configuration issues, critical events
//    // are always surfaced.
//
//    destinationIndex = FindOrCreateLogDestination(config, c_logDestinationRDLogger);
//    if (destinationIndex >= 0)
//    {
//        AddRule(NULL, g_preInitializeRDLoggerLogLevelMask, "*", (1<<destinationIndex));
//    }
//}
//
//
//// Rules are in the format:
////
//// [LogRules]
//// Rule1=areas,severity,title,output location
//// Rule2=...
////
//// Use * to indicate everything:
////
//// e.g. Rule1=*,W,*,stdout
////      Rule2=*,*,*,foo
////
//// For areas you can provide a list of areas in the form:
//// Rule1=csm|netlib|en|pn,severity,title,output location
////
//// You can also use ~ to remove things, e.g. for everything except netlib:
//// Rule1=*|~netlib,severity,title,output location
////
//// IMPORTANT NONE: The rule database is updated without using locks, while other people
////                 may be reading from it.  Therefore, care must be taken to ensure that,
////                 when race conditions happen, they don't cause anything bad to happen.
////
////                 The main thing that needs to be avoided is having a rule output to
////                 a destination that has not been initialized yet.  So a destination
////                 needs to be initialized before a rule can point to it.
////
//void Logger::ReadRules(ConfigParser *config, WCHAR *customSectionName)
//{
//    char sectionNameBuffer[64];
//    const char *sectionName;
//
//    if (config == NULL)
//        return;
//
//    char moduleName[MAX_PATH+1];
//    if (GetModuleFileNameA(NULL, moduleName, sizeof(moduleName)) == 0)
//    {
//        fprintf(stderr, "Unable to get module path\n");
//        return;
//    }
//
//    // moduleName is e.g. foo\bar\csm.exe
//    char *moduleNameFilenamePart = strrchr(moduleName, '\\');
//    if (moduleNameFilenamePart == NULL)
//    {
//        moduleNameFilenamePart = moduleName;
//    }
//    else
//    {
//        moduleNameFilenamePart++;
//    }
//
//    // Remove all existing rules -- this means that until the rules are read in, we won't be logging anything
//    ClearRules();
//
//    // If the section [LogRules_<modulename>] exists then use that,
//    // otherwise if the section [LogRules_<customSectionName>] exists then use that,
//    // otherwise use [LogRules]
//    // e.g. [LogRules] or [LogRules_csm.exe] or [LogRules_XBlobFE]
//    sprintf(sectionNameBuffer, "%s_%s", c_logRulesSectionName, moduleNameFilenamePart);
//
//    if (config->DoesSectionExist(sectionNameBuffer))
//    {
//        sectionName = sectionNameBuffer;
//    }
//    else
//    {
//        sectionName = c_logRulesSectionName;
//        if (customSectionName != NULL)
//        {
//            sprintf(sectionNameBuffer, "%s_%ls", c_logRulesSectionName, customSectionName);
//            if (config->DoesSectionExist(sectionNameBuffer))
//            {
//                sectionName = sectionNameBuffer;
//            }
//        }
//    }
//
//    // Read whether we want to prefix the app name onto the log output filenames
//    config->GetBoolParameter(sectionName, "PrefixAppName", &g_prefixAppName);
//
//
//    // Read the upper bound on crash dump usage
//    m_maxCrashdumpUsage = ReadInt64FromEnvironmentOrConfig(
//        config, sectionName, "MaxCrashDumpQuota", c_defaultMaxCrashdumpUsage);
//
//    m_skipCrashDumpRamThresholdPercent = ReadIntFromEnvironmentOrConfig(
//        config, sectionName, "SkipCrashDumpRamThresholdPercent", m_skipCrashDumpRamThresholdPercent);
//
//    m_bNetlibDebugLogging = ReadBoolFromEnvironmentOrConfig(
//        config, sectionName, "NetlibDebugLogging", m_bNetlibDebugLogging);
//
//    ReadAndApplyFineGrainedTimerSetting(config, sectionName);
//
//    ReadMemoryLogSettings(config, sectionName);
//
//    // Now read the log entries, and what we want to do with them
//    for (int i = 0; i < c_logMaxRules; i++)
//    {
//        char rule[c_maxRuleLength];
//        char title[c_logMaxRuleTitleLength];
//        char area[1024];
//        char parameterName[32];
//        char *titleStart;
//        char *outputSectionName;
//        char *p;
//        char *areaPtr;
//        bool doneParsingAreas = false;
//        BOOL result;
//        int  levelMask;
//        int  destinationIndex;
//        BYTE areaMask[ (LogID_Count/8) + 1 ];
//
//        sprintf(parameterName, "Rule%d", i+1);
//
//        result = config->GetParameter(
//            sectionName,
//            parameterName,
//            rule,
//            sizeof(rule)
//        );
//        if (result == FALSE)
//            continue;
//
//        p = strchr(rule, ',');
//        if (p == NULL)
//            continue; // Invalid rule
//
//        int ruleLength = (int) (p-rule);
//        if (ruleLength >= sizeof(area))
//        {
//            fprintf(stderr, "Logging rule area name too large: %s\n", rule);
//            continue;
//        }
//
//        memcpy(area, rule, ruleLength);
//        area[ruleLength] = '\0';
//
//        memset(areaMask, 0, sizeof(areaMask));
//
//        // Now parse the log area
//        areaPtr = area;
//        do
//        {
//            char *currentAreaPtr; // points to beginning of current area
//
//            currentAreaPtr = areaPtr;
//
//            // Find next | or end of string
//            char *q = strchr(currentAreaPtr, '|');
//
//            if (q == NULL)
//            {
//                // No | means we're done after this area
//                doneParsingAreas = true;
//            }
//            else
//            {
//                // | means there's more, so null out the | and advance areaPtr for the next iteration
//                *q = '\0';
//                areaPtr = q+1;
//            }
//
//            if (strcmp(currentAreaPtr, "*") == 0)
//            {
//                memset(areaMask, 0xFF, sizeof(areaMask));
//            }
//            else
//            {
//                bool inverted = false;
//
//                // ~area means remove that area from the mask
//                if (*currentAreaPtr == '~')
//                {
//                    currentAreaPtr++;
//                    inverted = true;
//                }
//
//                int logID = AreaToLogID(currentAreaPtr);
//                if (logID < 0)
//                {
//                    fprintf(stderr, "Parsing log rules: area not found '%s'\n", currentAreaPtr);
//                    continue; // Invalid log ID
//                }
//
//                LogAssert(logID < LogID_Count);
//
//                if (inverted)
//                {
//                    areaMask[logID / 8] &= ~(1 << (logID & 7));
//                }
//                else
//                {
//                    areaMask[logID / 8] |= (1 << (logID & 7));
//                }
//            }
//        } while (doneParsingAreas == false);
//
//        // p currently points to the comma before the levels
//
//        // Allow multiple levels to be used
//        levelMask = 0;
//
//        while (TRUE)
//        {
//            p++;
//
//            if (*p == 'I')
//                levelMask |= 1 << LogLevel_Info;
//            else if (*p == 'D')
//                levelMask |= 1 << LogLevel_Debug;
//            else if (*p == 'S')
//                levelMask |= 1 << LogLevel_Status;
//            else if (*p == 'W')
//                levelMask |= 1 << LogLevel_Warning;
//            else if (*p == 'E')
//                levelMask |= 1 << LogLevel_Error;
//            else if (*p == 'X')
//                levelMask |= 1 << LogLevel_AppAlert;
//            else if (*p == 'A')
//                levelMask |= 1 << LogLevel_Assert;
//            else if (*p == '*')
//                levelMask = -1; // All levels (all bits set)
//            else
//                break; // invalid character, or comma, or null, etc.
//        }
//
//        if (*p != ',')
//            continue; // Invalid rule
//
//        p++;
//        titleStart = p;
//        p = strchr(titleStart, ',');
//        if (p == NULL)
//            continue; // Invalid rule
//
//        memcpy(title, titleStart, p-titleStart);
//        title[ p-titleStart ] = '\0';
//
//        p++;
//        outputSectionName = p;
//
//        if (0 == ::strcmp(c_logDestinationPopup, outputSectionName) ||
//            0 == ::strcmp(c_logDestinationDebugBreak, outputSectionName))
//        {
//            #pragma prefast(push)
//            #pragma prefast(disable:309, "Potential NULL argument 1 to 'SetUnhandledExceptionFilter'.")
//            ::SetUnhandledExceptionFilter(NULL);
//            #pragma prefast(pop)
//        }
//
//        destinationIndex = FindOrCreateLogDestination(config, outputSectionName);
//        if (destinationIndex < 0)
//            continue; // Unable to create
//
//        // We now have a fully initialized destination, so it's ok to point a rule at it
//        AddRule(areaMask, levelMask, title, (1<<destinationIndex));
//    }
//
//    RereadLogDestinationConfig(config);
//
//    // Construct a log string that contains log destination file information
//    // so that we can output this to the status log.
//    char logFileDestinationConfigurations[1025];
//    logFileDestinationConfigurations[0] = '\0';
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        if (m_logDestinations[i]->m_isFileDestination)
//        {
//            LogDestinationFile *fileDestination = static_cast<LogDestinationFile *>(m_logDestinations[i]);
//            if (fileDestination == NULL)
//            {
//                continue;
//            }
//
//            // The log destination file section name uses MAX_PATH and we allow 128 characters for the rest
//            // for each log file destination's configurations
//            char temp[MAX_PATH + 128 + 1];
//            sprintf(temp, "[%s: MaxTextFileSize=%d, MaxDiskUsage=%I64d, MemoryMapped=%d, EmitBinaryLogs=%d InlineCompressionLevel=%d WriteBufferSize=%d] ",
//                fileDestination->m_name,
//                fileDestination->m_maxTextFileSize,
//                fileDestination->m_maxAllowedDiskUsage,
//                fileDestination->m_fMemoryMapped,
//                fileDestination->m_fBinaryMode,
//                fileDestination->m_inlineCompressionLevel,
//                fileDestination->m_writeBufferSize);
//            if (strlen(temp) + strlen(logFileDestinationConfigurations) <= 1024)
//            {
//                strcat(logFileDestinationConfigurations, temp);
//            }
//            else
//            {
//                break;
//            }
//        }
//    }
//
//    // Memory log initialization assumes we've read all logger settings and rules.
//    CreateMemoryLogger(config, sectionName);
//
//    Log(
//        LogID_Logging, LogLevel_Status,
//        "LoggingConfiguration",
//        "Section name: '%s' (customSectionName = '%ls'), "
//        "MaxCrashDumpUsage: %llu, SkipCrashDumpRamThresholdPercent: %d. "
//        "File Configurations: %s",
//        sectionName,
//        customSectionName == NULL ? L"null" : customSectionName,
//        m_maxCrashdumpUsage,
//        m_skipCrashDumpRamThresholdPercent,
//        logFileDestinationConfigurations);
//}
//
//bool Logger::ReadBoolFromEnvironmentOrConfig(
//    __in const ConfigParser *config,
//    __in PCSTR sectionName,
//    __in PCSTR paramName,
//    __in bool defaultValue
//    )
//{
//    char buffer[16];
//
//    std::string envSettingName(sectionName);
//    envSettingName.append("::");
//    envSettingName.append(paramName);
//
//    //
//    // First check if an override exists in the environment.
//    //
//    DWORD result =
//        GetEnvironmentVariableA(envSettingName.c_str(), buffer, sizeof(buffer));
//
//    if ((result != 0) && (result <= sizeof(buffer)))
//    {
//        if ((_stricmp(buffer, "1") == 0) ||
//            (_stricmp(buffer, "true") == 0) ||
//            (_stricmp(buffer, "yes") == 0))
//        {
//            return true;
//        }
//        if ((_stricmp(buffer, "0") == 0) ||
//            (_stricmp(buffer, "false") == 0) ||
//            (_stricmp(buffer, "no") == 0))
//        {
//            return false;
//        }
//    }
//
//    //
//    // If not found in the environment, then read it from cosmos.ini.
//    //
//    bool val = defaultValue;
//
//    if (config != NULL)
//    {
//        if (!config->GetBoolParameter(sectionName, paramName, &val))
//        {
//            val = defaultValue;
//        }
//    }
//
//    return val;
//
//}
//
//
////
//// We allow some logging settings to be configured from the environment. This
//// is useful for jobs that don't have specific settings in cosmos.ini
//// but don't want to use the cosmos.ini defaults.
////
//// e.g. to override the cosmos.ini setting and force MaxFiles=1000, set
//// localcosmoslog::MaxFiles=1000 in the environment before launching the job.
////
//int Logger::ReadIntFromEnvironmentOrConfig(
//    __in const ConfigParser *config,
//    __in PCSTR sectionName,
//    __in PCSTR paramName,
//    __in int defaultValue
//    )
//{
//    char buffer[64];
//
//    std::string envSettingName(sectionName);
//    envSettingName.append("::");
//    envSettingName.append(paramName);
//
//    //
//    // First check if an override exists in the environment.
//    //
//    DWORD result =
//        GetEnvironmentVariableA(envSettingName.c_str(), buffer, sizeof(buffer));
//
//    if ((result != 0) && (result <= sizeof(buffer)))
//    {
//        char *endPtr = 0;
//        long tmp = strtol(buffer, &endPtr, 0);
//
//        if (endPtr == NULL || !*endPtr)
//        {
//            if (tmp >= _I32_MIN && tmp <= _I32_MAX)
//            {
//                return (__int32) tmp;
//            }
//        }
//    }
//
//    //
//    // If not found in the environment, then read it from cosmos.ini.
//    //
//    int val = defaultValue;
//
//    if (config != NULL)
//    {
//        if (!config->GetIntParameter(sectionName, paramName, &val, defaultValue))
//        {
//            val = defaultValue;
//        }
//    }
//
//    return val;
//}
//
//
//__int64 Logger::ReadInt64FromEnvironmentOrConfig(
//    __in const ConfigParser *config,
//    __in PCSTR sectionName,
//    __in PCSTR paramName,
//    __in __int64 defaultValue
//    )
//{
//    char buffer[128];
//
//    std::string envSettingName(sectionName);
//    envSettingName.append("::");
//    envSettingName.append(paramName);
//
//    //
//    // First check if an override exists in the environment.
//    //
//    DWORD result =
//        GetEnvironmentVariableA(envSettingName.c_str(), buffer, sizeof(buffer));
//
//    if ((result != 0) && (result <= sizeof(buffer)))
//    {
//        char *endPtr = 0;
//        __int64 tmp = _strtoi64(buffer, &endPtr, 0);
//
//        if (endPtr == NULL || !*endPtr)
//        {
//            if (tmp >= _I64_MIN && tmp <= _I64_MAX)
//            {
//                return tmp;
//            }
//        }
//    }
//
//    //
//    // If not found in the environment, then read it from cosmos.ini.
//    //
//    __int64 val = defaultValue;
//
//    if (config != NULL)
//    {
//        if (!config->GetInt64Parameter(sectionName, paramName, &val, defaultValue))
//        {
//            val = defaultValue;
//        }
//    }
//
//    return val;
//}
//
//
//void Logger::ReadMemoryLogSettings(
//    __in ConfigParser *config,
//    __in PCSTR sectionName
//    )
//{
//    m_bUseMemoryLog =
//        ReadBoolFromEnvironmentOrConfig(
//            config, sectionName, "UseMemoryLogging", FALSE);
//}
//
//void Logger::CreateMemoryLogger(
//    __in ConfigParser *config,
//    __in PCSTR sectionName
//    )
//{
//    if (!m_bUseMemoryLog)
//    {
//        return;
//    }
//
//    MemoryLog * pMemoryLog = new MemoryLog();
//    LogAssert(pMemoryLog != NULL);
//
//    // 32 MB per role by default.
//    bool binaryMode = false;
//    int compressionLevel = 0;
//
//    pMemoryLog->Init(32 * 1024 * 1024);
//
//    m_pMemoryLog = pMemoryLog;
//}
//
//void Logger::ReadAndApplyFineGrainedTimerSetting(
//    __in ConfigParser *config,
//    __in PCSTR sectionName
//    )
//{
//    // Read whether we want to use the fine-grained timer for the logging
//    // module. Note that this setting, if enabled, affects the whole system.
//
//    char timerString[16];
//    bool newValUseFineGrainedTimer = false;
//    BOOL result;
//    MMRESULT timerResult;
//
//    result =
//        config->GetParameter(
//            sectionName,
//            "UseFineGrainedTimer",
//            timerString,
//            (sizeof(timerString) - 1));
//
//    timerString[sizeof(timerString) - 1] = '\0';
//
//    if ((result) && (_stricmp(timerString, "true") == 0)) {
//        newValUseFineGrainedTimer = true;
//    }
//
//    // Not synchronizing updating of m_bUseFineGrainedTimer and
//    // the call to timeBeginPeriod. It isn't necessary.
//
//    if (newValUseFineGrainedTimer == m_bUseFineGrainedTimer) {
//
//        // No change.
//        return;
//    }
//
//    if (newValUseFineGrainedTimer) {
//
//        timerResult = timeBeginPeriod(1);
//
//        // This call must never fail as 1ms is an allowable
//        // setting.
//        LogAssert(timerResult == TIMERR_NOERROR);
//
//    } else {
//
//        timerResult = timeEndPeriod(1);
//        LogAssert(timerResult == TIMERR_NOERROR);
//
//    }
//
//    m_bUseFineGrainedTimer = newValUseFineGrainedTimer;
//}
//
//
//// For each log destination, re-read its changeable config entries
//// e.g. whether it outputs full log source info, buffer sizes, etc.
//void Logger::RereadLogDestinationConfig(ConfigParser *config)
//{
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        m_logDestinations[i]->ReadChangeableConfig(config);
//    }
//}
//
//// Find out whether the full dump log destination is enabled
//bool Logger::FullDumpEnabled()
//{
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        if (m_logDestinations[i]->FullDumpEnabled())
//            return true;
//    }
//    return false;
//}
//
//// Remove all existing rules
//// NOTE: This runs simultaneously with people reading from the rules, without any locks
//void Logger::ClearRules()
//{
//    m_numRules = 0;
//
//    for (int i = 0; i < c_logMaxRules; i++)
//    {
//        m_rules[i].m_logDestinationMask = 0;
//        memset(m_rules[i].m_areaMask, 0, sizeof(m_rules[i].m_areaMask));
//        m_rules[i].m_levelMask = 0;
//        memset(m_rules[i].m_title, 0, sizeof(m_rules[i].m_title));
//    }
//}
//
//// Given a particular log destination (e.g. "stdout" or some set of files), find the index we are
//// using to refer to it, or -1 if not found.
//int Logger::FindLogDestination(char *outputDestinationName)
//{
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        if (m_logDestinations[i] != NULL)
//        {
//            if (strcmp(m_logDestinations[i]->m_name, outputDestinationName) == 0)
//                return i;
//        }
//    }
//
//    return -1;
//}
//
//
//// Given a particular log destination name, find its ID and return it.
//// If it doesn't have an ID, create it
//// If there is insufficient space to create one, return -1
//int Logger::FindOrCreateLogDestination(ConfigParser *config, char *name)
//{
//    int index;
//
//    index = FindLogDestination(name);
//    if (index != -1)
//        return index;
//
//    if (m_numLogDestinations >= c_logMaxLogDestinations)
//        return -1; // Insufficient space available
//
//    // stdout and debugstring are special kinds of destination
//    if (strcmp(name, c_logDestinationStdout) == 0)
//        return CreateStdoutLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationStderr) == 0)
//        return CreateStderrLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationDebugString) == 0)
//        return CreateDebugStringLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationApplicationCallback) == 0)
//        return CreateApplicationCallbackLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationTerminate) == 0)
//        return CreateTerminateLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationPopup) == 0)
//        return CreatePopupLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationDebugBreak) == 0)
//        return CreateDebugBreakLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationRDLogger) == 0)
//        return CreateRDLoggerLogDestination(config, name);
//    else if (strcmp(name, c_logDestinationFullDumpLogger) == 0)
//        return CreateFullDumpLogDestination(config, name);
//
//    // Otherwise assume it's a destination which is a set of log files
//    return CreateFileLogDestination(config, name);
//}
//
//// Helper function
//// Given a LogDestination object and its name, initialize it and put it in the list
//// Return the log ID
//// If it fails to initialize, the log destination object is deleted and -1 is returned
//int Logger::CreateLogDestinationHelper(ConfigParser *config, LogDestination *destination, char *name)
//{
//    if (destination->Init(config, name) == FALSE)
//    {
//        delete destination;
//        return -1;
//    }
//
//    m_logDestinations[m_numLogDestinations] = destination;
//
//    // Interlocked increment because other threads may be reading from this location
//    // We are the only writer, however
//    InterlockedIncrement((long *) &m_numLogDestinations);
//
//    return m_numLogDestinations-1;
//}
//
//// Create the destination which is stdout
//// Return the log ID
//int Logger::CreateStdoutLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationStdout *destination = new LogDestinationStdout();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is stderr
//// Return the log ID
//int Logger::CreateStderrLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationStderr *destination = new LogDestinationStderr();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is outputdebugstring
//// Return the log ID
//int Logger::CreateDebugStringLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationDebugString *destination = new LogDestinationDebugString();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is applicationcallback
//// Return the log ID
//int Logger::CreateApplicationCallbackLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationApplicationCallback *destination = new LogDestinationApplicationCallback();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is debugbreak
//// Return the log ID
//int Logger::CreateDebugBreakLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationDebugBreak *destination = new LogDestinationDebugBreak();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is terminate
//// Return the log ID
//int Logger::CreateTerminateLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationTerminate *destination = new LogDestinationTerminate();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is terminate
//// Return the log ID
//int Logger::CreatePopupLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationPopup *destination = new LogDestinationPopup();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is rdlogger
//// Return the log ID
//int Logger::CreateRDLoggerLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationRDLog * destination = new LogDestinationRDLog();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create the destination which is fulldump
//// Return the log ID
//int Logger::CreateFullDumpLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationFullDump * destination = new LogDestinationFullDump();
//    return CreateLogDestinationHelper(config, destination, name);
//}
//
//// Create a log destination that is actually a set of files
////
//// The file destination database is not protected by locks
//// However, other people will only be reading from destination IDs already allocated.
//// Log destination IDs are never freed.
//// Only one thread will ever be writing to this structure at the same time.
//int Logger::CreateFileLogDestination(ConfigParser *config, char *name)
//{
//    LogDestinationFile *destination = new LogDestinationFile();
//
//    int logDestinationIndex = CreateLogDestinationHelper(config, destination, name);
//
//    if (logDestinationIndex != -1 && destination->m_fBinaryMode)
//    {
//        // This is a binary destination. Add it to the array of binary destinations. 
//        LogAssert(Logger::m_numBinaryLogDestinations < c_logMaxBinaryLogDestinations);
//
//        int binaryIndex = Logger::m_numBinaryLogDestinations;
//        
//        destination->m_binaryIndex = binaryIndex;
//        Logger::m_binaryLogDestinations[binaryIndex] = logDestinationIndex;
//
//        Logger::m_numBinaryLogDestinations++;
//    }
//
//    return logDestinationIndex;
//}
//
//
//// Add a new rule
//// Returns FALSE if unsuccessful
//// title = "*" means all
//// If areaMask == NULL, it means "use all areas", otherwise areaMask must be a bitmap of size (LogID_Count/8) + 1
//BOOL Logger::AddRule(BYTE *areaMask, int levelMask, char *title, int logDestinationMask)
//{
//    if (!m_preInitDone) {
//        BOOL f = DefaultInit();
//        if (!f) {
//            return FALSE;
//        }
//    }
//
//    if (m_numRules >= c_logMaxRules)
//        return FALSE;
//
//    LogRule *rule = &m_rules[m_numRules];
//
//    rule->m_levelMask = levelMask;
//
//    if (areaMask == NULL)
//    {
//        memset(rule->m_areaMask, 0xFF, (LogID_Count/8) + 1);
//    }
//    else
//    {
//        memcpy(rule->m_areaMask, areaMask, (LogID_Count/8) + 1);
//    }
//
//    strncpy(rule->m_title, title, sizeof(rule->m_title));
//    rule->m_title[ sizeof(rule->m_title)-1] = '\0';
//    rule->m_logDestinationMask = logDestinationMask;
//
//    // This needs to be interlocked because a logging thread may be reading this value
//    InterlockedIncrement((volatile long *) &m_numRules);
//
//    return TRUE;
//}
//
//BOOL Logger::EnableApplicationLogCallback()
//{
//    if (!m_preInitDone) {
//        BOOL f = DefaultInit();
//        if (!f) {
//            return FALSE;
//        }
//    }
//
//    int index = FindLogDestination(c_logDestinationApplicationCallback);
//    if (index == -1)
//    {
//        return FALSE;
//    }
//
//    return AddRule(NULL, g_preInitializeLogLevelMask, "*", (1<<index));
//}
//
//// Add a new log callback handler
//// Returns FALSE if unsuccessful
//BOOL Logger::AddApplicationLogCallback(LoggerApplicationCallback* handler,
//                                       void* cookie)
//{
//    if (!m_preInitDone) {
//        BOOL f = DefaultInit();
//        if (!f) {
//            return FALSE;
//        }
//    }
//
//    if (m_numCallbackHandlers >= c_logMaxCallbackHandlers)
//        return FALSE;
//
//    m_callbackHandlers[m_numCallbackHandlers].m_handler = handler;
//    m_callbackHandlers[m_numCallbackHandlers].m_cookie = cookie;
//
//    // This needs to be interlocked because a logging thread may be reading this value
//    InterlockedIncrement((volatile long *) &m_numCallbackHandlers);
//
//    return TRUE;
//}
//
//// Call back all the handlers with a log entry (which is a
//// NULL-terminated string. Handlers can only be added not removed
//void Logger::SendEntryToApplicationHandlers(const char* logEntry)
//{
//    int i;
//    for (i=0; i<m_numCallbackHandlers; ++i)
//    {
//        (*(m_callbackHandlers[i].m_handler))(m_callbackHandlers[i].m_cookie,
//                                             logEntry);
//    }
//}
//
//
//void Logger::ChainUnhandledExceptionFilter()
//{
//
//    if(m_bUnhandledExceptionFilterSet == false)
//    {
//        m_bUnhandledExceptionFilterSet = true;
//        Logger::m_nextExceptionFilterChain = ::SetUnhandledExceptionFilter(Logger::LogAndExitProcess);
//    }
//}
//
//
//XS_ACTIVITY_STATE Logger::GetCurrentActivityState()
//{
//    return s_activityState;
//}
//
//void Logger::SetCurrentActivityState(
//    __in_bcount_opt(activityStateLength) BYTE* activityState,
//    __in size_t activityStateLength)
//{
//    s_activityState.ActivityState = activityState;
//    s_activityState.ActivityStateLength = activityStateLength;
//}


// Returns whether this rule matches the provided parameters
BOOL LogRule::Matches(LogID area, LogLevel level, const char *title)
{
    if ((unsigned int) area >= (unsigned int) LogID_Count)
        return FALSE;

    if ((m_areaMask[area/8] & (1 << (area & 7))) == 0)
        return FALSE;

    if ((m_levelMask & (1 << level)) == 0)
        return FALSE;

    // No title means *
    if (m_title[0] == '*')
        return TRUE;

    return !strcmp(m_title, title);
}

// Read entries from the log configuration, that can be changed at run time
// This function needs to be able to run while logging is happening on other threads,
// because it will be called by our callback function when the config file changes.
//void Logger::ReadChangeableConfig(ConfigParser *config, WCHAR *customSectionName)
//{
//    ReadRules(config, customSectionName);
//}
//
//// Read log configuration from the config file
//// This is called once when we initialize
//BOOL Logger::ReadConfig(ConfigParser *config, WCHAR *customSectionName)
//{
//    ReadChangeableConfig(config, customSectionName);
//    return TRUE;
//}
//
//// Determine the directory used for logging, and create the directory if it doesn't already exist
//// This is done only once, at startup
//BOOL Logger::SetDirectory(const char * logsDir)
//{
//    if (logsDir == NULL)
//    {
//        const char *dataDirectory = SysInfo::GetDataDir();
//        // Default directory root is <datadir>\logs
//        sprintf(m_directoryName, "%s\\Logs", dataDirectory);
//    }
//    else
//    {
//        if (strcpy_s(m_directoryName, sizeof(m_directoryName), logsDir) != 0)
//        {
//            printf("SetDirectory: strcpy_s failed. LogsDir=%s", logsDir);
//            return FALSE;
//        }
//    }
//    return TRUE;
//}
//
//// This is called from main.cpp, since Counters are not initialized until after logging
//void Logger::InitCounters()
//{
//}
//
//// InitAppAlertClient will be called only upon encountering the first LogLevel_AppAlert
//// If there are no AppAlerts, we won't initialize this at all.  Furthermore, the
//// AppAlertClient will be initialized only if Netlib was initialized by the
//// application that logged the AppAlert.
//void Logger::InitAppAlertClient()
//{
//    if (!m_preInitDone) {
//        DefaultInit();
//    }
//
//    // We shouldn't be calling this unless netlib was initialized.
//    LogAssert (Netlib::IsNetlibInitialized() == true);
//
//    // Subsequent calls to InitAppAlertClient will result in a no-op.
//    if (m_bAppAlertClientInitialized == true)
//    {
//        Log (LogID_Logging, LogLevel_Warning, "Logger",
//            "AppAlert client already initialized.  Not doing anything.");
//        return;
//    }
//
//    // Do double-check locking for the m_AppAlertClient member here.
//    if (m_AppAlertClient == NULL)
//    {
//        // enter critical section
//        AutoCriticalSection lock(&m_AppAlertInitCritSec);
//
//        // Check again
//        if (m_AppAlertClient == NULL)
//        {
//            m_AppAlertClient = new AppAlertClient();
//
//            if (m_AppAlertClient == NULL)
//            {
//                Log(LogID_Logging, LogLevel_Error, "Logger",
//                    "Failed to initialize the AppAlert client object");
//
//                m_bAppAlertClientInitialized = false;
//            }
//            else
//            {
//                // start appalertservice in client mode.
//                int ret=0;
//                ret = m_AppAlertClient->StartAppAlertClient();
//
//                if (ret != 0)
//                {
//                    Log(LogID_Logging, LogLevel_Error, "Logger","Cannot start appalert client");
//                    delete m_AppAlertClient;
//                    m_AppAlertClient = NULL;
//                    m_bAppAlertClientInitialized = false;
//                }
//                else
//                {
//                    // Initialization succeeded.
//                    // Register an atexit method to destroy the netpacketsvc object
//                    // before netlib::stop is called by the application.
//                    atexit(&Logger::ShutdownAppAlertClient);
//
//                    m_bAppAlertClientInitialized = true;
//                }
//
//            }
//        }
//        // Leave critical section at end of scope.
//    }
//}

const BOOL Logger::s_DefaultInitOk = Logger::PreInit();

// Initialize logging independently of the configuration availability.
BOOL Logger::PreInit()
{
    if (m_pPreInitCritSec == NULL) {
        m_pPreInitCritSec = new CRITSEC();
    }

    AutoCriticalSection lock(m_pPreInitCritSec);

    if (InitializeLogTagHeaders() == FALSE)
    {
        fprintf(stderr, "InitializeLogTagHeaders() failed\n");
        return (FALSE);
    }

    return (TRUE);
}

//
// Routine Description:
//
//      Default init for the logging subsystem.
//
// Notes:
//
//      Initialize logging independently of the configuration availability.
//      The default rule sends all output (only) to the stderr log destination.
//
// Arguments:
//
// ReturnValue:
//
// Assumptions:
//
// ISSUE-2010-01-21-RSHANKAR:
//
//      When support for sending output to the role instance output window in devfabric
//      is available (RDBUG# 27696), that should be added as the additional default
//      log destination since stderr goes to \dev\null in both devfabric and real fabric.
//

//BOOL Logger::DefaultInit()
//{
//    if (m_pPreInitCritSec == NULL) {
//        m_pPreInitCritSec = new CRITSEC();
//    }
//
//    AutoCriticalSection lock(m_pPreInitCritSec);
//
//    if (!s_DefaultInitOk)
//    {
//        return (FALSE);
//    }
//
//    if (!m_preInitDone) {
//        m_preInitDone = true;
//
//        ConfigParser* config = new ConfigParser("",(IMutableConfiguration*)Configuration::GenerateDefaultConfig());
//        // Pre-create log destinations in order terminate, popup, debugstring, stdout
//        // This is because we output to the highest numbered log destinations first, and we want
//        // to potentially output to stdout before terminating
//        FindOrCreateLogDestination(config, c_logDestinationTerminate);
//        FindOrCreateLogDestination(config, c_logDestinationFullDumpLogger);
//        FindOrCreateLogDestination(config, c_logDestinationPopup);
//        FindOrCreateLogDestination(config, c_logDestinationDebugBreak);
//        FindOrCreateLogDestination(config, c_logDestinationDebugString);
//        FindOrCreateLogDestination(config, c_logDestinationApplicationCallback);
//        FindOrCreateLogDestination(config, c_logDestinationRDLogger);
//        FindOrCreateLogDestination(config, c_logDestinationStdout);
//        FindOrCreateLogDestination(config, c_logDestinationStderr);
//
//        CreateDefaultRules(config);
//        delete config;
//    }
//    return (TRUE);
//}
//
//
//bool GetBuildName(__out std::string& buildName)
//{
//    const size_t MAX_BUILD_NAME_STRING_SIZE = 128;
//
//    bool result = false;
//    FILE* buildNameFile;
//
//    // clear __out variable
//    buildName.clear();
//    buildName.reserve(MAX_BUILD_NAME_STRING_SIZE);
//
//    if (fopen_s(&buildNameFile, c_buildNameFileName, "r") == 0)
//    {
//        char c = (char)fgetc(buildNameFile);
//        while (c != EOF && c != '\r' && c != '\n' && c != '\0')
//        {
//            buildName.push_back(c);
//            if (buildName.size() >= MAX_BUILD_NAME_STRING_SIZE)
//            {
//                buildName.clear();
//                break;
//            }
//            c = (char)fgetc(buildNameFile);
//        }
//
//        fclose(buildNameFile);
//    }
//
//    if (buildName.empty())
//    {
//        buildName.assign("Unknown Build");
//    }
//
//    return result;
//}
//
//// Initialize logging
//BOOL Logger::Init(WCHAR *customSectionName, const char * logsDir, BOOL slimMode)
//{
//    if (!s_DefaultInitOk)
//    {
//        return (FALSE);
//    }
//
//    if (!m_preInitDone) {
//        BOOL f = DefaultInit();
//        if (!f) {
//            return FALSE;
//        }
//    }
//
//    Logger::m_slimMode = !!slimMode;
//
//    BOOL success = FALSE;
//    ConfigParser *config = Config::OpenConfig(NULL);
//    if (config == NULL)
//    {
//        // Do not continue if we don't have a proper config file
//        fprintf(stderr, "Warning, no logging config file available, using default values\n");
//        goto exit;
//    }
//
//    // Set/create the logging directory
//    if (SetDirectory(logsDir) == FALSE)
//    {
//        fprintf(stderr, "SetDirectory() failed\n");
//        goto exit;
//    }
//
//    if (ReadConfig(config, customSectionName) == FALSE)
//    {
//        fprintf(stderr, "ReadConfig() failed\n");
//        goto exit;
//    }
//
//    if (!slimMode)
//    {
//        // Now we can set up our unhandled exception filter. Note that the logger must be initialized
//        // in order to have a functional handler.
//#ifndef _DEBUG
//        Logger::ChainUnhandledExceptionFilter();
//#endif
//
//        GetBuildName(m_buildName);
//        Log(LogID_Logging, LogLevel_Status, "Process started", "Build Name: %s; BUILD_NUMBER: %s;",
//            m_buildName.c_str(), BUILD_NUMBER);
//    }
//
//    success = SpinLogFlushThread();
//    
//exit:
//    if (config != NULL)
//        Config::CloseConfig(config);
//
//    return success;
//}
//
//
//const char* Logger::GetBuildString()
//{
//    return m_buildName.c_str();
//}
//
//void Logger::Stop()
//{
//    if (!m_preInitDone) {
//        DefaultInit();
//    }
//
//    m_bStopped = true;
//
//    if (m_hThread != NULL)
//    {
//        QueueUserAPC(&Logger::StopThread, m_hThread, 0);
//        WaitForThreadOrExit(m_hThread, m_dwThreadID);
//        CloseHandle(m_hThread);
//        m_hThread = NULL;
//    }
//    Flush();
//}
//
//void Logger::ShutdownAppAlertClient()
//{
//    if (!m_preInitDone) {
//        DefaultInit();
//    }
//
//    // stop the appalertclient
//    if (m_bAppAlertClientInitialized == true)
//    {
//        m_AppAlertClient->StopAppAlertClient();
//        m_bAppAlertClientInitialized = false;
//        delete m_AppAlertClient;
//        m_AppAlertClient = NULL;
//    }
//}
//
//bool Logger::SendAppAlert (const char * buffer)
//{
//    if (!m_preInitDone) {
//        BOOL f = DefaultInit();
//        if (!f) {
//            return false;
//        }
//    }
//
//    // These should never happen, so assert them.
//    LogAssert (buffer != NULL);
//    LogAssert (m_AppAlertClient != NULL);
//
//    // Get IP and port number to send alert packet to AppAlertServer
//    struct in_addr cli_addr;
//    unsigned short uPortNum;
//    bool bRet = false;
//    bRet = SysInfo::GetAddressAndPort(SysInfo::GetMachineName(),
//                              (ServiceType)Service_AppAlert,
//                              &cli_addr,
//                              &uPortNum);
//    if (!bRet)
//    {
//        Log(LogID_Logging, LogLevel_Error, "Logger",
//            "Could not get client IP address and port number for AppAlertServer");
//        return false;
//    }
//
//    // ::fprintf (stderr, "Logger::SendAppAlert - sending [%s]\n", buffer);
//
//    // Send buffer across as an alert packet.
//    Packet *Pkt = m_AppAlertClient->CreateAppAlertPacket(buffer);
//
//    Pkt->SetServerAddr(cli_addr.S_un.S_addr, uPortNum);
//    Pkt->m_Hdr.m_Xid = m_NumAlertsSent++;
//
//    m_AppAlertClient->SendAppAlertPacket(Pkt, 10000);
//
//    return true;
//}
//
//void Logger::SetCrashing()
//{
//    m_isCrashing = true;
//}
//
//bool Logger::IsCrashing()
//{
//    return m_isCrashing;
//}
//
//void Logger::SetNoLostLogEntries(
//    bool fNoLostLogEntries
//    )
//{
//    m_fNoLostLogEntries = fNoLostLogEntries;
//}
//
//bool Logger::CanLostLogEntries()
//{
//    return !m_fNoLostLogEntries;
//}
//
//// 
//// Only allow 1 CsTerminateProcess() or Generating crash dump to succeed.
////
//bool Logger::SetDumpOrTerminateProcess()
//{
//    if (InterlockedExchange((volatile LONG*) &m_DumpOrTerminateInProgress, 1) != 0)
//    {
//        return false;
//    }
//    return true;
//}
//
//void Logger::LogMVar(void **ppPreprocessBlock, const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, const char *fmt, va_list args)
//{
//    void *pvPreprocessBlock=NULL;
//
//    format_preprocess_block *pPreprocessBlock;
//
//    if (!m_bUseMemoryLog || (level >= LogLevel_Error))
//    {
//        // Short-circuit
//        const char* filename = ExtractFileNameFromPath(file);
//
//        LogV(filename, function, line, logID, level, title, fmt, args);
//
//        if (m_numBinaryLogDestinations == 0)
//        {
//            // Stop here if there are no binary destinations. Otherwise we need
//            // post to the memory logger thread to write it to the binary
//            // destination.
//            return;
//        }
//    }
//
//    if (m_pMemoryLog == NULL)
//    {
//        // Memory logger not initialized yet. This could happen at process startup when some threads 
//        // log before the logger initialization routine is complete.
//        return;
//    }
//
//    if(ppPreprocessBlock != NULL)
//    {
//        pvPreprocessBlock = *ppPreprocessBlock;
//    }
//
//    if(pvPreprocessBlock)
//    {
//        pPreprocessBlock = (format_preprocess_block *)pvPreprocessBlock;
//    }
//    else
//    {
//
//        const char* filename = ExtractFileNameFromPath(file);
//
//        if((pPreprocessBlock = preprocess_format(fmt)) == NULL)
//        {
//            Log(LogID_Logging, LogLevel_Error,"Failed to format output", "Calling File = %s Function = %s, Calling Line = %d ", file, function, line);
//            return;
//        }
//
//        // No storage provided so the block is temporary and must be
//        // deleted after writing the current entry.
//        bool bTemporary = (ppPreprocessBlock == NULL);
//
//        // TODO: We can have two blocks with the same UID in a given log file, if we crash and
//        // recover. This is fine but can be improved by scanning the log file when we open
//        // it and starting with (the highest UID in the file + 1).
//        FpbUidType uid = 0;
//
//        if (!bTemporary)
//        {
//            uid = InterlockedIncrement((PLONG) &g_nextBlockUid);
//        }
//
//        pPreprocessBlock->Init(filename, function, line, logID, level, bTemporary, uid);
//
//        if(ppPreprocessBlock != NULL)
//        {
//            // Save a pointer to the preprocess block in the storage provided by the caller.
//            if(InterlockedCompareExchangePointer(ppPreprocessBlock,pPreprocessBlock,0) != 0)
//            {
//                // another thread beat us to it, use their copy.
//                delete[] (char *)pPreprocessBlock;
//                pPreprocessBlock = (format_preprocess_block *)*ppPreprocessBlock;
//            }
//        }
//    }
//
//    // Determine how much we need in the memory log buffer.
//    UINT32 bytesNeeded = 0;
//
//    __try
//    {
//        // Avoid crashing if there is a mismatch between the format specifiers and the actual 
//        // parameter list e.g. Log("Account: %s, BlobName:%s", accountName)
//        // We have seen roles cycling because of this relatively minor defect.
//        // Instead of crashing, log the location of the offending log statement. The log maintains
//        // the logging level of the offending log statement.
//        bytesNeeded = LogEntry::GetBytesNeeded(pPreprocessBlock, args);
//    }
//    __except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
//                                            EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
//    {
//        Log(LogID_Logging, (LogLevel)pPreprocessBlock->m_level,
//            "Invalid parameter list", 
//            "Mismatch between the format specifiers and the parameter list. Calling File = %s Function = %s, Calling Line = %d, Format spec = %s", 
//            pPreprocessBlock->m_filename, 
//            pPreprocessBlock->m_function, 
//            pPreprocessBlock->m_line, 
//            pPreprocessBlock->m_format);
//
//            return;
//    }
//
//    // Add space for the commit token.
//    bytesNeeded += sizeof(UINT64);
//
//    // Write the entry to the given address.
//    // We write at pAddress + 1 because the commit token should be written first.
//    FILETIME ft;
//    GetSystemTimeAsFileTime(&ft);
//
//    // Attempt to reserve space in the memory log buffer
//    UINT32 bytesReserved;
//
//RetryReserveBuffer:
//    PVOID pAddress = m_pMemoryLog->ReserveMemLogBuffer(bytesNeeded, &bytesReserved);
//    if (pAddress == NULL)
//    {
//        // If the app doesn't want to lose log entries, retry until success
//        if (CanLostLogEntries() || m_pMemoryLog->IsOnLogOutputThread())
//        {
//            // Memory log buffer is full. We drop this entry.
//            m_pMemoryLog->AddLostEntries(1, 0, 0);
//
//            if (pPreprocessBlock->IsTemporary())
//            {
//                delete[] (char *)pPreprocessBlock;
//                pPreprocessBlock = NULL;
//            }
//
//            return;
//        }
//
//        Sleep (10);
//        goto RetryReserveBuffer;
//    }
//
//    WriteLogEntry(
//                    (UINT64 *)pAddress + 1,
//                    bytesReserved - sizeof(UINT64),
//                    pPreprocessBlock,
//                    title,
//                    GetCurrentThreadId(),
//                    ft,
//                    GetCurrentActivityId(),
//                    GetCurrentEntryPointId(),
//                    args);
//
//    // By setting this we tell the memory logger that the entry is complete.
//    m_pMemoryLog->CommitMemLogBuffer(pAddress, bytesReserved);
//}
//
////
//// Create a preprocess block based on the supplied information. The block will
//// be used subsequently for faster logging.
//// Returns the preprocess block. Caller is responsible to call delete.
//// Returns NULL if an error occurs.
////
//format_preprocess_block * Logger::CreatePreprocessFormat(
//    __in LogID logID,
//    __in LogLevel level,
//    __in const char *file,
//    __in const char *function,
//    __in const int line,
//    __in const char *title,
//    __in const char *fmt)
//{
//    format_preprocess_block *pPreprocessBlock;
//
//    const char* filename = ExtractFileNameFromPath(file);
//
//    if((pPreprocessBlock = preprocess_format(fmt)) == NULL)
//    {
//        Log(LogID_Logging, LogLevel_Error,"Failed to format output", "Calling File = %s Function = %s, Calling Line = %d ", file, function, line);
//        goto exit;
//    }
//
//    // TODO: We can have two blocks with the same UID in a given log file, if we crash and
//    // recover. This is fine but can be improved by scanning the log file when we open
//    // it and starting with (the highest UID in the file + 1).
//    FpbUidType uid = 0;
//
//    uid = InterlockedIncrement((PLONG) &g_nextBlockUid);
//
//    pPreprocessBlock->Init(filename, function, line, logID, level, false, uid);
//
//exit:
//
//    return pPreprocessBlock;
//}
//
////
//// Determine whether a given log entry must be output as formatted text
//// or raw binary, or both by matching the entry's logID and level against
//// the rules database.
////
//// logID [in] - Supplies the logID of the entry.
////
//// level [in] - Supplies the severity level of the entry.
////
//// pMask [out] - Returns a bitmask identifying the log destinations that
////               will consume a given entry.
////
//// pNeedFormatted [out] - Returns true if one or more entries in the bitmask
////                        accept formatted log output.
////
//// pBinaryLogDestinationCount [in] - Returns the number of binary log destinations where the log
////                                   entry should be written to.
////
//// pBinaryLogDestinationIndex [in] - Returns an array of binary log destinations where the log entry
////                                   should be written to.
////
//void Logger::GetOutputModes(
//    __in LogID logID,
//    __in LogLevel level,
//    __in_opt PCSTR title,
//    __out bool * pfNeedFormatted,
//    __out INT * pBinaryLogDestinationCount,
//    __out_ecount(*pBinaryLogDestinationCount) INT * pBinaryLogDestinationIndex)
//{
//    UINT logDestinationMask = 0;
//
//    INT binaryLogDestinationCount = 0;
//
//    *pfNeedFormatted = false;
//    *pBinaryLogDestinationCount = 0;
//
//    // Build the list of log destinations that will consume this entry.
//    for (int i = 0; i < m_numRules; i++)
//    {
//        if (m_rules[i].Matches(logID, level, title))
//        {
//            logDestinationMask |= m_rules[i].m_logDestinationMask;
//        }
//    }
//
//    // Check whether each matching destination can consume binary output.
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        LogDestination* pDest = m_logDestinations[i];
//
//        if ((logDestinationMask >> i) & 1)
//        {
//            if (pDest->IsBinary())
//            {
//                pBinaryLogDestinationIndex[binaryLogDestinationCount] = i;
//                binaryLogDestinationCount++;
//            }
//            else
//            {
//                // If a single destination needs text output then we must format.
//                *pfNeedFormatted = true;
//            }
//        }
//    }
//
//    *pBinaryLogDestinationCount = binaryLogDestinationCount;
//}
//
////
//// Send a raw (binary) log entry to one or more log destinations. Optionally
//// outputs the format preprocess block (FPB) if it hasn't been output in the
//// current epoch.
////
//// pFormatBlockBuffer [in] - Supplies a serialized FPB describing the log
////                           entry.
////
//// formatBlockLen [in] - Supplies the length of the serialized FPB.
////
//// pEntryBuffer [in] - Supplies the serialized log entry.
////
//// entryLen [in] - Supplies the length of the serialized log entry.
////
//// logDestinationMask[in] - Supplies a bitmask identifying the log
////                          destinations that will consume the log entry.
////
//// epoch [in] - Supplies the caller's epoch. If the actual epoch has
////              changed then the log entry is no longer valid and cannot
////              be logged.
////
//// minTimestamp [in] - The minimum timestamp across all log entries logged in
////                     this call.
////
//// maxTimestamp [in] - The minimum timestamp across all log entries logged in
////                     this call.
////
//// formatBlockNewEpoch [out] - Returns The current logging epoch.
////
//bool Logger::LogBinaryInEpoch(
//    __in_bcount(formatBufferLen) const PVOID pFormatBlockBuffer,
//    __in INT formatBlockLen,
//    __in_bcount(entryLength) const PVOID pEntryBuffer,
//    __in INT entryLen,
//    __in INT binaryLogDestinationIndex,
//    __in INT64 epoch,
//    __in UINT64 minTimestamp,
//    __in UINT64 maxTimestamp
//    )
//{
//    bool fSucceeded = true;
//    LogDestination* pDest = m_logDestinations[binaryLogDestinationIndex];
//
//    pDest->LockBuffer();
//
//    fSucceeded &=
//        pDest->AppendBinaryDataInEpoch(
//            NULL, 0,
//            pFormatBlockBuffer,
//            formatBlockLen,
//            pEntryBuffer, entryLen,
//            epoch,
//            minTimestamp,
//            maxTimestamp);
//
//    pDest->UnlockBuffer();
//
//    return fSucceeded;
//}
//
//
//bool Logger::ReserveBytes(
//    __in INT binaryLogDestinationIndex,
//    __in INT bytesNeeded,
//    __out INT64 * pEpoch)
//{
//    bool fSucceeded = false;
//    LogDestination* pDest = m_logDestinations[binaryLogDestinationIndex];
//
//    pDest->LockBuffer();
//
//    fSucceeded = pDest->ReserveBytes(bytesNeeded, pEpoch);
//
//    pDest->UnlockBuffer();
//
//    return fSucceeded;
//}

// Maps log levels to characterid
// Debug,Info,Status,Warning,Error,AppAlert,Assert
static char s_logLevelMap[] = "diswexa";

// Maps hexadecimal digit to the corresponding ASCII character.
static char s_HexDigitMap[] = "0123456789abcdef";

extern void GuidWrite2(char *szOut, size_t cbOut, const GUID *pGuid, bool fBraces = true);

#define COPY_TO_BUFFER_BYTES(buffer, source, length)\
    memcpy(buffer, source, length);\
    buffer += (length);

#define COPY_TO_BUFFER_STR(buffer, str)\
    while (*str) *buffer++ = *str++;

#define COPY_TO_BUFFER_INT(buffer, bufferLen, value)\
    _itoa_s(value, buffer, bufferLen, 10);\
    buffer += strlen(buffer);

#define COPY_TO_BUFFER_GUID(buffer, guid)\
    GuidWrite2(buffer, c_guidStringLen, guid, false);\
    buffer += c_guidStringLen;

#define COPY_TO_BUFFER_2DIGITS(buffer, value)\
    *buffer++ = (char)('0' + (value) / 10);\
    *buffer++ = (char)('0' + (value) % 10);

#define COPY_TO_BUFFER_3DIGITS(buffer, value) \
    *buffer++ = (char)('0' + (value) / 100);\
    *buffer++ = (char)('0' + ((value) % 100) / 10);\
    *buffer++ = (char)('0' + (value) % 10);

#define COPY_TO_BUFFER_4DIGITS(buffer, value) \
    *buffer++ = (char)('0' + (value) / 1000);\
    *buffer++ = (char)('0' + ((value) % 1000) / 100);\
    *buffer++ = (char)('0' + ((value) % 100) / 10);\
    *buffer++ = (char)('0' + (value) % 10);

#define COPY_TO_BUFFER_UINT32_HEX(buffer, value)\
    *buffer++ = s_HexDigitMap[((value) >> 28) & 0xF];\
    *buffer++ = s_HexDigitMap[((value) >> 24) & 0xF];\
    *buffer++ = s_HexDigitMap[((value) >> 20) & 0xF];\
    *buffer++ = s_HexDigitMap[((value) >> 16) & 0xF];\
    *buffer++ = s_HexDigitMap[((value) >> 12) & 0xF];\
    *buffer++ = s_HexDigitMap[((value) >>  8) & 0xF];\
    *buffer++ = s_HexDigitMap[((value) >>  4) & 0xF];\
    *buffer++ = s_HexDigitMap[((value)      ) & 0xF];

// Static strings used by the log entry text formatting code
static CHAR s_activityIdPrefix[] = " ActivityId=\"";
static CHAR s_entryIdPrefix[] = " EntryId=\"";
static CHAR s_filePrefix[] = " SrcFile=\"";
static CHAR s_funcPrefix[] = " SrcFunc=\"";
static CHAR s_linePrefix[] = " SrcLine=\"";
static CHAR s_pidPrefix[] = " Pid=\"";
static CHAR s_tidPrefix[] = " Tid=\"";
static CHAR s_tsPrefix[] = " TS=\"0x";


//
// Function to construct the log entry string. This adds the overhead of an
// extra function call in the logging path but it allows a log entry to be
// formatted in an offline mode without duplicating the code.
//
char *Logger::MakeLogEntry(
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
    )
{
    int   srcLen = 0;
    PCHAR bufferEnd = buffer + (bufferLen - 3); // Leave space for '\r', '\n', '\0'.

    // In compact-mode formatting, source info is included only if it is non-empty
    if (fNeedSourceInfo &&
        (!fCompact || (file != NULL && *file != 0) || (function != NULL && *function != 0) || line != 0))
    {
        while (true)
        {
            const int lineMaxStrlen = 20;   // Use max 20 digits for line number.

            int   fileStrlen = 0;
            int   funcStrlen = 0;
            int   lineStrlen = 0;

            // Remove the path prefix from the source filename
            // Take everything after the last slash
            const char *p = strrchr(file, '\\');
            if (p != NULL)
                file = p+1;

            // Old code didn't ensure balanced quotes, which could make regular
            // expression searches unreliable through storage diagnostics.
            //
            // Old Code:
            //
            // srcLen = _snprintf(sourceInfo, sizeof(sourceInfo), "SrcFile=\"%s\" SrcFunc=\"%s\" SrcLine=\"%d\" ", file, function, line);

            PCHAR sourceInfoStart = sourceInfo;

            //
            // Source File, Source Function, Source Line (eg SrcFile="xxx"SrcFunc="yyy"SrcLine="z")
            //
            // For each field we first check whether enough space for the prefix,
            // field, suffix and NULL terminator. If not, we bail out.
            //

            // Source File
            fileStrlen = (int)strlen(file);

            if ((srcBufferLen - srcLen) < (int)(sizeof(s_filePrefix) + fileStrlen + 1))
            {
                // don't copy anything
                break;
            }

            COPY_TO_BUFFER_BYTES(sourceInfo, s_filePrefix, sizeof(s_filePrefix) - 1);
            COPY_TO_BUFFER_BYTES(sourceInfo, file, fileStrlen);
            *sourceInfo++ = '\"';
            *sourceInfo = 0;

            srcLen = (int)(sourceInfo - sourceInfoStart);


            // Source Function
            funcStrlen = (int)strlen(function);

            if ((srcBufferLen - srcLen) < (int)(sizeof(s_funcPrefix) + funcStrlen + 1))
            {
                // don't copy anything
                break;
            }


            COPY_TO_BUFFER_BYTES(sourceInfo, s_funcPrefix, sizeof(s_funcPrefix) - 1);
            COPY_TO_BUFFER_BYTES(sourceInfo, function, funcStrlen);
            *sourceInfo++ = '\"';
            *sourceInfo = 0;

            srcLen = (int)(sourceInfo - sourceInfoStart);

            // Line
            if ((srcBufferLen - srcLen) < (sizeof(s_linePrefix) + lineMaxStrlen + 1))
            {
                // don't copy anything
                break;
            }

            COPY_TO_BUFFER_BYTES(sourceInfo, s_linePrefix, sizeof(s_linePrefix) - 1);
            COPY_TO_BUFFER_INT(sourceInfo, srcBufferLen - (sourceInfo - sourceInfoStart), line);
            *sourceInfo++ = '\"';
            *sourceInfo = 0;

            srcLen = (int)(sourceInfo - sourceInfoStart);

            break;
        }
    }

    // Put in date,area,title,

    // Assume that the date, area, and title cannot overflow the end of the localBuffer
    // This is a reasonable assumption because date is only 20 characters, areas are hard-coded names,
    // and title is a user supplied constant string
    *buffer++ = s_logLevelMap[level - LogLevel_Debug];
    *buffer++ = ',';

    // Formatted datetime stamp
    SYSTEMTIME utc;
    FILETIME ft;
    FILETIME *pft;

    if(ft_in == NULL)
    {
        GetSystemTimeAsFileTime(&ft);
        pft = &ft;
    }
    else
    {
        pft = ft_in;
    }

    FileTimeToSystemTime(pft, &utc);

    // Timestamp formatted as MM/DD/YYYY HH:mm:ss[.xxx]
    COPY_TO_BUFFER_2DIGITS(buffer, utc.wMonth);
    *buffer++ = '/';

    COPY_TO_BUFFER_2DIGITS(buffer, utc.wDay);
    *buffer++ = '/';

    COPY_TO_BUFFER_4DIGITS(buffer, utc.wYear);
    *buffer++ = ' ';

    COPY_TO_BUFFER_2DIGITS(buffer, utc.wHour);
    *buffer++ = ':';

    COPY_TO_BUFFER_2DIGITS(buffer, utc.wMinute);
    *buffer++ = ':';

    COPY_TO_BUFFER_2DIGITS(buffer, utc.wSecond);

    if (g_fMillisecondTimestamps)
    {
        *buffer++ = '.';
        COPY_TO_BUFFER_3DIGITS(buffer, utc.wMilliseconds);
    }

    *buffer++ = ',';

    // Log area
    if (!fCompact)
    {
        CHAR * logIdName = g_LogIDNames[logID];
        COPY_TO_BUFFER_STR(buffer, logIdName);
        *buffer++ = ',';
    }

    //Title can't have ','s, otherwise, the collection server won't be able to
    //parse the right field to db.
    // Leave 255 characters for putting PID, TID and log data.
    const char *pch = title;
    *titleStart = buffer;
    if (!fCompact)
    {
        while (*pch && buffer < bufferEnd - 255)
        {
            *buffer++ = (',' == *pch) || ('\r' == *pch) || ('\n' == *pch) ?
                '.' : *pch; ++pch;
        }

        *buffer++ = ',';
    }

    // Record where the title ends
    // This is in case we need to log source file info, so we know where to insert it
    *titleEnd = buffer;

    // write the current activity id.
    GUID activityId = (pActivityId == NULL ?
                                    GetCurrentActivityId() : *pActivityId);
    if(activityId != nullGUID)
    {
        COPY_TO_BUFFER_BYTES(buffer, s_activityIdPrefix, sizeof(s_activityIdPrefix) - 1);
        COPY_TO_BUFFER_GUID(buffer, &activityId);
        *buffer++ = '\"';
    }

    // write the entry point's activity id.
    GUID entryPointId = (pEntryPointId == NULL ?
                                    GetCurrentEntryPointId() : *pEntryPointId);
    if(entryPointId != nullGUID)
    {
        COPY_TO_BUFFER_BYTES(buffer, s_entryIdPrefix, sizeof(s_entryIdPrefix) - 1);
        COPY_TO_BUFFER_GUID(buffer, &entryPointId);
        *buffer++ = '\"';
    }

    // write the processid and threadid
    if (pid == 0)
    {
        pid = GetCurrentProcessId();
        tid = GetCurrentThreadId();
    }

    // PID
    COPY_TO_BUFFER_BYTES(buffer, s_pidPrefix, sizeof(s_pidPrefix) - 1);
    COPY_TO_BUFFER_INT(buffer, bufferEnd - buffer, pid);
    *buffer++ = '\"';


    // TID
    COPY_TO_BUFFER_BYTES(buffer, s_tidPrefix, sizeof(s_tidPrefix) - 1);
    COPY_TO_BUFFER_INT(buffer, bufferEnd - buffer, tid);
    *buffer++ = '\"';

    // TimeStamp
    COPY_TO_BUFFER_BYTES(buffer, s_tsPrefix, sizeof(s_tsPrefix) - 1);
    COPY_TO_BUFFER_UINT32_HEX(buffer, pft->dwHighDateTime);
    COPY_TO_BUFFER_UINT32_HEX(buffer, pft->dwLowDateTime);
    *buffer++ = '\"';
    *buffer++ = ' ';

    *messageStart = buffer;

    // For each tag provided, convert into Name=Value pair and add to buffer
    while (tag != LogTag_End)
    {
        // Strip out type information from the LogTag and get just the log tag index
        int index = LogTagToIndex(tag);
        if (index < 0 || index >= LogTagCountIndex)
            break; // Index out of range

        // Append Name=" to the buffer
        if (buffer + g_logTagNameHeaders[index].m_length >= bufferEnd)
            break; // Not enough room, stop here

        memcpy(buffer, g_logTagNameHeaders[index].m_header, g_logTagNameHeaders[index].m_length);
        buffer += g_logTagNameHeaders[index].m_length;

        // Now append Value" to the buffer
        switch (LogTagToType(tag))
        {
            case LogTagType_String:
            {
                char *string = va_arg(args, char*);

                if (string == NULL)
                    string = "(null)";

                int   stringLength = (int) strlen(string);

                const char * pchEnd;

                if ((buffer + stringLength + 3) >= bufferEnd)
                {
                    // Copy what we can
                    int availableSpace = (int) (bufferEnd - buffer - 3);

                    if (availableSpace > 0)
                    {
                        pch = string;
                        pchEnd = string + availableSpace;
                        while (pch < pchEnd)
                        {
                            *buffer++ = (('\r' == *pch) || ('\n' == *pch)) ?
                                '.' : *pch;
                            ++pch;
                        }
                        *buffer++ = '\"';
                        *buffer++ = ' ';
                    }

                    goto exitNoRoom; // Not enough room, stop here
                }

                pch = string;
                pchEnd = string + stringLength;
                while (pch < pchEnd)
                {
                    *buffer++ = (('\r' == *pch) || ('\n' == *pch)) ?
                        '.' : *pch;
                    ++pch;
                }
                *buffer++ = '\"';
                *buffer++ = ' ';
                break;
            }

            default:

                // Other format tags are not actively used by the logging system
                // so we move them out to another function.
                if (!FormatTag(&buffer, bufferEnd, tag, &args))
                {
                    // Not enough room.
                    goto exitNoRoom;
                }

                break;
        }

        tag = va_arg(args, LogTag);
    }

exitNoRoom:

    *pSrcLen = srcLen;

    // Add \r\n\0
    *buffer++ = '\r';
    *buffer++ = '\n';
    *buffer = '\0'; // Don't increment buffer, because we don't want to write out a null terminator

    return buffer;
}


//
// Function to format tags other than LogTagType_String. This is legacy
// and not being used by XStore.
//
bool Logger::FormatTag(
    __in char ** bufferPtr,
    __in char * bufferEnd,
    __in LogTag tag,
    __in va_list * args)
{
    bool fitted = false;
    const char * pch = NULL;

    char * buffer = *bufferPtr;

    switch (LogTagToType(tag))
    {
        default:
            //LogAssert(FALSE); // Error, unknown parameter type!
			//jwesker

        case LogTagType_VarString:
        {
            int stringLength = (int)va_arg((*args), int);
            char *string = va_arg((*args), char*);

            if (string == NULL || stringLength == 0)
                string = "(null)";

            const char * pchEnd;

            if ((buffer + stringLength + 3) >= bufferEnd)
            {
                // Copy what we can
                int availableSpace = (int) (bufferEnd - buffer - 3);

                if (availableSpace > 0)
                {
                    pch = string;
                    pchEnd = string + availableSpace;
                    while (pch < pchEnd && *pch != '\0')
                    {
                        *buffer++ = (('\r' == *pch) || ('\n' == *pch)) ?
                            '.' : *pch;
                        ++pch;
                    }
                    *buffer++ = '\"';
                    *buffer++ = ' ';
                }

                goto exitNoRoom; // Not enough room, stop here
            }

            pch = string;
            pchEnd = string + stringLength;
            while (pch < pchEnd && *pch != '\0')
            {
                *buffer++ = (('\r' == *pch) || ('\n' == *pch)) ?
                    '.' : *pch;
                ++pch;
            }
            *buffer++ = '\"';
            *buffer++ = ' ';
            break;
        }

        case LogTagType_WideString:
        {
            wchar_t *string = va_arg((*args), wchar_t*);

            if (string == NULL)
                string = L"(null)";

            int   stringLength = (int) wcslen(string);
            const wchar_t * wpchEnd;
            const wchar_t * wpch;

            if ((buffer + stringLength + 3) >= bufferEnd)
            {
                // Copy what we can
                int availableSpace = (int) (bufferEnd - buffer - 3);

                if (availableSpace > 0)
                {
                    wpch = string;
                    wpchEnd = string + availableSpace;
                    while (wpch < wpchEnd)
                    {
                        *buffer++ = (('\r' == *wpch) || ('\n' == *wpch)) ?
                            '.' : *((char*)wpch);
                        ++wpch;
                    }
                    *buffer++ = '\"';
                    *buffer++ = ' ';
                }

                goto exitNoRoom; // Not enough room, stop here
            }

            wpch = string;
            wpchEnd = string + stringLength;
            while (wpch < wpchEnd)
            {
                *buffer++ = (('\r' == *wpch) || ('\n' == *wpch)) ?
                    '.' : *((char*)wpch);
                ++wpch;
            }
            *buffer++ = '\"';
            *buffer++ = ' ';
            break;
        }

        case LogTagType_VarWideString:
        {
            int   stringLength = (int) va_arg((*args), int);
            wchar_t *string = va_arg((*args), wchar_t*);

            if (string == NULL || stringLength == 0)
                string = L"(null)";

            const wchar_t * wpchEnd;
            const wchar_t * wpch;

            if ((buffer + stringLength + 3) >= bufferEnd)
            {
                // Copy what we can
                int availableSpace = (int) (bufferEnd - buffer - 3);

                if (availableSpace > 0)
                {
                    wpch = string;
                    wpchEnd = string + availableSpace;
                    while (wpch < wpchEnd && *wpch != L'\0')
                    {
                        *buffer++ = (('\r' == *wpch) || ('\n' == *wpch)) ?
                            '.' : *((char*)wpch);
                        ++wpch;
                    }
                    *buffer++ = '\"';
                    *buffer++ = ' ';
                }

                goto exitNoRoom; // Not enough room, stop here
            }

            wpch = string;
            wpchEnd = string + stringLength;
            while (wpch < wpchEnd && *wpch != L'\0')
            {
                *buffer++ = (('\r' == *wpch) || ('\n' == *wpch)) ?
                    '.' : *((char*)wpch);
                ++wpch;
            }
            *buffer++ = '\"';
            *buffer++ = ' ';
            break;
        }

        case LogTagType_Int32:
        {
            // Ensure enough room for <number>"<space>
            // 12 ascii chars will be large enough to hold a 32-bit int
            if ((buffer + (3+12)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            int int32Value = va_arg((*args), int);
            buffer += sprintf(buffer, "%d\" ", int32Value);
            break;
        }

        case LogTagType_UInt32:
        {
            // Ensure enough room for <number>"<space>
            // 12 ascii chars will be large enough to hold a 32-bit int
            if ((buffer + (3+12)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            int int32Value = va_arg((*args), int);
            buffer += sprintf(buffer, "%u\" ", int32Value);
            break;
        }

        case LogTagType_UInt64:
        {
            // Ensure enough room for <number>"<space>
            if ((buffer + (3+24)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            unsigned __int64 int64Value = va_arg((*args), unsigned __int64);
            buffer += sprintf(buffer, "%I64u\" ", int64Value);
            break;
        }

        case LogTagType_Hex32:
        {
            // Ensure enough room for 0x<8 digits>"<space>
            // 12 ascii chars will be large enough to hold a 0x32-bit hex
            if ((buffer + (3+12)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            int int32Value = va_arg((*args), int);
            buffer += sprintf(buffer, "0x%08X\" ", int32Value);
            break;
        }

        case LogTagType_Int64:
        {
            // Ensure enough room for <number>"<space>
            if ((buffer + (3+24)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            __int64 int64Value = va_arg((*args), __int64);
            buffer += sprintf(buffer, "%I64d\" ", int64Value);
            break;
        }

        case LogTagType_Hex64:
        {
            // Ensure enough room for 0x<16 digits>"<space>
            if ((buffer + (3+20)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            __int64 int64Value = va_arg((*args), __int64);
            buffer += sprintf(buffer, "0x%016I64X\" ", int64Value);
            break;
        }

        // For floats, the compiler will promote to double when it passes it in on the stack
        // Therefore, you have to take a double off the stack; if you take off a float, you will crash
        case LogTagType_Float:
        {
            // Ensure enough room for <number>"<space>
            if ((buffer + (3+24)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            double doubleValue = va_arg((*args), double);
            buffer += sprintf(buffer, "%f\" ", doubleValue);
            break;
        }

        case LogTagType_Time:
        {
            // Ensure enough room for <number>"<space>
            if ((buffer + (3+20)) >= bufferEnd)
                goto exitNoRoom; // Not enough room, stop here

            unsigned __int64 int64Value = va_arg(args, unsigned __int64);

            SYSTEMTIME systemTime;

            if (FileTimeToSystemTime((FILETIME *)&int64Value, &systemTime))
            {
                buffer += sprintf(
                    buffer,
                    "%02d/%02d/%d %02d:%02d:%02d\" ",
                    systemTime.wMonth,
                    systemTime.wDay,
                    systemTime.wYear,
                    systemTime.wHour,
                    systemTime.wMinute,
                    systemTime.wSecond
                );
            }
            else
            {
                buffer += sprintf(buffer, "Incorrect time\" ");
            }

            break;
        }
    }

    fitted = true;

exitNoRoom:

    *bufferPtr = buffer;

    return fitted;
}

//
//void Logger::LogV(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, LogTag tag, va_list args,
//                  FILETIME *ft_in, DWORD tid, DWORD pid, GUID *pActivityId, GUID *pEntryPointId)
//{
//    DWORD        lastWin32Error;
//    unsigned int logDestinationMask = 0;
//    char         localBuffer[c_logMaxEntrySize];
//    char         sourceInfo[c_logMaxSourceInfoSize]; // space for source file info (file,function,line). Not null terminated
//    int          srcLen = 0;
//    bool         fNeedSourceInfo = false;
//
//    if (!m_preInitDone) {
//        BOOL f = DefaultInit();
//        if (!f) {
//            return;
//        }
//    }
//
//    if (level == LogLevel_Assert && m_bUnhandledExceptionFilterSet == false)
//    {
//        WriteMiniDump (c_minidumpTimeout, false /*fullDump*/);
//        ::Terminate(1);
//    }
//
//    // Check for invalid log entry ID
//    if (((unsigned int) logID) >= (unsigned int) LogID_Count)
//        return;
//
//    // See which output destinations we accumulate for this log entry
//    for (int i = 0; i < m_numRules; i++)
//    {
//        if (m_rules[i].Matches(logID, level, title))
//            logDestinationMask |= m_rules[i].m_logDestinationMask;
//    }
//
//    // If there are no log destinations that matched any rules, then we don't log this
//    if (logDestinationMask == 0)
//    {
//        return;
//    }
//
//    // Determine whether any log destination wants full source info
//    // If true, we need to do log formatting twice
//    sourceInfo[0] = '\0';
//
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        if (((logDestinationMask >> i) & 1) && (m_logDestinations[i]->m_logSourceInfo))
//        {
//            fNeedSourceInfo = true;
//            break;
//        }
//    }
//
//    // Save the last Win32 error in case we overwrite it
//    lastWin32Error = GetLastError();
//
//    PCHAR titleStart;
//    PCHAR titleEnd;
//    PCHAR messageStart;
//
//    char *buffer =
//        MakeLogEntry(
//            localBuffer, sizeof(localBuffer),
//            sourceInfo, sizeof(sourceInfo),
//            fNeedSourceInfo,
//            file, function, line, logID, level,
//            title, tag, args,
//            ft_in, tid, pid, pActivityId, pEntryPointId,
//            Logger::m_slimMode,
//            &srcLen,
//            &titleStart,
//            &titleEnd,
//            &messageStart);
//
//    if (buffer == NULL)
//    {
//        // Hit unrecoverable error while formatting the log entry.
//        return;
//    }
//
//    // Go through each log destination in the mask, and output the log info there
//
//    bool fIncludeMs = g_fMillisecondTimestamps;
//
//    // Start with the highest numbered log destinations first
//    // The reason is, the low ordered log destinations are created as Assert, Terminate, etc., and we
//    // want to write to the log files before terminating the process
//    for (int curDestination = m_numLogDestinations-1; curDestination >= 0; curDestination--)
//    {
//        LogDestination* dest = m_logDestinations[curDestination];
//
//        // If the destination is in binary mode then don't write formatted output.
//        if (((logDestinationMask >> curDestination) & 1) && (!dest->IsBinary()))
//        {
//            dest->LockBuffer();
//
//            if (dest->AcceptStructuredData())
//            {
//                dest->AppendStructuredData(localBuffer,
//                        (char*)file, (char*)function, line,
//                        g_LogIDNames[logID],
//                        titleStart, (int) (titleEnd - titleStart),
//                        messageStart, (int) (buffer - messageStart));
//            }
//            else
//            {
//                dest->AppendData(localBuffer, (int) (titleEnd-localBuffer),
//                             sourceInfo, (dest->m_logSourceInfo) ? srcLen : 0,
//                             titleEnd, (int) (buffer - titleEnd));
//            }
//
//            dest->UnlockBuffer();
//        }
//    }
//
//    if (level >= LogLevel_Assert)
//    {
//        Logger::Flush(true);
//
//        int curDestination = m_numLogDestinations-1;
//        // if the list of destination did not include popup, debugbreak or terminate, call it
//        for (; curDestination >= 0; curDestination--)
//        {
//            if ((logDestinationMask >> curDestination) & 1)
//            {
//                LogDestination* dest = m_logDestinations[curDestination];
//
//                if (0 == ::strcmp(c_logDestinationPopup, dest->m_name) ||
//                    0 == ::strcmp(c_logDestinationDebugBreak, dest->m_name))
//                {
//                    break;
//                }
//            }
//        }
//        if (curDestination < 0)
//        {
//            curDestination = FindLogDestination(c_defaultAssertDestination);
//            LogDestination* dest = m_logDestinations[curDestination];
//            dest->LockBuffer();
//
//            dest->AppendData(localBuffer, (int) (titleEnd-localBuffer),
//                             sourceInfo, (dest->m_logSourceInfo) ? srcLen : 0,
//                             titleEnd, (int) (buffer - titleEnd));
//
//            dest->UnlockBuffer();
//        }
//    }
//
//    SetLastError(lastWin32Error);
//}
//
//void Logger::LogV(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, LogTag tag, ...)
//{
//    va_list args;
//    va_start(args, tag);
//    LogV(file, function, line, logID, level, title, tag, args);
//    va_end(args);
//}
//
//void Logger::LogVEx(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title,
//                  FILETIME *ft, DWORD tid, DWORD pid, GUID *pActivityId, GUID *pEntryPointId, LogTag tag, ... )
//{
//    va_list args;
//    va_start(args, tag);
//    LogV(file, function, line, logID, level, title, tag, args, ft, tid, pid, pActivityId, pEntryPointId);
//    va_end(args);
//}
//
//// This function is called directly by the Log() macro
//// fmt+args may be NULL, indicating no formatting/data is present
//void Logger::LogV(const char *file, const char *function, const int line, LogID logID, LogLevel level, const char *title, const char *fmt, va_list args)
//{
//    if (fmt == NULL)
//    {
//        LogV(file, function, line, logID, level, title, LogTag_End);
//    }
//    else
//    {
//        char    LocalBuffer[c_logMaxEntrySize];
//        size_t  count;
//
//        // empty string in case formatting fails.
//        LocalBuffer[0]='\0';
//
//        count = VsprintfEx(LocalBuffer, sizeof(LocalBuffer), fmt, args);
//
//        // Force null termination
//        LocalBuffer[sizeof(LocalBuffer)-1] = '\0';
//
//        LogV(file, function, line, logID, level, title, LogTag_String1, LocalBuffer, LogTag_End);
//    }
//}
//
//PREFAST_NO_EXIT
//void LogAssertV(const char *file, const char *function, const int line, LogID logID, const char *title)
//{
//    GetBaseLogger()->LogM(NULL, file, function, line, logID, LogLevel_Assert, title);
//}
//
//PREFAST_NO_EXIT
//void LogAssertV(const char *file, const char *function, const int line, LogID logID, const char *title, const char *fmt, ...)
//{
//    va_list args;
//    va_start(args, fmt);
//
//    GetBaseLogger()->LogMVar(NULL, file, function, line, logID, LogLevel_Assert, title, fmt, args);
//
//    va_end(args);
//}
//
//// Flush the contents of all log buffers for all log destinations
//void Logger::Flush(bool flushBuffers /*= false*/)
//{
//    if (m_pMemoryLog != NULL)
//    {
//        m_pMemoryLog->Flush();
//    }
//
//    if (!m_preInitDone) {
//        DefaultInit();
//    }
//
//    // m_numLogDestinations can be increased by another thread, but that's ok
//    for (int i = 0; i < m_numLogDestinations; i++)
//    {
//        m_logDestinations[i]->Flush(flushBuffers);
//    }
//}

// Base initialization method for a log destination
// Initialization methods for derived classes should call this first
//BOOL LogDestination::Init(ConfigParser *config, char *destinationName)
//{
//    m_logSourceInfo = FALSE;
//    m_isFileDestination = FALSE;
//    strncpy(m_name, destinationName, sizeof(m_name));
//    m_name[ sizeof(m_name)-1 ] = '\0';
//
//    ReadChangeableConfig(config);
//    ReadUnchangeableConfig(config);
//
//    return TRUE;
//}
//
//void LogDestination::LockBuffer()
//{
//    m_criticalSection.Enter();
//}
//
//void LogDestination::UnlockBuffer()
//{
//    m_criticalSection.Leave();
//}
//
//// Given a source buffer and count, which are not null terminated, fill in a dest buffer,
//// of size destCount, and ensure it is null terminated
//// destSize must be >= 1
//void LogDestination::MakeNullTerminatedString(char *dest, int destSize,
//                                              const char *source, int sourceCount,
//                                              char** end, int* remaining)
//{
//    if (sourceCount >= destSize)
//    {
//        sourceCount = destSize-1;
//    }
//    memcpy(dest, source, sourceCount);
//    dest[sourceCount] = '\0';
//
//    if (end)
//    {
//        *end = &dest[sourceCount];
//    }
//    if (remaining)
//    {
//        *remaining = destSize - sourceCount;
//    }
//}
//
//// Given a source buffer and count, which are not null terminated, fill in a dest buffer,
//// of size destCount, and ensure it is null terminated
//// destSize must be >= 1
//// Return value indicates success
//bool LogDestination::MakeNullTerminatedWString(WCHAR *dest, int destSize,
//                                              const char *source, int sourceCount,
//                                              WCHAR** end, int* remaining)
//{
//    // Assume there's room to expand the string, and if not, try again cutting the source short
//    // MutliByteToWideChar() doesn't handle empty source strings.
//    int charsWritten = MultiByteToWideChar(CP_UTF8, 0, source, sourceCount, dest, destSize);
//    if ((charsWritten == 0) && (sourceCount != 0))
//    {
//        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
//        {
//            // This may cut the source in the middle of a unicode character, but the incomplete
//            // character is ignored by the call to MultiByteToWideChar()
//            sourceCount = destSize-1;
//            charsWritten = MultiByteToWideChar(CP_UTF8, 0, source, sourceCount, dest, destSize);
//            if (charsWritten == 0)
//            {
//                return false;
//            }
//        }
//        else
//        {
//            return false;
//        }
//    }
//
//    // Make sure there's room for the terminating NULL
//    // and that later calculations are correct
//    if (charsWritten == destSize)
//    {
//        --charsWritten;
//    }
//
//    dest[charsWritten] = L'\0';
//
//    if (end)
//    {
//        *end = &dest[charsWritten];
//    }
//    if (remaining)
//    {
//        if (sourceCount == 0)
//        {
//            *remaining = 0;
//        }
//        else
//        {
//            *remaining = destSize - charsWritten;
//        }
//    }
//    return true;
//}
//
//// Base class initialization method for a log destination
//// Read the entries which can be changed at run-time
//void LogDestination::ReadChangeableConfig(ConfigParser *config)
//{
//    int temp;
//
//    // Read whether we are logging full source line info
//    if (config == NULL)
//    {
//        m_logSourceInfo = c_defaultLoggingLogSourceInfo;
//    }
//    else
//    {
//        config->GetIntParameter(m_name, "LogSourceInfo", &temp, c_defaultLoggingLogSourceInfo);
//        m_logSourceInfo = temp;
//    }
//}


//LogDestinationFile::LogDestinationFile()
//{
//    m_status = NO_ERROR;
//    m_maxAllowedDiskUsage = 0;
//    m_allowedDiskUsage = 0;
//    m_textFileDiskUsage = 0;
//    m_binaryFileDiskUsage = 0;
//    m_currentFileNumber = 0;
//    m_fBinaryMode = false;
//    m_binaryIndex = 0;
//    m_binaryEpoch = 0;
//    m_binarySerializationContext = NULL;
//    m_inlineCompressionLevel = 0;
//    m_spareCount = 0;
//    m_currentLogBuffer = NULL;
//    m_fileHandle = NULL;
//    m_fileSize = 0;
//    m_pendingFlushCount = 0;
//    m_createQueue = NULL;
//    m_flushQueue = NULL;
//    m_availableQueue = NULL;
//    m_hWorkerThread = NULL;
//    m_hWorkerWakeupEvent = NULL;
//    m_lastAlertTickCount = 0;
//    m_fileErrorCount = 0;
//}
//
//
//// Base initialization method for a log destination that is a file
//BOOL LogDestinationFile::Init(ConfigParser *config, char *destinationName)
//{
//    m_fileHandle            = NULL;
//
//    char moduleName[MAX_PATH+1];
//    if (GetModuleFileNameA(NULL, moduleName, sizeof(moduleName)) == 0)
//    {
//        fprintf(stderr, "Unable to get module path\n");
//        return FALSE;
//    }
//
//    // moduleName is e.g. foo\bar\csm.exe
//    char *moduleNameFilenamePart = strrchr(moduleName, '\\');
//    if (moduleNameFilenamePart == NULL)
//    {
//        moduleNameFilenamePart = moduleName;
//    }
//    else
//    {
//        moduleNameFilenamePart++;
//    }
//
//    // Initialize base class
//    if (LogDestination::Init(config, destinationName) == FALSE)
//        return FALSE;
//
//    m_isFileDestination = TRUE;
//
//    // Get base name for logging files
//    // e.g. Filebase=BadLogs means logs will be BadLogs0.log, BadLogs1.log, etc.
//    // If it's not present, we will just use the destination name as the base
//    if (config == NULL)
//    {
//        strcpy(m_filenameBase, destinationName);
//    }
//    else
//    {
//        config->GetParameter(
//            destinationName,
//            c_logFileBaseParameterName,
//            m_filenameBase,
//            sizeof(m_filenameBase),
//            destinationName
//        );
//    }
//
//    if (g_prefixAppName)
//    {
//        sprintf(&m_filenameBase[strlen(m_filenameBase)], "_%s", moduleNameFilenamePart);
//    }
//
//    BuildFileMapFromDisk();
//
//    // Allocate the descriptors for the buffers and queue them all to
//    // the create queue.
//    for (int i = 0; i < m_spareCount + 1; i++)
//    {
//        LOG_BUFFER_DESCRIPTOR * lbDesc = new LOG_BUFFER_DESCRIPTOR;
//        LogAssert(lbDesc != NULL);
//
//        ZeroMemory(lbDesc, sizeof(*lbDesc));
//
//        if (!m_fMemoryMapped)
//        {
//            // For non memory mapped logging we need to allocate a buffer where we will
//            // write entries. When the buffer is full we write to file.
//            lbDesc->m_writeBuffer = new BYTE[m_writeBufferSize];
//            lbDesc->m_writeBufferSize = m_writeBufferSize;
//        }
//
//        Enqueue(&m_createQueue, lbDesc);
//    }
//
//    // Spin the worker thread that will maintain the requested number of log
//    // buffers, flush buffers to files, enforce logging quota and compress old
//    // log files.
//    m_hWorkerWakeupEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
//    LogAssert(m_hWorkerWakeupEvent != NULL);
//
//    m_hWorkerThread =
//        (HANDLE) ::_beginthreadex(
//            NULL, 0, LogDestinationFile::WorkerThreadStarter, this, 0, NULL);
//    LogAssert(m_hWorkerThread != NULL);
//
//    // Spin wait to give a chance to the thread to initialize buffers. Without this wait, the
//    // higher layer may log before the thread is fully initialized, resulting in entry loss.
//    for (int i = 0; i < 100; i++)
//    {
//        INT64 epoch;
//
//        LockBuffer();
//
//        bool reserved = ReserveBytes(1, &epoch);
//
//        UnlockBuffer();
//
//        if (reserved)
//        {
//            // done
//            break;
//        }
//
//        Sleep(10);
//    }
//
//    return TRUE;
//}
//
//
//// For a log destination which is a set of files
//// Read config entries that can be changed at run time
//void LogDestinationFile::ReadChangeableConfig(ConfigParser *config)
//{
//    LogDestination::ReadChangeableConfig(config);
//
//    m_maxAllowedDiskUsage =
//        Logger::ReadInt64FromEnvironmentOrConfig(
//            config, m_name, "MaxAllowedDiskUsageInMb", c_defaultMaxAllowedDiskUsage) * (1024 * 1024);
//    m_allowedDiskUsage = m_maxAllowedDiskUsage;
//}
//
//// For a log destination which is a set of files
//// Read config entries that cannot be changed at run time
//void LogDestinationFile::ReadUnchangeableConfig(ConfigParser *config)
//{
//    LogDestination::ReadUnchangeableConfig(config);
//
//    if (config == NULL)
//    {
//        m_writeBufferSize = c_defaultLoggingWriteBufferSize;
//        m_fMemoryMapped = c_fDefaultMemoryMapped;
//        m_fBinaryMode = c_fBinaryMode;
//        m_maxTextFileSize = c_maxTextFileSize;
//    }
//    else
//    {
//        config->GetIntParameter(
//            m_name, "BufferSize", &m_writeBufferSize, c_defaultLoggingWriteBufferSize);
//
//        m_fMemoryMapped =
//            Logger::ReadBoolFromEnvironmentOrConfig(
//                config, m_name, "MemoryMapped", c_fDefaultMemoryMapped);
//
//        m_spareCount =
//            Logger::ReadIntFromEnvironmentOrConfig(
//                config, m_name, "SpareCount", m_fMemoryMapped ? c_defaultLogFileSpareCount :
//                                                                c_defaultLogBufferSpareCount);
//        m_fBinaryMode =
//            Logger::ReadBoolFromEnvironmentOrConfig(
//                config, m_name, "BinaryMode", c_fBinaryMode);
//
//        m_inlineCompressionLevel =
//            Logger::ReadIntFromEnvironmentOrConfig(
//                config, m_name, "InlineCompressionLevel", c_inlineCompressionLevel);
//
//        if (m_fBinaryMode && !Logger::m_bUseMemoryLog)
//        {
//            // Can't use binary logging without memory logger being turned on.
//            m_fBinaryMode = false;
//        }
//
//        if (!m_fBinaryMode)
//        {
//            // Fields only valid with binary logging.
//            m_inlineCompressionLevel = 0;
//            m_binaryIndex = 0;
//        }
//
//        m_maxTextFileSize =
//            Logger::ReadIntFromEnvironmentOrConfig(
//                config, m_name, "MaxTextFileSize", c_maxTextFileSize);
//
//        LogAssert(m_maxTextFileSize <= c_maxTextFileSizeLimit);
//    }
//}
//
//
////
//// Returns true if the destination is accepting unformatted (binary)
//// log output.
////
//bool LogDestinationFile::IsBinary() const
//{
//    return !!m_fBinaryMode;
//}
//
////
//// Return the file extension for the given log file type.
////
//PCSTR LogDestinationFile::GetExtensionForFileType(const LogFileType fileType)
//{
//    switch(fileType)
//    {
//        case LogFileTypeText:
//            return c_textFileExtension;
//
//        case LogFileTypeBinary:
//            return c_binaryFileExtension;
//
//        default:
//            LogAssert(false);
//    }
//
//    return NULL;
//}
//
//
////
//// Make the absolute path name for log file number <number>
//// e.g. c:\data\logs\localCosmosLog_en.exe_000000.log.
////
//void LogDestinationFile::MakeFilename(__in int fileNumber, __inout_ecount(MAX_PATH) char *pFilename, __in LogFileType fileType) const
//{
//    MakeFilenamePrefix(pFilename);
//
//    sprintf(
//        &pFilename[strlen(pFilename)],
//        "_%6.6d.%s",
//        fileNumber,
//        GetExtensionForFileType(fileType));
//}
//
////
//// Make the absolute path name prefix for a log file; this does not include the "_0.log" part
//// e.g. c:\data\logs\localCosmosLog_en.exe
////
//void LogDestinationFile::MakeFilenamePrefix(__in_ecount(MAX_PATH) char *pFilename) const
//{
//    // If the filename base has a colon in it or starts with a slash, use it as an absolute path
//    // Otherwise, prefix the appropriate log output directory
//    if ((m_filenameBase[0] == '\\') || (strchr(m_filenameBase, ':') != NULL))
//    {
//        strcpy(pFilename, m_filenameBase);
//    }
//    else
//    {
//        sprintf(pFilename, "%s\\%s", Logger::m_directoryName, m_filenameBase);
//    }
//}
//
//// Invoked when the file system returns OutOfSpace errors. The logger reduces its logging quota in
//// an attempt to dynamically recover from the out of space state. 
//void LogDestinationFile::AdjustLoggingQuota()
//{
//    if (!m_fBinaryMode)
//    {
//        // Only apply to binary files
//        goto exit;
//    }
//
//    // Attempt to go down by 1% of max allowed.
//    UINT64 delta = m_maxAllowedDiskUsage / 100;
//
//    // Never go lower than 25% of max allowed
//    if (
//            m_allowedDiskUsage <= delta ||
//            ((m_allowedDiskUsage - delta) <= (m_maxAllowedDiskUsage >> 2)))
//    {
//        goto exit;
//    }
//
//    
//    Log(
//        LogID_Logging, LogLevel_Status, "",
//        "Reducing logging quota. "
//        "Current=%I64u, Max=%I64u, New=%I64u.",
//        m_allowedDiskUsage, m_maxAllowedDiskUsage, (m_allowedDiskUsage - delta));
//
//    m_allowedDiskUsage -= delta;
//
//exit:
//
//    return;
//}
//
//
////
//// Delete as many files as necessary to make space for a new log file
//// on disk while staying under quota.
////
//// Executes on the worker thread.
////
//void LogDestinationFile::DeleteOldLogFiles()
//{
//    UINT64 targetDiskUsage = (m_allowedDiskUsage - m_maxTextFileSize);
//
//    // Delete txt/bin files until we satisfy the quota.
//    while((TotalSizeOfFilesOnDisk() > targetDiskUsage))
//    {
//        LogFileType fileType;
//
//        int nextFileNumberToDelete = GetOldestNonCompressedFileNumber(&fileType);
//
//        if (nextFileNumberToDelete == -1)
//        {
//            //
//            // No more files, we should handle it gracefully although this means
//            // our quota is ... zero.
//            break;
//        }
//
//        DeleteLogFileAndUpdateStats(nextFileNumberToDelete, fileType);
//    }
//
//    LogAssert(TotalSizeOfFilesOnDisk() <= targetDiskUsage);
//}
//
////
//// Look at a filename like crawler12345.log and read the 12345 part
//// file.name includes only the filename and not the pathname.
////
//// pFileName [in] - Supplies the name of the file.
////
//int LogDestinationFile::GetNumberFromFileName(__in PCSTR pFileName)
//{
//    const char *p = pFileName;
//
//    // Find any number inside here and parse it
//    while (*p != '\0' && !isdigit(((unsigned char) *p)))
//    {
//        p++;
//    }
//
//    return atoi(p);
//}
//
////
//// Remove a file from the filesMap if one exists matching the given fileNumber
//// and file type. Updates the disk usage counters.
////
//// Executes on the worker thread.
////
//// fileNumber [in] - Supplies the fileNumber to look for.
////
//// LogFileType [in] - Whether we should remove a binary or a text file.
////
//void LogDestinationFile::RemoveFileFromMap(__in int fileNumber, __in LogFileType fileType)
//{
//    LogFilesMapType& filesMap = GetMapForFileType(fileType);
//    LogFilesMapType::iterator it = filesMap.find(fileNumber);
//
//    if (it != filesMap.end())
//    {
//        UINT64 size = (UINT64) it->second;
//        filesMap.erase(it);
//        DiskFileUsageSubtract(size, fileType);
//    }
//}
//
////
//// Creates an entry for the file in the correct file map. If the entry is
//// already present, it is updated. Also updates the disk usage counters.
////
//// Executes on the worker thread.
////
//// fileNumber [in] - Supplies the fileNumber to add.
////
//// size [in] - Supplies the size of the file.
////
//// fileType [in] - Whether we should add a binary or a text file.
////
//void LogDestinationFile::AddFileToMap(__in int fileNumber, __in int size, __in LogFileType fileType)
//{
//    LogFilesMapType& filesMap = GetMapForFileType(fileType);
//    LogFilesMapType::iterator it = filesMap.find(fileNumber);
//
//    if (it != filesMap.end())
//    {
//        // The file is in the map already. This can occur if an old log file
//        // was reopened for writing. Update the total disk usage by subtracting
//        // the old usage and adding the new disk usage below.
//        LogAssert(fileType == LogFileTypeText || fileType == LogFileTypeBinary);
//        DiskFileUsageSubtract(it->second, fileType);
//    }
//
//    filesMap[fileNumber] = size;
//    DiskFileUsageAdd(size, fileType);
//}
//
////
//// Scan the log destination directory for all matching files of the
//// specified type and add them to the map.
////
//// fileType [in] - Identifies the file map to build.
////
//// Returns the highest numbered file of the given type. Returns -1 if
//// no files of the specified type were found on the disk.
////
//int LogDestinationFile::ScanAndAddFilesToMap(__in LogFileType fileType)
//{
//    int highest = -1;
//
//    struct _finddata_t file;
//    intptr_t handle;
//
//    char wildcardPathname[MAX_PATH];
//
//    //
//    // Make wildcard path to find all logs of our service
//    //
//    MakeFilenamePrefix(wildcardPathname);
//    strcat(wildcardPathname, "_*.");
//    strcat(wildcardPathname, GetExtensionForFileType(fileType));
//
//    if ((handle = _findfirst(wildcardPathname, &file)) == -1L)
//    {
//        return -1; // no files found
//    }
//
//    do
//    {
//        int number = GetNumberFromFileName(file.name);
//
//        AddFileToMap(number, file.size, fileType);
//
//        if (number > highest)
//        {
//            highest = number;
//        }
//
//    } while (_findnext(handle, &file) == 0);
//
//    _findclose(handle);
//
//    // Because we precreate memory mapped files, most likely the last files are
//    // empty. We need to delete them.
//    if (fileType == LogFileTypeText || fileType == LogFileTypeBinary)
//    {
//        while (highest >= 0)
//        {
//            bool isEmpty = CheckEmptyFile(highest, fileType);
//
//            if (!isEmpty)
//            {
//                // We stop at the first non empty file.
//                break;
//            }
//
//            DeleteLogFileAndUpdateStats(highest, fileType);
//
//            highest--;
//        }
//    }
//
//    return highest;
//}
//
//
////
//// Scan the log destination directory for all matching files and save the
//// state in the corresponding file map. Update the disk usage counters.
////
//// Determine the file number of the next file we should start appending to.
////
//void LogDestinationFile::BuildFileMapFromDisk()
//{
//    m_currentFileNumber = 0;
//
//    int latestTextFile = ScanAndAddFilesToMap(LogFileTypeText);
//    int latestBinaryFile = ScanAndAddFilesToMap(LogFileTypeBinary);
//
//    //
//    // Find the type and number of the latest log file.
//
//
//    if ((latestTextFile == -1) &&
//        (latestBinaryFile == -1))
//    {
//        // The role is starting up for the first time.
//        return;
//    }
//
//    if (m_fBinaryMode)
//    {
//        if (latestBinaryFile > latestTextFile)
//        {
//            m_currentFileNumber = latestBinaryFile;
//        }
//        else
//        {
//            // We can't append to a text file, so open a new file.
//            m_currentFileNumber = latestTextFile + 1;
//            return;
//        }
//    }
//    else
//    {
//        if (latestTextFile > latestBinaryFile)
//        {
//            m_currentFileNumber = latestTextFile;
//        }
//        else
//        {
//            // We can't append to a binary file, so open a new file.
//            m_currentFileNumber = latestBinaryFile + 1;
//            return;
//        }
//    }
//
//    return;
//}
//
//
////
//// Check whether the specified file is empty or not. The file is empty if one
//// of the following is true:
////
////  - Size is zero
////  - First byte is zero
////  - First byte is EOF (0xA1)
////
//bool LogDestinationFile::CheckEmptyFile(__in int fileNumber, __in LogFileType fileType)
//{
//    bool fEmpty = false;
//    char destFilename[MAX_PATH];
//    HANDLE fileHandle = INVALID_HANDLE_VALUE;
//    BYTE b;
//    DWORD dwRead;
//
//    MakeFilename(fileNumber, destFilename, fileType);
//
//    fileHandle = CreateFileA(
//                                destFilename,
//                                GENERIC_READ,
//                                FILE_SHARE_READ | FILE_SHARE_WRITE,
//                                NULL,
//                                OPEN_EXISTING,
//                                0,
//                                NULL);
//    if (fileHandle == INVALID_HANDLE_VALUE)
//    {
//        goto exit;
//    }
//
//    // Read the first byte from the file. If zero or EOF then we conclude it is
//    // an empty file.
//    if (!ReadFile(fileHandle, &b, 1, &dwRead, NULL))
//    {
//        fEmpty = (GetLastError() == ERROR_HANDLE_EOF);
//        goto exit;
//    }
//
//    fEmpty = (dwRead == 0 || b == 0 || b == 0x1A);
//
//exit:
//
//    if (fileHandle != INVALID_HANDLE_VALUE)
//    {
//        CloseHandle(fileHandle);
//    }
//
//    return fEmpty;
//}
//
//
////
//// Delete a particular numbered log file, update the file map and the disk
//// usage counters.
////
//// Must be invoked with the lock held.
////
//// number [in]  - The number of the log file to be deleted.
////                If number < 0 then it is ignored.
////
//// fileType [in] - Whether we should delete a binary or a text file.
////
//// Executes on the worker thread.
////
//bool LogDestinationFile::DeleteLogFileAndUpdateStats(__in int fileNumber, __in LogFileType fileType)
//{
//    bool fDeleted = false;
//    char destFilename[MAX_PATH];
//
//    MakeFilename(fileNumber, destFilename, fileType);
//
//    //
//    // If DeleteFile fails due to any reason other than FILE_NOT_FOUND then
//    // leave it in the map. We'll try to delete it again later. This can occur
//    // if the file is in use by another process.
//    //
//    if (::DeleteFileA(destFilename) || (::GetLastError() == ERROR_FILE_NOT_FOUND))
//    {
//        fDeleted = true;
//        RemoveFileFromMap(fileNumber, fileType);
//    }
//
//    return fDeleted;
//}
//
//bool LogDestinationFile::OpenFile(PCSTR pFileName, __out HANDLE * pFileHandle, __out bool *pFCreated)
//{
//    bool fCreated = false;
//    HANDLE fileHandle = NULL;
//
//    fileHandle =
//        CreateFileA(
//            pFileName,
//            GENERIC_WRITE | (m_fMemoryMapped ? GENERIC_READ : 0),
//            FILE_SHARE_READ,
//            NULL,
//            OPEN_EXISTING,
//            FILE_FLAG_SEQUENTIAL_SCAN,
//            NULL);
//
//    if (fileHandle == INVALID_HANDLE_VALUE)
//    {
//        fileHandle =
//            CreateFileA(
//                pFileName,
//                GENERIC_WRITE | (m_fMemoryMapped ? GENERIC_READ : 0),
//                FILE_SHARE_READ,
//                NULL,
//                OPEN_ALWAYS,
//                FILE_FLAG_SEQUENTIAL_SCAN,
//                NULL);
//
//        fCreated = true;
//    }
//
//    if (fileHandle == INVALID_HANDLE_VALUE)
//    {
//        *pFCreated = false;
//        *pFileHandle = NULL;
//        return false;
//    }
//
//    *pFCreated = fCreated;
//    *pFileHandle = fileHandle;
//    return true;
//}
//
//void LogDestinationFile::WriteTimestamps(
//    __in HANDLE streamHandle,
//    __in UINT64 minTimestamp,
//    __in UINT64 maxTimestamp)
//{
//    // Check the timestamps are valid.
//    if (minTimestamp == 0 || minTimestamp == (UINT64)-1 ||
//        maxTimestamp == 0 || maxTimestamp == (UINT64)-1)
//    {
//        return;
//    }
//
//    DWORD w;
//
//    if (!WriteFile(streamHandle, &minTimestamp, sizeof(minTimestamp), &w, NULL) ||
//        !WriteFile(streamHandle, &maxTimestamp, sizeof(maxTimestamp), &w, NULL))
//    {
//        return;
//    }
//}
//
//// Invoke this method to determine whether the current log file error should be raised as an error 
//// or as an alert.
//LogLevel LogDestinationFile::CheckIfAlertOrError()
//{
//    m_fileErrorCount++;
//
//    // Do not alert if the error count is below the threshold. This condition prevents spam alerts
//    // due to transient errors.
//    // The threshold is calculated such that we do not alert for the first hour.
//    if (m_fileErrorCount < (60 * 60 * 1000 / c_WorkerRetryIntervalMs))
//    {
//        return LogLevel_Error;
//    }
//
//    // Do not alert if we already alerted less than 24 hour ago.
//    if (m_lastAlertTickCount > 0 && (GetTickCount64() - m_lastAlertTickCount) < 24 * 60 * 60 * 1000)
//    {
//        return LogLevel_Error;
//    }
//
//    m_lastAlertTickCount = GetTickCount64();
//
//    return LogLevel_AppAlert;
//}
//
//// Crash the process on unactionable OS bug.
//VOID
//LogDestinationFile::CheckSystemError(
//    __in DWORD error)
//{
//    // Unactionable error caused by OS bug. It should be very rare.
//    // We crash the role without alerting.
//    if (
//        //error == ERROR_LOCK_VIOLATION ||
//        error == ERROR_SEM_TIMEOUT)
//    {
//        m_status = error;   // Don't attempt to log in the crash dump code.
//        Logger::Crash();
//    }
//}
//
////
////
//// Create a new log file for the current file number.
//// If the log file that we want to create already exists, we will use it instead
//// to avoid creating a new log file every time a process restarts.
////
//// Executes on the worker thread.
////
//HANDLE LogDestinationFile::WorkerCreateLogFile(
//    __out int * pUsedSize,
//    __out_opt HANDLE * pTsStreamHandle,
//    __out_opt BYTE ** pMapAddress,
//    __out_opt INT * pMapSize)
//{
//    DWORD error;
//    bool fCreated;
//    char filename[MAX_PATH];
//    HANDLE fileHandle = NULL;
//    HANDLE tsStreamHandle = NULL;
//    BYTE * mapAddress = NULL;
//    int fileSize = 0;
//
//    // Initialize the OUT parameter
//    *pUsedSize = 0;
//
//    if (pTsStreamHandle != NULL)
//    {
//        *pTsStreamHandle = NULL;
//    }
//
//    if (pMapAddress != NULL)
//    {
//        *pMapAddress = NULL;
//    }
//
//    if (pMapSize != NULL)
//    {
//        *pMapSize = NULL;
//    }
//
//    // Attempt to create/open the log file on disk
//    LogFileType fileType = (m_fBinaryMode ? LogFileTypeBinary : LogFileTypeText);
//
//    MakeFilename(m_currentFileNumber, filename, fileType);
//
//    if (!DirUtils::MakeDirectoryForFile(filename))
//    {
//        goto Failed;
//    }
//
//    if (!OpenFile(filename, &fileHandle, &fCreated)) {
//
//        error = GetLastError();
//
//        if (error == ERROR_DISK_FULL)
//        {
//            AdjustLoggingQuota();
//        }
//
//        // We will only alert the first time we encounter the error.
//        LogLevel level = CheckIfAlertOrError();
//
//        Log(
//            LogID_Logging, level,
//            "Error creating log file",
//            "Logger: Error %d creating log file %s. ErrorCount=%I64u, PID=%u.",
//            error, filename, m_fileErrorCount, GetCurrentProcessId());
//
//        goto Failed;
//    }
//
//    if (!fCreated)
//    {
//        fileSize = GetFileSize(fileHandle, NULL);
//
//        if (fileSize == INVALID_FILE_SIZE)
//        {
//            fileSize = 0;
//        }
//    }
//    else
//    {
//        if (pTsStreamHandle != NULL)
//        {
//            // For new files we create an alternate stream to hold the minimum
//            // and maximum entry timestamps in the file. Having that information will
//            // improve our log search performance (we'll identify the files to search faster).
//            size_t flen = strlen(filename);
//
//            strcat_s(filename, MAX_PATH, ":logts");
//
//            bool streamCreated;
//
//            // We don't worry if this fails since it's not critical
//            OpenFile(filename, &tsStreamHandle, &streamCreated);
//
//            // Set the filename to what it was before since it 's being used later.
//            filename[flen] = '\0';
//        }
//    }
//
//    if (m_fMemoryMapped)
//    {
//        HANDLE hMap = CreateFileMapping(
//                                            fileHandle,
//                                            NULL,
//                                            PAGE_READWRITE,
//                                            0,
//                                            m_maxTextFileSize,
//                                            NULL);
//
//        if (hMap == NULL)
//        {
//            error = GetLastError();
//
//            if (error == ERROR_DISK_FULL)
//            {
//                AdjustLoggingQuota();
//            }
//
//            CheckSystemError(error);
//
//            LogLevel level = CheckIfAlertOrError();
//
//            Log(
//                LogID_Logging, level,
//                "Error creating log file mapping",
//                "Logger: Error %d creating log file %s mapping. ErrorCount=%I64u, PID=%u.",
//                error, filename, m_fileErrorCount, GetCurrentProcessId());
//
//            goto Failed;
//        }
//
//        mapAddress = (BYTE *)MapViewOfFile(
//                                            hMap,
//                                            FILE_MAP_READ | FILE_MAP_WRITE,
//                                            0, 0,
//                                            m_maxTextFileSize);
//
//        if (mapAddress == NULL)
//        {
//            error = GetLastError();
//
//            LogLevel level = CheckIfAlertOrError();
//
//            Log(
//                LogID_Logging, level,
//                "Error mapping view of log file",
//                "Logger: Error %d mapping view of log file %s. ErrorCount=%I64u, PID=%u.",
//                error, filename, m_fileErrorCount, GetCurrentProcessId());
//
//            CloseHandle(hMap);
//            goto Failed;
//        }
//
//        // Close the map now (we don't need it anymore)
//        CloseHandle(hMap);
//
//        if (fileSize == m_maxTextFileSize)
//        {
//            // The unused tail of the memory mapped file will be all
//            // zeroes so we scan back looking for the first non-zero.
//            BYTE* scan = &mapAddress[fileSize - 1];
//
//            if (m_fBinaryMode)
//            {
//                // Skip past zeroes and look for EOF.
//                while (scan > mapAddress)
//                {
//                    if (*scan != 0)
//                    {
//                        if (*scan != 0x1a)
//                        {
//                            // Backed up too far. Don't overwrite non-EOF.
//                            ++scan;
//                        }
//
//                        break;
//                    }
//                    --scan;
//                }
//            }
//            else
//            {
//                // Skip past zeroes and the EOF.
//                while (scan > mapAddress)
//                {
//                    if ((*scan != 0) && (*scan != 0x1a))
//                    {
//                        // Backed up too far.
//                        ++scan;
//                        break;
//                    }
//
//                    --scan;
//                }
//            }
//
//            // Skip ahead of the trailing NULL in binary mode.
//            // Overwrite the trailing NULL in text mode.
//            fileSize = int(scan - mapAddress);
//        }
//    }
//    else
//    {
//        // Seek to end of file where new writes should go.
//        SetFilePointer(fileHandle, 0, NULL, FILE_END);
//    }
//
//    // Reset the error count for the recently created log file.
//    m_fileErrorCount = 0;
//
//    //
//    // Account for the full file size on disk.
//    //
//    AddFileToMap(m_currentFileNumber, m_maxTextFileSize, fileType);
//
//    m_currentFileNumber++;
//
//
//    *pUsedSize = fileSize;
//
//    if (pTsStreamHandle != NULL)
//    {
//        *pTsStreamHandle = tsStreamHandle;
//    }
//
//    if (pMapAddress != NULL)
//    {
//        *pMapAddress = mapAddress;
//    }
//
//    if (pMapSize != NULL)
//    {
//        *pMapSize = m_maxTextFileSize;
//    }
//
//    return fileHandle;
//
//Failed:
//
//    if (fileHandle != NULL)
//    {
//        CloseHandle(fileHandle);
//        fileHandle = NULL;
//
//        if (mapAddress != NULL)
//        {
//            UnmapViewOfFile(mapAddress);
//        }
//
//        if (tsStreamHandle != NULL)
//        {
//            CloseHandle(tsStreamHandle);
//            tsStreamHandle = NULL;
//        }
//    }
//
//    return fileHandle;
//}
//
//
////
//// Flush the specified log buffer.
//// For memory mapped logging, flushing the buffer means unmapping the file view
//// and closing the file.
//// For non memory mapped logging flushing the buffer means writing into the
//// current file. If the current file is full we close the file and create a new
//// one.
////
//// Executes on the worker thread.
////
//bool LogDestinationFile::WorkerFlushLogBuffer(
//    __in LOG_BUFFER_DESCRIPTOR * lbDesc)
//{
//    bool result = true;
//
//    if (m_fMemoryMapped)
//    {
//        UnmapViewOfFile(lbDesc->m_writeBuffer);
//        lbDesc->m_writeBuffer = NULL;
//        lbDesc->m_writeBufferSize = 0;
//
//        // Shorten file to correct length
//        SetFilePointer(lbDesc->m_fileHandle, lbDesc->m_writeBufferPosition, NULL, FILE_BEGIN);
//        SetEndOfFile(lbDesc->m_fileHandle);
//
//        SetFileLastUpdateTime(lbDesc->m_fileHandle);
//
//        CloseHandle(lbDesc->m_fileHandle);
//        lbDesc->m_fileHandle = NULL;
//        lbDesc->m_writeBufferPosition = 0;
//
//        if (lbDesc->m_tsStreamHandle != NULL)
//        {
//            WriteTimestamps(lbDesc->m_tsStreamHandle, lbDesc->m_minTimestamp, lbDesc->m_maxTimestamp);
//            CloseHandle(lbDesc->m_tsStreamHandle);
//            lbDesc->m_tsStreamHandle = NULL;
//        }
//
//        lbDesc->m_minTimestamp = 0;
//        lbDesc->m_maxTimestamp = 0;
//
//    }
//    else
//    {
//        if (m_fileHandle == NULL)
//        {
//            // Create a new file.
//            m_fileHandle = WorkerCreateLogFile(&m_fileSize, NULL, NULL, NULL);
//        }
//
//        if (m_fileHandle == NULL)
//        {
//            // If we cannot create the file we'll return failure and have the
//            // worker thread retry later.
//            result = false;
//            goto exit;
//        }
//
//        // Write the content of the buffer to the file.
//        DWORD bytesWritten = 0;
//
//        if (!WriteFile(
//                        m_fileHandle,
//                        lbDesc->m_writeBuffer,
//                        lbDesc->m_writeBufferPosition,
//                        &bytesWritten,
//                        NULL))
//        {
//            DWORD error = GetLastError();
//
//            if (error == ERROR_DISK_FULL)
//            {
//                AdjustLoggingQuota();
//            }
//
//            CheckSystemError(error);
//
//            LogLevel level = CheckIfAlertOrError();
//
//            LogFileType fileType = (m_fBinaryMode ? LogFileTypeBinary : LogFileTypeText);
//
//            CHAR filename[MAX_PATH];
//
//            MakeFilename(m_currentFileNumber, filename, fileType);
//
//            Log(
//                LogID_Logging, level,
//                "Failure flushing app logs to disk",
//                "Logger: Error %d when attempting to flush logs to disk. "
//                "Filename=%s, ErrorCount=%I64u, PID=%u.",
//                error, filename, m_fileErrorCount, GetCurrentProcessId());
//
//            // If we cannot write we'll return failure and have the worker thread
//            // retry later.
//            result = false;
//            goto exit;
//        }
//
//        m_fileSize += bytesWritten;
//
//        SetFileLastUpdateTime(m_fileHandle);
//
//        // Close current file if above the threshold size
//        if (m_fileSize >= m_maxTextFileSize)
//        {
//            CloseHandle(m_fileHandle);
//            m_fileHandle = NULL;
//            m_fileSize = 0;
//        }
//
//
//        // Note that we will reuse the buffer so we only need to adjust the
//        // write position
//        lbDesc->m_writeBufferPosition = 0;
//    }
//
//exit:
//
//    return result;
//
//}
//
//// LTCG unreachable code
//#pragma warning (disable: 4702)
//
////
//// Thread start routine for the worker thread.
////
//UINT LogDestinationFile::WorkerThreadStarter(
//    __in void * context)
//{
//    LogDestinationFile * ldf = (LogDestinationFile *)context;
//
//    ldf->WorkerRun();
//
//    return 0;
//}

#pragma warning (default: 4702)


////
//// Worker thread loop
////
//VOID LogDestinationFile::WorkerRun()
//{
//    LOG_BUFFER_DESCRIPTOR * lbDesc;
//    DWORD waitTimeout;
//
//    // Set a zero timeout the first time we execute the loop to make sure we
//    // start work on pre-creating the files immediately.
//    waitTimeout = 0;
//
//    for ( ; ; )
//    {
//        DWORD result = WaitForSingleObject(m_hWorkerWakeupEvent, waitTimeout);
//        LogAssert(result != WAIT_FAILED);
//
//        // Set the default timeout for the next wait. Usually we want to wait
//        // until someone wakes us up. That infinite wait can be overwritten if
//        // we need to retry an operation, for example failure to create a log file.
//        waitTimeout = INFINITE;
//
//        // On wakeup check if there are any pending requests. We always do a
//        // housekeeping on wakeup just in case.
//        CompressOrDeleteFiles();
//
//        // Flush all buffers queued for flush.
//        while ((lbDesc = Dequeue(&m_flushQueue)) != NULL)
//        {
//            bool flushSuccess = WorkerFlushLogBuffer(lbDesc);
//
//            // Housekeeping since flushing may have created a new file.
//            CompressOrDeleteFiles();
//
//            if (!flushSuccess)
//            {
//                // We weren't able to flush the file. We return the
//                // descriptor to the flush queue and set the wakeup timeout
//                // for retry.
//                Push(&m_flushQueue, lbDesc);
//                waitTimeout = c_WorkerRetryIntervalMs;
//                break;
//            }
//
//            LogAssert(m_pendingFlushCount > 0);
//            InterlockedDecrement(&m_pendingFlushCount);
//
//            Enqueue(&m_createQueue, lbDesc);
//        }
//
//        // Create new log files if needed
//        while ((lbDesc = Dequeue(&m_createQueue)) != NULL)
//        {
//            if (m_fMemoryMapped)
//            {
//                lbDesc->m_fileHandle = WorkerCreateLogFile(
//                                                    &lbDesc->m_writeBufferPosition,
//                                                    &lbDesc->m_tsStreamHandle,
//                                                    &lbDesc->m_writeBuffer,
//                                                    &lbDesc->m_writeBufferSize);
//
//                // Housekeeping since we created a new file.
//                CompressOrDeleteFiles();
//
//                if (lbDesc->m_fileHandle == NULL)
//                {
//                    // We weren't able to create the file. We return the
//                    // descriptor to the create queue and set the wakeup timeout
//                    // for retry.
//                    Enqueue(&m_createQueue, lbDesc);
//                    waitTimeout = c_WorkerRetryIntervalMs;
//                    break;
//                }
//            }
//            else
//            {
//                // Nothing to do for non memory mapped files since the buffer
//                // is already created. We'll just queue the buffer to the
//                // available queue.
//            }
//
//            // Reset the min and max timestamps before making the buffer available.
//            lbDesc->m_minTimestamp = (UINT64)-1;
//            lbDesc->m_maxTimestamp = 0;
//
//            Enqueue(&m_availableQueue, lbDesc);
//        }
//
//    }
//}
//
//VOID LogDestinationFile::Enqueue(LOG_BUFFER_DESCRIPTOR ** pQueue, LOG_BUFFER_DESCRIPTOR * lbDesc)
//{
//    LogAssert(lbDesc->m_next == NULL);
//
//    m_queueCritSec.Enter();
//
//    if (*pQueue == NULL)
//    {
//        *pQueue = lbDesc;
//    }
//    else
//    {
//        // Walk to the end of the queue.
//        LOG_BUFFER_DESCRIPTOR * p = *pQueue;
//
//        while (p->m_next != NULL)
//        {
//            p = p->m_next;
//        }
//
//        p->m_next = lbDesc;
//    }
//
//    m_queueCritSec.Leave();
//}
//
//
//LOG_BUFFER_DESCRIPTOR * LogDestinationFile::Dequeue(LOG_BUFFER_DESCRIPTOR ** pQueue)
//{
//    m_queueCritSec.Enter();
//
//    LOG_BUFFER_DESCRIPTOR * p = *pQueue;
//
//    if (p == NULL)
//    {
//        // Empty queue.
//        goto exit;
//    }
//
//    *pQueue = p->m_next;
//
//    p->m_next = NULL;
//
//exit:
//
//    m_queueCritSec.Leave();
//
//    return p;
//}
//
//VOID LogDestinationFile::Push(LOG_BUFFER_DESCRIPTOR ** pQueue, LOG_BUFFER_DESCRIPTOR * lbDesc)
//{
//    LogAssert(lbDesc->m_next == NULL);
//
//    m_queueCritSec.Enter();
//
//    if (*pQueue != NULL)
//    {
//        lbDesc->m_next = *pQueue;
//    }
//
//    *pQueue = lbDesc;
//
//    m_queueCritSec.Leave();
//}
//
//// Wait until the worker thread flushes all buffers or the specified timeout elapses.
//VOID LogDestinationFile::SpinWaitUntilFlushCompleted(DWORD timeoutMs)
//{
//    DWORD start = GetTickCount();
//
//    for ( ; ; )
//    {
//        if (m_pendingFlushCount == 0 || 
//            (GetTickCount() - start >= timeoutMs))
//        {
//            break;
//        }
//
//        Sleep(10);
//    }
//}
//
//LogFilesMapType& LogDestinationFile::GetMapForFileType(const LogFileType fileType)
//{
//    switch(fileType)
//    {
//        case LogFileTypeText:
//            return m_textFilesMap;
//
//        case LogFileTypeBinary:
//            return m_binaryFilesMap;
//
//        default:
//            LogAssert(false);
//    }
//
//    // Unreachable.
//    return *((LogFilesMapType*) NULL);
//}
//
//const LogFilesMapType& LogDestinationFile::GetMapForFileType(const LogFileType fileType) const
//{
//    switch(fileType)
//    {
//        case LogFileTypeText:
//            return m_textFilesMap;
//
//        case LogFileTypeBinary:
//            return m_binaryFilesMap;
//
//        default:
//            LogAssert(false);
//    }
//
//    // Unreachable.
//    return *((LogFilesMapType*) NULL);
//}
//
//
//void LogDestinationFile::DiskFileUsageAdd(const UINT64 bytes, const LogFileType fileType)
//{
//    switch(fileType)
//    {
//        case LogFileTypeText:
//            m_textFileDiskUsage += bytes;
//            break;
//
//        case LogFileTypeBinary:
//            m_binaryFileDiskUsage += bytes;
//            break;
//
//        default:
//            LogAssert(false);
//    }
//}
//
//void LogDestinationFile::DiskFileUsageSubtract(const UINT64 bytes, const LogFileType fileType)
//{
//    switch(fileType)
//    {
//        case LogFileTypeText:
//            m_textFileDiskUsage -= bytes;
//            break;
//
//        case LogFileTypeBinary:
//            m_binaryFileDiskUsage -= bytes;
//            break;
//
//        default:
//            LogAssert(false);
//    }
//}
//
//UINT64 LogDestinationFile::TotalSizeOfFilesOnDisk() const
//{
//    return (m_textFileDiskUsage + m_binaryFileDiskUsage);
//}
//
//
//// Check whether any log files are candidates for deletion and/or compression and
//// deletes/compresses them.
////
//// Executes on the worker thread.
////
//void LogDestinationFile::CompressOrDeleteFiles()
//{
//    //
//    // First delete some files to make space right away.
//    // This is critical so that the current thread can continue to log.
//    //
//    DeleteOldLogFiles();
//}
//
////
//// Return the fileNumber of the oldest text file from the file map.
////
//// Executes on the worker thread.
////
//int LogDestinationFile::GetOldestFileNumber(LogFileType fileType) const
//{
//    const LogFilesMapType& filesMap = GetMapForFileType(fileType);
//
//    if (filesMap.size() == 0)
//    {
//        return -1;
//    }
//
//    return filesMap.begin()->first;
//}
//
//
//int LogDestinationFile::GetOldestNonCompressedFileNumber(LogFileType * fileType)
//{
//    // Uncompressed is either text or binary
//    int oldestTextFileNumber = GetOldestFileNumber(LogFileTypeText);
//    int oldestBinaryFileNumber = GetOldestFileNumber(LogFileTypeBinary);
//
//    // Cast to UINT for compare to handle the -1 file number (not exist)
//    if ((UINT)oldestTextFileNumber <= (UINT)oldestBinaryFileNumber)
//    {
//        // Text is oldest
//        *fileType = LogFileTypeText;
//        return oldestTextFileNumber;
//    }
//
//    // Binary is oldest
//    *fileType = LogFileTypeBinary;
//    return oldestBinaryFileNumber;
//}
//
//// Flush this log file
////
//// Call this only if you don't have the lock
////
//// Flush transfers the log entries from the internal buffers to the target log file. The
//// flushBuffers parameter allows you to control the flush operation as follows:
////
////      - TRUE: flush the buffers and WAIT until the flush operation completes or times out. This
////        is an expensive operation and should be used only upon an imminent process termination.
////        The crash dump infrastructure uses this flag to make sure no log entries are lost when
////        a process crashes.
////      - FALSE: flush the buffers but DO NOT WAIT for the flush to complete. Instead, the flush
////        will eventually complete asynchronously. The logger flush thread uses this option to 
////        periodically flush the buffers and make the log entries visible to log search.
////
//void LogDestinationFile::Flush(bool flushBuffers)
//{
//    if (!m_fMemoryMapped)
//    {
//        LockBuffer();
//
//        if (m_currentLogBuffer != NULL && m_currentLogBuffer->m_writeBufferPosition > 0)
//        {
//            SwitchBuffers();
//        }
//
//        UnlockBuffer();
//
//        if (flushBuffers)
//        {
//            // Wait for the flush thread to complete the job. Specify a finite timeout to avoid
//            // freezing the process forever in case the flush cannot complete (bad disk for example)
//            SpinWaitUntilFlushCompleted(1000);
//        }
//    }
//}
//
//// Make sure the last write time gets updated
//// For some reason, without this code, the file times of logs are often not updated when new data is written
//void LogDestinationFile::SetFileLastUpdateTime(
//    __in HANDLE fileHandle)
//{
//    if (fileHandle != NULL)
//    {
//        SYSTEMTIME systemtime;
//        FILETIME filetime;
//
//        GetSystemTime(&systemtime);
//
//        if (SystemTimeToFileTime(&systemtime, &filetime))
//        {
//            SetFileTime(
//                fileHandle,
//                NULL,     // create time
//                NULL,     // last access time
//                &filetime // last write time
//            );
//        }
//    }
//}
//
////
//// Reserve the specified number of bytes in the logging buffer. If there is
//// enough space in the current buffer it returns success immediately. If not,
//// it attempts to switch buffers and make the reservation in the new file.
////
//// bytesNeeded [in] - Supplies the number of bytes to reserve.
////
//// pEpoch [out] - Returns the epoch where the reservation was made.
////
//// Return Value:
////
////  Returns true if reservation was successful, false if not.
////
//// Note:
////
////  The buffer lock must be held when calling this function.
////
////  The reservation is valid only until the subsequent AppendData call. Therefore,
////  it is up to the upeer layer to make sure that ReserveBytes/AppendData calls
////  are atomic.
////
//bool LogDestinationFile::ReserveBytes(
//    __in INT bytesNeeded,
//    __out INT64 * pEpoch)
//{
//    bool success = false;
//
//    *pEpoch = INVALID_EPOCH;
//
//    // Check whether we have enough in the current buffer.
//    if (m_currentLogBuffer != NULL)
//    {
//        INT bytesAvailable = m_currentLogBuffer->m_writeBufferSize -
//                                        m_currentLogBuffer->m_writeBufferPosition;
//        if (bytesNeeded <= bytesAvailable)
//        {
//            // We've got enough.
//            success = true;
//            *pEpoch = m_binaryEpoch;
//            goto exit;
//        }
//    }
//
//
//    // We do not have enough free space in the current buffer. Flush current
//    // buffer and get the next available one.
//    SwitchBuffers();
//
//    // Check the current buffer again. Most likely we have unless the bytes
//    // requested is a very large amount.
//    if (m_currentLogBuffer != NULL)
//    {
//        INT bytesAvailable = m_currentLogBuffer->m_writeBufferSize -
//                                        m_currentLogBuffer->m_writeBufferPosition;
//        if (bytesNeeded <= bytesAvailable)
//        {
//            // We've got enough.
//            success = true;
//            *pEpoch = m_binaryEpoch;
//            goto exit;
//        }
//    }
//
//
//exit:
//
//    return success;
//}
//
//void LogDestinationFile::SwitchBuffers()
//{
//    if (m_currentLogBuffer != NULL)
//    {
//        InterlockedIncrement(&m_pendingFlushCount);
//
//        Enqueue(&m_flushQueue, m_currentLogBuffer);
//        m_currentLogBuffer = NULL;
//
//        // Wakeup the worker to process the request.
//        SetEvent(m_hWorkerWakeupEvent);
//    }
//
//    // Attempt to get the next available buffer.
//    m_currentLogBuffer = Dequeue(&m_availableQueue);
//
//    if (m_currentLogBuffer != NULL && m_fBinaryMode)
//    {
//        // Add a header to update versioning information for the parser.
//        InterlockedIncrement64(&m_binaryEpoch);
//
//        BinaryLogProcessHeader hdr;
//        hdr.Init();
//        AppendBinaryDataInEpoch(
//            NULL, 0, NULL, 0, &hdr, sizeof(hdr), INVALID_EPOCH, (UINT64)-1, 0);
//    }
//}
//
//
//
////
//// Write binary data to the log file. Open a new log file if there is
//// insufficient space in the current log file.
////
//// pAncillaryData [in] - Ancillary data associated with the log entry.
////                       This is always output. Optional.
////
//// ancillaryLen [in] - Size of the ancillary data.
////
//// pFormat [in] - Formatting information associated with the log entry.
////                This is output only if the caller's epoch is not current.
////                Optional.
////
//// formatLen [in] - Size of the format info length.
////
//// pData [in] - Log entry data.
////
//// dataLen [in] - Size of the log entry data.
////
//// epoch[in] - The epoch in which the formatting data was last
////             output. If the supplied epoch is not current then the
////             formatting information is always output. Set to
////             INVALID_EPOCH to force flushing the format info.
////
//// minTimestamp [in] - The minimum timestamp across all log entries logged in
////                     this call. Pass 0 if not known.
////
//// maxTimestamp [in] - The minimum timestamp across all log entries logged in
////                     this call. Pass -1 if not known.
////
//// Return value:
////      true if the data was successfully copied.
////      false if the data couldn't be copied.
////
//bool LogDestinationFile::AppendBinaryDataInEpoch(
//    __in_bcount_opt(ancillaryLen) const PVOID pAncillaryData,
//    __in int ancillaryLen,
//    __in_bcount_opt(formatLen) const PVOID pFormat,
//    __in int formatLen,
//    __in_bcount(dataLen) const PVOID pData,
//    __in int dataLen,
//    __in INT64 epoch,
//    __in UINT64 minTimestamp,
//    __in UINT64 maxTimestamp
//    )
//{
//    INT64 epochSnapshot;
//    bool fSucceeded = false;
//
//    if (m_status != NO_ERROR)
//    {
//        // Can't log.
//        goto Done;
//    }
//
//    // Check whether we have enough space in the current buffer and allocate
//    // a new buffer if we don't.
//    if (!ReserveBytes(ancillaryLen + formatLen + dataLen, &epochSnapshot))
//    {
//        // Can't log.
//        goto Done;
//    }
//
//    //
//    // Opening a new log file may have incremented the epoch.
//    // Check whether the caller's epoch is still valid.
//    //
//    if ((epoch != INVALID_EPOCH) && (epoch != epochSnapshot))
//    {
//        goto Done;
//    }
//
//    int count = ancillaryLen  + dataLen + formatLen;
//
//    // Copy into the destination buffer
//    // We should have space by now, given the above code,
//    BYTE* buf = &m_currentLogBuffer->m_writeBuffer[m_currentLogBuffer->m_writeBufferPosition];
//
//    if (ancillaryLen > 0)
//    {
//        memcpy(buf, pAncillaryData, ancillaryLen);
//        buf += ancillaryLen;
//    }
//
//    if (formatLen> 0)
//    {
//        memcpy(buf, pFormat, formatLen);
//        buf += formatLen;
//    }
//
//    memcpy(buf, pData, dataLen);
//
//    m_currentLogBuffer->m_writeBufferPosition += count;
//
//    if (m_fMemoryMapped &&
//        (m_currentLogBuffer->m_writeBufferPosition < m_currentLogBuffer->m_writeBufferSize))
//    {
//        // Insert EOF marker. This is used to mark the logical end of
//        // memory mapped text and binary files. For binary files the
//        // logical EOF must be detected by scanning backwards for 0x1a
//        // starting from the physical EOF.
//        m_currentLogBuffer->m_writeBuffer[m_currentLogBuffer->m_writeBufferPosition] = 0x1a;
//    }
//
//    // Roll in the new timestamps.
//    if (minTimestamp < m_currentLogBuffer->m_minTimestamp)
//    {
//        m_currentLogBuffer->m_minTimestamp = minTimestamp;
//    }
//
//    if (maxTimestamp > m_currentLogBuffer->m_maxTimestamp)
//    {
//        m_currentLogBuffer->m_maxTimestamp = maxTimestamp;
//    }
//
//    fSucceeded = true;
//
//Done:
//    return fSucceeded;
//}
//
//
//// Take the text given by (buffer, count) and append that and \r\n to the log
//// You must have the lock to call this function
//void LogDestinationFile::AppendData(char *prefix, int prefixCount,
//                                    char* srcInfo, int srcInfoCount,
//                                    char* desc, int descCount)
//{
//    AppendBinaryDataInEpoch(
//        prefix, prefixCount,
//        srcInfo, srcInfoCount,
//        desc, descCount,
//        INVALID_EPOCH,
//        0, (UINT64)-1);
//}

//void LogDestinationStdout::AppendData(char *prefix, int prefixCount,
//                                      char* srcInfo, int srcInfoCount,
//                                      char* desc, int descCount)
//{
//    fwrite(prefix, 1, prefixCount, stdout);
//    fwrite(srcInfo, 1, srcInfoCount, stdout);
//    fwrite(desc, 1, descCount, stdout);
//}
//
//void LogDestinationStderr::AppendData(char *prefix, int prefixCount,
//                                      char* srcInfo, int srcInfoCount,
//                                      char* desc, int descCount)
//{
//    fwrite(prefix, 1, prefixCount, stderr);
//    fwrite(srcInfo, 1, srcInfoCount, stderr);
//    fwrite(desc, 1, descCount, stderr);
//}
//
//// Buffer is not guaranteed to be null terminated, so need to make a null terminated string
//void LogDestinationDebugString::AppendData(char *prefix, int prefixCount,
//                                           char* srcInfo, int srcInfoCount,
//                                           char* desc, int descCount)
//{
//    char localBuffer[c_logMaxEntrySize];
//    char* dest = localBuffer;
//    int remaining = (int) sizeof(localBuffer);
//    MakeNullTerminatedString(dest, remaining, prefix, prefixCount, &dest, &remaining);
//    MakeNullTerminatedString(dest, remaining, srcInfo, srcInfoCount, &dest, &remaining);
//    MakeNullTerminatedString(dest, remaining, desc, descCount, &dest, &remaining);
//
//    OutputDebugStringA(localBuffer);
//}
//
//// Buffer is not guaranteed to be null terminated, so need to make a null terminated string
//void LogDestinationApplicationCallback::AppendData(char *prefix, int prefixCount,
//                                                   char* srcInfo, int srcInfoCount,
//                                                   char* desc, int descCount)
//{
//    char localBuffer[c_logMaxEntrySize];
//    char* dest = localBuffer;
//    int remaining = (int) sizeof(localBuffer);
//    MakeNullTerminatedString(dest, remaining, prefix, prefixCount, &dest, &remaining);
//    MakeNullTerminatedString(dest, remaining, srcInfo, srcInfoCount, &dest, &remaining);
//    MakeNullTerminatedString(dest, remaining, desc, descCount, &dest, &remaining);
//
//    Logger::SendEntryToApplicationHandlers(localBuffer);
//}
//
//// Make a popup
//// buffer is not guaranteed to be null terminated
//void LogDestinationPopup::AppendData(char *prefix, int prefixCount,
//                                     char* srcInfo, int srcInfoCount,
//                                     char* desc, int descCount)
//{
//    Logger::Flush();
//
//    char localBuffer[c_logMaxEntrySize];
//    char moduleName[MAX_PATH+1];
//
//    if (GetModuleFileNameA(NULL, moduleName, sizeof(moduleName)) == 0)
//    {
//        moduleName[0] = '\0';
//        return;
//    }
//
//    _snprintf(
//        localBuffer, sizeof(localBuffer), "Module: %s\r\n\r\n%.*s\r\n%.*s\r\n%.*s\r\n",
//        moduleName,
//        prefixCount, prefix,
//        srcInfoCount, srcInfo,
//        descCount, desc);
//    localBuffer[sizeof(localBuffer)-1] = '\0';
//
//    HWND hWndParent = NULL;
//    BOOL fNonInteractive = FALSE;
//    HWINSTA hwinsta;
//    USEROBJECTFLAGS uof;
//    DWORD nDummy;
//    UInt32 uType = MB_TASKMODAL|MB_ICONHAND|MB_ABORTRETRYIGNORE|MB_SETFOREGROUND;
//
//    if (NULL == (hwinsta = GetProcessWindowStation()) ||
//        !GetUserObjectInformation(hwinsta, UOI_FLAGS, &uof, sizeof(uof), &nDummy) ||
//        (uof.dwFlags & WSF_VISIBLE) == 0)
//    {
//        fNonInteractive = TRUE;
//    }
//    if (fNonInteractive)
//    {
//        uType |= MB_SERVICE_NOTIFICATION;
//    }
//    else
//    {
//        hWndParent = GetActiveWindow();
//
//        if (hWndParent != NULL)
//        {
//            hWndParent = GetLastActivePopup(hWndParent);
//        }
//    }
//
//    int nCode = MessageBoxA(hWndParent,
//                            localBuffer,
//                            "Assert! -- Hit Retry to enter the debugger",
//                            uType);
//
//    if (IDABORT == nCode)
//    {
//        ::Terminate(3);
//    }
//
//    if (IDRETRY == nCode)
//    {
//        DebugBreak();
//    }
//    // else ignore and continue execution
//}
//
//// buffer is not guaranteed to be null terminated
//void LogDestinationDebugBreak::AppendData(char *, int, char*, int, char*, int)
//{
//    // Doesn't flush the log in this case
//    DebugBreak();
//}
//
//// Make the process terminate
//void LogDestinationTerminate::AppendData(char *, int, char*, int, char*, int)
//{
//    Logger::Flush(true);
//    if (IsDebuggerPresent())
//    {
//        DebugBreak();
//    }
//
//    // this NULL dereference was added to stop the process while generating a crash dump, and not allowing thread
//    // continuation after the crash has been generated.  Previously we were creating a crash dump by faulting
//    // in a try{}except() handler invoked from within Logger::LogV(), that would allow other threads to continue
//    // when the crash dump was done, and before we finally called ::Terminate() here.  Now we rely on the
//    // exception filter we have installed to generate the crash dump.  Until our exception filter is installed,
//    // the old path will be used.
//#pragma prefast(suppress:11, "We intentionly generated NULL referencing here, to invoke the exception handler")
//
//    //NULL dereference invokes our exception filter/handler Logger::LogAndExitProcess()
//    int * ptr = NULL;
//    *ptr = 0;
//
//    // we should never continue after asserting.
//    ::Terminate(1);
//}
//
//
//// Write a full dump
//void LogDestinationFullDump::AppendData(char *, int, char *, int, char *, int)
//{
//    Logger::WriteMiniDump( c_minidumpTimeout, true/*fullDump*/);
//}


//// This thread just flushes the logs every second
//unsigned int Logger::LogFlushThread(void *)
//{
//    while (TRUE)
//    {
//        SleepEx(10 * 1000, TRUE);
//        if (m_bStopped)
//        {
//            return 0;
//        }
//        Flush(false);
//    }
//}
//
//// Spin a thread to flush the logs every second
//BOOL Logger::SpinLogFlushThread()
//{
//    unsigned int threadID;
//    m_hThread = (HANDLE)_beginthreadex(NULL, 0, LogFlushThread, NULL, 0, &threadID);
//    if (m_hThread == NULL)
//    {
//        Log(LogID_Logging, LogLevel_Error, "Failed to create log flush thread");
//        return false;
//    }
//    m_dwThreadID = threadID;
//    return true;
//}
//
//// Compute the full path name for the directory where minidumps should be stored.
//bool Logger::GetMiniDumpDataDir (char * szMiniDumpDataDir, size_t cbMiniDumpDataDir)
//{
//    if (szMiniDumpDataDir == NULL)
//        return false;
//
//    size_t cMin = min (cbMiniDumpDataDir, (size_t)MAX_INT32); // GetDataFile uses int instead of size_t
//    // Construct absolute path for the folder where the files will be written.
//    // If SysInfo is not initialized, then the path will be "\minidumps".
//    if (!SysInfo::GetDataFile (szMiniDumpDataDir, (int)cMin, "minidumps"))
//        return false;
//    return true;
//}


////
//// All crash dumps are prefixed by the name of the EXE
//// and module.
////
//// e.g. EnPn-en.exe.4912.full.GMT-2008-08-05-21-42-52.dmp
////
//// Let's generate that prefix i.e. "EnPn-en.exe" in our example.
////
//const char *GetCrashDumpModulePrefix(
//    __in char *pathNameStorage,
//    __in UINT cbNameStorage
//    )
//{
//    // Get the relative path of the current process with the component name and the process
//    // name in the format of <component>-<process>. For example, "d:\app\watchdog.12345\watchdog.exe"
//    // would become something like "watchdog.12345-watchdog.exe".
//
//    const char *cszDumpAndCrashLogFileNameBase;
//
//    // Get the full path of the current process
//    GetModuleFileNameA(NULL, pathNameStorage, cbNameStorage);
//
//    // create the relative path in the <component>-<process> format
//    char * szShortProgramNameTemp = strrchr (pathNameStorage, '\\');
//    // There should be at least one directory level
//    if (szShortProgramNameTemp == NULL)
//        return NULL;
//
//    *szShortProgramNameTemp = '-';
//    cszDumpAndCrashLogFileNameBase = strrchr (pathNameStorage, '\\');
//
//    if (cszDumpAndCrashLogFileNameBase == NULL)
//    {
//        // In the fabric machines, there is only one directory level for role exe's,
//        // so, just point to the program name
//        cszDumpAndCrashLogFileNameBase = szShortProgramNameTemp;
//    }
//
//    cszDumpAndCrashLogFileNameBase ++; // skip the '\' char for multiple level directories, or the '-' for single level.
//
//    return cszDumpAndCrashLogFileNameBase;
//
//}
//
////
//// For current process, compute the filenames for the minidump and the
//// crash log file. The minidump would contain the dump and the crash
//// log file would contain information when the crash happend and
//// at what time the process should be done writing the dump.
////
//static bool GetCurrentProcessDumpAndCrashLogFileNameFullPath (
//    const char * cszMiniDumpDataDir,    // the directory to store the files
//    SYSTEMTIME stCurrent,             // the timestamp to use as the current time
//    bool fullDump,
//    char * szDumpFileFullPath,
//    size_t cbDumpFileFullPath,
//    char * szCrashLogFileFullPath,
//    size_t cbCrashLogFileFullPath)
//{
//    if (cszMiniDumpDataDir == NULL ||
//        szDumpFileFullPath == NULL ||
//        szCrashLogFileFullPath == NULL)
//        return false;
//
//    char szCurrentProgramNameFullPath[MAX_PATH];
//
//    const char *cszDumpAndCrashLogFileNameBase =
//        GetCrashDumpModulePrefix(
//            szCurrentProgramNameFullPath,
//            sizeof(szCurrentProgramNameFullPath));
//
//    DWORD dwProcessId = GetCurrentProcessId();
//
//    char szTempPrefix[MAX_PATH];
//
//    HRESULT h;
//    h = StringCbPrintfA (
//        szTempPrefix, sizeof(szTempPrefix),
//        "%s\\%s.%u.%s.GMT-%04u-%02u-%02u-%02u-%02u-%02u",
//        cszMiniDumpDataDir, cszDumpAndCrashLogFileNameBase,
//        dwProcessId,fullDump?"full":"mini",
//        stCurrent.wYear,stCurrent.wMonth,stCurrent.wDay,
//        stCurrent.wHour,stCurrent.wMinute,stCurrent.wSecond
//        );
//    if (FAILED(h))
//        return false;
//
//    h = StringCbPrintfA (szDumpFileFullPath, cbDumpFileFullPath, "%s.dmp", szTempPrefix);
//    if (FAILED(h))
//    {
//        SetLastError(h);
//        return false;
//    }
//
//    h = StringCbPrintfA (szCrashLogFileFullPath, cbCrashLogFileFullPath, "%s.crash", szTempPrefix);
//    if (FAILED(h))
//    {
//        SetLastError(h);
//        return false;
//    }
//
//    return true;
//}
//
//static bool GetProcessStartTime (HANDLE hProcess, DateTime * pdtStart)
//{
//    FILETIME ftCreate, ftExit, ftKernel, ftUser;
//    if (!GetProcessTimes (hProcess, &ftCreate, &ftExit, &ftKernel, &ftUser))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot get process start time", "Last Error Code %u", GetLastError());
//        return false;
//    }
//    pdtStart->Set (ftCreate);
//    return true;
//}
//
//// Asserts so we can safely cast between these types.
//C_ASSERT(sizeof(FILETIME) == sizeof(ULONGLONG));
//C_ASSERT(sizeof(FILETIME) == sizeof(UInt64));
//
//// This file writes to the crash log. It is paired up with the function
//// ReadCrashLogFile. The two functions should match.
//// In the crash log file, we would like to store the process ID and the
//// process start time, these 2 values can uniquely identify a process.
//// We would also specify a time that the process is expected to die. If
//// by the specified time, the process still exists, that means the process
//// needs to be killed by someone.
//bool WriteCrashLogFile (HANDLE hProcess, DWORD dwProcessId, const char * cszCrashLogFileFullPath, unsigned int timeoutMilliSeconds)
//{
//    // get the current process start time
//    DateTime dtStart;
//    if (!GetProcessStartTime(hProcess, &dtStart))
//        return false;
//
//    // compute the expiration time
//    FILETIME ftExpire;
//    GetSystemTimeAsFileTime(&ftExpire);
//    UInt64 u64Expire; // should be using ULARGE_INTEGER but we can use these types interchangeably.
//    memcpy (&u64Expire, &ftExpire, sizeof(u64Expire)); // according to MSDN, we need to do this to avoid memory alignment problem on 64bit Windows
//    UInt64 u64Timeout = (UInt64) timeoutMilliSeconds * (UInt64) 10000; // 1 millisecond = 10000 100-nanoseconds
//    u64Expire += u64Timeout;
//    memcpy (&ftExpire, &u64Expire, sizeof(ftExpire));
//    DateTime dtExpire (ftExpire);
//
//    // open to write the crash file
//    FILE * fp = fopen (cszCrashLogFileFullPath, "w");
//    if (fp == NULL)
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Write Crash Log File", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    ::ScopeGuard fileGuard = ::MakeGuard (fclose, fp);
//
//    // write the crash log file
//    char line[1024];
//    fprintf (fp, "%I64u\n", (Int64)dtExpire.ConvertToUInt64()); // accurate expiration time the code actually uses
//    dtExpire.ToString (line, sizeof(line));
//    fprintf (fp, "%s\n", line); // human friendly expiration time for debugging
//    fprintf (fp, "%u\n", dwProcessId); // write the process id
//    fprintf (fp, "%I64u\n", (Int64)dtStart.ConvertToUInt64()); // accurate process start time the code actually uses
//    dtStart.ToString (line, sizeof(line));
//    fprintf (fp, "%s\n", line); // human friendly expiration time for debugging
//    fclose(fp);
//
//    Log(LogID_Logging, LogLevel_Info, "Wrote Crash Log File", "File %s, ProcessId %u", cszCrashLogFileFullPath, dwProcessId);
//    return true;
//}
//
//// This file reads to the crash log. It is paired up with the function
//// WriteCrashLogFile. The two functions should match.
//// In the crash log file, we would like to store the process ID and the
//// process start time, these 2 values can uniquely identify a process.
//// We would also specify a time that the process is expected to die. If
//// by the specified time, the process still exists, that means the process
//// needs to be killed by someone.
//static bool ReadCrashLogFile (
//    const char * cszCrashLogFileFullPath,
//    DateTime * pdtExpire,
//    DWORD * pdwProcessId,
//    DateTime * pdtProcessStart)
//{
//    FILE * fp = fopen (cszCrashLogFileFullPath, "r");
//    if (fp == NULL)
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Read Crash Log File", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    ::ScopeGuard fileGuard = ::MakeGuard (fclose, fp);
//
//    char line[1024];
//    ZeroMemory(line, sizeof(line));
//    // read the exact expiration time in the form of UInt64
//    if (!fgets(line, sizeof(line)-1, fp))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Read Exact Expire Time", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    char *endptr;
//    UInt64 u64Expire = _strtoui64 (line, &endptr, 10);
//    if (*endptr != 0 && !isspace(*endptr))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Parse Exact Expire Time", "Text %s, File %s, Last Error Code %u", line, cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    // read the expiration time
//    if (!fgets(line, sizeof(line)-1, fp))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Read Friendly Expire Time", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    *pdtExpire = DateTime::ConvertToDateTime(u64Expire);
//
//    // read the process id
//    if (!fgets(line, sizeof(line)-1, fp))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Read Process Id", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    *pdwProcessId = atoi(line);
//    if (*pdwProcessId == 0)
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Parse Process Id", "Text %s, File %s, Last Error Code %u", line, cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//
//    // read the exact start time in the form of UInt64
//    if (!fgets(line, sizeof(line)-1, fp))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Read Exact Start Time", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    UInt64 u64Start = _strtoui64 (line, &endptr, 10);
//    if (*endptr != 0 && !isspace(*endptr))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Parse Exact Start Time", "Text %s, File %s, Last Error Code %u", line, cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//
//    // read the process start time
//    if (!fgets(line, sizeof(line)-1, fp))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Read Process Start Time", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    *pdtProcessStart = DateTime::ConvertToDateTime(u64Start);
//
//    return true;
//}
//
//// Given a crash file that are written together with a minidump, this program will read the crash file and
//// use the information in the crash file to decide what to do. If the process has expired and the process is
//// still running, then this function will terminate that process. If there is the process is no longer running,
//// this fucntion will delete the crash file.
//// If dtCurrent is NULL, no expiration check is performed and the process is always terminated.
//static bool CheckAndKillAProcessFromCrashLogFile (const char * cszCrashLogFileFullPath, const DateTime * pdtCurrent)
//{
//    DateTime dtExpire, dtStartTimeInLog, dtCurrentStartTime;
//    DWORD dwProcessId;
//
//    if (!ReadCrashLogFile (cszCrashLogFileFullPath, &dtExpire, &dwProcessId, &dtStartTimeInLog))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Failed to Read Crash Log File", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    Log(LogID_Logging, LogLevel_Info, "Read Crash Log File", "File %s Expire %I64u Process %u StartTime %I64u", 
//        cszCrashLogFileFullPath, dtExpire.ConvertToUInt64(), dwProcessId, dtStartTimeInLog.ConvertToUInt64());
//
//    if (pdtCurrent != NULL)
//    {
//        if (*pdtCurrent < dtExpire)
//        {
//            Log(LogID_Logging, LogLevel_Info, "Not Expired Yet", "File %s, Expire %lu, Current %lu, Last Error Code %u",
//                cszCrashLogFileFullPath, dtExpire.ConvertToUInt64(), pdtCurrent->ConvertToUInt64(), GetLastError());
//            return true; // if the process has not expired yet, we don't want to kill it
//        }
//
//        Log(LogID_Logging, LogLevel_Info, "Expired", "File %s, Expire %lu, Current %lu",
//            cszCrashLogFileFullPath, dtExpire.ConvertToUInt64(), pdtCurrent->ConvertToUInt64());
//    }
//
//    // the process has expired, we need to check if the process still exists.
//    // note: to uniquely identify a process through time, we need both process id
//    // and process start time since the system recycle process id
//    HANDLE hProcess = OpenProcess (PROCESS_QUERY_INFORMATION|PROCESS_TERMINATE|SYNCHRONIZE, false, dwProcessId);
//    if (hProcess != NULL)
//    {
//        ::ScopeGuard processGuard = ::MakeGuard (CloseHandle, hProcess);
//
//        // Eventhough we can get a handle to a process, the process might have already
//        // exited. The OS might just keep it around a bit. We should check to see if
//        // it has exited.
//        DWORD dwWait = WaitForSingleObject(hProcess, 0);
//        if (dwWait != WAIT_OBJECT_0)
//        {
//            // the process has not exited
//            // we will check the start time to make sure this is the process we are looking for
//            if (!GetProcessStartTime(hProcess, &dtCurrentStartTime))
//            {
//                Log(LogID_Logging, LogLevel_Error, "Failed to Get Process Start Time", "File %s, Process %u, Last Error Code %u", cszCrashLogFileFullPath, dwProcessId, GetLastError());
//                return false;
//            }
//
//            if (dtStartTimeInLog == dtCurrentStartTime)
//            {
//                // The process has the same start time, it is the same process still not died
//                Log(LogID_Logging, LogLevel_Info, "Calling TerminateProcess", "File %s, Process %u", 
//                    cszCrashLogFileFullPath, dwProcessId);
//                if (!TerminateProcess(hProcess, 1))
//                {
//                    DWORD dwError = GetLastError();
//                    Log(LogID_Logging, LogLevel_Error, "Failed to Kill Process", "File %s, Process %u, Last Error Code %u", cszCrashLogFileFullPath, dwProcessId, dwError);
//                    return false;
//                }
//            }
//            else
//            {
//                Log(LogID_Logging, LogLevel_Warning,
//                    "Same PID With Different Start Time", "File %s, Process %u, Start Time in Log %I64u, Start Time in System %I64u",
//                    cszCrashLogFileFullPath, dwProcessId, dtStartTimeInLog.ConvertToUInt64(), dtCurrentStartTime.ConvertToUInt64());
//            }
//
//        }
//    }
//    else
//    {
//        DWORD dwError = GetLastError();
//        Log(LogID_Logging, LogLevel_Info, "OpenProcess failed", "File %s Last Error Code %u", 
//        cszCrashLogFileFullPath, dwError);
//    }
//
//    Log(LogID_Logging, LogLevel_Info, "Deleting File", "File %s", cszCrashLogFileFullPath);
//    // When the code reach here, either the process has been long dead or it has been killed successfully.
//    // In that case, we no longer need the crash log file.
//    if (!DeleteFileA (cszCrashLogFileFullPath))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Failed To Delete File", "File %s, Last Error Code %u", cszCrashLogFileFullPath, GetLastError());
//        return false;
//    }
//    Log(LogID_Logging, LogLevel_Info, "Successfully Deleted File", "File %s", cszCrashLogFileFullPath);
//    return true;
//}

//// For every minidump written, we will write out a crash log file. It contains information about
//// the process that was writing the minidump and when the process expected to be considered as hung.
//// This function will go through the minidump data dir and make sure those hunging process
//// are killed.
//// Note: the expire time is normally the current system time. We expose the parameter to make it easier
//// for testing. I don't want to use any date time data structure we implemented (such as DateTime) because
//// I want to avoid circluar includes.
//// If the expiration time is NULL, no expiration check is performed and the process is always considered
//// a hung.
//bool Logger::KillPossibleHungMiniDumpWritingProcess (const SYSTEMTIME * stExpire)
//{
//    DateTime dtExpire, * pdtExpire = NULL;
//    if (stExpire != NULL)
//    {
//        dtExpire.Set (*stExpire);
//        pdtExpire = &dtExpire;
//    }    
//
//    char szMiniDumpDataDir[MAX_PATH];
//    if (!Logger::GetMiniDumpDataDir(szMiniDumpDataDir, sizeof(szMiniDumpDataDir)))
//        return false;
//
//    // We will go through all the *.crash files under the minidump dir
//    char szFilesToFind[MAX_PATH];
//    HRESULT h = StringCbPrintfA (szFilesToFind, sizeof(szFilesToFind), "%s\\*.crash", szMiniDumpDataDir);
//    if(FAILED(h))
//        return false;
//
//    WIN32_FIND_DATAA findFileData;
//    HANDLE hFind = FindFirstFileA (szFilesToFind, &findFileData);
//    if (hFind == INVALID_HANDLE_VALUE)
//    {
//        DWORD dwError = GetLastError();
//        if (dwError != ERROR_PATH_NOT_FOUND && dwError != ERROR_FILE_NOT_FOUND)
//        {
//            Log(LogID_Logging, LogLevel_Error, "Failed to Search MiniDumpDataDir", "Pattern %s, Last Error Code %u", szFilesToFind, dwError);
//            return false;
//        }
//        else
//            return true; // If there are no crash files found, there is nothing to do, but the function is considered succeeded
//    }
//    else
//    {
//        ::ScopeGuard findGuard = ::MakeGuard (FindClose, hFind);
//        do
//        {
//            // generate the full path of the crash file found
//            char szFileFullPath[MAX_PATH];
//            HRESULT h = StringCbPrintfA (szFileFullPath, sizeof(szFileFullPath), "%s\\%s", szMiniDumpDataDir, findFileData.cFileName);
//            if (FAILED(h))
//                return false;
//
//            // check to see if the process associated with the crash file has exited, if not, terminate the process
//            // if the process is successfully terminated, the crash file will be deleted, otherwise, the file would be
//            // left alone.
//            Log(LogID_Logging, LogLevel_Info, "Trying to Check and Removing Hanging Process", "File %s", szFileFullPath);
//            if (!CheckAndKillAProcessFromCrashLogFile (szFileFullPath, pdtExpire))
//            {
//                Log(LogID_Logging, LogLevel_Error, "Failed to Clean", "File %s, Last Error Code %u", szFileFullPath, GetLastError());
//            }
//        } while (FindNextFileA(hFind, &findFileData) != 0);
//
//        DWORD dwError = GetLastError();
//        if (dwError != ERROR_NO_MORE_FILES)
//        {
//            Log(LogID_Logging, LogLevel_Info, "Find Next File Failed", "Last Error Code %u", dwError);
//            return false;
//        }
//        return true;
//    }
//}
//
//
//ULONGLONG
//Logger::EnumerateAllCrashDumpsInDirectory(
//    __in PCSTR szMiniDumpDataDir,
//    __out std::map<ULONGLONG, CrashdumpFileInfo>& dumpMap
//    )
//{
//    WIN32_FIND_DATAA findFileData;
//    HANDLE hFind;
//    BOOL succeeded;
//
//    ULONGLONG totalDumpUsage = 0;
//
//    //
//    // Since the Find*File routines do not guarantee any sort order,
//    // we must first enumerate all files and store their creation
//    // timestamps.
//    //
//
//    //
//    // Get the crash dump filename prefix for the current
//    // module. We won't look at crash dumps generated by
//    // other EXEs.
//    //
//    char dumpFilePrefixStorage[MAX_PATH];
//    const char *dumpFilePrefix =
//        GetCrashDumpModulePrefix(
//            dumpFilePrefixStorage, sizeof(dumpFilePrefixStorage));
//
//    std::string filePath(szMiniDumpDataDir);
//    filePath.append("\\");
//    filePath.append(dumpFilePrefix);
//    filePath.append("*.dmp");
//
//    hFind =
//        FindFirstFileA(
//            filePath.c_str(),
//            &findFileData);
//
//    succeeded = (hFind != INVALID_HANDLE_VALUE);
//
//    while (succeeded) {
//
//        //
//        // FILETIME can be safely cast as a ULONGLONG.
//        // Both are 64 bit integers with bytes in little
//        // endian order.
//        //
//
//        ULONGLONG creationTime =
//            (*((PULONGLONG) &findFileData.ftCreationTime));
//
//        std::map<ULONGLONG, CrashdumpFileInfo>::iterator it;
//
//        it = dumpMap.find(creationTime);
//
//        //
//        // Two dumps with the exact same timestamp?
//        // Should never happen but just ignore
//        // the second one, it should get cleaned
//        // up later.
//        //
//
//        if (it == dumpMap.end()) {
//            std::string fqFileName(szMiniDumpDataDir);
//
//            CrashdumpFileInfo dumpInfo;
//
//            fqFileName.append("\\");
//            fqFileName.append(findFileData.cFileName);
//
//            dumpInfo.fileName = fqFileName;
//            dumpInfo.fileSize =
//                (((ULONGLONG) findFileData.nFileSizeHigh) << 32) +
//                findFileData.nFileSizeLow;
//
//            dumpMap[creationTime] = dumpInfo;
//
//            totalDumpUsage += dumpInfo.fileSize;
//        }
//
//        succeeded = FindNextFileA(hFind, &findFileData);
//
//    }
//
//    if (hFind != INVALID_HANDLE_VALUE) {
//        FindClose(hFind);
//    }
//
//    return totalDumpUsage;
//}
//
//
////
//// Attempt to delete the oldest crash dumps in the supplied
//// directory until we are at or below the threshold. We will
//// not try to delete crash dumps that were created by other
//// executables to avoid losing all crash dumps for that
//// executable. This is not a perfect solution as it is
//// possible that we lose all our own crash dumps and yet
//// fail to write a new crash dump because it still won't
//// fit in available disk space. This is possible if we are
//// running very low on disk space due to excessive usage
//// by log files/data and when crash dumps are not the
//// largest consumer of disk space.
////
//ULONGLONG
//Logger::DeleteOldCrashDumps(
//    __in PCSTR szMiniDumpDataDir,
//    __in ULONGLONG totalDumpUsage,
//    __out std::map<ULONGLONG, CrashdumpFileInfo>& dumpMap
//    )
//{
//    Size_t countExistingDumps;
//    ULONGLONG diskSpaceReclaimed = 0;
//
//    std::map<ULONGLONG, CrashdumpFileInfo>::iterator it;
//
//    countExistingDumps = dumpMap.size();
//
//    //
//    // The iterator will enumerate them in order of their
//    // file timestamps so that we delete the oldest
//    // crash dumps first.
//    //
//    for (it = dumpMap.begin(); it != dumpMap.end(); ++it) {
//
//        CrashdumpFileInfo& dumpInfo = it->second;
//
//
//        if ((totalDumpUsage < m_maxCrashdumpUsage) ||
//            (countExistingDumps == 1)) {
//
//            //
//            // Stop deleting. We have enough disk
//            // space available now. Also don't delete
//            // the last crash dump since we could fail
//            // while writing a new dump and leave no
//            // way to debug any failures at all.
//            //
//
//            break;
//        }
//
//        BOOL succeeded = DeleteFileA(dumpInfo.fileName.c_str());
//
//        //
//        // If we succeeded, then let's also delete the .crash file.
//        //
//        if (succeeded) {
//            std::string crashFileName = dumpInfo.fileName;
//
//            size_t extension = crashFileName.rfind(".dmp");
//
//            Log(
//                LogID_Logging, LogLevel_Error,
//                "Deleting old crash dumps to reclaim space",
//                "Deleting file %s",
//                crashFileName.c_str());
//
//            if (extension != std::string::npos) {
//
//                crashFileName.replace(extension, 4, ".crash");
//                (void) DeleteFileA(crashFileName.c_str());
//
//            }
//
//            --countExistingDumps;
//            totalDumpUsage -= dumpInfo.fileSize;
//            diskSpaceReclaimed += dumpInfo.fileSize;
//        }
//    }
//
//    return diskSpaceReclaimed;
//}
//
//
//bool
//Logger::CheckMinidumpDirectory(
//    __in PCSTR szMiniDumpDataDir
//    )
//{
//    //
//    // Make sure that the minidump directory is already created
//    //
//
//    if (!DirUtils::MakeDirectory(szMiniDumpDataDir))
//    {
//        Log(
//            LogID_Logging, LogLevel_Error,
//            "Failed to create minidump dir",
//            "Dir %s, Last Error Code %u",
//            szMiniDumpDataDir, GetLastError());
//        return false;
//    }
//
//
//    std::map<ULONGLONG, CrashdumpFileInfo> dumpMap;
//
//    ULONGLONG totalDumpUsage =
//        EnumerateAllCrashDumpsInDirectory(szMiniDumpDataDir, dumpMap);
//
//
//    ULONGLONG diskSpaceReclaimed =
//        DeleteOldCrashDumps(szMiniDumpDataDir, totalDumpUsage, dumpMap);
//
//    //
//    // After deleting crash dumps, are we under quota? If so
//    // then allow a new crash dump to be created.
//    //
//
//    if ((totalDumpUsage - diskSpaceReclaimed) < m_maxCrashdumpUsage)
//    {
//        return true;
//    }
//    else
//    {
//        Log(
//            LogID_Logging,
//            LogLevel_Error,
//            "Check crash dump quota",
//            "Insufficient space to write a crash dump. "
//            "Current crash dump disk usage is %I64d",
//            (totalDumpUsage - diskSpaceReclaimed));
//
//        return false;
//    }
//}
//
//
////
//// This function is called by Logger::LogAndExitProcess.
////
//// This function is invoked whenever a role instance generates an
//// unrecoverable exception and it is responsible for writing out a
//// process minidump that can be used later to debug the exceptions.
////
//// This function notifies the crash dump process to take a dump of the current process. If the dump is taken
//// successfully, the current process is terminated by crash dump process. Otherwise, TerminateProcess
//// is called at the end of this function. In either case, the function never returns.
////
//// There are two reasons to take the dump out of proc:
////   1. Taking a dump in proc can result in deadlock since MiniDumpWriteDump needs to suspend the current
////      process's threads.
////   2. After the dump is taken, there is a window for threads to continue executing before the process
////      is terminated. This can result in data corruption if the process is modifying the shared data.
////
//void Logger::WriteExceptionMiniDump (EXCEPTION_POINTERS* exceptionPointers, unsigned int timeoutMilliSeconds, bool fullDump)
//{
//    bool ret = false;
//
//    Log(LogID_Logging,
//        LogLevel_Error,
//        "WriteExceptionMiniDump called.");
//
//    // If there are multiple threads attempting to take a dump, only let the first thread continue.
//    if (!SetDumpOrTerminateProcess ())
//    {
//        WaitForSingleObject(GetCurrentProcess(), INFINITE);
//        goto Cleanup;
//    }
//
//    // Check whether we need to skip the dump. Under severe memory pressure, some roles may choose
//    // to skip dump creation since the system is paging out on the same spindle and the dump may
//    // take a very long time or have undesired side effects.
//    if (m_skipCrashDumpRamThresholdPercent > 0)
//    {
//        int availableRamPercent = 0;
//        
//        PERFORMANCE_INFORMATION perfinfo;
//        memset(&perfinfo, 0, sizeof(perfinfo));
//        if (GetPerformanceInfo( &perfinfo, (DWORD)sizeof(perfinfo)))
//        {
//            availableRamPercent = (int)(100 * perfinfo.PhysicalAvailable / perfinfo.PhysicalTotal);
//        }
//
//        if (availableRamPercent < m_skipCrashDumpRamThresholdPercent)
//        {
//            // Exit without taking a dump. Alert because we want to know how often this is
//            // happening.
//            Log(
//                LogID_Logging,
//                LogLevel_AppAlert,
//                "Crashing without creating a dump file",
//                "Skipping crash dump creation because the amount of available RAM is less than %d percent. "
//                "PhysicalAvailable=%I64u bytes, PhysicalTotal=%I64u bytes.", 
//                m_skipCrashDumpRamThresholdPercent,
//                (UINT64)perfinfo.PhysicalAvailable * perfinfo.PageSize,
//                (UINT64)perfinfo.PhysicalTotal * perfinfo.PageSize);
//
//            goto Cleanup;
//        }
//
//        // Must take dump
//        Log(LogID_Logging, LogLevel_Error,
//            "Continue to crash dump creation", 
//            "PhysicalAvailable=%I64u bytes, PhysicalTotal=%I64u bytes. "
//            "PercentAvailable=%d, PercentAvailableThreshold=%d",
//            (UINT64)perfinfo.PhysicalAvailable * perfinfo.PageSize,
//            (UINT64)perfinfo.PhysicalTotal * perfinfo.PageSize,
//            availableRamPercent,
//            m_skipCrashDumpRamThresholdPercent);
//    }
//
//    // flush out the current logs before preparing writing minidumps.
//    Logger::Flush(true);
//
//    // Construct absolute path for the folder where the files will be written.
//    // If SysInfo is not initialized, then the path will be "\minidumps".
//    char szMiniDumpDataDir[MAX_PATH];
//    if (!Logger::GetMiniDumpDataDir (szMiniDumpDataDir, sizeof(szMiniDumpDataDir)))
//    {
//        Log(LogID_Logging, LogLevel_Error, "Cannot Get Minidump Dir", "Last Error Code %u", GetLastError());
//        goto Cleanup;
//    }
//
//    if (!CheckMinidumpDirectory(szMiniDumpDataDir)) {
//        goto Cleanup;
//    }
//
//    // figure out the file name for the mini dump
//    // and the log file about the crash
//    char szDumpFileFullPath[MAX_PATH];
//    char szCrashLogFileFullPath[MAX_PATH];
//    {
//        SYSTEMTIME stNow;
//        GetSystemTime(&stNow);
//
//        bool f = GetCurrentProcessDumpAndCrashLogFileNameFullPath (
//            szMiniDumpDataDir, stNow, fullDump,
//            szDumpFileFullPath, sizeof(szDumpFileFullPath),
//            szCrashLogFileFullPath, sizeof(szCrashLogFileFullPath));
//        if (!f)
//        {
//            Log(LogID_Logging, LogLevel_Error, "Cannot Get file path for crash dumps", "Last Error Code %u", GetLastError());
//            goto Cleanup;
//        }
//    }
//
//    Log(LogID_Logging,
//        LogLevel_Error,
//        "Writing crash log file",
//        "Crash log file %s",
//        szCrashLogFileFullPath);
//
//    // flush out the current logs.
//    Logger::Flush(true);
//
//    // write the crash log so if the process hangs, it can be picked
//    // up by the cleaning service (killhungprocess) later
//    if (!WriteCrashLogFile (GetCurrentProcess(), GetCurrentProcessId(), szCrashLogFileFullPath, timeoutMilliSeconds))
//        goto Cleanup;
//
//    Log(LogID_Logging,
//        LogLevel_Error,
//        "Initiate out of proc dump",
//        "Dump file %s",
//        szDumpFileFullPath);
//
//    // flush out the current logs before start writing minidumps.
//    Logger::Flush(true);
//
//    NotifyCrashDumpProcess(exceptionPointers, szDumpFileFullPath, fullDump, &ret);
//
//    Log(LogID_Logging,
//        LogLevel_Error,
//        "Failed to take dump out of proc",
//        "Dump file %s",
//        szDumpFileFullPath);
//
//Cleanup:
//
//    Logger::Flush(true);
//
//    ::Terminate(1);
//}
//
////
//// This function notifies the external process to take a dump of the current process.
////
//VOID
//Logger::NotifyCrashDumpProcess(
//    __in EXCEPTION_POINTERS *exceptionPointers,
//    __in PCSTR szFilePath,
//    __in bool fullDump,
//    __out bool *pRet
//    )
//{
//    *pRet = false;
//
//    char pipeName[MAX_PATH];
//    BOOL retry = TRUE;
//    HANDLE pipeHandle = INVALID_HANDLE_VALUE;
//
//    if (!BuildPipeNameFromRoleDataDir(SysInfo::GetDataDir(), ARRAYSIZE(pipeName), pipeName))
//    {
//        Log(LogID_Logging,
//            LogLevel_Error,
//            "Failed to build pipe name",
//            "DataDir %s",
//            SysInfo::GetDataDir());
//
//        goto Cleanup;
//    }
//
//    while (TRUE)
//    {
//        Log(LogID_Logging,
//            LogLevel_Error,
//            "Connecting to pipe server.",
//            "Pipe name %s",
//            pipeName);
//
//        Logger::Flush(true);
//
//        pipeHandle = CreateFileA(
//            pipeName,
//            (GENERIC_READ | GENERIC_WRITE),
//            0,
//            NULL,
//            OPEN_EXISTING,
//            0,
//            NULL);
//
//        if (pipeHandle != INVALID_HANDLE_VALUE)
//        {
//            break;
//        }
//
//        //
//        // If the pipe is not opened yet, sleep for a minute and try one more time.
//        //
//
//        if ((GetLastError() == ERROR_FILE_NOT_FOUND) && retry)
//        {
//            Sleep(60000);
//            retry = FALSE;
//
//            Log(LogID_Logging,
//                LogLevel_Error,
//                "Pipe server has not started. Wait for 60 seconds to retry",
//                "Pipe name %s",
//                pipeName);
//        }
//        else
//        {
//            Log(LogID_Logging,
//                LogLevel_Error,
//                "Failed to connect to named pipe",
//                "Pipe name %s",
//                pipeName);
//
//            goto Cleanup;
//        }
//    }
//
//    XS_DUMP_MESSAGE dumpMessage;
//    ZeroMemory(&dumpMessage, sizeof(dumpMessage));
//
//    dumpMessage.ExceptionRecord = *(exceptionPointers->ExceptionRecord);
//    dumpMessage.ContextRecord = *(exceptionPointers->ContextRecord);
//    dumpMessage.ProcessId = GetCurrentProcessId();
//    dumpMessage.ThreadId = GetCurrentThreadId();
//
//    if (fullDump)
//    {
//        dumpMessage.DumpFlags = (MiniDumpNormal | MiniDumpWithFullMemory | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo);
//
//        //
//        // If an excluded region is requested, add the excluded memory region to the dump message.
//        //
//
//        if (g_ExcludedMemoryRegionSizeInBytes > 0)
//        {
//            Log(LogID_Logging,
//                LogLevel_Error,
//                "Excluded memory region from the dump",
//                "Starting address %I64x, size %I64d",
//                g_ExcludedMemoryRegionBaseAddress,
//                g_ExcludedMemoryRegionSizeInBytes);
//
//            Logger::Flush(true);
//
//            dumpMessage.ExcludedMemoryRegionBaseAddress = g_ExcludedMemoryRegionBaseAddress;
//            dumpMessage.ExcludedMemoryRegionSizeInBytes = g_ExcludedMemoryRegionSizeInBytes;
//        }
//    }
//    else
//    {
//        dumpMessage.DumpFlags = MiniDumpWithDataSegs;
//    }
//
//    HRESULT hr = StringCchCopyA(
//        dumpMessage.DumpFilePath,
//        ARRAYSIZE(dumpMessage.DumpFilePath),
//        szFilePath);
//
//    if (FAILED(hr))
//    {
//        Log(LogID_Logging,
//            LogLevel_Error,
//            "Failed to copy dump file full path",
//            "File path %s",
//            szFilePath);
//
//        goto Cleanup;
//    }
//
//    Log(LogID_Logging,
//        LogLevel_Error,
//        "Sending dump request.",
//        "Pipe name %s",
//        pipeName);
//
//    Logger::Flush(true);
//
//    DWORD pipeMode = PIPE_READMODE_MESSAGE;
//    if (!SetNamedPipeHandleState(pipeHandle, &pipeMode, NULL, NULL))
//    {
//        Log(LogID_Logging,
//            LogLevel_Error,
//            "Failed to set pipe state",
//            "Last error 0x%0x",
//            GetLastError());
//
//        goto Cleanup;
//    }
//
//    DWORD bytesWritten;
//    if (!WriteFile(pipeHandle, &dumpMessage, sizeof(dumpMessage), &bytesWritten, NULL))
//    {
//        Log(LogID_Logging,
//            LogLevel_Error,
//            "Failed to send crash dump message",
//            "Last error 0x%0x",
//            GetLastError());
//
//        goto Cleanup;
//    }
//
//    //
//    // After the dump process receives the dump request, it will take the dump and terminate current process.
//    // Under normal circumstance, the code below will not be executed. If the dump process cannot respond
//    // within 15 seconds, terminate the current process without taking a dump.
//    //
//    // N.B. The 15 second sleep is based on the lease renew time of xtable server. The idea is to minimize
//    //      the time for a crashing xtable server to run after its lease has expired.
//    //
//
//    *pRet = true;
//    Sleep(15000);
//
//Cleanup:
//
//    Logger::Flush(true);
//
//    if (pipeHandle != INVALID_HANDLE_VALUE)
//    {
//        CloseHandle(pipeHandle);
//    }
//
//    return;
//}
//
//LONG Logger::LogAndExitProcess(EXCEPTION_POINTERS* exceptionPointers)
//{
//    LogAndExitProcess(exceptionPointers, 15*60*1000, Logger::FullDumpEnabled());
//}
//
//void Logger::LogAndExitProcess(
//    __in EXCEPTION_POINTERS * exceptionPointers, 
//    __in unsigned int timeoutMilliSeconds, 
//    __in bool fullDump)
//{
//    __try
//    {
//        // Make sure the crashing flag is set.
//        SetCrashing();
//
//        Log(
//            LogID_Logging,
//            LogLevel_Error,
//            "An unhandled exception was thrown, exiting process..."
//            );
//        Logger::Flush(true);
//
//        // If we are using the fine grained timer, then we
//        // must make a best effort to match the call to
//        // timeBeginPeriod with timeEndPeriod.
//
//        if (m_bUseFineGrainedTimer) {
//            timeEndPeriod(1);
//            m_bUseFineGrainedTimer = false;
//        }
//
//        WriteExceptionMiniDump (exceptionPointers, 15*60*1000, Logger::FullDumpEnabled());
//
//#if defined(_WIN64)
//        // We aren't supposed to ever get here...
//        goto terminate;
//#endif
//    }
//    __except(EXCEPTION_EXECUTE_HANDLER)
//    {
//        // The exception handler failed. Continue below.
//    }
//
//    // Make a last attempt to log an error, then terminate the process.
//    __try
//    {
//        char * title = "Crash handler failed to handle the exception.";
//
//        LogV(__FILE__, __FUNCTION__, __LINE__, LogID_Logging, LogLevel_Error, title, NULL, NULL);
//
//        Logger::Flush(true);
//    }
//    __except(EXCEPTION_EXECUTE_HANDLER)
//    {
//        // There's nothing else we can do but terminate.
//    }
//
//
//#if defined(_WIN64)
//terminate:
//#endif
//
//    ::TerminateProcess(GetCurrentProcess(), 1);
//}
//
//LONG Logger::WriteMiniDumpExceptionFilter(
//    __in EXCEPTION_POINTERS* exceptionPointers, 
//    __in unsigned int timeoutMilliSeconds, 
//    __in bool fullDump)
//{
//    LogAndExitProcess(exceptionPointers, timeoutMilliSeconds, fullDump);
//}


// This function will write a process mini dump.
// We need to turn off compiler optimization to guareente
// this function to work. Otherwise, sometimes under AMD64 retail build,
// the exception in this function might not happen.
#pragma optimize ("", off)
//bool Logger::WriteMiniDump (unsigned int timeoutMilliSeconds, bool fullDump)
//{
//    // According to MSDN (and with experience), the system
//    // call that writes out the mini dump doesn't work well without the exception pointers.
//    // Thus, in order to write out a mini dump, we need to get the thread state before
//    // starting to write the dump.
//    //
//    // Therefore, we need a hack here. We generate a NULL-referencing exception to get the
//    // thread state. We are not using "throw" because it creates more garbage in the stack
//    // trace. We could also call the mini dump function from a new worker thread and filter
//    // this worker thread from the dump, but that seems to be too much work as well.
//    // We have also tried to use RaiseException function, but that couldn't do the job.
//
//    __try
//    {
//        int * ptr = NULL;
//#pragma prefast(suppress:11, "We intentionly generated NULL referencing here, reviewed by minara")
//        *ptr = 0;
//    }
//    __except (WriteMiniDumpExceptionFilter(GetExceptionInformation(), timeoutMilliSeconds, fullDump))
//    {
//#pragma prefast(suppress:322, "The work has to be done in exception filter above, nothing to do here, reviewed by minara")
//        // do nothing
//    }
//
//    return false;
//}


// Crash the current process by generating an unhandled exception.
__declspec(noreturn) void Logger::Crash()
{
    int *ptr = NULL;
#pragma prefast(suppress:11, "Intentional NULL derefernce")
    *ptr = 0;
}

#pragma optimize ("", on)
    // Important: Since the optimize pragma doesn't seem to support push/pop, I put this function at
    // the end of the file, so it won't affect any other functions. If you add your function after
    // this line, you will be affected by the possiblly changed optimization settings.


