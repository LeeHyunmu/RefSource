#ifndef __SYNCHRONIZATION_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SYNCHRONIZATION_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|CEvent
//|
//---------------------------------------------------------------
class CEvent
{
public:
	HANDLE m_hEvent;
	CEvent(HANDLE hEvent=INVALID_HANDLE_VALUE);
	~CEvent();
	BOOL Create(LPCTSTR pstrName = NULL, BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPSECURITY_ATTRIBUTES pEventAttributes = NULL);
	BOOL Open(LPCTSTR pstrName, DWORD dwDesiredAccess = EVENT_ALL_ACCESS, BOOL bInheritHandle = TRUE);
	BOOL IsOpen() const;
	void Close();
	void Attach(HANDLE hEvent);
	HANDLE Detach();
	BOOL ResetEvent();
	BOOL SetEvent();
	BOOL PulseEvent();
	BOOL IsSignalled() const;
	BOOL WaitForEvent(DWORD dwTimeout = INFINITE);
	operator HANDLE() const; 
};


/////////////////////////////////////////////////////////////////////////////
// CCriticalSection
//--------------------------------------------------------------
//|
//|CCriticalSection
//|
//---------------------------------------------------------------
#ifndef __ATLBASE_H__

class CCriticalSection
{
public:
	CRITICAL_SECTION m_sec;

	void Init(); 
	void Term(); 
	void Lock(); 
	void Unlock(); 
};
//--------------------------------------------------------------
//|
//|CAutoCriticalSection
//|
//---------------------------------------------------------------
class CAutoCriticalSection
{
public:
	CRITICAL_SECTION m_sec;

	CAutoCriticalSection(); 
	~CAutoCriticalSection(); 
	void Lock(); 
	void Unlock(); 
};

#endif // __ATLBASE_H__
#endif __SYNCHRONIZATION_B7A15A9D01DC403996FC45F7F92F52D3_H__

