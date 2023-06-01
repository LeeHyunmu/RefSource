#ifndef __GEVENT_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GEVENT_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class GEvent
{
public:
	//이벤트 핸들 생성
	static BOOL Create(HANDLE& hevent, LPCTSTR pstrName = NULL, BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPSECURITY_ATTRIBUTES pEventAttributes = NULL);
	//이벤트 이름으로 생성한 객체를 다시 연다
	static BOOL Open(HANDLE& hevent, LPCTSTR pstrName, DWORD dwDesiredAccess = EVENT_ALL_ACCESS, BOOL bInheritHandle = TRUE);
	//이벤트가 연린 상태인가
	static BOOL IsOpen(const HANDLE hevent);
	//이벤트 핸들을 닫다
	static void Close(HANDLE& hevent);
	//
	static BOOL ResetEvent(const HANDLE hevent);
	//이벤트
	static BOOL SetEvent(const HANDLE hevent);
	//
	static BOOL PulseEvent(const HANDLE hevent);
	//
	static BOOL IsSignalled(const HANDLE hevent);
	//대기 상태
	static BOOL WaitForEvent(const HANDLE hevent, DWORD dwTimeout = INFINITE);

};
#endif __GEVENT_B7A15A9D01DC403996FC45F7F92F52D3_H__

