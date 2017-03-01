/*++

Module Name:

    BinaryLogReader.cpp

Synopsis:

    Reader over binary log files.

--*/

#pragma once

#include "logids.h"
#include "logging.h"
#include "ilogreader.h"

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport) 
#else
#define DLL_API __declspec(dllimport) 
#endif

#define MAX_LOG_ENTRY_SIZE      (9 * 1024)
#define MAX_PARAMETER_COUNT      (256)
#define MAX_SOURCE_INFO_SIZE    (256)

class LogEntry;
class BinaryLogDecodeSession;
class XPressWrapper;

class  BinaryLogReader : public ILogReader
{
private:

    static const int c_maxBufferSize = 64 * 1024;

private:

    // Log decode session
    BinaryLogDecodeSession * m_session;

    // A buffer to hold the unserialized entry.
    CHAR m_currentEntryBuffer[MAX_LOG_ENTRY_SIZE];

    // Pointer to the current unserialized log entry. This points inside the
    // entry buffer.
    LogEntry * m_currentLogEntry;

    // Holds the current line, formatted
    CHAR m_currentLine[MAX_LOG_ENTRY_SIZE];

    // Buffer holding the current uncompressed block, if any.
    CHAR m_uncompressedBuffer[c_maxBufferSize];

    // Number of bytes consumed from the uncompressed buffer
    UINT m_uncompressedBufferPos;

    // Number of bytes in the uncompressed buffer
    UINT m_uncompressedBufferSize;

    // XPress library wrapper.
    XPressWrapper * m_xpress;

    // Compact entry rendering.
    BOOL m_compactRendering;

	WCHAR m_logPath[256];

	UINT m_firstSize;

public:

    BinaryLogReader();

    ~BinaryLogReader();

public:

    //
    // ILogReader methods
    //
    virtual DWORD OpenReader(
        __in LPCWSTR logPath);

    virtual VOID CloseReader();

    virtual DWORD MoveNext();

    virtual ULONGLONG GetEntryLineNumber();

    virtual ULONGLONG GetEntryTimestamp();

    virtual VOID GetEntryTitle(
        __out_ecount(titleLen) const char ** title,
        __out ULONG * titleLen);

    virtual GUID GetEntryActivityId();

	virtual char GetLoggingLevel();

	virtual int GetLoggingLevelI();

	virtual LPCSTR GetEntryFormatted();

    virtual void QueryStats(
        __out UINT64 * bytesSkipped,
        __out UINT64 * bytesDecoded);

	virtual DWORD ReFresh();
	//jwesker
	void getFormatedText(PCHAR);

    int BinaryLogReader::GetFormatDataCSharpStyle(PCHAR format, Size_t formatSize, PCHAR parameters, Size_t parametersSize, Size_t indexWidthLength[], Size_t count);
    int BinaryLogReader::GetFormatDataCStyle(PCSTR &format, PCHAR parameters, Size_t parametersSize, Size_t indexWidthLength[], Size_t count);

	PCSTR getEntryFileName();

	PCSTR getEntryFunction();

	PCSTR getEntryTitle();

	int getEntryLine();

	int getEntryTid();

	int getEntryPid();

	long long getEntryTime();

	void getEntryTS(PCHAR);

	double getPersentage();

private:

    DWORD DecodeAndSaveFormatBlock(
        __in const CHAR *pBuffer,
        __in ULONG bufferLen,
        __out PULONG pBytesConsumed);

    DWORD VerifyBinaryLogFileHeader(
        __in_bcount(len) PCHAR pBuffer,
        __in ULONG len);

    DWORD DecodeLogEntry(
        __in const CHAR *pBuffer,
        __in ULONG bufferLen,
        __in ULONG baseFileOffset,
        __out PULONG pBytesConsumed);

    PCHAR MakeLogEntryWrapper(
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
        ...);

    void AdvanceReadPointer(
        __in BOOL advanceBuffer,
        __in ULONG size);

    DWORD
    DecompressBlock(
        __in const CHAR * pBuffer,
        __in ULONG bufferLen,
        __out PULONG pBytesConsumed);

	void LocateLast();
};


