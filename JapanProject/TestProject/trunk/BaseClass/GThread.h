#ifndef __GTHREAD_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GTHREAD_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GThread
{
public:
	typedef struct _GThreadMember{
		HANDLE m_hThread;       // Handle to thread
		DWORD m_dwThreadID;     // Thread ID
		BOOL m_bSuspended;      // Thread currently suspended?
		BOOL m_bLoop;			//루프 탈출 코드
		HANDLE m_hCloseEvent;	//Close Event
	}GThreadMember;


	//스래드 초기화
	static BOOL Creat(GThreadMember* _Gthread, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pParam = NULL, int iPriority = THREAD_PRIORITY_NORMAL);
	//스래드 종료
	static void Release(GThreadMember* _Gthread);
	//대기 상태를 만든다
	static BOOL Suspend(GThreadMember _Gthread);
	//대기 상태를 푼다
	static BOOL Resume(GThreadMember _Gthread);
	//작동 중인 스래드인가
	static BOOL IsRunning(const GThreadMember _Gthread);
	//스래드를 Loop 타임
	static BOOL WaitForThread(const GThreadMember _Gthread, DWORD dwTimeout = INFINITE);
	//스래드 강제 종료
	static BOOL Terminate(GThreadMember* _Gthread, DWORD dwExitCode = 0);
	//스래드 종료 코드
	static BOOL GetExitCode(GThreadMember _Gthread, DWORD* pExitCode);
};

#endif // __GTHREAD_B7A15A9D01DC403996FC45F7F92F52D3_H__

