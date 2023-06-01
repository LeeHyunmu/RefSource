
#include "stdAfx.h"
#include "../Client/Client.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>

#define DEFAULTPORT 9000
#define DEFAULTIP _T("127.0.0.1")
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMobileClient::CMobileClient():
sock(0),
hReadEvent(NULL),
hWriteEvent(NULL),
bEnd(FALSE),
port(DEFAULTPORT),
ip(DEFAULTIP),
wsendCode(0)
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
BOOL CMobileClient::initClient(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();
	memset(buf, 0x00, sizeof(TCHAR)*1024);
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
		if(!bEnd)
		{
			if(!ClientMain())
			{
				continue;
			}
		}
	}	
	closesocket(sock); sock=0;
	WSACleanup();
	return 0;
}
int CMobileClient::recvn(SOCKET s, char *buf, int len, int flags, BOOL bfull)
{
	int received;
	char *ptr = buf;
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

BOOL CMobileClient::ClientMain()
{
	int retval;
	//������ ���� Ŭ���� ������
	ICrc* crc32 = obj->GetCrc32();
	baseDATA20* bsdt =(baseDATA20*) buf;
	retval = send(sock,(const char*)buf, bsdt->dwdatalen,0);
	if(retval == SOCKET_ERROR)
	{
		debug->err_display("send()");
		SetEvent(hReadEvent);
		debug->Write("Send �غ����");
		return FALSE;
	}
	debug->Write("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�",retval);
	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
	{
		//��볻�� ��ȸ ���� ����ü ����
		DATA162*    recvdata=(DATA162*)bsdt->data;
		//���� ������ ũ��
		DWORD datalen=sizeof(baseDATA20)+sizeof(DATA162)-2;
		while(TRUE)
		{
			debug->Write("recv ������");
			retval = recvn(sock, (char*)buf, datalen, 0);
			if(retval == SOCKET_ERROR)
			{
				debug->err_display("recv()");
				SetEvent(hReadEvent);
				return FALSE;
			}
			else if(retval == 0)
			{
				SetEvent(hReadEvent);
				return FALSE;
			}
			//������ ���� Ȯ��
			if(crc32->GetCheck(buf,bsdt->dwdatalen))
				debug->Write("������ ���� OK");
			else
				debug->Write("������ ���� NO");
			if(recvdata->dwnum==0)
			{
				debug->Write("�� �̻� ���� �����Ͱ� ����");
				break;
			}
			debug->Write("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�",retval);
			debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
			debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
				recvdata->dwcode, recvdata->dwusemoney, recvdata->usename, recvdata->usecardnum, recvdata->useplace, recvdata->usetime, recvdata->useadress);
		}
	}
	else
	{
		DATA36* recvdata=(DATA36*)bsdt->data;
		//���� ������ ũ��
		DWORD datalen=sizeof(baseDATA20)+sizeof(DATA36)-2;
		debug->Write("recv ������");
		retval = recvn(sock, (char*)buf, datalen, 0);
		if(retval == SOCKET_ERROR)
		{
			debug->err_display("recv()");
			SetEvent(hReadEvent);
			return FALSE;
		}
		else if(retval == 0)
		{
			SetEvent(hReadEvent);
			return FALSE;
		}
		//������ ���� Ȯ��
		if(crc32->GetCheck(buf,bsdt->dwdatalen))
			debug->Write("������ ���� OK");
		else
			debug->Write("������ ���� NO");
		debug->Write("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�",retval);
		debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d,����Ÿ:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
	}
	wsendCode=0;
	SetEvent(hReadEvent);
	debug->Write("Send �غ����");
	return TRUE;
}
void CMobileClient::SendData(int code, TCHAR* CardNum, TCHAR* data)
{
	WaitForSingleObject(hReadEvent, INFINITE);
	//������ ���� Ŭ���� ������
	ICrc* crc32 = obj->GetCrc32();
	baseDATA20* bsdt =(baseDATA20*) buf;
	memset(bsdt,0,sizeof(baseDATA20));
	DATA36* senddata=(DATA36*)bsdt->data;
	memset(senddata,0,sizeof(DATA36));
	//ũ�⸦ ���Ѵ�
	bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // ���� ����ü ũ��
	bsdt->dwBaseCode=code;
	_tcscpy_s(bsdt->szCardNum,sizeof(bsdt->szCardNum),CardNum);
	_tcscpy_s(senddata->info,sizeof(senddata->info),data);
	//üũ ������ �Է�
	crc32->GetResult(buf,bsdt->dwdatalen-4);
	//������ ���� Ȯ��
	if(crc32->GetCheck(buf,bsdt->dwdatalen))
	{
		SetEvent(hWriteEvent); debug->Write("������ ���� OK");
	}
	else
		debug->Write("������ ���� NO");
	SetEvent(hReadEvent);
}
void CMobileClient::ThreadClose()
{
	WaitForSingleObject(hReadEvent, INFINITE);
	bEnd=TRUE;
	SetEvent(hWriteEvent);
	Stop();
	Release();
}
BOOL CMobileClient::ServerConnect(u_short _port, CString _ip)
{
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
		debug->err_display("socket()");
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
		debug->err_display("connect()");
		closesocket(sock); sock=INVALID_SOCKET;
		WSACleanup();
		MessageBox(NULL, "�������ӿ� �����߽��ϴ�", "Ȯ��", MB_OK);
		return FALSE;
	}
	else
	{
		Start();
		debug->Write("Send �غ����");
	}
	return TRUE;
}