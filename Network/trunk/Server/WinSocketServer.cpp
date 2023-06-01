#include "stdAfx.h"
#include "WinSocketServer.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CWinSocketServer::~CWinSocketServer()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
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
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CWinSocketServer::initInterfaceObject(IMyObject* _obj)
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
void CWinSocketServer::initWinSocket()
{
	AcceptThread.Create(AcceptProcess, (LPVOID)obj);
}
//--------------------------------------------------------------
//|
//|  ������
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
	debug->Write(_T("�����γ��ӿ��� ��巹�� ��� ����"));
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
	debug->Write(_T("��巹������ ������ �̸� ��� ���� ����"));
	return "";
}
BOOL CWinSocketServer::MakeServer(u_short _port, CString _ip)
{
	int retval;
	//���� �ʱ�ȭ
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
	debug->Write(_T("��������"));
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
		//���� ���÷���
		debug->Write(_T("[TCP����]Ŭ���̾�Ʈ����: ���鸸���IP�ּ�=%s, ��Ʈ��ȣ=%d"),
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
		//�����͹ޱ�
		retval = recv(client_sock, buf, MAX_PATH,0);
		if(retval==SOCKET_ERROR)
		{
			debug->Write(_T("recv()"));
			break;
		}
		else if(retval==0) break;

		//���
		//buf[retval]='\0';
		debug->Write(_T("[TCP/%s:%d] %s"),
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
		//�ٽ� ������
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
	//����
	debug->Write(_T("[TCP����]Ŭ���̾�Ʈ���� :IP�ּ�=%s, ��Ʈ��ȣ=%d"),
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
}
//--------------------------------------------------------------
//|
//|  ������
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