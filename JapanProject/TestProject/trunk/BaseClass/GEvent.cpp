
#include "stdafx.h"
#include "Gevent.h"

BOOL GEvent::Create(HANDLE& hevent, LPCTSTR pstrName, BOOL bManualReset, BOOL bInitialState, LPSECURITY_ATTRIBUTES pEventAttributes)
{
	if(hevent!=INVALID_HANDLE_VALUE) return true;
	hevent = ::CreateEvent(pEventAttributes, bManualReset, bInitialState, pstrName);
	return hevent != INVALID_HANDLE_VALUE;
}

BOOL GEvent::Open(HANDLE& hevent, LPCTSTR pstrName, DWORD dwDesiredAccess, BOOL bInheritHandle)
{
	if(hevent!=INVALID_HANDLE_VALUE) return true;
	hevent = ::OpenEvent(dwDesiredAccess, bInheritHandle, pstrName);
	return hevent != INVALID_HANDLE_VALUE;
}

BOOL GEvent::IsOpen(HANDLE hevent)
{
	return hevent != INVALID_HANDLE_VALUE;
}

void GEvent::Close(HANDLE& hevent)
{
	if( hevent == INVALID_HANDLE_VALUE ) return;
	::CloseHandle(hevent);
	hevent = INVALID_HANDLE_VALUE;
}

BOOL GEvent::ResetEvent(HANDLE hevent)
{
	if(hevent==INVALID_HANDLE_VALUE) return false;
	return ::ResetEvent(hevent);
}

BOOL GEvent::SetEvent(HANDLE hevent)
{
	if(hevent==INVALID_HANDLE_VALUE) return false;
	return ::SetEvent(hevent);
}

BOOL GEvent::PulseEvent(HANDLE hevent)
{
	if(hevent==INVALID_HANDLE_VALUE) return false;
	return ::PulseEvent(hevent);
}

BOOL GEvent::IsSignalled(HANDLE hevent)
{
	if(hevent==INVALID_HANDLE_VALUE) return false;
	return ::WaitForSingleObject(hevent, 0) == WAIT_OBJECT_0;
}

BOOL GEvent::WaitForEvent(HANDLE hevent, DWORD dwTimeout)
{
	if(hevent==INVALID_HANDLE_VALUE) return false;
	return ::WaitForSingleObject(hevent, dwTimeout) == WAIT_OBJECT_0;
}
