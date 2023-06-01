#include "stdAfx.h"
#include "WinSocketServer.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CWinSocketServer::CWinSocketServer():
debug(NULL),
obj(NULL),
AcceptThread(NULL),
bClose(FALSE),
listen_sock(INVALID_SOCKET)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CWinSocketServer::~CWinSocketServer()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CWinSocketServer::Release()
{
	for (int i=0; i<(int)RecvThread.size(); i++)
	{
		delete RecvThread[i]; RecvThread[i]=NULL;
	}
	RecvThread.clear();
	//closesocket()
	closesocket(listen_sock);
	WSACleanup();
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CWinSocketServer::initInterfaceObject(IMyObject* _obj)
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
void CWinSocketServer::initWinSocket()
{
	AcceptThread.Create(AcceptProcess, (LPVOID)obj);
}
//--------------------------------------------------------------
//|
//|  스래드
//|
//---------------------------------------------------------------
DWORD WINAPI CWinSocketServer::AcceptProcess(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IWinSocket* socketserver = obj->GetWinSocketObject();
	if(socketserver)
		socketserver->Acceptfunc();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CWinSocketServer::GetIPAddr(TCHAR* name, IN_ADDR* addr)
{
	HOSTENT* ptr=gethostbyname(name);
	if(ptr)
	{
		memcpy(addr, ptr->h_addr, ptr->h_length);
		return inet_ntoa(*(addr));
	}
	debug->Write(_T("도메인네임에서 어드레스 얻기 실패"));
	return "";
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CWinSocketServer::GetDomainName(IN_ADDR addr, TCHAR* name)
{
	HOSTENT* ptr=gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if(ptr)
	{
		_tcscpy_s(name, (_tcslen(ptr->h_name)*sizeof(TCHAR))+1 , ptr->h_name);
		return ptr->h_name;
	}
	debug->Write(_T("어드레스에서 도메인 이름 얻기 실패 실패"));
	return "";
}
BOOL CWinSocketServer::MakeServer(u_short _port, CString _ip)
{
	int retval;
	//윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa) !=0)
		return FALSE;
	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM,0);
	if(listen_sock == INVALID_SOCKET)
	{
		debug->Write(_T("socket()"));
		return FALSE;
	}
	//bind()
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(_port);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if(retval==SOCKET_ERROR)
	{
		debug->Write(_T("bind()"));
		closesocket(listen_sock); listen_sock=INVALID_SOCKET;
		WSACleanup();
		return FALSE;
	}
	retval = listen(listen_sock, SOMAXCONN);
	if(retval==SOCKET_ERROR)
	{
		debug->Write(_T("listen()"));
		closesocket(listen_sock); listen_sock=INVALID_SOCKET;
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
void CWinSocketServer::Acceptfunc()
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	while (1)
	{
		//AcceptEvent.WaitForEvent()
		//accept()
		addrlen = sizeof(clientaddr);
		client_sock=accept(listen_sock,(SOCKADDR*)&clientaddr, &addrlen);
		if(client_sock==INVALID_SOCKET)
		{
			debug->Write(_T("accept()"));
			continue;
		}
		//정보 디스플레이
		debug->Write(_T("[TCP서버]클라이언트접속: 공백만들기IP주소=%s, 포트번호=%d"),
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		MakeRecvThread(client_sock);

	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWinSocketServer::MakeRecvThread(SOCKET sock)
{
	RecvThreadInfo* recvinfo = new RecvThreadInfo;
	recvinfo->sock=sock;
	recvinfo->obj=obj;
	recvinfo->RecvThread.Create(RecvProcess, (LPVOID)recvinfo);
	RecvThread.push_back(recvinfo);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWinSocketServer::Recvfunc(SOCKET client_sock)
{	
	int retval;
	SOCKADDR_IN clientaddr;
	int addrlen;
	TCHAR buf[MAX_PATH];
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		//데이터받기
		retval = recv(client_sock, buf, MAX_PATH,0);
		if(retval==SOCKET_ERROR)
		{
			debug->Write(_T("recv()"));
			break;
		}
		else if(retval==0) break;

		//출력
		//buf[retval]='\0';
		debug->Write(_T("[TCP/%s:%d] %s"),
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
		//다시 보내기
		retval = send(client_sock, buf, retval,0);
		if(retval==SOCKET_ERROR)
		{
			debug->Write(_T("send()"));
			break;
		}
	}
	//closesocket()
	DeleteRecvThread(client_sock);
	closesocket(client_sock);
	//정보
	debug->Write(_T("[TCP서버]클라이언트종료 :IP주소=%s, 포트번호=%d"),
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
}
//--------------------------------------------------------------
//|
//|  스래드
//|
//---------------------------------------------------------------
DWORD WINAPI CWinSocketServer::RecvProcess(LPVOID lpArg)
{
	RecvThreadInfo* recvinfo = (RecvThreadInfo*)lpArg;
	IMyObject *obj = recvinfo->obj;
	IWinSocket* socketserver = obj->GetWinSocketObject();
	if(socketserver)
		socketserver->Recvfunc(recvinfo->sock);
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWinSocketServer::DeleteRecvThread(SOCKET sock)
{
	for (int i=0; i<(int)RecvThread.size(); i++)
	{
		if(RecvThread[i]->sock==sock)
		{
			delete RecvThread[i]; RecvThread[i]=NULL;
			RecvThread.erase (RecvThread.begin()+i);
		}
	}
}