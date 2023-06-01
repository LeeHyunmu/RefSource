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
		BOOL m_bLoop;			//���� Ż�� �ڵ�
		HANDLE m_hCloseEvent;	//Close Event
	}GThreadMember;


	//������ �ʱ�ȭ
	static BOOL Creat(GThreadMember* _Gthread, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pParam = NULL, int iPriority = THREAD_PRIORITY_NORMAL);
	//������ ����
	static void Release(GThreadMember* _Gthread);
	//��� ���¸� �����
	static BOOL Suspend(GThreadMember _Gthread);
	//��� ���¸� Ǭ��
	static BOOL Resume(GThreadMember _Gthread);
	//�۵� ���� �������ΰ�
	static BOOL IsRunning(const GThreadMember _Gthread);
	//�����带 Loop Ÿ��
	static BOOL WaitForThread(const GThreadMember _Gthread, DWORD dwTimeout = INFINITE);
	//������ ���� ����
	static BOOL Terminate(GThreadMember* _Gthread, DWORD dwExitCode = 0);
	//������ ���� �ڵ�
	static BOOL GetExitCode(GThreadMember _Gthread, DWORD* pExitCode);
};

#endif // __GTHREAD_B7A15A9D01DC403996FC45F7F92F52D3_H__

