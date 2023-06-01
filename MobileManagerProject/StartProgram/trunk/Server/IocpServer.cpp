
#include "stdAfx.h"
#include "IocpServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
#pragma comment( lib , "ws2_32.lib")

#define BUFSIZE 100
typedef struct //���� ������ ����üȭ.
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
typedef struct // ������ ���� ������ ����üȭ.
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CIocpServer::CIocpServer()
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CIocpServer::~CIocpServer()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CIocpServer::initServer()
{
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CIocpServer::Release()
{
}
//--------------------------------------------------------------
//|
//|ComPort  ������ �Լ�
//|
//---------------------------------------------------------------
//�� ��� �Ϸῡ ���� �������� �ൿ ����
unsigned int __stdcall CIocpServer::CompletionThread(void* pComPort)
{
	HANDLE hCompletionPort =(HANDLE)pComPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;

	while(1)
	{
		// 5. ������� �Ϸ� �� ������ ���� ����.
		GetQueuedCompletionStatus(hCompletionPort,    // Completion Port
			&BytesTransferred,   // ���� �� ����Ʈ ��
			(LPDWORD)&PerHandleData,
			(LPOVERLAPPED*)&PerIoData, // OVERLAPPED ����ü ������.
			INFINITE
			);

		if(BytesTransferred==0) //EOF ���۽�.
		{
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue; 
		} 

		// 6. �޽���! Ŭ���̾�Ʈ�� ����.
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
//|ComPort  ���� ���� �Լ�
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
		MessageBox(NULL,"WSAStartup() error!","���Ͽ���",MB_OK);

	//1. Completion Port ����.
	hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&SystemInfo);

	//2. Completion Port ���� ����� �ϷḦ ��� �ϴ� �����带 CPU ���� ��ŭ ����.
	for(i=0; i<(int)SystemInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	servAddr.sin_family=AF_INET;
	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servAddr.sin_port=htons(atoi("9190")); // �� �κп��� �������ڸ� �ȳ־��ֽŵ� ���̳׿�.
	// ���������� 9190�̶�� �־�����ϴ�.

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);

	while(TRUE)
	{ 
		SOCKET hClntSock;
		SOCKADDR_IN clntAddr; 
		int addrLen=sizeof(clntAddr);

		printf("Ŭ���̾�Ʈ�� ���� �����...\n");
		hClntSock=accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen); 

		// ���� �� Ŭ���̾�Ʈ�� ���� �ڵ� ������ �ּ� ������ ����.
		PerHandleData=(LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA)); 
		PerHandleData->hClntSock=hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		//3. Overlapped ���ϰ� CompletionPort�� ����.
		CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);

		// ���� �� Ŭ���̾�Ʈ�� ���� ���۸� ���� �ϰ� OVERLAPPED ����ü ���� �ʱ�ȭ.
		PerIoData=(LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED)); 
		PerIoData->wsaBuf.len=BUFSIZE;
		PerIoData->wsaBuf.buf=PerIoData->buffer;
		Flags=0;

		//4. ��ø�� ������ �Է�.
		WSARecv(PerHandleData->hClntSock, // ������ �Է� ����.
			&(PerIoData->wsaBuf),  // ������ �Է� ���� ������.
			1,       // ������ �Է� ������ ��.
			(unsigned long*)&RecvBytes,    
			(unsigned long*)&Flags,
			&(PerIoData->overlapped), // OVERLAPPED ����ü ������.
			NULL
			);  
	}
	return TRUE;
}