
#include "stdAfx.h"
#include "Synchronization.h"
//--------------------------------------------------------------
//|
//|CLeeEvent
//|
//---------------------------------------------------------------
CLeeEvent::CLeeEvent(HANDLE hEvent):
m_hEvent(INVALID_HANDLE_VALUE),
m_bEvent(TRUE)
{ 

}
CLeeEvent::~CLeeEvent()
{
	Close();
}

BOOL CLeeEvent::Create(LPCTSTR pstrName, BOOL bManualReset, BOOL bInitialState, LPSECURITY_ATTRIBUTES pEventAttributes)
{
	//ATLASSERT(pstrName==NULL || !::IsBadStringPtr(pstrName,(UINT)-1));
	ATLASSERT(m_hEvent==INVALID_HANDLE_VALUE);
	m_hEvent = ::CreateEvent(pEventAttributes, bManualReset, bInitialState, pstrName);
	ATLASSERT(m_hEvent!=INVALID_HANDLE_VALUE);
	return m_hEvent != INVALID_HANDLE_VALUE;
}

BOOL CLeeEvent::Open(LPCTSTR pstrName, DWORD dwDesiredAccess, BOOL bInheritHandle)
{
//	ATLASSERT(!::IsBadStringPtr(pstrName,(UINT)-1));
	ATLASSERT(m_hEvent==INVALID_HANDLE_VALUE);
	m_hEvent = ::OpenEvent(dwDesiredAccess, bInheritHandle, pstrName);
	ATLASSERT(m_hEvent!=INVALID_HANDLE_VALUE);
	return m_hEvent != INVALID_HANDLE_VALUE;
}

BOOL CLeeEvent::IsOpen() const
{
	return m_hEvent != INVALID_HANDLE_VALUE;
}

void CLeeEvent::Close()
{
	if( m_hEvent == INVALID_HANDLE_VALUE ) return;
	m_bEvent=FALSE;
	::SetEvent(m_hEvent);
	::CloseHandle(m_hEvent);
	m_hEvent = INVALID_HANDLE_VALUE;
}

void CLeeEvent::Attach(HANDLE hEvent)
{
	ATLASSERT(m_hEvent==INVALID_HANDLE_VALUE);
	m_hEvent= hEvent;
}  

HANDLE CLeeEvent::Detach()
{
	HANDLE hEvent = m_hEvent;
	m_hEvent = INVALID_HANDLE_VALUE;
	return hEvent;
}

BOOL CLeeEvent::ResetEvent()
{
	ATLASSERT(m_hEvent!=INVALID_HANDLE_VALUE);
	return ::ResetEvent(m_hEvent);
}

BOOL CLeeEvent::SetEvent()
{
	if(m_hEvent==INVALID_HANDLE_VALUE) return FALSE;
	return ::SetEvent(m_hEvent);
}

BOOL CLeeEvent::PulseEvent()
{
	ATLASSERT(m_hEvent!=INVALID_HANDLE_VALUE);
	return ::PulseEvent(m_hEvent);
}

BOOL CLeeEvent::IsSignalled() const
{
	ATLASSERT(m_hEvent!=INVALID_HANDLE_VALUE);
	return ::WaitForSingleObject(m_hEvent, 0) == WAIT_OBJECT_0;
}

BOOL CLeeEvent::WaitForEvent(DWORD dwTimeout)
{
	if(m_bEvent==FALSE) return FALSE;
	ATLASSERT(m_hEvent!=INVALID_HANDLE_VALUE);
	if(dwTimeout!=INFINITE) return ::WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_TIMEOUT;
	return ::WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0;
}
CLeeEvent::operator HANDLE() const 
{ 
	return m_hEvent; 
}
//--------------------------------------------------------------
//|
//|CCriticalSection
//|
//---------------------------------------------------------------
#ifndef __ATLBASE_H__

void CCriticalSection::Init() 
{
	::InitializeCriticalSection(&m_sec);
}

void CCriticalSection::Term() 
{
	::DeleteCriticalSection(&m_sec);
}

void CCriticalSection::Lock() 
{
	::EnterCriticalSection(&m_sec);
}

void CCriticalSection::Unlock() 
{
	::LeaveCriticalSection(&m_sec);
}
//--------------------------------------------------------------
//|
//|CAutoCriticalSection
//|
//---------------------------------------------------------------
CAutoCriticalSection::CAutoCriticalSection() 
{
	::InitializeCriticalSection(&m_sec);
}

CAutoCriticalSection::~CAutoCriticalSection() 
{
	::DeleteCriticalSection(&m_sec);
}

void CAutoCriticalSection::Lock() 
{
	::EnterCriticalSection(&m_sec);
}

void CAutoCriticalSection::Unlock() 
{
	::LeaveCriticalSection(&m_sec);
}
#endif // __ATLBASE_H__

