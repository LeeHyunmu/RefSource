
#ifndef __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//
//|서버 콜백 함수
//|
//---------------------------------------------------------------
//TCP 서버 
typedef BOOL (*TCPRECVCALLBACK)(
LPVOID lpArg,
SOCKET& sock,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);
//UDP 서버
typedef BOOL (*UDPRECVCALLBACK)(
LPVOID lpArg,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);
//TCP 클라이언트
typedef BOOL (*TCPCLIENTRECVCALLBACK)(
LPVOID lpArg,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);
//UDP 클라이언트
typedef BOOL (*UDPCLIENTRECVCALLBACK)(
LPVOID lpArg,
char buf[],
int& recvbytes,
int& maxrecv,
int& sendbytes,
int& totalsend);

//--------------------------------------------------------------
//|
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IServer
{
	//순수가상 소멸자
	virtual ~IServer()=0;
	//종료
	virtual void Release()=0;
	//초기 설정
	virtual BOOL initInterfaceObject(IMyObject* _obj)=0;
	//서버 생성(TCP)
	virtual BOOL CreateTcpServer(u_short _nport, TCPRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//서버 생성(UDP)
	virtual BOOL CreateUdpServer(u_short _nport, UDPRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//클라이언트 생성 및 접속(TCP)
	virtual BOOL CreateTcpClient(u_short _port, CString _ip, TCPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//클라이언트 생성 및 접속(UDP)
	virtual BOOL CreateUdpClient(u_short _port, CString _ip, UDPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)=0;
	//브로드케스팅
	virtual BOOL Sendbroadcast(u_short _port, char SendrecvMsg[], UINT& sendrecvlen, UINT totalrecv)=0;
	// Tcp 서버 send 함수
	virtual BOOL TcpServerSend(SOCKET _sock)=0;
	// Udp 서버 send 함수
	virtual BOOL UdpServerSend()=0;
	// Tcp 클라이언트 send 함수
	virtual BOOL TcpClientSend()=0;
	// Udp 클라이언트 send 함수
	virtual BOOL UdpClientSend()=0;
	//==========================================================================
	//서버 IP 주소
	virtual CString GetLocalServerIp()=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__