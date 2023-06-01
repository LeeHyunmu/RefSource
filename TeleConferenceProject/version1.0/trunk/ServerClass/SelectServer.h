
#ifndef __SELECTSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SELECTSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//
#define SELECTTCPSERVERBUFSIZE		2048
#define SELECTUDPSERVERBUFSIZE		512
#define SELECTTCPCLIENTBUFSIZE		2048
#define SELECTUDPCLIENTBUFSIZE		512
#define SELECTUDPBROADCASTBUFSIZE	512
class CSelectServer : public CServerBase
{
	//=====================================
	//TCP ���� 
	//====================================
	// ���� ���� ������ ���� ����ü
	struct SOCKETINFOTCP
	{
		SOCKET sock;
		char buf[SELECTTCPSERVERBUFSIZE+1];
		int recvbytes;
		int maxrecv;
		int sendbytes;
		int totalsend;
	};
	int nTotalSockets;
	SOCKETINFOTCP *SocketInfoArray[FD_SETSIZE];
	//=====================================
	//UDP ���� 
	//====================================
	// ���� ���� ������ ���� ����ü
	struct SOCKETINFOUDP
	{
		// ������ ��ſ� ����� ����
		SOCKADDR_IN peeraddr;
		char buf[SELECTUDPSERVERBUFSIZE+1];
		int recvbytes;
		int maxrecv;
		int sendbytes;
		int totalsend;
	};
	SOCKETINFOUDP  udpinfo;
	//=====================================
	//TCP Ŭ���̾�Ʈ 
	//====================================
	// ���� ���� ������ ���� ����ü
	struct SOCKETINFOTCPCLIENT
	{
		char buf[SELECTTCPCLIENTBUFSIZE+1];
		int recvbytes;
		int maxrecv;
		int sendbytes;
		int totalsend;
	};
	SOCKETINFOTCPCLIENT  tcpclientinfo;
	//=====================================
	//UDP Ŭ���̾�Ʈ
	//====================================
	// ���� ���� ������ ���� ����ü
	struct SOCKETINFOUDPCLIENT
	{
		char buf[SELECTUDPCLIENTBUFSIZE+1];
		int recvbytes;
		int maxrecv;
		int sendbytes;
		int totalsend;
	};
	SOCKETINFOUDPCLIENT  udpclientinfo;
public:
	//������
	CSelectServer();
	//�Ҹ���
	~CSelectServer();
private:
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	//��Ĺ ���� �߰�
	BOOL AddSocketInfo(SOCKET sock);
	//���� ���� ����
	void RemoveSocketInfo(int nIndex);
	//���� ���� ��� ����
	void AllRemoveSocketInfo();
	//
	BOOL SelectTcpLoop(CSelectServer *server);
	//Udp Loop
	BOOL UdpLoop(CSelectServer *server);
	//Tcp Client Loop
	BOOL TcpClientLoop(CSelectServer *server);
	//Udp Client Loop
	BOOL UdpClientLoop(CSelectServer *server);
	//TCP ���� ���Ͽ��� ���ϰ� ���� ���� ����ü�� ���� �´�
	SOCKETINFOTCP* GetTcpServerInfo(SOCKET _sock);
//=================������ ���� ��ü===========================//
	//=====================================
	//TCP ���� 
	//====================================
	//������ �Լ�
	static DWORD WINAPI SelectTcpServerProcess(LPVOID lpArg);
	//������ ��ü
	CThread  SelectTcpServerthread;
	//�̺�Ʈ ��ü
	CEvent  SelectTcpServerEvent;
	//���� �̺�Ʈ ��ü
	CEvent  CloseSelectTcpServerEvent;
	//���� �÷���
	BOOL	bTcpClose;
	//Tcp Server Recv �ݹ� �Լ�
	TCPRECVCALLBACK TcpServerCall;
	//ȣ���� Ŭ���� ��ü ������
	LPVOID TcpServerlpArg;
	//=====================================
	//UDP ���� 
	//====================================
	//������ �Լ�
	static DWORD WINAPI SelectUdpServerProcess(LPVOID lpArg);
	//������ ��ü
	CThread  SelectUdpServerthread;
	//�̺�Ʈ ��ü
	CEvent  SelectUdpServerEvent;
	//���� �̺�Ʈ ��ü
	CEvent  CloseSelectUdpServerEvent;
	//���� �÷���
	BOOL	bUdpClose;
	//Udp Server Recv �ݹ� �Լ�
	UDPRECVCALLBACK UdpServerCall;
	//ȣ���� Ŭ���� ��ü ������
	LPVOID UdpServerlpArg;
	//=====================================
	//TCP Client 
	//====================================
	//������ �Լ�
	static DWORD WINAPI SelectTcpClientProcess(LPVOID lpArg);
	//������ ��ü
	CThread  SelectTcpClientthread;
	//�̺�Ʈ ��ü
	CEvent  SelectTcpClientEvent;
	//���� �̺�Ʈ ��ü
	CEvent  CloseSelectTcpClientEvent;
	//���� �÷���
	BOOL	bTcpClientClose;
	//Tcp Client Recv �ݹ� �Լ�
	TCPCLIENTRECVCALLBACK TcpClientCall;
	//ȣ���� Ŭ���� ��ü ������
	LPVOID TcpClientlpArg;
	//=====================================
	//UDP Client 
	//====================================
	//������ �Լ�
	static DWORD WINAPI SelectUdpClientProcess(LPVOID lpArg);
	//������ ��ü
	CThread  SelectUdpClientthread;
	//�̺�Ʈ ��ü
	CEvent  SelectUdpClientEvent;
	//���� �̺�Ʈ ��ü
	CEvent  CloseSelectUdpClientEvent;
	//���� �÷���
	BOOL	bUdpClientClose;
	//Udp Client Recv �ݹ� �Լ�
	UDPCLIENTRECVCALLBACK UdpClientCall;
	//ȣ���� Ŭ���� ��ü ������
	LPVOID UdpClientlpArg;
//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			���� ����
//======================================================//
	//���� �÷���
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
public:
	// ��ü ����
	virtual void Release();
	// ��ü �ʱ�ȭ
	virtual BOOL initInterfaceObject(IMyObject* _obj);
	//���� ����(TCP)
	virtual BOOL CreateTcpServer(u_short _nport, TCPRECVCALLBACK callFunc, LPVOID lpArg);
	//���� ����(UDP)
	virtual BOOL CreateUdpServer(u_short _nport, UDPRECVCALLBACK callFunc, LPVOID lpArg);
	//Ŭ���̾�Ʈ ���� �� ����(TCP)
	virtual BOOL CreateTcpClient(u_short _port, CString _ip, TCPCLIENTRECVCALLBACK callFunc, LPVOID lpArg);
	//Ŭ���̾�Ʈ ���� �� ����(UDP)
	virtual BOOL CreateUdpClient(u_short _port, CString _ip, UDPCLIENTRECVCALLBACK callFunc, LPVOID lpArg);
	//��ε��ɽ���
	virtual BOOL Sendbroadcast(u_short _port, char SendrecvMsg[], UINT& sendrecvlen, UINT totalrecv);
	// Tcp ���� send �Լ�
	virtual BOOL TcpServerSend(SOCKET _sock);
	// Udp ���� send �Լ�
	virtual BOOL UdpServerSend();
	// Tcp Ŭ���̾�Ʈ send �Լ�
	virtual BOOL TcpClientSend();
	// Udp Ŭ���̾�Ʈ send �Լ�
	virtual BOOL UdpClientSend();
//=======================================================//
	//���� IP �ּ�
	virtual CString GetLocalServerIp();

};
#endif// __SELECTSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
