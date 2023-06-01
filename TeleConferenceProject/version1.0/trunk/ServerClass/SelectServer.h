
#ifndef __SELECTSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SELECTSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
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
	//TCP 서버 
	//====================================
	// 소켓 정보 저장을 위한 구조체
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
	//UDP 서버 
	//====================================
	// 소켓 정보 저장을 위한 구조체
	struct SOCKETINFOUDP
	{
		// 데이터 통신에 사용할 변수
		SOCKADDR_IN peeraddr;
		char buf[SELECTUDPSERVERBUFSIZE+1];
		int recvbytes;
		int maxrecv;
		int sendbytes;
		int totalsend;
	};
	SOCKETINFOUDP  udpinfo;
	//=====================================
	//TCP 클라이언트 
	//====================================
	// 소켓 정보 저장을 위한 구조체
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
	//UDP 클라이언트
	//====================================
	// 소켓 정보 저장을 위한 구조체
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
	//생성자
	CSelectServer();
	//소멸자
	~CSelectServer();
private:
//======================================================//
//			일반함수 선언
//======================================================//
	//소캣 정보 추가
	BOOL AddSocketInfo(SOCKET sock);
	//소켓 정보 삭제
	void RemoveSocketInfo(int nIndex);
	//소켓 정보 모두 삭제
	void AllRemoveSocketInfo();
	//
	BOOL SelectTcpLoop(CSelectServer *server);
	//Udp Loop
	BOOL UdpLoop(CSelectServer *server);
	//Tcp Client Loop
	BOOL TcpClientLoop(CSelectServer *server);
	//Udp Client Loop
	BOOL UdpClientLoop(CSelectServer *server);
	//TCP 서버 소켓에서 소켓과 같은 정보 구조체를 가져 온다
	SOCKETINFOTCP* GetTcpServerInfo(SOCKET _sock);
//=================스래드 관련 객체===========================//
	//=====================================
	//TCP 서버 
	//====================================
	//스래드 함수
	static DWORD WINAPI SelectTcpServerProcess(LPVOID lpArg);
	//스래드 객체
	CThread  SelectTcpServerthread;
	//이벤트 객체
	CEvent  SelectTcpServerEvent;
	//종료 이벤트 객체
	CEvent  CloseSelectTcpServerEvent;
	//종료 플래그
	BOOL	bTcpClose;
	//Tcp Server Recv 콜백 함수
	TCPRECVCALLBACK TcpServerCall;
	//호출할 클래스 객체 포인터
	LPVOID TcpServerlpArg;
	//=====================================
	//UDP 서버 
	//====================================
	//스래드 함수
	static DWORD WINAPI SelectUdpServerProcess(LPVOID lpArg);
	//스래드 객체
	CThread  SelectUdpServerthread;
	//이벤트 객체
	CEvent  SelectUdpServerEvent;
	//종료 이벤트 객체
	CEvent  CloseSelectUdpServerEvent;
	//종료 플래그
	BOOL	bUdpClose;
	//Udp Server Recv 콜백 함수
	UDPRECVCALLBACK UdpServerCall;
	//호출할 클래스 객체 포인터
	LPVOID UdpServerlpArg;
	//=====================================
	//TCP Client 
	//====================================
	//스래드 함수
	static DWORD WINAPI SelectTcpClientProcess(LPVOID lpArg);
	//스래드 객체
	CThread  SelectTcpClientthread;
	//이벤트 객체
	CEvent  SelectTcpClientEvent;
	//종료 이벤트 객체
	CEvent  CloseSelectTcpClientEvent;
	//종료 플래그
	BOOL	bTcpClientClose;
	//Tcp Client Recv 콜백 함수
	TCPCLIENTRECVCALLBACK TcpClientCall;
	//호출할 클래스 객체 포인터
	LPVOID TcpClientlpArg;
	//=====================================
	//UDP Client 
	//====================================
	//스래드 함수
	static DWORD WINAPI SelectUdpClientProcess(LPVOID lpArg);
	//스래드 객체
	CThread  SelectUdpClientthread;
	//이벤트 객체
	CEvent  SelectUdpClientEvent;
	//종료 이벤트 객체
	CEvent  CloseSelectUdpClientEvent;
	//종료 플래그
	BOOL	bUdpClientClose;
	//Udp Client Recv 콜백 함수
	UDPCLIENTRECVCALLBACK UdpClientCall;
	//호출할 클래스 객체 포인터
	LPVOID UdpClientlpArg;
//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			변수 선언
//======================================================//
	//종료 플래그
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
public:
	// 객체 해제
	virtual void Release();
	// 객체 초기화
	virtual BOOL initInterfaceObject(IMyObject* _obj);
	//서버 생성(TCP)
	virtual BOOL CreateTcpServer(u_short _nport, TCPRECVCALLBACK callFunc, LPVOID lpArg);
	//서버 생성(UDP)
	virtual BOOL CreateUdpServer(u_short _nport, UDPRECVCALLBACK callFunc, LPVOID lpArg);
	//클라이언트 생성 및 접속(TCP)
	virtual BOOL CreateTcpClient(u_short _port, CString _ip, TCPCLIENTRECVCALLBACK callFunc, LPVOID lpArg);
	//클라이언트 생성 및 접속(UDP)
	virtual BOOL CreateUdpClient(u_short _port, CString _ip, UDPCLIENTRECVCALLBACK callFunc, LPVOID lpArg);
	//브로드케스팅
	virtual BOOL Sendbroadcast(u_short _port, char SendrecvMsg[], UINT& sendrecvlen, UINT totalrecv);
	// Tcp 서버 send 함수
	virtual BOOL TcpServerSend(SOCKET _sock);
	// Udp 서버 send 함수
	virtual BOOL UdpServerSend();
	// Tcp 클라이언트 send 함수
	virtual BOOL TcpClientSend();
	// Udp 클라이언트 send 함수
	virtual BOOL UdpClientSend();
//=======================================================//
	//서버 IP 주소
	virtual CString GetLocalServerIp();

};
#endif// __SELECTSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
