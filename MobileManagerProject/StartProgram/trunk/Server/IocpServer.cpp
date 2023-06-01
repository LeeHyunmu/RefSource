
#include "stdAfx.h"
#include "IocpServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
#pragma comment( lib , "ws2_32.lib")

#define BUFSIZE 100
typedef struct //소켓 정보를 구조체화.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
typedef struct // 소켓의 버퍼 정보를 구조체화.
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CIocpServer::CIocpServer()
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CIocpServer::~CIocpServer()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CIocpServer::initServer()
{
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CIocpServer::Release()
{
}
//--------------------------------------------------------------
//|
//|ComPort  스래드 함수
//|
//---------------------------------------------------------------
//입 출력 완료에 따른 쓰레드의 행동 정의
unsigned int __stdcall CIocpServer::CompletionThread(void* pComPort)
{
	HANDLE hCompletionPort =(HANDLE)pComPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;

	while(1)
	{
		// 5. 입출력이 완료 된 소켓의 정보 얻음.
		GetQueuedCompletionStatus(hCompletionPort,    // Completion Port
			&BytesTransferred,   // 전송 된 바이트 수
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED 구조체 포인터.
			INFINITE
			);

		if(BytesTransferred==0) //EOF 전송시.
		{
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue; 
		} 

		// 6. 메시지! 클라이언트로 에코.
		PerIoData->wsaBuf.len=BytesTransferred;
		WSASend(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, 0, NULL, NULL);

		// RECEIVE AGAIN
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;

		flags=0;
		WSARecv(PerHandleData->hClntSock,
			&(PerIoData->wsaBuf),
			1,
			NULL,
			&flags,
			&(PerIoData->overlapped),
			NULL
			);      
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|ComPort  소켓 생성 함수
//|
//---------------------------------------------------------------
BOOL CIocpServer::CreateServerSocket()
{
	WSADATA wsaData;
	HANDLE hCompletionPort;
	SYSTEM_INFO SystemInfo;
	SOCKADDR_IN servAddr;
	LPPER_IO_DATA PerIoData;
	LPPER_HANDLE_DATA PerHandleData;

	SOCKET hServSock;
	int RecvBytes;
	int i, Flags;

	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) /* Load Winsock 2.2 DLL */
		MessageBox(NULL,"WSAStartup() error!","소켓에러",MB_OK);

	//1. Completion Port 생성.
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&SystemInfo);

	//2. Completion Port 에서 입출력 완료를 대기 하는 쓰레드를 CPU 개수 만큼 생성.
	for(i=0; i<(int)SystemInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(atoi("9190")); // 이 부분에서 도스인자를 안넣어주신듯 보이네요.
	// 임의적으로 9190이라고 넣어줬습니다.

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);

	while(TRUE)
	{ 
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr; 
		int addrLen=sizeof(clntAddr);

		printf("클라이언트의 접속 대기중...\n");
		hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen); 

		// 연결 된 클라이언트의 소켓 핸들 정보와 주소 정보를 설정.
		PerHandleData=(LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA)); 
		PerHandleData->hClntSock=hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		//3. Overlapped 소켓과 CompletionPort의 연결.
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);

		// 연결 된 클라이언트를 위한 버퍼를 설정 하고 OVERLAPPED 구조체 변수 초기화.
		PerIoData=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED)); 
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;
		Flags=0;

		//4. 중첩된 데이터 입력.
		WSARecv(PerHandleData->hClntSock, // 데이터 입력 소켓.
			&(PerIoData->wsaBuf),  // 데이터 입력 버퍼 포인터.
			1,       // 데이터 입력 버퍼의 수.
			(unsigned long*)&RecvBytes,    
			(unsigned long*)&Flags,
			&(PerIoData->overlapped), // OVERLAPPED 구조체 포인터.
			NULL
			);  
	}
	return TRUE;
}