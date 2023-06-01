
#include "stdAfx.h"
#include "ServerBase.h"
//--------------------------------------------------------------
//|
//|윈속 라이브러리 링크
//|
//---------------------------------------------------------------
#pragma comment( lib , "ws2_32.lib")
//--------------------------------------------------------------
//|
//|윈속 초기화 카운트
//|
//---------------------------------------------------------------
BOOL CServerBase::bwsacount=0;
//--------------------------------------------------------------
//|
//|생성자
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
//|소멸자
//|
//---------------------------------------------------------------
CServerBase::~CServerBase()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CServerBase::initServerBase(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutputObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CServerBase::ReleaseBase()
{
	//=====================================================
	// 서버 소켓 닫기
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
	// 윈속 종료
	//======================================================
	Cleanup();
}
//--------------------------------------------------------------
//|
//| 윈속 초기화
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
//| 윈속 종료
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
//| 소켓 close
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
	// IP, PORT를 서버 소켓에 bind
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
	// OVERLAPPED 기능으르로 소켓을 생성하였을 때는 2중 아무거나 써도 된다
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
		debug->Write("IP:[%s] PORT:[%d]접속", strIP, port);
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
//|넌블로킹 소켓으로 전환한다(TCP)
//|
//---------------------------------------------------------------
BOOL CServerBase::ChangeTcpNonblocking(SOCKET Socket)
{
	u_long on = TRUE;
	// 넌블로킹 소켓으로 전환
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
//|넌블로킹 소켓으로 전환한다(UDP)
//|
//---------------------------------------------------------------
BOOL CServerBase::ChangeUdpNonblocking()
{
	u_long on = TRUE;
	// 넌블로킹 소켓으로 전환
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
	// 윈속 초기화
	//======================================================
	Startup();
	//=====================================================
	// 로컬 IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  소켓확인(기존 포트와 같으면)
	//======================================================
	if(m_hTcpSocket&&(m_nTcpPort==_nport))
	{
		return TRUE;
	}
	//=====================================================
	// Tcp 서버 소켓 닫기
	//======================================================
	if(m_hTcpSocket)
		fnCloseSocket(m_hTcpSocket);
	//=====================================================
	// 서버 생성 포트
	//======================================================
	m_nTcpPort=_nport;
	//=====================================================
	// OVERLAPPED 기능으르로 소켓을 생성한다(IOCP 기능을 사용하기 위한 설정)
	//======================================================
	//m_hSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//=====================================================
	// 소켓을 생성한다
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
	// 넌 블로킹 소켓으로 전환
	//======================================================
	ChangeTcpNonblocking(m_hTcpSocket);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| TCP Server 종료
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseTcp()
{
	//Tcp Server
	fnCloseSocket(m_hTcpSocket);
	//port 삭제
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
	// 윈속 초기화
	//======================================================
	Startup();
	//=====================================================
	// 로컬 IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  소켓확인(기존 포트와 같으면)
	//======================================================
	if(m_hUdpSocket&&(m_nUdpPort==_nport))
	{
		return TRUE;
	}
	//=====================================================
	// Udp 서버 소켓 닫기
	//======================================================
	if(m_hUdpSocket)
		fnCloseSocket(m_hUdpSocket);
	//=====================================================
	// 서버 생성 포트
	//======================================================
	m_nUdpPort=_nport;
	//=====================================================
	// OVERLAPPED 기능으르로 소켓을 생성한다(IOCP 기능을 사용하기 위한 설정)
	//======================================================
	//m_hSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//=====================================================
	// 소켓을 생성한다
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
//| Udp Server 종료
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseUdp()
{
	//Udp Server
	fnCloseSocket(m_hUdpSocket);
	//port 삭제
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
	// 윈속 초기화
	//======================================================
	Startup();
	//=====================================================
	// 로컬 IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  소켓확인(기존 포트와 같으면)
	//======================================================
	if(m_hTcpClientSocket&&(m_nTcpClientPort==_port&&_ip.Compare(TCPClient_IP)))
	{
		return TRUE;
	}
	//=====================================================
	//  소켓 닫기
	//======================================================
	if(m_hTcpClientSocket)
		fnCloseSocket(m_hTcpClientSocket);
	//=====================================================
	// TCP 접속 IP
	//======================================================
	TCPClient_IP=_ip;
	//=====================================================
	// TCP 접속 포트
	//======================================================
	m_nTcpClientPort=_port;
	//=====================================================
	//=====================================================
	// 소켓을 생성한다
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
	// 넌 블로킹 소켓으로 전환
	//======================================================
	ChangeTcpNonblocking(m_hTcpClientSocket);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| TCP 클라이언트 종료
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseClientTcp()
{
	//Tcp Client
	fnCloseSocket(m_hTcpClientSocket);
	//port 삭제
	m_nTcpClientPort=0;
	//ip 삭제
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
	// 윈속 초기화
	//======================================================
	Startup();
	//=====================================================
	// 로컬 IP
	//======================================================
	ServerLocalip();
	//=====================================================
	//  소켓확인(기존 포트와 같으면)
	//======================================================
	if(m_hUdpClientSocket&&(m_nUdpClientPort==_port&&_ip.Compare(UDPClient_IP)))
	{
		return TRUE;
	}
	//=====================================================
	//  소켓 닫기
	//======================================================
	if(m_hUdpClientSocket)
		fnCloseSocket(m_hUdpClientSocket);
	//=====================================================
	// TCP 접속 IP
	//======================================================
	UDPClient_IP=_ip;
	//=====================================================
	// TCP 접속 포트
	//======================================================
	m_nUdpClientPort=_port;
	//=====================================================
	//=====================================================
	// 소켓을 생성한다
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
	// 넌 블로킹 소켓으로 전환
	//======================================================
	ChangeTcpNonblocking(m_hUdpClientSocket);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| UDP 클라이언트 종료
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseClientUdp()
{
	//Udp Client
	fnCloseSocket(m_hUdpClientSocket);
	//port 삭제
	m_nUdpClientPort=0;
	//ip 삭제
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
	// 윈속 초기화
	//======================================================
	Startup();
	//=====================================================
	// 로컬 IP
	//======================================================
	ServerLocalip();
	int retval;
	//=====================================================
	//  소켓확인(기존 포트와 같으면)
	//======================================================
	if(m_hUdpBroadcastSocket&&m_nUdpBroadcastPort==_port)
	{
		sock = m_hUdpBroadcastSocket;
		return TRUE;
	}
	//=====================================================
	// 브로드케스트 소켓
	//======================================================
	if(m_hUdpBroadcastSocket)
	{
		fnCloseSocket(m_hUdpBroadcastSocket);
	}
	//=====================================================
	// UDP 브로드케스트 포트
	//======================================================
	m_nUdpBroadcastPort=_port;
	//=====================================================
	//=====================================================
	// 소켓을 생성한다
	//======================================================
	m_hUdpBroadcastSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_hUdpBroadcastSocket == INVALID_SOCKET)
	{
		err_display("socket()");	return FALSE;
	}
	//=====================================================
	// option 세팅
	//=====================================================
	// broadcast op
	int optval = TRUE;
	retval = setsockopt(m_hUdpBroadcastSocket, SOL_SOCKET, SO_BROADCAST, 
		(char *)&optval, sizeof(optval));
	if(retval == SOCKET_ERROR)
	{
		err_display("setsockopt()"); return FALSE;
	}
	//sock를 넘긴다
	sock = m_hUdpBroadcastSocket;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Broadcast 소켓 종료
//|
//---------------------------------------------------------------
void CServerBase::CloseServerBaseUdpbroadcast()
{
	//브로드케스팅 종료
	fnCloseSocket(m_hUdpBroadcastSocket);
	//port 삭제
	m_nUdpBroadcastPort=0;
}
//--------------------------------------------------------------
//|
//| 로컬 IP
//|
//---------------------------------------------------------------
BOOL CServerBase::ServerLocalip()
{
	if(Local_IP.GetLength())
		return TRUE;
	//=====================================================
	// 로컬 IP를 구한다
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
		debug->Write("로컬IP:%s",Local_IP);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|소켓함수 오류 에디트 출력
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
//|TCP 소켓
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetTCPSocket()
{
	return m_hTcpSocket;
}
//--------------------------------------------------------------
//|
//|UDP 소켓
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetUDPSocket()
{
	return m_hUdpSocket;
}
//--------------------------------------------------------------
//|
//|TCP Client 소켓
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetTCPClientSocket()
{
	return m_hTcpClientSocket;
}
//--------------------------------------------------------------
//|
//|UDP Client소켓
//|
//---------------------------------------------------------------
SOCKET CServerBase::GetUDPClientSocket()
{
	return m_hUdpClientSocket;
}
//--------------------------------------------------------------
//|
//|서버 IP 주소
//|
//---------------------------------------------------------------
CString CServerBase::GetServerIp()
{
	return Local_IP;
}
//--------------------------------------------------------------
//|
//|서버 포트 번호(TCP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetTcpServerPort()
{
	return m_nTcpPort;
}
//--------------------------------------------------------------
//|
//|서버 포트 번호(UDP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetUdpServerPort()
{
	return m_nUdpPort;
}
//--------------------------------------------------------------
//|
//|Client 포트 번호(TCP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetTcpClientPort()
{
	return m_nTcpClientPort;
}
//--------------------------------------------------------------
//|
//|Client 포트 번호(UDP)
//|
//---------------------------------------------------------------
u_short CServerBase::GetUdpClientPort()
{
	return m_nUdpClientPort;
}
//--------------------------------------------------------------
//|
//|TCP 클라이언트 IP 주소
//|
//---------------------------------------------------------------
CString CServerBase::GetTcpClientIp()
{
	return TCPClient_IP;
}
//--------------------------------------------------------------
//|
//|UDP 클라이언트 IP 주소
//|
//---------------------------------------------------------------
CString CServerBase::GetUdpClientIp()
{
	return UDPClient_IP;
}