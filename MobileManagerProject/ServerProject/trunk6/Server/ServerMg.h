
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

	static DWORD WINAPI WorkerClntThread(LPVOID lpArg);
	static DWORD WINAPI ProcessClntServer(LPVOID lpArg);
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
	HANDLE   m_hclntIocp;
	HANDLE  mclntthread;
	HANDLE  mthread;
	vechThread workerthread;
	DWORD dwServerThreadCount;
	DWORD dwClientThreadCount;
	BOOL  bEndServer;
private:
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	//�����޴��� �ʱ�ȭ
	virtual BOOL initServerMg(IMyObject* _obj);
	//���ø��� ��Ʈ �ڵ��� ���� �´�
	virtual HANDLE GetCompletionPort();
	virtual HANDLE GetClntCompletionPort();
	//������ �����Ѵ�
	virtual BOOL ServerStart(u_short nPort);
	//������ �����Ѵ�(���� �Ⱦ��� ����)
	virtual void ServerStop();
	//�񵿱� ������ recv�� Ŭ���̾�Ʈ ���� ���� ���
	virtual SOCKET MakeCompetionPortClient(SOCKET hSock);
	//���� ������ ���� ó�� �Լ�
	virtual void SendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag);
	//���� ������ ó�� �Լ�
	virtual void RecvHandling(SOCKETINFO* _info, DWORD flag);
	//���� ����
	virtual BOOL GetEndServer();
//=======================================================//

};
#endif// __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__