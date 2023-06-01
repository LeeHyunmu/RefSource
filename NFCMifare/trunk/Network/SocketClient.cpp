
#include "stdafx.h"
#include "../OutputDebug/OutputDebug.h"
const UINT CSocketClient::DEFAULTPORT = 9000;
const CString CSocketClient::DEFAULTIP = _T("127.0.0.1");
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSocketClient::CSocketClient():
sock(0),
port(DEFAULTPORT),
ip(DEFAULTIP)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSocketClient::~CSocketClient()
{
	if(NfcThread.IsRunning())
	{
		NfcThread.Release();
	}
	RespEvent.WaitForEvent(200);
	ReleaseClient();
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
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
BOOL CSocketClient::initClient(HWND _hwnd)
{
	BOOL b=FALSE;

	//========================================
	// 디버그 출력 클래스
	//========================================
	hWndp = _hwnd;
	//버퍼 초기화
	memset(recvbuf, 0x00, BUFFSIZE_4096);
	memset(tempbuf, 0x00, BUFFSIZE_1024);
	//스래드 생성
	NfcThread.Create(ThreadProcess, (LPVOID)this);
	//스래드 이벤트
	RespEvent.Create();
	return b;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CSocketClient::ReleaseClient()
{
	closesocket(sock); sock=0;
	WSACleanup();
}
int CSocketClient::recvn(SOCKET s, char *buf, int len, int flags, BOOL bfull)
{
	int received;
	char *ptr = &buf[0];
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

void CSocketClient::ClientMain()
{
	int retval;
	retval = send(sock, tempbuf, tempbuflen,0);
	if(retval == SOCKET_ERROR)
	{
	}
	while(TRUE)
	{
		retval = recvn(sock, tempbuf, tempbuflen, 0);
		if(retval == SOCKET_ERROR)
		{
		}
		else if(retval == 0)
		{
		}
	}
}
BOOL CSocketClient::SendData(int code, CString type, byte* data, UINT datalen)
{
	BOOL b=FALSE;
	return b;
}
BOOL CSocketClient::ServerConnect(u_short _port, CString _ip)
{
	//포트 세팅
	if(_port) port=_port;
	else port=DEFAULTPORT;
	//ip 세팅
	if(_ip.GetLength()) ip=_ip;
	else  ip=DEFAULTIP;

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
		MessageBox(NULL, "서버접속에 실패했습니다", "확인", MB_OK);
		return FALSE;
	}
	else
	{
		DebugEditCall("Send 준비상태");
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CSocketClient::ThreadProcess(LPVOID lpArg)
{
	CSocketClient *socket = (CSocketClient *)lpArg;
	while (socket->NfcThread.IsRunning() && socket->NfcThread.WaitForThread(SOCKETCLIENTTHREAD_LOOPTIME)==FALSE)
	{
		socket->ClientMain();
	}
	socket->RespEvent.SetEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//| //
//|
//---------------------------------------------------------------
void CSocketClient::DebugEditCall(CString str)
{
#if DEBUGVIEWER
	UINT len = str.GetLength();
	byte* data = new byte[len+5];
	ZeroMemory(data, len+5);
	memcpy(&data[0], &len, 4);
	memcpy(&data[4], str.GetBuffer(), len);
	PostMessage(hWndp, WM_COMMANDDEBUGSTRING,0,(LPARAM)data);
#endif//DEBUGVIEWER
}