
#include "stdAfx.h"
#include "ServerMg.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CServerMg::CServerMg():
obj(NULL),
debug(NULL),
iocp(NULL),
m_hIocp(INVALID_HANDLE_VALUE) 
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CServerMg::~CServerMg()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CServerMg::initServerMg(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();
	iocp=obj->GetIocpServer();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CServerMg::Release()
{
	for(int i=0; i<(int)workerthread.size(); i++)
	{
		TerminateThread(workerthread[i], 0);
		CloseHandle(workerthread[i]);
	}
	workerthread.clear();
	if(mthread)
	{
		TerminateThread(mthread, 0);
		CloseHandle(mthread);
		mthread=NULL;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CServerMg::ServerStart(u_short nPort)
{
	if(nPort < 5000 || nPort > 10000)
	{
		MessageBox(NULL, "포트 번호 에러(5001 - 9999)", "오류", MB_ICONINFORMATION);
		return FALSE;
	}
	if(iocp->Initialize(nPort)==FALSE)
		return FALSE;
	// IOCP를 생성한다.
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIocp == NULL )
	{
		MessageBox(NULL, "IOCP 생성 실패", "알림", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}
	////////////////////////////////////////////////////////////////
	// IOCP 처리 스레드를 생성한다.
	SYSTEM_INFO si;
	GetSystemInfo( &si ); // CPU 개수 확인
	// CPU * 2 개의 작업 스레드 생성
	for(DWORD i = 0 ; i < si.dwNumberOfProcessors * 2; i ++ )
	{
		HANDLE hThread=NULL;
		DWORD dwThreadId1;
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)obj, 0, &dwThreadId1);
		if(hThread != NULL)
		{
			workerthread.push_back(hThread);
		}
	}

	////////////////////////////////////////////////////////////////
	// 메인 스레드 생성
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);
	debug->Write("서버시작");
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CServerMg::ServerStop()
{
	for(int i=0; i<(int)workerthread.size(); i++)
	{
		TerminateThread(workerthread[i], 0);
		CloseHandle(workerthread[i]);
	}
	workerthread.clear();
	if(mthread)
	{
		TerminateThread(mthread, 0);
		CloseHandle(mthread);
		mthread=NULL;
	}
	::CloseHandle(m_hIocp); 
	m_hIocp = INVALID_HANDLE_VALUE; 
	iocp->fnCloseSocket();
	debug->Write("서버정지");
}
DWORD WINAPI CServerMg::WorkerThread(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServer *iocp = obj->GetIocpServer();
	IOutput * debug = obj->GetOutput();
	IServerMg* my = obj->GetServerMg();
	HANDLE hIocp=my->GetCompletionPort();

	SOCKET      client_socket;
	SOCKADDR_IN clientAddr;
	//DATA        sendData;
	SOCKETINFO*  _info=NULL;
	int         iAddrLen;
	//int         nItem;
	int         iRetval;
	DWORD       dwTransfer;
	CString     str, strIP, strPort;
	u_short		port;
	//BOOL        bFlag;
	while(1)
	{
		iRetval = GetQueuedCompletionStatus(hIocp, &dwTransfer, (LPDWORD)&client_socket, (LPOVERLAPPED*)&_info, INFINITE );
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			if(!_info)
				continue;
			SOCKETINFO*  socketinfo;
			if(!iocp->fnGetSockInfo(&socketinfo, _info->hSock))
				continue;
			iAddrLen = sizeof(clientAddr);
			getpeername(_info->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
			strIP="";
			strIP = inet_ntoa(clientAddr.sin_addr);
			port = ntohs(clientAddr.sin_port);
			debug->Write("IP:[%s] PORT:[%d]접속해제", strIP, port);
			iocp->fnRemoveSocketInfo(_info->hSock);
			continue;
		}
		//======================================================//
		//	받은 내용이 있을 때 처리	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			//======================================================//
			//	관리서버로부터 처리 요청	
			//=======================================================//
			if(_info->data.wCode & RECVCODE_MYBISERVERSEND)
			{
				_info->wType[0] = 0;
				_tcscpy_s(_info->szCardNum,20,_info->data.szCardNum);
				iAddrLen = sizeof(clientAddr);
				getpeername(_info->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
				strIP="";
				strIP = inet_ntoa(clientAddr.sin_addr);
				port = ntohs(clientAddr.sin_port);
				debug->Write("IP:[%s] PORT:[%d]", strIP, port);
				debug->Write("0x%08x, %s, %s", _info->data.wCode, _info->data.szCardNum, _info->data.szData);
				_info->data.wCode^=RECVCODE_MYBISERVERSEND;
				_info->data.wCode|=RECVCODE_MYBISERVERRECV;
				//======================================================//
				//	카드 Enable 	
				//=======================================================//
				if(_info->data.wCode & RECVCODE_CARDENABLE)
				{
					debug->Write("카드 Enable");
					if(iocp)
						iocp->fnWSASend(_info->hSock, &_info->data, sizeof(DATA));
				}
				//======================================================//
				//	사용자 변경(권종 변경) 	
				//=======================================================//
				else if(_info->data.wCode & RECVCODE_USERCHANGE)
				{
					debug->Write("사용자 변경(권종 변경)");
					if(iocp)
						iocp->fnWSASend(_info->hSock, &_info->data, sizeof(DATA));
				}
				//======================================================//
				//	소득공제 	
				//=======================================================//
				else if(_info->data.wCode & RECVCODE_DEDUCTION)
				{
					debug->Write("소득공제");
					if(iocp)
						iocp->fnWSASend(_info->hSock, &_info->data, sizeof(DATA));
				}
				//======================================================//
				//	사용 내역 체크 	
				//=======================================================//
				else if(_info->data.wCode & RECVCODE_USECHECK)
				{
					debug->Write("사용내역 체크");
				}
			}
			// 다시 받을 수 있게 I/O 요청을 한다.
			iocp->fnWSARecv(_info->hSock);
		}
		//======================================================//
		//	보낸 데이터가 있을 때 처리	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			// 처리할 일이 있으면 처리
			_info->wType[1] = 0;
			debug->Write("보내기 완료");
		}
	}
	return 0;
}

DWORD WINAPI CServerMg::ProcessServer(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServer *iocp = obj->GetIocpServer();
	IOutput * debug = obj->GetOutput();
	IServerMg* my = obj->GetServerMg();
	SOCKET socket;
	HANDLE  hResult;
	while(1)
	{
		socket = iocp->fnAccept();
		if(socket == INVALID_SOCKET)
		{
			if(WSAGetLastError() != WSAEWOULDBLOCK)
				debug->err_display("accept()");
			return 0;
		}
		// 소켓과 입출력 완료 포트 연결
		hResult = CreateIoCompletionPort((HANDLE)socket, my->GetCompletionPort(), (DWORD)socket , 0);
		if(hResult == NULL)
			return -1;
		// 비동기 입출력 시작
		iocp->fnAddSocketInfo(socket);
		iocp->fnWSARecv(socket);
	}
	return 0;
}
HANDLE CServerMg::GetCompletionPort()
{
	return m_hIocp;
}