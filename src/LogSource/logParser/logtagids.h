//
// This file lists all the known tag values for logging.
//
// This file is included twice, with DeclareTag() defined to do different things, to initialize
// different data structures with the logging code.
//
// Add any new named log tags to this file
// The IDs MUST be sequential
// Update LogTag_End if you add a new one
//
// The value in quotes is the name under which it will appear in the log file;
// e.g. Filename="Foo"
//

// Use these as generic log tags parameters if you don't feel it makes sense to add a new
// log tag entry.  There really is no real overhead for adding more, though, so if you're
// going to log the same data item in more than a few places, you probably want to consider
// making a log tag for it, in case we want WatchDog to be able to key off it.
DeclareTag(0, LogTag_Int1,          "Int1",             LogTagType_Int32),
DeclareTag(1, LogTag_Int2,          "Int2",             LogTagType_Int32),
DeclareTag(2, LogTag_Int64_1,       "Int64_1",          LogTagType_Int64),
DeclareTag(3, LogTag_Int64_2,       "Int64_2",          LogTagType_Int64),
DeclareTag(4, LogTag_String1,       "String1",          LogTagType_String),
DeclareTag(5, LogTag_String2,       "String2",          LogTagType_String),
DeclareTag(6, LogTag_WString1,      "WString1",         LogTagType_WideString), 
DeclareTag(7, LogTag_WString2,      "WString2",         LogTagType_WideString),
DeclareTag(8, LogTag_UInt1,         "UInt1",            LogTagType_UInt32),
DeclareTag(9, LogTag_UInt2,         "UInt2",            LogTagType_UInt32),
DeclareTag(10, LogTag_U32X1,         "Hex1",            LogTagType_Hex32), 
DeclareTag(11, LogTag_U32X2,         "Hex2",            LogTagType_Hex32),
DeclareTag(12, LogTag_U64X1,         "Hex64_1",         LogTagType_Hex64), 
DeclareTag(13, LogTag_U64X2,         "Hex64_2",         LogTagType_Hex64),
DeclareTag(14, LogTag_Float1,        "Float1",          LogTagType_Float),

DeclareTag(15, LogTag_Filename,     "Filename",          LogTagType_String),
DeclareTag(16, LogTag_TargetServer, "TargetServer",      LogTagType_String),
DeclareTag(17, LogTag_QueryString,  "Q",                 LogTagType_String),
DeclareTag(18, LogTag_TraceID,      "TraceID",           LogTagType_String),
DeclareTag(19, LogTag_URL,          "URL",              LogTagType_String),
DeclareTag(20, LogTag_ErrorCode,    "ErrorCode",        LogTagType_UInt32),
DeclareTag(21, LogTag_ThreadID,    "ThreadID",          LogTagType_Int32),
DeclareTag(22, LogTag_Description, "Desc",              LogTagType_String),

// ids for ini error
DeclareTag(23, LogTag_Section,     "Section",           LogTagType_String),
DeclareTag(24, LogTag_Param1,      "Param1",            LogTagType_String),
DeclareTag(25, LogTag_Param2,      "Param2",            LogTagType_String),
DeclareTag(26, LogTag_Param3,      "Param3",            LogTagType_String),
DeclareTag(27, LogTag_Param4,      "Param4",            LogTagType_String),

// ids for SQL ODBC error
DeclareTag(28, LogTag_SQLMessage,          "SQLMessage",        LogTagType_String),
DeclareTag(29, LogTag_SQLErrorCode,        "SQLErrorCode",      LogTagType_Int32),
DeclareTag(30, LogTag_SQLState,            "SQLState",          LogTagType_String),

// ids for SQL BCP error
DeclareTag(31, LogTag_SQLBCPFunction,      "SQLBCPFunction",    LogTagType_String),
DeclareTag(32, LogTag_SQLBCPToTable,       "SQLBCPToTable",     LogTagType_String),
DeclareTag(33, LogTag_SQLBCPColumn,        "SQLBCPColumn",      LogTagType_Int32),
DeclareTag(34, LogTag_SQLBCPRowCountSum,   "SQLBCPRowCountSum", LogTagType_Int32),
DeclareTag(35, LogTag_SQLBCPRowCountInc,   "SQLBCPRowCountInc", LogTagType_Int32),
DeclareTag(36, LogTag_SQLBCPRowCountCur,   "SQLBCPRowCountCur", LogTagType_Int32),

// ids for aggregator machine status notifications
DeclareTag(37, LogTag_ISNName,        "ISNName", LogTagType_String),
DeclareTag(38, LogTag_ISNService,     "ISNService", LogTagType_String),
DeclareTag(39, LogTag_ISNFailureType, "ISNFailureType", LogTagType_String),

// ids for Caption Generator
DeclareTag(40, LogTag_DocId,            "DocId",            LogTagType_Hex64),
DeclareTag(41, LogTag_ContentChunkId,   "ContentChunkId",   LogTagType_Int32),

// ids for Netlib
DeclareTag(42, LogTag_Port,         "Port",             LogTagType_UInt32),
DeclareTag(43, LogTag_IP,           "IP",               LogTagType_String),
DeclareTag(44, LogTag_NumericIP,    "NumericIP",        LogTagType_Hex32),
DeclareTag(45, LogTag_LastError,    "LastError",        LogTagType_UInt32),

// ids for Fcslite
DeclareTag(46, LogTag_PacketId,             "PacketId",             LogTagType_UInt32),
DeclareTag(47, LogTag_ConfigName,           "ConfigName",           LogTagType_String),
DeclareTag(48, LogTag_ResultBase,           "ResultBase",           LogTagType_UInt32),
DeclareTag(49, LogTag_ResultsCount,         "ResultsCount",         LogTagType_UInt32),
DeclareTag(50, LogTag_MaxResultsPerHost,    "MaxResultsPerHost",    LogTagType_UInt32),
DeclareTag(51, LogTag_FcsOptions,           "FcsOptions",           LogTagType_Hex64),
DeclareTag(52, LogTag_AggregatorOptions,    "AggregatorOptions",    LogTagType_Hex64),
DeclareTag(53, LogTag_QueryOptions,         "QueryOptions",         LogTagType_Hex64),
DeclareTag(54, LogTag_CaptionOptions,       "CaptionOptions",       LogTagType_Hex64),
DeclareTag(55, LogTag_NextTierToQuery,      "NextTierToQuery",      LogTagType_UInt32),
DeclareTag(56, LogTag_MaxTiersInIndex,      "MaxTiersInIndex",      LogTagType_UInt32),
DeclareTag(57, LogTag_RowID,                "RowID",                LogTagType_UInt32),

// These should have been in the generic section but are here to be in the proper range
DeclareTag(58, LogTag_UInt641,       "UInt641",          LogTagType_UInt64),
DeclareTag(59, LogTag_UInt642,       "UInt642",          LogTagType_UInt64),

#if defined(_M_AMD64)
DeclareTag(60, LogTag_Sizet1,       "Size_t1",      LogTagType_UInt64),
DeclareTag(61, LogTag_Sizet2,       "Size_t2",      LogTagType_UInt64),
#else
DeclareTag(60, LogTag_Sizet1,       "Size_t1",      LogTagType_UInt32),
DeclareTag(61, LogTag_Sizet2,       "Size_t2",      LogTagType_UInt32),
#endif

DeclareTag(62, LogTag_ActiveMCP,    "Active",           LogTagType_Int32),
DeclareTag(63, LogTag_VoteCount,    "VoteCount",        LogTagType_Int32),

#if defined(_M_AMD64)
DeclareTag(64, LogTag_Ptr1,       "Ptr1",       LogTagType_Hex64),
DeclareTag(65, LogTag_Ptr2,       "Ptr2",       LogTagType_Hex64),
#else
DeclareTag(64, LogTag_Ptr1,       "Ptr1",       LogTagType_Hex32),
DeclareTag(65, LogTag_Ptr2,       "Ptr2",       LogTagType_Hex32),
#endif

DeclareTag(66, LogTag_Latitude, "Latitude",     LogTagType_Float),
DeclareTag(67, LogTag_Longitude, "Longitude",    LogTagType_Float),

DeclareTag(68, LogTag_VarString1,       "VarString1",          LogTagType_VarString),
DeclareTag(69, LogTag_VarString2,       "VarString2",          LogTagType_VarString),
DeclareTag(70, LogTag_VarWString1,      "VarWString1",         LogTagType_VarWideString), 
DeclareTag(71, LogTag_VarWString2,      "VarWString2",         LogTagType_VarWideString),

// ids for service manager
DeclareTag(72, LogTag_PID,  "ProcessID",  LogTagType_UInt32),
DeclareTag(73, LogTag_PPID, "ParentProcessID", LogTagType_UInt32),

// ids for fex
DeclareTag(74, LogTag_FEXLatency,  "FEXLatency",  LogTagType_Float),
DeclareTag(75, LogTag_TLALatency, "TLALatency", LogTagType_Float),
DeclareTag(76, LogTag_CDGLatency,  "CDGLatency",  LogTagType_Float),
DeclareTag(77, LogTag_ISNLatency, "ISNLatency", LogTagType_Float),
DeclareTag(78, LogTag_FcsStatus, "FcsStatus", LogTagType_Hex64),
DeclareTag(79, LogTag_MachineNeeded, "MachineNeeded", LogTagType_UInt32),
DeclareTag(80, LogTag_MachineComplete, "MachineComplete", LogTagType_UInt32),
DeclareTag(81, LogTag_MachineTimedOut, "MachineTimedOut", LogTagType_UInt32),
DeclareTag(82, LogTag_Federator, "Federator",      LogTagType_String),
DeclareTag(83, LogTag_RawUrl, "RawUrl",      LogTagType_VarString),
DeclareTag(84, LogTag_StatusCode, "StatusCode",      LogTagType_UInt32),

// ISN Total Time
DeclareTag(85, LogTag_ISNMaxTotalMachine, "ISNMaxTotalMachine", LogTagType_String),
DeclareTag(86, LogTag_ISNMaxTotalLatency, "ISNMaxTotalLatency", LogTagType_Float),
DeclareTag(87, LogTag_ISNMaxTotalStartTime, "ISNMaxTotalStartTime", LogTagType_Time),
DeclareTag(88, LogTag_ISNAvgTotalLatency, "ISNAvgTotalLatency", LogTagType_Float),

// ISN Queue Time
DeclareTag(89, LogTag_ISNMaxQueueMachine, "ISNMaxQueueMachine", LogTagType_String),
DeclareTag(90, LogTag_ISNMaxQueueLatency, "ISNMaxQueueLatency", LogTagType_Float),
DeclareTag(91, LogTag_ISNMaxQueueStartTime, "ISNMaxQueueStartTime", LogTagType_Time),
DeclareTag(92, LogTag_ISNAvgQueueLatency, "ISNAvgQueueLatency", LogTagType_Float),

// ISN Ranker Time
DeclareTag(93, LogTag_ISNMaxRankerMachine, "ISNMaxRankerMachine", LogTagType_String),
DeclareTag(94, LogTag_ISNMaxRankerLatency, "ISNMaxRankerLatency", LogTagType_Float),
DeclareTag(95, LogTag_ISNMaxRankerStartTime, "ISNMaxRankerStartTime", LogTagType_Time),
DeclareTag(96, LogTag_ISNAvgRankerLatency, "ISNAvgRankerLatency", LogTagType_Float),

// ISN Pages Scored Time
DeclareTag(97, LogTag_ISNMaxPagesScoredMachine, "ISNMaxPagesScoredMachine", LogTagType_String),
DeclareTag(98, LogTag_ISNMaxPagesScored, "ISNMaxPagesScored", LogTagType_UInt64),
DeclareTag(99, LogTag_ISNMaxPagesScoredStartTime, "ISNMaxPagesScoredStartTime", LogTagType_Time),
DeclareTag(100, LogTag_ISNAvgPagesScored, "ISNAvgPagesScored", LogTagType_Float),

// ISN Pages Matched Time
DeclareTag(101, LogTag_ISNMaxPagesMatchedMachine, "ISNMaxPgsMatchedMachine", LogTagType_String),
DeclareTag(102, LogTag_ISNMaxPagesMatched, "ISNMaxPgsMatched", LogTagType_UInt64),
DeclareTag(103, LogTag_ISNMaxPagesMatchedStartTime, "ISNMaxPgsMatchedStartTime", LogTagType_Time),
DeclareTag(104, LogTag_ISNAvgPagesMatched, "ISNAvgPgsMatched", LogTagType_Float),

// ISN Pages In Corpus Time
DeclareTag(105, LogTag_ISNMaxPagesInCorpusMachine, "ISNMaxPgsInCorpusMachine", LogTagType_String),
DeclareTag(106, LogTag_ISNMaxPagesInCorpus, "ISNMaxPgsInCorpus", LogTagType_UInt64),
DeclareTag(107, LogTag_ISNMaxPagesInCorpusStartTime, "ISNMaxPgsInCorpusStartTime", LogTagType_Time),
DeclareTag(108, LogTag_ISNAvgPagesInCorpus, "ISNAvgPagesInCorpus", LogTagType_Float),

// ISN QueueLength Length
DeclareTag(109, LogTag_ISNMaxQueueLengthMachine, "ISNMaxQueueLengthMachine", LogTagType_String),
DeclareTag(110, LogTag_ISNMaxQueueLength, "ISNMaxQueueLength", LogTagType_Float),
DeclareTag(111, LogTag_ISNMaxQueueLengthStartTime, "ISNMaxQueueLengthStartTime", LogTagType_Time),
DeclareTag(112, LogTag_ISNAvgQueueLength, "ISNAvgQueueLength", LogTagType_Float),

// Reissue
DeclareTag(113, LogTag_ReissueLatency, "ReissueLatency", LogTagType_Float),
DeclareTag(114, LogTag_ReissueCount, "ReissueCount", LogTagType_UInt32),
DeclareTag(115, LogTag_MaxCDLatency, "MaxCDLatency", LogTagType_Float),
DeclareTag(116, LogTag_MachineAnswered, "MachineAnswered", LogTagType_UInt32),
DeclareTag(117, LogTag_MachineQueried, "MachineQueried", LogTagType_UInt32),
DeclareTag(118, LogTag_MaxTierQueried, "MaxTierQueried", LogTagType_UInt32),

// Version
DeclareTag(119, LogTag_CDVersion, "CDVersion", LogTagType_UInt32),
DeclareTag(120, LogTag_CDVersionCount, "CDVersionCount", LogTagType_UInt32),
DeclareTag(121, LogTag_IndexVersion, "IndexVersion", LogTagType_UInt64),
DeclareTag(122, LogTag_RankVersion, "RankVersion", LogTagType_UInt64),

// Watchdog
DeclareTag(123, LogTag_Property, "Property", LogTagType_String),
DeclareTag(124, LogTag_Level, "Level", LogTagType_String),
DeclareTag(125, LogTag_Machinename, "machinename", LogTagType_String),    

// Caching
DeclareTag(126, LogTag_HitCount, "HitCount", LogTagType_UInt64),    
DeclareTag(127, LogTag_TotalEstimatedMatches, "TotalEstimatedMatches", LogTagType_UInt64),    

// fex filter tags
DeclareTag(128, LogTag_Mkt, "Mkt", LogTagType_String),
DeclareTag(129, LogTag_Flight, "Flight", LogTagType_String),
DeclareTag(130, LogTag_Brand, "Brand", LogTagType_String),
DeclareTag(131, LogTag_VariantID, "Variant", LogTagType_UInt32),
    
DeclareTag(132, LogTag_RequestTime, "RequestTime", LogTagType_String),
DeclareTag(133, LogTag_Method, "Method", LogTagType_String),
DeclareTag(134, LogTag_Host, "Host", LogTagType_String),
DeclareTag(135, LogTag_BytesRecv, "BytesRecv", LogTagType_UInt32),
DeclareTag(136, LogTag_BytesSent, "BytesSent", LogTagType_UInt32),
DeclareTag(137, LogTag_BlockedStatus, "BlockedStatus",      LogTagType_UInt32),
DeclareTag(138, LogTag_Latency, "Latency", LogTagType_Float),
DeclareTag(139, LogTag_UserAgent, "UserAgent", LogTagType_String),
DeclareTag(140, LogTag_Referer, "Referer", LogTagType_String),
DeclareTag(141, LogTag_Cookies, "Cookies", LogTagType_String),
DeclareTag(142, LogTag_GetHeaderXUpSubno, "GetHeaderXUpSubno", LogTagType_String),
DeclareTag(143, LogTag_GetHeaderXUpUpLink, "GetHeaderXUpUpLink", LogTagType_String),

DeclareTag(144, LogTag_RSLMsg, "Msg", LogTagType_String),
DeclareTag(145, LogTag_RSLMsgLen, "MsgLen", LogTagType_UInt32),
DeclareTag(146, LogTag_Offset, "Offset", LogTagType_UInt64),
DeclareTag(147, LogTag_RSLState, "State", LogTagType_UInt32),
DeclareTag(148, LogTag_RSLMemberId, "MemberId", LogTagType_UInt64),
DeclareTag(149, LogTag_RSLBallotId, "BallotId", LogTagType_UInt32),
DeclareTag(150, LogTag_RSLDecree, "Decree", LogTagType_UInt64),
DeclareTag(151, LogTag_RSLBallot, "Ballot", LogTagType_String),
DeclareTag(152, LogTag_RSLMsgVersion, "MsgVersion", LogTagType_UInt32),

DeclareTag(153, LogTag_OldIndexVersion, "OldIndexVersion", LogTagType_UInt64),
DeclareTag(154, LogTag_NewIndexVersion, "NewIndexVersion", LogTagType_UInt64),

// Clusterbuilder
DeclareTag(155, LogTag_NumArticlesLoaded, "NumArticlesLoaded", LogTagType_UInt32),
DeclareTag(156, LogTag_NumArticlesRefs, "NumArticleRefs", LogTagType_UInt32),
DeclareTag(157, LogTag_NumArticles, "NumArticles", LogTagType_UInt32),
DeclareTag(158, LogTag_NumClusterRefs, "NumClusterRefs", LogTagType_UInt32),
DeclareTag(159, LogTag_NumClusters, "NumClusters", LogTagType_UInt32),
DeclareTag(160, LogTag_NumLanguageModelRowRefs, "NumLanguageModelRowRefs", LogTagType_UInt32),
DeclareTag(161, LogTag_NumLanguageModelRows, "NumLanguageModelRows", LogTagType_UInt32),
DeclareTag(162, LogTag_NumChunksExpired, "NumChunksExpired", LogTagType_UInt32),
DeclareTag(163, LogTag_NumClustersExpired, "NumClustersExpired", LogTagType_UInt32),
DeclareTag(164, LogTag_NumArticlesExpired, "NumArticlesExpired", LogTagType_UInt32),
DeclareTag(165, LogTag_NumStopTokensLoaded, "NumStopTokensLoaded", LogTagType_UInt32),
DeclareTag(166, LogTag_NumNoClusterTokensLoaded, "NumNoClusterTokensLoaded", LogTagType_UInt32),

DeclareTag(167, LogTag_PrefixString, "Prefix", LogTagType_String),
DeclareTag(168, LogTag_PrefixOptions, "PrefixOptions", LogTagType_Hex64),

// ids for fex (latencies in ms)
DeclareTag(169, LogTag_FederationLatency,  "FederationLatency",  LogTagType_Float),
DeclareTag(170, LogTag_HttpSysLatency,  "HttpSysLatency",  LogTagType_Float),
DeclareTag(171, LogTag_TotalLatency,  "TotalLatency",  LogTagType_Float),

// Cosmos
DeclareTag(172, LogTag_Cluster, "Cluster", LogTagType_String),
DeclareTag(173, LogTag_Namespace, "Namespace", LogTagType_String),
DeclareTag(174, LogTag_NodeName, "Node", LogTagType_String),
DeclareTag(175, LogTag_ServiceType, "SvcType", LogTagType_String),

DeclareTag(176, LogTag_Command, "Command", LogTagType_String),
DeclareTag(177, LogTag_Service, "Service", LogTagType_String),


DeclareTag(178, LogTag_OID, "OID", LogTagType_Hex64),
DeclareTag(179, LogTag_EID, "ExtentID", LogTagType_String),
DeclareTag(180, LogTag_RefCount, "RefCount", LogTagType_UInt32),

// Other (non-cosmos) tags
DeclareTag(181, LogTag_APProxyCommandID, "ID", LogTagType_UInt64),

//Logging for Fex C Logs
DeclareTag(182, LogTag_QLocation, "QLoc", LogTagType_String),
DeclareTag(183, LogTag_QLatitude, "QLat", LogTagType_Float),
DeclareTag(184, LogTag_QLongitude, "QLong", LogTagType_Float),

//Windows Live Searchpane Action tracing
DeclareTag(185, LogTag_P4_ActionID, "ActionID", LogTagType_UInt32),
DeclareTag(186, LogTag_P4_SessionID, "SessionID", LogTagType_String),
DeclareTag(187, LogTag_P4_ActionTime, "ActionTime", LogTagType_String),
DeclareTag(188, LogTag_P4_ResultType, "ResultType", LogTagType_UInt32),
DeclareTag(189, LogTag_P4_Market, "Market", LogTagType_String),
DeclareTag(190, LogTag_P4_SearchSource, "SearchSource", LogTagType_UInt32),
DeclareTag(191, LogTag_P4_TargetPage, "TargetPage", LogTagType_UInt32),
DeclareTag(192, LogTag_P4_ExceptionID, "ExceptionID", LogTagType_UInt64),
DeclareTag(193, LogTag_P4_SearchTerm, "SearchTerm", LogTagType_String),
DeclareTag(194, LogTag_P4_ActiveURL, "ActiveURL", LogTagType_String),
DeclareTag(195, LogTag_P4_ExceptionMessage, "ExceptionMessage", LogTagType_String),
DeclareTag(196, LogTag_P4_QuickSearch, "QuickSearch", LogTagType_UInt32),
DeclareTag(197, LogTag_P4_AnswerType, "AnswerType", LogTagType_String),
DeclareTag(198, LogTag_P4_TutorialMode, "TutorialMode", LogTagType_UInt32),

//AppID Tracing for SOAP API
DeclareTag(199, LogTag_AppID, "AppID", LogTagType_String),

//Logging of Reverse IP Loc, Lat and Long for Fex C Logs
DeclareTag(200, LogTag_IPLocation, "IPLoc", LogTagType_String),
DeclareTag(201, LogTag_IPLatitude, "IPLat", LogTagType_Float),
DeclareTag(202, LogTag_IPLongitude, "IPLong", LogTagType_Float),

//adding one more latency type for logging
DeclareTag(203, LogTag_DPSLatency,  "DPSLatency",  LogTagType_Float), 
DeclareTag(204, LogTag_RawQuery, "RawQuery", LogTagType_String),

// Answers stuff
DeclareTag(205, LogTag_Market,  "Market",  LogTagType_String),
DeclareTag(206, LogTag_QueryTokenID,  "QueryTokenID",  LogTagType_UInt64),
DeclareTag(207, LogTag_GrammarTokenID,  "GrammarTokenID",  LogTagType_UInt64),
DeclareTag(208, LogTag_AnswerRequest,  "AnsRequest", LogTagType_String),
DeclareTag(209, LogTag_AnswerResponse,  "AnsResponse", LogTagType_String),
DeclareTag(210, LogTag_AnswerReject,  "AnsReject", LogTagType_String),
DeclareTag(211, LogTag_AnswerLogVersion,  "LogVersion", LogTagType_UInt32),

// Latencies for C-logs
DeclareTag(212, LogTag_TotalLatency_C,      "TotLat",  LogTagType_Float),
DeclareTag(213, LogTag_DPSLatency_C,        "DPSLat",  LogTagType_Float), 
DeclareTag(214, LogTag_HttpSysLatency_C,    "HttpSysLat",  LogTagType_Float),
DeclareTag(215, LogTag_FederationLatency_C, "FedLat",  LogTagType_Float),
DeclareTag(216, LogTag_FEXLatency_C,        "FEXLat",  LogTagType_Float),
DeclareTag(217, LogTag_Latency_C,           "Latency", LogTagType_Float),

//General CLogging
DeclareTag(218, LogTag_CLogVersion,     "CLogVersion", LogTagType_String),
DeclareTag(219, LogTag_FEXBuild,        "FexBuild", LogTagType_String),
DeclareTag(220, LogTag_DataCenter,      "DataCenter", LogTagType_String),

// Speller request parameters
DeclareTag(221, LogTag_SpellerTimeout,          "Timeout",           LogTagType_UInt32),
DeclareTag(222, LogTag_SpellerTargetCorrection, "TargetCorrection",  LogTagType_String), 
DeclareTag(223, LogTag_SpellerConfig,           "Config",            LogTagType_String),
DeclareTag(224, LogTag_SpellerOptions,          "Options",           LogTagType_Hex64),

// Tag to identify spilling in indexserve
DeclareTag(225, LogTag_SpillStatus,             "SpillStatus",     LogTagType_UInt32),

// More cosmos tags
DeclareTag(226, LogTag_RemoteMachine, "RemoteMachine", LogTagType_String ),

// This must be the final tag, and it must have type None
DeclareTag(227, LogTag_End,         "End",               LogTagType_None),
