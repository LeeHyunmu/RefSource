
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
//|생성자
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
#include <Wlanapi.h>
#pragma comment(lib, "Wlanapi.lib")
#include <winsock2.h>
#include <ws2bth.h>
// #include <BluetoothAPIs.h>
// BOOL __stdcall callback(ULONG uAttribId, LPBYTE pValueStream, ULONG cbStreamSize, LPVOID pvParam)
// {
// 	SDP_ELEMENT_DATA element;
// 	if(ERROR_SUCCESS != BluetoothSdpGetElementData(pValueStream, cbStreamSize, & element))
// 	{
// 		return false;
// 	}
// 	return true;
// }
BOOL CMobileClient::initClient(IMyObject* _obj)
{
	findBluetooth();
	//WlanGetAvailableNetworkList 
	obj=_obj;
	debug=obj->GetOutput();
	memset(buf, 0x00, sizeof(TCHAR)*1024);
	hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	if(hReadEvent == NULL) return FALSE;
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if(hWriteEvent == NULL) return FALSE;
	return TRUE;
}
int CMobileClient::findBluetooth()
{	
	//setup windows sockets
	WSADATA wsaData;
	WORD wVersionRequested=MAKEWORD(2,0);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
	{ 

	}
	//prepare the inquiry data structure
	DWORD qs_len = sizeof(WSAQUERYSET);
	WSAQUERYSET *qs= (WSAQUERYSET*)malloc(qs_len);
	ZeroMemory (qs, qs_len);
	qs->dwSize = sizeof(WSAQUERYSET);
	qs->dwNameSpace = NS_NLA;

	DWORD flags= LUP_CONTAINERS;
	flags |= LUP_FLUSHCACHE |LUP_RETURN_NAME | LUP_RETURN_ADDR;
	HANDLE h;

	//start the device enquiry
	if(SOCKET_ERROR == WSALookupServiceBegin (qs, flags, &h))
	{
		return 0;
	}

	//iterate through the enquiry resultd
	bool done = false;
	while (!done)
	{
		if(NO_ERROR == WSALookupServiceNext (h, flags, &qs_len, qs))
		{
			char buf[40] ={0};
			SOCKADDR_BTH *sa= (SOCKADDR_BTH*)qs->lpcsaBuffer->RemoteAddr.lpSockaddr;
			BTH_ADDR result= sa->btAddr;
			DWORD bufsize =sizeof(buf);
			WSAAddressToString(qs->lpcsaBuffer->RemoteAddr.lpSockaddr,sizeof(SOCKADDR_BTH), NULL,buf, &bufsize);
			//printf("found: %s - %s\n", buf,qs->lpszServiceInstanceName);
		}
		else
		{
			int error= WSAGetLastError();
			if (error ==WSAEFAULT)
			{
				free(qs);
			}
			else if(error= WSA_E_NO_MORE)
			{
				//printf("inquiry complete\n");
				done=true;
			}
			else
			{
				//printf("uh oh..error code\n", error);
				done=true;
			}
		}
	}
	WSALookupServiceEnd(h);
	free(qs);
	WSACleanup();
	return 0;
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
		//사용내역 조회 받을 구조체 연결
		baseDATA20* bsdt =(baseDATA20*) buf;
		if(bsdt->dwBaseCode == RECVCODE_WHERECORDERROR)
		{
			debug->Write("보낸 곳 에러코드"); return 0;
		}
		else if(bsdt->dwBaseCode == RECVCODE_WHATDATAERROR)
		{
			debug->Write("데이터 처리 에러코드"); return 0;
		}
		else if(bsdt->dwBaseCode == RECVCODE_DATAERROR)
		{
			debug->Write("받은데이터에 에러가 있다"); return 0;
		}
		else if(bsdt->dwBaseCode == RECVCODE_BASEERROR)
		{
			debug->Write("처리중 에러 발생"); return 0;
		}
	}
	return (len-left);
}

BOOL CMobileClient::ClientMain()
{
	int retval;
	//데이터 검증 클래스 포인터
	ICrc* crc32 = obj->GetCrc32();
	baseDATA20* bsdt =(baseDATA20*) buf;
	retval = send(sock,(const char*)buf, bsdt->dwdatalen,0);
	if(retval == SOCKET_ERROR)
	{
		debug->err_display("send()");
		SetEvent(hReadEvent);
		debug->Write("Send 준비상태");
		return FALSE;
	}
	debug->Write("[TCP 클라이언트] %d바이트를 보냈습니다",retval);
	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
	{
		//사용내역 조회 받을 구조체 연결
		DATA162*    recvdata=(DATA162*)bsdt->data;
		//받을 데이터 크기
		DWORD datalen=sizeof(baseDATA20)+sizeof(DATA162)-2;
		while(TRUE)
		{
			debug->Write("recv 대기상태");
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
			//데이터 검증 확인
			if(crc32->GetCheck(buf,bsdt->dwdatalen))
				debug->Write("데이터 검증 OK");
			else
				debug->Write("데이터 검증 NO");
			if(recvdata->dwnum==0)
			{
				debug->Write("더 이상 받을 데이터가 없다");
				break;
			}
			debug->Write("[TCP 클라이언트] %d바이트를 받았습니다",retval);
			debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
			debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
				recvdata->dwcode, recvdata->dwusemoney, recvdata->usename, recvdata->usecardnum, recvdata->useplace, recvdata->usetime, recvdata->useadress);
		}
	}
	else
	{
		DATA36* recvdata=(DATA36*)bsdt->data;
		//받을 데이터 크기
		DWORD datalen=sizeof(baseDATA20)+sizeof(DATA36)-2;
		debug->Write("recv 대기상태");
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
		//데이터 검증 확인
		if(crc32->GetCheck(buf,bsdt->dwdatalen))
			debug->Write("데이터 검증 OK");
		else
			debug->Write("데이터 검증 NO");
		debug->Write("[TCP 클라이언트] %d바이트를 받았습니다",retval);
		debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d,데이타:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
	}
	wsendCode=0;
	SetEvent(hReadEvent);
	debug->Write("Send 준비상태");
	return TRUE;
}
void CMobileClient::SendData(int code, TCHAR* CardNum, TCHAR* data)
{
	WaitForSingleObject(hReadEvent, INFINITE);
	//데이터 검증 클래스 포인터
	ICrc* crc32 = obj->GetCrc32();
	baseDATA20* bsdt =(baseDATA20*) buf;
	memset(bsdt,0,sizeof(baseDATA20));
	DATA36* senddata=(DATA36*)bsdt->data;
	memset(senddata,0,sizeof(DATA36));
	//크기를 구한다
	bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // 보낼 구조체 크기
	bsdt->dwBaseCode=code;
	_tcscpy_s(bsdt->szCardNum,sizeof(bsdt->szCardNum),CardNum);
	_tcscpy_s(senddata->info,sizeof(senddata->info),data);
	//체크 데이터 입력
	crc32->GetResult(buf,bsdt->dwdatalen-4);
	//데이터 검증 확인
	if(crc32->GetCheck(buf,bsdt->dwdatalen))
	{
		SetEvent(hWriteEvent); debug->Write("데이터 검증 OK");
	}
	else
		debug->Write("데이터 검증 NO");
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
	//윈속 초기화
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
		MessageBox(NULL, "서버접속에 실패했습니다", "확인", MB_OK);
		return FALSE;
	}
	else
	{
		Start();
		debug->Write("Send 준비상태");
	}
	return TRUE;
}