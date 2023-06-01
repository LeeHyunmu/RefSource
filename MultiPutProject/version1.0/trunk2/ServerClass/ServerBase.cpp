
#include "stdAfx.h"
#include "ServerBase.h"
//--------------------------------------------------------------
//|
//|���� ���̺귯�� ��ũ
//|
//---------------------------------------------------------------
#pragma comment( lib , "ws2_32.lib")
//--------------------------------------------------------------
//|
//|���� �ʱ�ȭ ī��Ʈ
//|
//---------------------------------------------------------------
BOOL CServerBase::bwsacount=0;
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CServerBase::CServerBase():
obj(NULL),
debug(NULL),
m_hTcpSocket(0),
m_hUdpSocket(0),
m_hTcpClientSocket(0),
m_hUdpClientSocket(0),
m_hUdpBroadcastSocket(0),
Local_IP(""),
m_nTcpPort(0),
m_nUdpPort(0),
m_nTcpClientPort(0),
m_nUdpClientPort(0),
m_nUdpBroadcastPort(0),
TCPClient_IP(""),
UDPClient_IP("")
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CServerBase::~CServerBase()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CServerBase::initServerBase(IMyObject* _obj)
{
	//=====================================================
	// �� ��ü ���� Ŭ���� ������Ʈ
	//======================================================
	obj=_obj;
	//=====================================================
	// ����� ��� Ŭ����
	//======================================================
	debug=obj->GetOutputObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CServerBase::ReleaseBase()
{
	//=====================================================
	// ���� ���� �ݱ�
	//======================================================
	//Tcp
	fnCloseSocket(m_hTcpSocket);
	//Udp
	fnCloseSocket(m_hUdpSocket);
	//Tcp
	fnCloseSocket(m_hTcpClientSocket);
	//Udp
	fnCloseSocket(m_hUdpClientSocket);
	//Broadcast
	fnCloseSocket(m_hUdpBroadcastSocket);
	//=====================================================
	// ���� ����
	//======================================================
	Cleanup();
}
//--------------------------------------------------------------
//|
//| ���� �ʱ�ȭ
//|
//---------------------------------------------------------------
void CServerBase::Startup()
{
	if(bwsacount==FALSE)
	{
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) 
		{
			bwsacount=TRUE;
		}
	}
}
//--------------------------------------------------------------
//|
//| ���� ����
//|
//---------------------------------------------------------------
void CServerBase::Cleanup()
{
	if(bwsacount==TRUE)
	{
		WSACleanup();
	}
}
//--------------------------------------------------------------
//|
//| ���� close
//|
//---------------------------------------------------------------
void CServerBase::fnCloseSocket(SOCKET& Socket)
{
	if(Socket)
	{
		::shutdown(Socket, SD_BOTH);
		closesocket(Socket);
		Socket = NULL;
	}
}
//--------------------------------------------------------------
//|
//| Bind()
//|
//---------------------------------------------------------------
BOOL CServerBase::fnBind(SOCKET Socket, u_short nPort)
{
	//=====================================================
	// IP, PORT�� ���� ���Ͽ� bind
	//======================================================
	SOCKADDR_IN  sockAddr;  // Server Addr
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(nPort);
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(Socket, (LPSOCKADDR)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		err_display("bind()");
		fnCloseSocket(Socket);
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------
//|
//| Listen()
//|
//---------------------------------------------------------------
BOOL CServerBase::fnListen()
{
	//=====================================================
	// 
	//======================================================
	if (listen(m_hTcpSocket, SOMAXCONN) == SOCKET_ERROR) 
	{
		err_display("listen()");
		fnCloseSocket(m_hTcpSocket);
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------
//|
//| Accept()
//|
//---------------------------------------------------------------
SOCKET CServerBase::fnAccept()
{
	SOCKADDR_IN sockAddr;
	SOCKET hClientSocket;
	int iAddrLen = sizeof(SOCKADDR_IN);
	//=====================================================
	// OVERLAPPED ��������� ������ �����Ͽ��� ���� 2�� �ƹ��ų� �ᵵ �ȴ�
	//======================================================
	//hClientSocket = WSAAccept(m_hSocket, (LPSOCKADDR)&sockAddr, &iAddrLen, NULL, NULL);
	hClientSocket = accept(m_hTcpSocket, (LPSOCKADDR)&sockAddr, &iAddrLen);
	if(hClientSocket == INVALID_SOCKET) 
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			err_display("accept()");
		return NULL;
	}
	CString strIP = inet_ntoa(sockAddr.sin_addr);
	u_short port = ntohs(sockAddr.sin_port);
	if(debug)
	{
		debug->Write("IP:[%s] PORT:[%d]����", strIP, port);
	}
	return hClientSocket;
}
//--------------------------------------------------------------
//|
//| connect()
//|
//---------------------------------------------------------------
BOOL CServerBase::fnconnect(SOCKET sock, u_short _port, CString _ip)
{
	int retval;
	//connect()
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_port);
	serveraddr.sin_addr.s_addr = inet_addr(_ip);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if(retval==SOCKET_ERROR)
	{
		err_display("connect()");
		fnCloseSocket(sock);
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|�ͺ��ŷ �������� ��ȯ�Ѵ�(TCP)
//|
//---------------------------------------------------------------
BOOL CServerBase::ChangeTcpNonblocking(SOCKET Socket)
{
	u_long on = TRUE;
	// �ͺ��ŷ �������� ��ȯ
	if(ioctlsocket(Socket, FIONBIO, &on) == SOCKET_ERROR) 
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			err_display("ioctlsocket()");
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|�ͺ��ŷ �������� ��ȯ�Ѵ�(UDP)
//|
//---------------------------------------------------------------
BOOL CServerBase::ChangeUdpNonblocking()
{
	u_long on = TRUE;
	// �ͺ��ŷ �������� ��ȯ
	if(ioctlsocket(m_hUdpSocket, FIONBIO, &on) == SOCKET_ERROR) 
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			err_display("ioctlsocket()");
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Bind/Listen(TCP)
//|
//---------------------------------------------------------------
BOOL CServerBase::CreateServerBaseTcp(u_short _nport)
{
	//=====================================================
	// ���� �ʱ�ȭ
	//======================================================
	Startup();
	//=====================================================
	// ���� IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  ����Ȯ��(���� ��Ʈ�� ������)
	//======================================================
	if(m_hTcpSocket&&(m_nTcpPort==_nport))
	{
		return TRUE;
	}
	//=====================================================
	// Tcp ���� ���� �ݱ�
	//======================================================
	if(m_hTcpSocket)
		fnCloseSocket(m_hTcpSocket);
	//=====================================================
	// ���� ���� ��Ʈ
	//======================================================
	m_nTcpPort=_nport;
	//=====================================================
	// OVERLAPPED ��������� ������ �����Ѵ�(IOCP ����� ����ϱ� ���� ����)
	//======================================================
	//m_hSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//=====================================================
	// ������ �����Ѵ�
	//======================================================
	m_hTcpSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_hTcpSocket == INVALID_SOCKET)
	{
		err_display("socket()");	return FALSE;
	}
	//=====================================================
	// Bind
	//======================================================
	if(!fnBind(m_hTcpSocket, m_nTcpPort))		return FALSE;
	//=====================================================
	// Listen
	//======================================================
	if(!fnListen())		return FALSE;
	//=====================================================
	// �� ���ŷ �������� ��ȯ
	//======================================================
	ChangeTcpNonblocking(m_hTcpSocket);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| TCP Server ����
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseTcp()
{
	//Tcp Server
	fnCloseSocket(m_hTcpSocket);
	//port ����
	m_nTcpPort=0;

}
//--------------------------------------------------------------
//|
//| Bind(UDP)
//|
//---------------------------------------------------------------
BOOL CServerBase::CreateServerBaseUdp(u_short _nport)
{
	//=====================================================
	// ���� �ʱ�ȭ
	//======================================================
	Startup();
	//=====================================================
	// ���� IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  ����Ȯ��(���� ��Ʈ�� ������)
	//======================================================
	if(m_hUdpSocket&&(m_nUdpPort==_nport))
	{
		return TRUE;
	}
	//=====================================================
	// Udp ���� ���� �ݱ�
	//======================================================
	if(m_hUdpSocket)
		fnCloseSocket(m_hUdpSocket);
	//=====================================================
	// ���� ���� ��Ʈ
	//======================================================
	m_nUdpPort=_nport;
	//=====================================================
	// OVERLAPPED ��������� ������ �����Ѵ�(IOCP ����� ����ϱ� ���� ����)
	//======================================================
	//m_hSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//=====================================================
	// ������ �����Ѵ�
	//======================================================
	m_hUdpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_hUdpSocket == INVALID_SOCKET)
	{
		err_display("socket()");	return FALSE;
	}
	//=====================================================
	// Bind
	//======================================================
	if(!fnBind(m_hUdpSocket, m_nUdpPort))		return FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Udp Server ����
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseUdp()
{
	//Udp Server
	fnCloseSocket(m_hUdpSocket);
	//port ����
	m_nUdpPort=0;

}
//--------------------------------------------------------------
//|
//| Connect (TCP)
//|
//---------------------------------------------------------------
BOOL CServerBase::CreateServerBaseTcpClient(u_short _port, CString _ip)
{
	//=====================================================
	// ���� �ʱ�ȭ
	//======================================================
	Startup();
	//=====================================================
	// ���� IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  ����Ȯ��(���� ��Ʈ�� ������)
	//======================================================
	if(m_hTcpClientSocket&&(m_nTcpClientPort==_port&&_ip.Compare(TCPClient_IP)))
	{
		return TRUE;
	}
	//=====================================================
	//  ���� �ݱ�
	//======================================================
	if(m_hTcpClientSocket)
		fnCloseSocket(m_hTcpClientSocket);
	//=====================================================
	// TCP ���� IP
	//======================================================
	TCPClient_IP=_ip;
	//=====================================================
	// TCP ���� ��Ʈ
	//======================================================
	m_nTcpClientPort=_port;
	//=====================================================
	//=====================================================
	// ������ �����Ѵ�
	//======================================================
	m_hTcpClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_hTcpClientSocket == INVALID_SOCKET)
	{
		err_display("socket()");	return FALSE;
	}
	//=====================================================
	// connect
	//======================================================
	if(!fnconnect(m_hTcpClientSocket, m_nTcpClientPort, TCPClient_IP))		return FALSE;
	//=====================================================
	// �� ���ŷ �������� ��ȯ
	//======================================================
	ChangeTcpNonblocking(m_hTcpClientSocket);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| TCP Ŭ���̾�Ʈ ����
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseClientTcp()
{
	//Tcp Client
	fnCloseSocket(m_hTcpClientSocket);
	//port ����
	m_nTcpClientPort=0;
	//ip ����
	TCPClient_IP=_T("");
}
//--------------------------------------------------------------
//|
//| Connect (UDP)
//|
//---------------------------------------------------------------
BOOL CServerBase::CreateServerBaseUdpClient(u_short _port, CString _ip)
{
	//=====================================================
	// ���� �ʱ�ȭ
	//======================================================
	Startup();
	//=====================================================
	// ���� IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  ����Ȯ��(���� ��Ʈ�� ������)
	//======================================================
	if(m_hUdpClientSocket&&(m_nUdpClientPort==_port&&_ip.Compare(UDPClient_IP)))
	{
		return TRUE;
	}
	//=====================================================
	//  ���� �ݱ�
	//======================================================
	if(m_hUdpClientSocket)
		fnCloseSocket(m_hUdpClientSocket);
	//=====================================================
	// TCP ���� IP
	//======================================================
	UDPClient_IP=_ip;
	//=====================================================
	// TCP ���� ��Ʈ
	//======================================================
	m_nUdpClientPort=_port;
	//=====================================================
	//=====================================================
	// ������ �����Ѵ�
	//======================================================
	m_hUdpClientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_hUdpClientSocket == INVALID_SOCKET)
	{
		err_display("socket()");	return FALSE;
	}
	//=====================================================
	// connect
	//======================================================
	if(!fnconnect(m_hUdpClientSocket, m_nUdpClientPort, UDPClient_IP))		return FALSE;
	//=====================================================
	// �� ���ŷ �������� ��ȯ
	//======================================================
	ChangeTcpNonblocking(m_hUdpClientSocket);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| UDP Ŭ���̾�Ʈ ����
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseClientUdp()
{
	//Udp Client
	fnCloseSocket(m_hUdpClientSocket);
	//port ����
	m_nUdpClientPort=0;
	//ip ����
	UDPClient_IP=_T("");
}
//--------------------------------------------------------------
//|
//| Broadcast (UDP)
//|
//---------------------------------------------------------------
BOOL CServerBase::CreateServerBaseUdpbroadcast(SOCKET& sock, u_short _port)
{
	//=====================================================
	// ���� �ʱ�ȭ
	//======================================================
	Startup();
	//=====================================================
	// ���� IP
	//======================================================
	ServerLocalip();
	int retval;
	//=====================================================
	//  ����Ȯ��(���� ��Ʈ�� ������)
	//======================================================
	if(m_hUdpBroadcastSocket&&m_nUdpBroadcastPort==_port)
	{
		sock = m_hUdpBroadcastSocket;
		return TRUE;
	}
	//=====================================================
	// ��ε��ɽ�Ʈ ����
	//======================================================
	if(m_hUdpBroadcastSocket)
	{
		fnCloseSocket(m_hUdpBroadcastSocket);
	}
	//=====================================================
	// UDP ��ε��ɽ�Ʈ ��Ʈ
	//======================================================
	m_nUdpBroadcastPort=_port;
	//=====================================================
	//=====================================================
	// ������ �����Ѵ�
	//======================================================
	m_hUdpBroadcastSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_hUdpBroadcastSocket == INVALID_SOCKET)
	{
		err_display("socket()");	return FALSE;
	}
	//=====================================================
	// option ����
	//=====================================================
	// broadcast op
	int optval = TRUE;
	retval = setsockopt(m_hUdpBroadcastSocket, SOL_SOCKET, SO_BROADCAST, 
		(char *)&optval, sizeof(optval));
	if(retval == SOCKET_ERROR)
	{
		err_display("setsockopt()"); return FALSE;
	}
	//sock�� �ѱ��
	sock = m_hUdpBroadcastSocket;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Broadcast ���� ����
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseUdpbroadcast()
{
	//��ε��ɽ��� ����
	fnCloseSocket(m_hUdpBroadcastSocket);
	//port ����
	m_nUdpBroadcastPort=0;
}
//--------------------------------------------------------------
//|
//| ���� IP
//|
//---------------------------------------------------------------
BOOL CServerBase::ServerLocalip()
{
	if(Local_IP.GetLength())
		return TRUE;
	//=====================================================
	// ���� IP�� ���Ѵ�
	//======================================================
	char Host[500];
	if (gethostname(Host, sizeof(Host)) == SOCKET_ERROR)
	{
		err_display("gethostname()");	return FALSE;
	}
	struct hostent* pk_Host = gethostbyname(Host);
	if (!pk_Host)
	{
		err_display("gethostbyname()");	return FALSE;
	}
	BYTE* pu8_Addr = (BYTE*)((DWORD*)pk_Host->h_addr_list[0]);
	Local_IP.Format(_T("%d.%d.%d.%d"), pu8_Addr[0],pu8_Addr[1],pu8_Addr[2],pu8_Addr[3]);
	if(debug)
	{
		debug->Write("����IP:%s",Local_IP);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|�����Լ� ���� ����Ʈ ���
//|
//---------------------------------------------------------------
void CServerBase::err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,0,NULL);
	if(debug)
	{
		debug->Write("ErrorCode:[%s]%s",msg, (LPCTSTR)lpMsgBuf);
	}
	LocalFree(lpMsgBuf);
}
//--------------------------------------------------------------
//|
//|TCP ����
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetTCPSocket()
{
	return m_hTcpSocket;
}
//--------------------------------------------------------------
//|
//|UDP ����
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetUDPSocket()
{
	return m_hUdpSocket;
}
//--------------------------------------------------------------
//|
//|TCP Client ����
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetTCPClientSocket()
{
	return m_hTcpClientSocket;
}
//--------------------------------------------------------------
//|
//|UDP Client����
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetUDPClientSocket()
{
	return m_hUdpClientSocket;
}
//--------------------------------------------------------------
//|
//|���� IP �ּ�
//|
//---------------------------------------------------------------
CString CServerBase::GetServerIp()
{
	return Local_IP;
}
//--------------------------------------------------------------
//|
//|���� ��Ʈ ��ȣ(TCP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetTcpServerPort()
{
	return m_nTcpPort;
}
//--------------------------------------------------------------
//|
//|���� ��Ʈ ��ȣ(UDP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetUdpServerPort()
{
	return m_nUdpPort;
}
//--------------------------------------------------------------
//|
//|Client ��Ʈ ��ȣ(TCP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetTcpClientPort()
{
	return m_nTcpClientPort;
}
//--------------------------------------------------------------
//|
//|Client ��Ʈ ��ȣ(UDP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetUdpClientPort()
{
	return m_nUdpClientPort;
}
//--------------------------------------------------------------
//|
//|TCP Ŭ���̾�Ʈ IP �ּ�
//|
//---------------------------------------------------------------
CString CServerBase::GetTcpClientIp()
{
	return TCPClient_IP;
}
//--------------------------------------------------------------
//|
//|UDP Ŭ���̾�Ʈ IP �ּ�
//|
//---------------------------------------------------------------
CString CServerBase::GetUdpClientIp()
{
	return UDPClient_IP;
}