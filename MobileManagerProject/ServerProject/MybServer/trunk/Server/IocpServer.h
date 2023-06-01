
#ifndef __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CIocpServer : public IServer
{
public:
	//������
	CIocpServer();
	//�Ҹ���
	~CIocpServer();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	BOOL fnBind();
	BOOL fnListen();
	//���� ����
	void Cleanup();
//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			���� ����
//======================================================//
public:
	vecSocketInfo  SockInfo;
	SOCKET      m_hSocket;

private:
	u_short     m_nPort;
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();

	virtual BOOL initServer(IMyObject* _obj);
	//�ʱ�ȭ
	virtual BOOL Initialize(u_short _nport);
	//������ �ݴ´�
	virtual void fnCloseSocket();
	//�޴� �Լ�
	virtual DWORD fnWSARecv(SOCKET hSock, DWORD dwFlags = 0);
	//������ �Լ�
	virtual DWORD fnWSASend(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0);
	// ��ü ������
	virtual DWORD fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0);

	//���� ���� �߰�
	virtual BOOL fnAddSocketInfo(SOCKET hSock, SOCKET hConnectSock=0);
	//�������� ����
	virtual BOOL fnRemoveSocketInfo(SOCKET hSock);
	//�������� ��������
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock);
	//Ŭ���̾�Ʈ ����
	virtual SOCKET fnAccept();
	//�񵿱� Ŭ���̾�Ʈ�� ���� ����� �Լ�
	virtual BOOL CompletionPortClientSocket(CString _ip, u_short _port, SOCKET& hClientSock);
//=======================================================//

};
#endif// __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
