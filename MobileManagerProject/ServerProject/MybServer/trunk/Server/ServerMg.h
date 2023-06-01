
#ifndef __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CServerMg : public IServerMg
{
public:
	//������
	CServerMg();
	//�Ҹ���
	~CServerMg();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//

//=================������ �Լ�===========================//
	static DWORD WINAPI WorkerThread(LPVOID lpArg);
	static DWORD WINAPI ProcessServer(LPVOID lpArg);
//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IServer* iocp;
//=======================================================//
//			���� ����
//======================================================//
public:
	HANDLE   m_hIocp;
	HANDLE  mthread;
	vechThread workerthread;
private:
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL initServerMg(IMyObject* _obj);
	virtual HANDLE GetCompletionPort();
	virtual BOOL ServerStart(u_short nPort);
	virtual void ServerStop();
//=======================================================//

};
#endif// __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
