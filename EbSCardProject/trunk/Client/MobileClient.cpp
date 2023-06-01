
#include "stdAfx.h"
#include "../Client/Client.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
#pragma comment( lib , "ws2_32.lib")

#define LOOPFLAGE_ACTIVE	0x00000001 //루프 안쪽 실행
#define LOOPFLAGE_END		0x00000002 //루프 안쪽 실행 안함
//--------------------------------------------------------------
//|
//|생성자
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
//|소멸자
//|
//---------------------------------------------------------------
CMobileClient::~CMobileClient()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CMobileClient::initInterfaceObject(IMyObject* _obj)
{
	//WlanGetAvailableNetworkList 
	obj=_obj;
	debug=obj->GetOutputObject();
	//버퍼 초기화
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
//|사용중인 객체를 메모리에서 제거한다
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
//|스래드
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
	debug->Write("소켓 클라이언트 종료");
	return 0;
}
//--------------------------------------------------------------
//|
//|받기 위한 함수(사용자 정의)
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
//|스래드(보내기 받기 이벤트 시 작동 함수)
//|
//---------------------------------------------------------------
BOOL CMobileClient::ClientMain()
{
	int retval;
	if(dwSendSize)
	{
		retval = send(sock,(const CHAR*)buf, dwSendSize,0);
		debug->Write("서버로 보내기");
	}
	else if(dwRecvSize)
	{
		retval = recvn(sock, buf, dwRecvSize, 0);
		debug->Write("서버에서 받기");
	}
	if(retval == SOCKET_ERROR)
	{
		SetEvent(hReadEvent);
		debug->Write("소켓 통신 에러 발생");
		return FALSE;
	}
	SetEvent(hReadEvent);
	debug->Write("다음 준비상태");
	return TRUE;
}
//--------------------------------------------------------------
//|
//|Send (보낼 데이터가 있을 때
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
//|Recv 받을 데이터가 있을 때
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
//|Send , Recv 이벤트 상황을 알기 위한 함수
//|
//---------------------------------------------------------------
BOOL CMobileClient::WaitForReadEvent(DWORD dwTimeout) const
{
	ATLASSERT(hReadEvent);
	return ::WaitForSingleObject(hReadEvent, dwTimeout) == WAIT_OBJECT_0;
}
//--------------------------------------------------------------
//|
//|스래드 종료
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
//|클라이언트 접속
//|
//---------------------------------------------------------------
BOOL CMobileClient::ServerConnect(u_short _port, CString _ip)
{
	if(sock!=INVALID_SOCKET)
	{
		debug->Write("서버 접속 상태");
		return TRUE;
	}
	if(_port) port=_port;
	if(_ip.GetLength()) ip=_ip;

	int retval;
	//윈속 초기화
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
		debug->Write("서버 접속 실패");
		return FALSE;
	}
	else
	{
		if(_IsAborted())
		{
			Start();
			dwLoop=LOOPFLAGE_ACTIVE;
			debug->Write("스래드 스타트");
		}
	}
	debug->Write("서버 접속 성공");
	return TRUE;
}
//--------------------------------------------------------------
//|
//|클라이언트가 연결 상태인가?
//|
//---------------------------------------------------------------
BOOL CMobileClient::ActiveClient()
{
	return dwLoop&LOOPFLAGE_ACTIVE;
}