
#include "stdafx.h"
#include "GThread.h"
#include "GEvent.h"

void GThread::Release(GThreadMember* _Gthread)  
{
	if(_Gthread->m_hThread == NULL ) return;
	if( ::CloseHandle(_Gthread->m_hThread) == FALSE ) return;
	GEvent::Create(_Gthread->m_hCloseEvent);
	_Gthread->m_bLoop=FALSE;
	if(GEvent::WaitForEvent(_Gthread->m_hCloseEvent, 500)==FALSE)
	{
		Terminate(_Gthread);
	}
	GEvent::Close(_Gthread->m_hCloseEvent);
	_Gthread->m_hThread = NULL;
	_Gthread->m_dwThreadID=0;
	_Gthread->m_hCloseEvent= INVALID_HANDLE_VALUE;
}

// Operations

BOOL GThread::Creat(GThreadMember* _Gthread, LPTHREAD_START_ROUTINE pThreadProc, void* pParam, int iPriority)
{
	if(_Gthread==NULL||pThreadProc==NULL) return false;
	if(_Gthread->m_hThread!=NULL) return true;
	_Gthread->m_hThread = ::CreateThread(NULL, 0, pThreadProc, pParam, CREATE_SUSPENDED, &_Gthread->m_dwThreadID);
	if( _Gthread->m_hThread == NULL ) return false;
	if( iPriority != THREAD_PRIORITY_NORMAL ) {
		if( !::SetThreadPriority(_Gthread->m_hThread, iPriority) ) {
			ATLASSERT(!"Couldn't set thread priority");
		}
	}
	if( ::ResumeThread(_Gthread->m_hThread) == (DWORD) -1 ) return false;
	_Gthread->m_bLoop=true;
	_Gthread->m_hCloseEvent= INVALID_HANDLE_VALUE;
	_Gthread->m_bSuspended = false;
	return true;
}

BOOL GThread::Suspend(GThreadMember _Gthread)
{
	if(_Gthread.m_hThread==NULL) return false;
	if( _Gthread.m_bSuspended ) return true;
	if( ::SuspendThread(_Gthread.m_hThread) == (DWORD) -1 ) return FALSE;
	_Gthread.m_bSuspended = true;
	return true;
}

BOOL GThread::Resume(GThreadMember _Gthread)
{
	if(_Gthread.m_hThread==NULL) return false;
	if( !_Gthread.m_bSuspended ) return true;
	if( ::ResumeThread(_Gthread.m_hThread) == (DWORD) -1 ) return FALSE;
	_Gthread.m_bSuspended = false;
	return true;
}

BOOL GThread::IsRunning(const  GThreadMember _Gthread)
{
	DWORD dwCode = 0;
	if( _Gthread.m_hThread == NULL || _Gthread.m_bLoop==FALSE ) return FALSE;
	::GetExitCodeThread(_Gthread.m_hThread, &dwCode);
	return dwCode == STILL_ACTIVE;
}

BOOL GThread::WaitForThread(const  GThreadMember _Gthread, DWORD dwTimeout)
{
	if(_Gthread.m_hThread==NULL) return false;
	return ::WaitForSingleObject(_Gthread.m_hThread, dwTimeout) == WAIT_OBJECT_0;
}

BOOL GThread::Terminate(GThreadMember* _Gthread, DWORD dwExitCode)
{
	// See Q254956 why calling this could be a bad idea!
	if(_Gthread->m_hThread==NULL) return false;
	return ::TerminateThread(_Gthread->m_hThread, dwExitCode);
}

BOOL GThread::GetExitCode(GThreadMember _Gthread, DWORD* pExitCode)
{
	if(_Gthread.m_hThread==NULL||pExitCode==NULL) return false;
	return ::GetExitCodeThread(_Gthread.m_hThread, pExitCode);
}
