
#ifndef __SERVERBASE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SERVERBASE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CServerBase : public IServer
{
public:
	//생성자
	CServerBase();
	//소멸자
	virtual ~CServerBase();
//======================================================//
//			일반함수 선언
//======================================================//
	//소켓 종료
	void fnCloseSocket(SOCKET& Socket);
	//bind 함수
	BOOL fnBind(SOCKET Socket, u_short nPort);
	//Listen 함수
	BOOL fnListen();
	//accept 함수
	SOCKET fnAccept();
	//로컬 IP 알아내기
	BOOL ServerLocalip();
	//넌블로킹으로 서버 소켓 전환(TCP)
	BOOL ChangeTcpNonblocking(SOCKET Socket);
	//넌블로킹으로 서버 소켓 전환(UDP)
	BOOL ChangeUdpNonblocking();
	//클라이언트 생성
	//클라이언트 접속
	BOOL fnconnect(SOCKET sock, u_short _port, CString _ip);
	//디버그 출력 함수
	void err_display(char* msg);
//======================================================//
//			윈속 초기화 및 종료
//======================================================//
	// 윈속 초기화 카운트
	static BOOL bwsacount;
	//윈속 초기화
	static void Startup();
	//윈속 종료
	static void Cleanup();
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			변수 선언
//======================================================//
private:
	//서버 소켓 객체(TCP)
	SOCKET      m_hTcpSocket;
	//서버 소켓 객체(UDP)
	SOCKET      m_hUdpSocket;
	//클라이언트 소켓 객체(TCP)
	SOCKET      m_hTcpClientSocket;
	//클라이언트 소켓 객체(UDP)
	SOCKET      m_hUdpClientSocket;
	//브로드케스트 소켓 객체(UDP)
	SOCKET		m_hUdpBroadcastSocket;
	//로컬 IP
	CString		Local_IP;
	//TCP 접속 IP
	CString		TCPClient_IP;
	//UDP 접속 IP
	CString		UDPClient_IP;
	//TCP 서버 생성 포트 번호
	u_short     m_nTcpPort;
	//Udp 서버 생성 포트 번호
	u_short     m_nUdpPort;
	//TCP 클라이언트 접속 포트 번호
	u_short     m_nTcpClientPort;
	//Udp 클라이언트 접속 포트 번호
	u_short     m_nUdpClientPort;
	//Udp 브로드케스트 포트 번호
	u_short     m_nUdpBroadcastPort;

public:
	//TCP 소켓 
	SOCKET GetTCPSocket();
	//UDP 소켓
	SOCKET GetUDPSocket();
	//TCP 클라이언트 소켓 
	SOCKET GetTCPClientSocket();
	//UDP 클라이언트 소켓
	SOCKET GetUDPClientSocket();
	//서버 IP 주소
	CString GetServerIp();
	//서버 포트 번호(TCP)
	u_short GetTcpServerPort();
	//서버 포트 번호(UDP)
	u_short GetUdpServerPort();
	//클라이언트 접속 포트 번호(TCP)
	u_short GetTcpClientPort();
	//클라이언트 접속 포트 번호(UDP)
	u_short GetUdpClientPort();
	//TCP 클라이언트 IP 주소
	CString GetTcpClientIp();
	//UDP 클라이언트 IP 주소
	CString GetUdpClientIp();
//======================================================//
//			정의 함수
//======================================================//
	//종료
	virtual void ReleaseBase();
	//초기화
	virtual BOOL initServerBase(IMyObject* _obj);
	//서버 생성(TCP)
	virtual BOOL CreateServerBaseTcp(u_short _nport);
	//서버 생성(TCP) 종료
	virtual void CloseServerBaseTcp();
	//서버 생성(UDP)
	virtual BOOL CreateServerBaseUdp(u_short _nport);
	//서버 생성(UDP) 종료
	virtual void CloseServerBaseUdp();
	//클라이언트 접속(TCP)
	virtual BOOL CreateServerBaseTcpClient(u_short _port, CString _ip);
	//클라이언트 접속(TCP) 종료
	virtual void CloseServerBaseClientTcp();
	//클라이언트 접속(UDP)
	virtual BOOL CreateServerBaseUdpClient(u_short _port, CString _ip);
	//클라이언트 접속(UDP) 종료
	virtual void CloseServerBaseClientUdp();
	//브로드케스트 (UDP)
	virtual BOOL CreateServerBaseUdpbroadcast(SOCKET& sock, u_short _port);
	//브로드케스트 (UDP) 종료
	virtual void CloseServerBaseUdpbroadcast();
//=======================================================//

};
#endif// __SERVERBASE_74F736E25AB94b5dA789E4730F58F2A1_H__
