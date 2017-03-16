////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.
//
// Synopsis:
//
//  Defines a common interface for the log readers.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

//
// Forward-only interface to read the contents of a log. There will be one
// implementation for each log format we support. Currently we support binary
// and text logging.
//
class ILogReader
{
public:

    virtual DWORD OpenReader(
        __in LPCWSTR logPath) = 0;

    virtual VOID CloseReader() = 0;

    virtual DWORD MoveNext() = 0;

    virtual ULONGLONG GetEntryLineNumber() = 0;

    virtual ULONGLONG GetEntryTimestamp() = 0;

    virtual VOID GetEntryTitle(
        __out_ecount(titleLen) const char ** title,
        __out ULONG * titleLen) = 0;

    virtual GUID GetEntryActivityId() = 0;

    virtual LPCSTR GetEntryFormatted() = 0;

    virtual void QueryStats(
        __out UINT64 * bytesSkipped,
        __out UINT64 * bytesDecoded) = 0;

    virtual LPCSTR GetEntrySourceFile() = 0;

    virtual DWORD GetEntrySourceLine() = 0;

};

