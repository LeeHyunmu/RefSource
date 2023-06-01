#include "stdAfx.h"
#include "UdpWinSocketServer.h"
#include <WinSock2.h>

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CUdpWinSocketServer::CUdpWinSocketServer():
debug(NULL),
obj(NULL),
serverThread(NULL),
bClose(FALSE),
server_sock(INVALID_SOCKET)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CUdpWinSocketServer::~CUdpWinSocketServer()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CUdpWinSocketServer::Release()
{
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CUdpWinSocketServer::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// 오브젝트 관리 생성 관리 클래스
	//========================================
	obj=_obj;
	//========================================
	// 디버그 출력 클래스
	//========================================
	debug=obj->GetOutputObject();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CUdpWinSocketServer::initWinSocket()
{
	serverThread.Create(teleProcess, (LPVOID)obj);
}
//--------------------------------------------------------------
//|
//|  스래드
//|
//---------------------------------------------------------------
DWORD WINAPI CUdpWinSocketServer::teleProcess(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IUdpWinSocket* udpsocketserver = obj->GetUdpWinSocketObject();
	if(udpsocketserver)
		udpsocketserver->TeleUdpfunc();
	return 0;
}
//--------------------------------------------------------------
//|
//|  서버 만들기
//|
//---------------------------------------------------------------
BOOL CUdpWinSocketServer::MakeUdpServer(u_short _port, CString _ip)
{
	int retval;
	//윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa) !=0)
		return FALSE;
	//socket()
	server_sock = socket(AF_INET, SOCK_DGRAM,0);
	if(server_sock == INVALID_SOCKET)
	{
		debug->Write(_T("socket()"));
		return FALSE;
	}
	//bind()
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(server_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if(retval==SOCKET_ERROR)
	{
		debug->Write(_T("bind()"));
		closesocket(server_sock); server_sock=INVALID_SOCKET;
		WSACleanup();
		return FALSE;
	}
	initWinSocket();
	debug->Write(_T("서버시작"));
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CUdpWinSocketServer::TeleUdpfunc()
{
	SOCKADDR_IN clientaddr;
	int addrlen;
	while (1)
	{

	}
}