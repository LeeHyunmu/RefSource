
#ifndef __WINSOCKETSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __WINSOCKETSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinSocketServer : public IWinSocket
{
	struct RecvThreadInfo 
	{
		SOCKET sock;
		IMyObject* obj;
		CThread  RecvThread;
	};
	typedef vector<RecvThreadInfo*> vecThreadInfo;
public:
	//������
	CWinSocketServer();
	//�Ҹ���
	~CWinSocketServer();
	//=================������ �Լ�===========================//
	static DWORD WINAPI AcceptProcess(LPVOID lpArg);
	static DWORD WINAPI RecvProcess(LPVOID lpArg);
	//=======================================================//
	//			���� ����
	//======================================================//
	CThread  AcceptThread;
	BOOL bClose;
	SOCKET listen_sock;

	vecThreadInfo RecvThread;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//======================================================//
	//			�������̽� ������ 
	//======================================================//

	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	//���� �ʱ�ȭ
	void initWinSocket();
	//������ �������� IP addr ��������
	CString GetIPAddr(TCHAR* name, IN_ADDR* addr);
	//IP addr�� ������ ���� ��������
	CString GetDomainName(IN_ADDR addr, TCHAR* name);
	//Ŭ���̾�Ʈ ���ú� �����带 �����Ѵ�
	void MakeRecvThread(SOCKET sock);
	//�Ұ� ���� vector ����
	void DeleteRecvThread(SOCKET sock);
	//=======================================================//
	//======================================================//
	//			����
	//======================================================//
	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release();
	//Accept  �Լ�
	virtual void Acceptfunc();
	//������ �����
	virtual BOOL MakeServer(u_short _port, CString _ip);
	//������ ���ú� ���
	virtual void Recvfunc(SOCKET client_sock);
	//=======================================================//
};
#endif// __WINSOCKETSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
