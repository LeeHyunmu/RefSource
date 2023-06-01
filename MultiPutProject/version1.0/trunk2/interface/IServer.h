
#ifndef __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//
//|���� �ݹ� �Լ�
//|
//---------------------------------------------------------------
//TCP ���� 
typedef BOOL (*TCPRECVCALLBACK)(
LPVOID lpArg,
SOCKET& sock,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);
//UDP ����
typedef BOOL (*UDPRECVCALLBACK)(
LPVOID lpArg,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);
//TCP Ŭ���̾�Ʈ
typedef BOOL (*TCPCLIENTRECVCALLBACK)(
LPVOID lpArg,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);
//UDP Ŭ���̾�Ʈ
typedef BOOL (*UDPCLIENTRECVCALLBACK)(
LPVOID lpArg,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);

//--------------------------------------------------------------
//|
//|IServer �������̽�
//|
//---------------------------------------------------------------
interface IServer
{
	//�������� �Ҹ���
	virtual ~IServer()=0;
	//����
	virtual void Release()=0;
	//�ʱ� ����
	virtual BOOL initInterfaceObject(IMyObject* _obj)=0;
	//���� ����(TCP)
	virtual BOOL CreateTcpServer(u_short _nport, TCPRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//���� ����(UDP)
	virtual BOOL CreateUdpServer(u_short _nport, UDPRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//Ŭ���̾�Ʈ ���� �� ����(TCP)
	virtual BOOL CreateTcpClient(u_short _port, CString _ip, TCPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//Ŭ���̾�Ʈ ���� �� ����(UDP)
	virtual BOOL CreateUdpClient(u_short _port, CString _ip, UDPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//��ε��ɽ���
	virtual BOOL Sendbroadcast(u_short _port, char SendrecvMsg[], UINT& sendrecvlen, UINT totalrecv)=0;
	// Tcp ���� send �Լ�
	virtual BOOL TcpServerSend(SOCKET _sock)=0;
	// Udp ���� send �Լ�
	virtual BOOL UdpServerSend()=0;
	// Tcp Ŭ���̾�Ʈ send �Լ�
	virtual BOOL TcpClientSend()=0;
	// Udp Ŭ���̾�Ʈ send �Լ�
	virtual BOOL UdpClientSend()=0;
	//==========================================================================
	//���� IP �ּ�
	virtual CString GetLocalServerIp()=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__