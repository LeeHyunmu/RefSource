#include "stdAfx.h"
#include "UdpWinSocketServer.h"
#include <WinSock2.h>

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CUdpWinSocketServer::~CUdpWinSocketServer()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CUdpWinSocketServer::Release()
{
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CUdpWinSocketServer::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
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
//|  ������
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
//|  ���� �����
//|
//---------------------------------------------------------------
BOOL CUdpWinSocketServer::MakeUdpServer(u_short _port, CString _ip)
{
	int retval;
	//���� �ʱ�ȭ
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
	debug->Write(_T("��������"));
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