
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
//Recv, Send ����ü ����
//
#define  MAX_SOCKBUF	8192

//���̽� ����ü
// code - ���۵� �ڵ� �з�
// card number - ī�� �ѹ�
// total len - data ��ü ����
// 20byte ����
// data[1] ������
// dwdatalen = data[1] ������ ���� + ��ȿ�� üũ�ڵ�(4byte) ����
// CRC_32 ��ȿ�� üũ �� ���
//--------------------------------------------------------------
//|
//| �������� ������ ����ü
//|
//---------------------------------------------------------------
#pragma pack(push,1)
struct baseDATA20
{
	DWORD dwBaseCode;
	CHAR szCardNum[12];
	DWORD dwdatalen;
	CHAR  data[1];
};
struct DATA36
{
	CHAR  info[32];
	DWORD dwcheckorlen;
	CHAR  afterdata[1];
};
struct DATA162
{
	DWORD  dwnum;
	DWORD  dwcode;
	DWORD  dwusemoney;
	CHAR usename[21];
	CHAR usecardnum[11];
	CHAR useplace[21];
	CHAR useadress[61];
	CHAR usetime[32];
	DWORD dwcheckorlen;
	CHAR  afterdata[1];
};
#pragma pack(pop)
//--------------------------------------------------------------
//|
//| ���� ���� ����ü
//|
//---------------------------------------------------------------
struct SENDDATAINFO
{
	HANDLE		   hSendEvent;
	DWORD		   nTotalSendBytes;
	DWORD		   nSendBytes;
	BOOL		   bafterSenddata;
};
struct SOCKLIST
{
	SOCKET         hSock;
	SOCKET         hlinkSock;
};
// ���� ���� ������ ���� ����ü
struct SOCKETINFO
{
	WSAOVERLAPPED  wsaOv;
	DWORD          wType[2];
	WSABUF         wsaBuf[2];  //wsaBuf[0] : Recv, wsaBuf[1] : Send
	SOCKLIST       socklist;
	SENDDATAINFO   sendinfo;
	DWORD		   overlappedcode;
	CHAR		   data[MAX_SOCKBUF];
};
typedef vector<SOCKETINFO*> vecSocketInfo;


// ���� �� I/O ���ؽ�Ʈ
#define IO_TYPE_RECV              0x10000000
#define IO_TYPE_SEND              0x20000000
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
	virtual BOOL Initialize()=0;
	//���� Listen
	virtual BOOL ServerListen(u_short _nport)=0;
	//���� �ݱ�
	virtual void fnCloseSocket()=0;
	//�޴� �Լ�
	virtual DWORD fnWSARecv(SOCKET hSock, DWORD dwFlags = 0)=0;
	//������ �Լ�
	virtual DWORD fnWSASend(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;
	// ��ü ������
	virtual DWORD fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;

	//���� ���� �߰�
	virtual BOOL fnAddSocketInfo(SOCKET hSock, SOCKET hlinkSock=0)=0;
	//�������� ����
	virtual BOOL fnRemoveSocketInfo(SOCKET hSock)=0;
	//�������� ��������
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock)=0;
	//����
	virtual void Release()=0;
	//Ŭ���̾�Ʈ ����
	virtual SOCKET fnAccept()=0;
	//�񵿱� ���� ���� ����
	virtual HANDLE CompletionPortServerSocket(HANDLE hIocp)=0;
	//�񵿱� Ŭ���̾�Ʈ�� ���� ����� �Լ�
	virtual SOCKET CompletionPortClientSocket(HANDLE hIocp, SOCKET hSock)=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__