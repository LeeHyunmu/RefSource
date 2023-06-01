
#include "stdAfx.h"
#include "../Client/Client.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
#pragma comment( lib , "ws2_32.lib")

#define LOOPFLAGE_ACTIVE	0x00000001 //���� ���� ����
#define LOOPFLAGE_END		0x00000002 //���� ���� ���� ����
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMobileClient::CMobileClient():
sock(INVALID_SOCKET),
hReadEvent(NULL),
hWriteEvent(NULL),
dwLoop(LOOPFLAGE_END),
port(0),
ip(_T("")),
dwSendSize(0),
dwRecvSize(0)
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMobileClient::~CMobileClient()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CMobileClient::initInterfaceObject(IMyObject* _obj)
{
	//WlanGetAvailableNetworkList 
	obj=_obj;
	debug=obj->GetOutputObject();
	//���� �ʱ�ȭ
	memset(buf, 0x00, sizeof(CHAR)*MAX_SOCKBUF);
	//
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return FALSE;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CMobileClient::ReleaseClient()
{
	ThreadClose();
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);
}
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
DWORD CMobileClient::Run()
{
	while( !IsAborted() ) 
	{
		WaitForSingleObject(hWriteEvent, INFINITE);
		if(dwLoop&LOOPFLAGE_ACTIVE)
		{
			if(!ClientMain())
			{
				break;
			}
		}
	}	
	closesocket(sock); sock=INVALID_SOCKET;
	WSACleanup();
	dwLoop=LOOPFLAGE_END;
	debug->Write("���� Ŭ���̾�Ʈ ����");
	return 0;
}
//--------------------------------------------------------------
//|
//|�ޱ� ���� �Լ�(����� ����)
//|
//---------------------------------------------------------------
int CMobileClient::recvn(SOCKET s, CHAR *buf, int len, int flags, BOOL bfull)
{
	int received;
	CHAR *ptr = buf;
	int left = len;
	while(left>0)
	{
		received = recv(s, ptr, left, flags);
		if(received==SOCKET_ERROR)
			return SOCKET_ERROR;
		else if(received ==0)
		{
			if(!bfull)
				break;
		}
		left-=received;
		ptr+= received;
	}
	return (len-left);
}
//--------------------------------------------------------------
//|
//|������(������ �ޱ� �̺�Ʈ �� �۵� �Լ�)
//|
//---------------------------------------------------------------
BOOL CMobileClient::ClientMain()
{
	int retval;
	if(dwSendSize)
	{
		retval = send(sock,(const CHAR*)buf, dwSendSize,0);
		debug->Write("������ ������");
	}
	else if(dwRecvSize)
	{
		retval = recvn(sock, buf, dwRecvSize, 0);
		debug->Write("�������� �ޱ�");
	}
	if(retval == SOCKET_ERROR)
	{
		SetEvent(hReadEvent);
		debug->Write("���� ��� ���� �߻�");
		return FALSE;
	}
	SetEvent(hReadEvent);
	debug->Write("���� �غ����");
	return TRUE;
}
//--------------------------------------------------------------
//|
//|Send (���� �����Ͱ� ���� ��
//|
//---------------------------------------------------------------
BOOL CMobileClient::SendDatafn(DWORD _dwSendSize, void* data, DWORD dwTimeout)
{
	dwSendSize=_dwSendSize;
	memset(buf,0x00,sizeof(buf));
	memcpy_s(buf, sizeof(buf), data, _dwSendSize);
	SetEvent(hWriteEvent);
	if(WaitForThread(dwTimeout))
	{
		dwSendSize=0;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Recv ���� �����Ͱ� ���� ��
//|
//---------------------------------------------------------------
BOOL CMobileClient::RecvDatafn(DWORD _dwRecvSize, void* data, DWORD dwTimeout)
{
	dwRecvSize=_dwRecvSize;
	memset(buf,0x00,sizeof(buf));
	SetEvent(hWriteEvent);
	if(WaitForThread(dwTimeout))
	{
		dwRecvSize=0;
		memcpy_s(data, _dwRecvSize, buf, _dwRecvSize);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Send , Recv �̺�Ʈ ��Ȳ�� �˱� ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CMobileClient::WaitForReadEvent(DWORD dwTimeout) const
{
	ATLASSERT(hReadEvent);
	return ::WaitForSingleObject(hReadEvent, dwTimeout) == WAIT_OBJECT_0;
}
//--------------------------------------------------------------
//|
//|������ ����
//|
//---------------------------------------------------------------
void CMobileClient::ThreadClose()
{
	dwLoop=LOOPFLAGE_END;
	SetEvent(hWriteEvent);
	Stop();
	Release();
}
//--------------------------------------------------------------
//|
//|Ŭ���̾�Ʈ ����
//|
//---------------------------------------------------------------
BOOL CMobileClient::ServerConnect(u_short _port, CString _ip)
{
	if(sock!=INVALID_SOCKET)
	{
		debug->Write("���� ���� ����");
		return TRUE;
	}
	if(_port) port=_port;
	if(_ip.GetLength()) ip=_ip;

	int retval;
	//���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2),&wsa) !=0)
		return FALSE;
	//socket()
	sock = socket(AF_INET, SOCK_STREAM,0);
	if(sock == INVALID_SOCKET)
	{
		return FALSE;
	}
	//connect()
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(ip);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if(retval==SOCKET_ERROR)
	{
		closesocket(sock); sock=INVALID_SOCKET;
		WSACleanup();
		debug->Write("���� ���� ����");
		return FALSE;
	}
	else
	{
		if(_IsAborted())
		{
			Start();
			dwLoop=LOOPFLAGE_ACTIVE;
			debug->Write("������ ��ŸƮ");
		}
	}
	debug->Write("���� ���� ����");
	return TRUE;
}
//--------------------------------------------------------------
//|
//|Ŭ���̾�Ʈ�� ���� �����ΰ�?
//|
//---------------------------------------------------------------
BOOL CMobileClient::ActiveClient()
{
	return dwLoop&LOOPFLAGE_ACTIVE;
}