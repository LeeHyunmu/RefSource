#ifndef __GEVENT_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GEVENT_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class GEvent
{
public:
	//�̺�Ʈ �ڵ� ����
	static BOOL Create(HANDLE& hevent, LPCTSTR pstrName = NULL, BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPSECURITY_ATTRIBUTES pEventAttributes = NULL);
	//�̺�Ʈ �̸����� ������ ��ü�� �ٽ� ����
	static BOOL Open(HANDLE& hevent, LPCTSTR pstrName, DWORD dwDesiredAccess = EVENT_ALL_ACCESS, BOOL bInheritHandle = TRUE);
	//�̺�Ʈ�� ���� �����ΰ�
	static BOOL IsOpen(const HANDLE hevent);
	//�̺�Ʈ �ڵ��� �ݴ�
	static void Close(HANDLE& hevent);
	//
	static BOOL ResetEvent(const HANDLE hevent);
	//�̺�Ʈ
	static BOOL SetEvent(const HANDLE hevent);
	//
	static BOOL PulseEvent(const HANDLE hevent);
	//
	static BOOL IsSignalled(const HANDLE hevent);
	//��� ����
	static BOOL WaitForEvent(const HANDLE hevent, DWORD dwTimeout = INFINITE);

};
#endif __GEVENT_B7A15A9D01DC403996FC45F7F92F52D3_H__

