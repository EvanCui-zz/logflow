/*++

Module Name:

    BinaryLogger.cpp

Synopsis:

    Support for parsing XStore binary log files.

--*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "logids.h"
#include "types.h"
#include "logging.h"

#include "MemLogEntry.h"
#include "MemoryLog.h"
#include "MemLogSprintf.h"
#include "binarylogger.h"

#include <iostream>

BinaryLogDecodeSession::BinaryLogDecodeSession(
    )
{
    m_inFile = INVALID_HANDLE_VALUE;
	m_reachToEnd = FALSE;

    m_bytesDecoded = 0;

    m_readBuffer = NULL;
    m_readBufferOffset = 0;
    m_inputBytesRemaining = 0;

    m_fFreeReadBuffer = true;

    m_logEntriesDecoded = 0;
    m_formatBlocksDecoded = 0;

    m_bytesSkipped = 0;

    ZeroMemory(&m_lastActivityId, sizeof(GUID));
    ZeroMemory(&m_lastEntryId, sizeof(GUID));
    ZeroMemory(&m_lastTimestamp, sizeof(m_lastTimestamp));
    m_lastThreadId = 0;

    m_pid = 0;
    m_logEntryVersion = 0;
}

BinaryLogDecodeSession::~BinaryLogDecodeSession()
{
    if (m_inFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_inFile);
    }

    if (m_fFreeReadBuffer)
    {
        delete [] m_readBuffer;
    }

    while(!m_fpbMap.empty())
    {
        FpbMapType::iterator it = m_fpbMap.begin();
        delete it->second;
        it->second = NULL;
        m_fpbMap.erase(it);
    }
}

//
// Move data in the input buffer to the start so we can read in more
// data.
//
void BinaryLogDecodeSession::MoveDataToStartOfReadBuffer()
{
    MoveMemory(
        m_readBuffer,
        m_readBuffer + m_readBufferOffset,
        m_inputBytesRemaining);

    m_readBufferOffset = 0;
}

//
// Allocate the read buffer for file IO.
//
// Return value:
//      Returns true on success, false on failure.
//
bool BinaryLogDecodeSession::AllocateReadBuffer()
{
    ULONG size = FILE_READ_CHUNK_SIZE;

    m_readBuffer = new char[size];

    if (m_readBuffer == NULL)
    {
        return false;
    }

    m_readBufferSize = size;
    m_fFreeReadBuffer = true;

    return true;
}

DWORD BinaryLogDecodeSession::Refresh()
{
	m_inputBytesRemaining=0;
	m_readBufferOffset=0;
	DWORD bytesRead;
	ULONG size =
		MIN(FILE_READ_CHUNK_SIZE,
		(m_readBufferSize - m_inputBytesRemaining));

	LPOVERLAPPED overlapped=new OVERLAPPED;
	memset(overlapped,0,sizeof(OVERLAPPED));
	overlapped->Offset=this->m_bytesDecoded;
	//m_bytesDecoded=0;
	
	BOOL result =
		ReadFile(
		m_inFile,
		m_readBuffer + m_inputBytesRemaining,
		size,
		&bytesRead,
		overlapped);

	delete overlapped;

	if (!result)
	{
		DWORD error = GetLastError();
		return error;
	}

	//JWesker
	//note We don't have to close m_inFile in Appending Mode
	if (bytesRead == 0)
	{
		// We've read the entire file. Close the handle to make sure no more
		// reads are being attempted from CheckAndPageInEntry.
		CloseHandle(m_inFile);
		m_inFile = INVALID_HANDLE_VALUE;
		m_reachToEnd=TRUE;
	}

	m_inputBytesRemaining += bytesRead;

	return NO_ERROR;
}

//
// Read the given number of bytes from the input file.
//
// size [in] - Supplies the number of bytes to read.
//
// Return value:
//      NO_ERROR on success, Win32 error code otherwise.
//
DWORD BinaryLogDecodeSession::ReadInputFile(ULONG size)
{
    DWORD bytesRead;

    MoveDataToStartOfReadBuffer();

    size =
        MIN(size,
           (m_readBufferSize - m_inputBytesRemaining));

    BOOL result =
        ReadFile(
            m_inFile,
            m_readBuffer + m_inputBytesRemaining,
            size,
            &bytesRead,
            NULL);

    if (!result)
    {
        DWORD error = GetLastError();
        return error;
    }

	//JWesker
	//note We don't have to close m_inFile in Appending Mode
    if (bytesRead == 0)
    {
        // We've read the entire file. Close the handle to make sure no more
        // reads are being attempted from CheckAndPageInEntry.
        //CloseHandle(m_inFile);
        //m_inFile = INVALID_HANDLE_VALUE;
		m_reachToEnd=TRUE;
    }

    m_inputBytesRemaining += bytesRead;

    return NO_ERROR;
}

// Ensure we have at least one log entry in the read buffer. If not,
// then read it from the input file. It is not an error if we hit
// EOF.
DWORD BinaryLogDecodeSession::CheckAndPageInEntry()
{
//NOTE:JWesker
//a cc block can not be more than 128k
    if ((m_inputBytesRemaining >= MIN_BUFFER_BYTES) ||
        //(m_inFile == INVALID_HANDLE_VALUE)
		m_reachToEnd)
    {
        // Already have enough data or all available data is
        // already in memory.
        return NO_ERROR;
    }

    return ReadInputFile(FILE_READ_CHUNK_SIZE);
}

//
// Get the marker byte at the current read offset.
// Returns 0 if the marker byte could not be read.
//
UINT8 BinaryLogDecodeSession::GetCurrentMarker()
{
    if (m_inputBytesRemaining >= 1)
    {
        return m_readBuffer[m_readBufferOffset];
    }
    else
    {
        return 0;
    }
}

//
// Advance the read pointer by the given size.
//
// size [in] - Supplies the number of bytes to advance the read pointer.
//
void BinaryLogDecodeSession::AdvanceReadPointer(ULONG size)
{
    m_inputBytesRemaining -= size;
    m_readBufferOffset += size;
    m_bytesDecoded += size;
}

//
// Get a pointer to the current read location.
//
// size [out] - Returns the number of unconsumed bytes available
//              in the read buffer.
//
// Return value:
//      Pointer to the current read location.
//
PCHAR BinaryLogDecodeSession::GetReadPointer(PULONG size)
{
    *size = m_inputBytesRemaining;
    return (m_readBuffer + m_readBufferOffset);
}

//
// Cache the given format block in the current session. The session
// assumes ownership of the format block and it is freed when the session
// is destroyed.
//
// pBlock [in] - Supplies the format block to save.
//
// Return value:
//      None.
//
VOID BinaryLogDecodeSession::AddFormatBlock(format_preprocess_block *pBlock)
{
    FpbMapType::iterator it = m_fpbMap.find(pBlock->m_blockUid);

    if (it != m_fpbMap.end())
    {
		//std::cout<<"wrong ................."<<std::endl;
        // We found a second block with the same ID. This block
        // overrides the previous block definition so delete the
        // existing block.
        delete it->second;
        it->second = NULL;
        m_fpbMap.erase(it);
    }

    m_fpbMap[pBlock->m_blockUid] = pBlock;

    ++m_formatBlocksDecoded;
}

//
// Retrieve a format block with the given UID.
//
// uid [in] - Supplies the UID of the format block.
//
// Return value:
//      Returns a pointer to the format block. NULL if none was found with
//      the given UID. The session continues to maintain ownership of the
//      pointer.
//
format_preprocess_block *BinaryLogDecodeSession::GetFormatBlock(FpbUidType uid)
{
    FpbMapType::iterator it = m_fpbMap.find(uid);

    if (it != m_fpbMap.end())
    {
        return it->second;
    }

    return NULL;
}

