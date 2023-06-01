
#ifndef __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|������ ����ü ����
//|
//---------------------------------------------------------------
// ���� �ڷ�
struct DATA
{
	DWORD wCode;
	CHAR szCardNum[20];
	CHAR szData[1024];
};

// ���� ���� ������ ���� ����ü
struct SOCKETINFO
{
	WSAOVERLAPPED  wsaOv;
	WORD           wType[2];
	WSABUF         wsaBuf[2];  //wsaBuf[0] : Recv, wsaBuf[1] : Send
	SOCKET         hSock;
	CHAR           szCardNum[20];
	DATA		   data;
};
struct SOCKETLINK
{
	SOCKET hVMSock;
	SOCKET hMBSock;
};
typedef vector<SOCKETINFO*> vecSocketInfo;
typedef vector<SOCKETLINK> vecSoLk;

// ���� �� I/O ���ؽ�Ʈ
#define IO_TYPE_RECV              0x1000
#define IO_TYPE_SEND              0x2000
//--------------------------------------------------------------
//|
//|IServer �������̽�
//|
//---------------------------------------------------------------
interface IServer
{
	//�ʱ� ����
	virtual BOOL initServer(IMyObject* _obj)=0;
	//���� �ʱ�ȭ
	virtual BOOL Initialize(u_short _nport)=0;
	//���� �ݱ�
	virtual void fnCloseSocket()=0;
	//�޴� �Լ�
	virtual DWORD fnWSARecv(SOCKET hSock, DWORD dwFlags = 0)=0;
	//������ �Լ�
	virtual DWORD fnWSASend(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;
	// ��ü ������
	virtual DWORD fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;

	//���� ���� �߰�
	virtual BOOL fnAddSocketInfo(SOCKET hSock)=0;
	//�������� ����
	virtual BOOL fnRemoveSocketInfo(SOCKET hSock)=0;
	//�������� ��������
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock)=0;
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock, TCHAR *szCardNum)=0;
	//����
	virtual void Release()=0;
	//Ŭ���̾�Ʈ ����
	virtual SOCKET fnAccept()=0;
	//�񵿱� Ŭ���̾�Ʈ�� ���� ����� �Լ�
	virtual BOOL CompletionPortClientSocket(CString _ip, u_short _port, SOCKET& hClientSock)=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__