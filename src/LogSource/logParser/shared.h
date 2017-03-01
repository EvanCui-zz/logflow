#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

//This is an interface to disallow heap construction.
class INoHeapInstance
{
private:
    void* operator new(size_t);
};

// DDWORD is used to easily access a 64bit number as both a 64bit
//   and as two 32bit numbers
typedef union
{
	struct
	{
		DWORD	low;
		DWORD	high;
	} dw;
	DWORD64	ddw;
} DDWORD;

// critical section wrapper
//
class CRITSEC
{
private:
	CRITICAL_SECTION m_critsec;

public:
	CRITSEC()
	{
		InitializeCriticalSection(&m_critsec);
	}

	~CRITSEC()
	{
		DeleteCriticalSection(&m_critsec);
	}

	void	Enter()
	{
		EnterCriticalSection(&m_critsec);
	}

	BOOL TryEnter()
	{
	    return TryEnterCriticalSection(&m_critsec);
	}

	void	Leave()
	{
		LeaveCriticalSection(&m_critsec);
	}

    DWORD SetSpinCount(DWORD spinCount = 4000)
    {
        return SetCriticalSectionSpinCount(&m_critsec, spinCount);
    }
};

//Smart wrapper around CRITSEC so that we dont need to call enter/leave
class AutoCriticalSection : public INoHeapInstance
{
private:
    CRITSEC* m_pCritSec;
public:
    AutoCriticalSection(CRITSEC* pCritSec) : m_pCritSec(pCritSec)
    {
        this->m_pCritSec->Enter();
    }
    ~AutoCriticalSection()
    {
        this->m_pCritSec->Leave();
    }
};

