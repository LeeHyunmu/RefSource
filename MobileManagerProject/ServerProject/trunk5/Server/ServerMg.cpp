
#include "stdAfx.h"
#include "ServerMg.h"
#include "../ThdPoolObj/DbDataSendPoolObj.h"
#define  IOCP_SERVERCOUNT 2
#define  IOCP_CLIENTCOUNT 2
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CServerMg::CServerMg():
obj(NULL),
debug(NULL),
iocp(NULL),
m_hIocp(NULL),
m_hclntIocp(NULL),
mthread(NULL),
dwServerThreadCount(0),
dwClientThreadCount(0),
bEndServer(FALSE)
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
	//스래드 종료 플래그
	bEndServer=TRUE;
	if(m_hIocp)
	{
		for (DWORD i = 0; i<dwServerThreadCount; i++)
			::PostQueuedCompletionStatus(m_hIocp, 0, 0, (OVERLAPPED*)NULL);
		WaitForSingleObject(m_hIocp, INFINITE);
		CloseHandle(m_hIocp); m_hIocp=NULL;
	}
	if(m_hclntIocp)
	{
		for (DWORD i = 0; i<dwServerThreadCount; i++)
			::PostQueuedCompletionStatus(m_hclntIocp, 0, 0, (OVERLAPPED*)NULL);
		WaitForSingleObject(m_hclntIocp, INFINITE);
		CloseHandle(m_hclntIocp);m_hclntIocp=NULL;
	}

	for(int i=0; i<(int)workerthread.size(); i++)
	{
		TerminateThread(workerthread[i], 0);
		CloseHandle(workerthread[i]);workerthread[i]=NULL;
	}
	workerthread.clear();
	if(mthread)
	{
		TerminateThread(mthread, 0);
		CloseHandle(mthread);	mthread=NULL;
	}
}
//--------------------------------------------------------------
//|
//|서버 종료
//|
//---------------------------------------------------------------
BOOL CServerMg::GetEndServer()
{
	return bEndServer;
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
	if(iocp->ServerListen(nPort)==FALSE)
		return FALSE;
	// IOCP를 생성한다.
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIocp == NULL )
	{
		MessageBox(NULL, "IOCP 생성 실패", "알림", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}

	m_hclntIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hclntIocp == NULL )
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
	//서버용 스래드
	dwServerThreadCount = si.dwNumberOfProcessors * IOCP_SERVERCOUNT;
	for(DWORD i = 0 ; i < dwServerThreadCount; i ++ )
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
	//클라이언트용 스래드
	dwClientThreadCount = si.dwNumberOfProcessors * IOCP_CLIENTCOUNT;
	for(DWORD i = 0 ; i < dwClientThreadCount; i ++ )
	{
		HANDLE hThread=NULL;
		DWORD dwThreadId1;
		hThread = CreateThread(NULL, 0, WorkerClntThread, (LPVOID)obj, 0, &dwThreadId1);
		if(hThread != NULL)
		{
			workerthread.push_back(hThread);
		}
	}
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
//--------------------------------------------------------------
//|
//| Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CServerMg::WorkerThread(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServer *iocp = obj->GetIocpServer();
	IOutput * debug = obj->GetOutput();
	IServerMg* my = obj->GetServerMg();
	HANDLE hIocp=my->GetCompletionPort();

	SOCKET      client_socket;
	SOCKADDR_IN clientAddr;
	SOCKETINFO*  _info=NULL;
	int         iAddrLen;
	int         iRetval;
	DWORD       dwTransfer;
	CString     str, strIP, strPort;
	u_short		port;
	while(1)
	{
		iRetval = GetQueuedCompletionStatus(hIocp, &dwTransfer, (LPDWORD)&client_socket, (LPOVERLAPPED*)&_info, INFINITE );
		//======================================================//
		//	종료처리	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(my->GetEndServer())
			return 0;
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
			SOCKETINFO*  socketinfo;
			if(!iocp->fnGetSockInfo(&socketinfo, socklist->hSock))
				continue;
			iAddrLen = sizeof(clientAddr);
			getpeername(socklist->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
			strIP="";
			strIP = inet_ntoa(clientAddr.sin_addr);
			port = ntohs(clientAddr.sin_port);
			debug->Write("IP:[%s] PORT:[%d]접속해제", strIP, port);
			iocp->fnRemoveSocketInfo(socklist->hSock);
			continue;
		}
		//======================================================//
		//	받은 내용이 있을 때 처리	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			debug->Write("받기 오버랩을 진행 후..");
			_info->wType[0] = 0;
			//Recv 데이터 처리 함수 호출
			my->RecvHandling(_info,0);
		}
		//======================================================//
		//	보낸 데이터가 있을 때 처리	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			debug->Write("보내기 오버랩을 진행 후..");
			// 처리할 일이 있으면 처리
			_info->wType[1] = 0;
			//Send 데이터 처리 함수 호출
			my->SendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CServerMg::WorkerClntThread(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServer *iocp = obj->GetIocpServer();
	IOutput * debug = obj->GetOutput();
	IServerMg* my = obj->GetServerMg();
	HANDLE hIocp=my->GetClntCompletionPort();

	SOCKET      client_socket;
	SOCKADDR_IN clientAddr;
	SOCKETINFO*  _info=NULL;
	int         iAddrLen;
	int         iRetval;
	DWORD       dwTransfer;
	CString    strIP;
	u_short		port;
	while(1)
	{
		iRetval = GetQueuedCompletionStatus(hIocp, &dwTransfer, (LPDWORD)&client_socket, (LPOVERLAPPED*)&_info, INFINITE );
		//======================================================//
		//	종료처리	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(my->GetEndServer())
			return 0;
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
			SOCKETINFO*  socketinfo;
			if(!iocp->fnGetSockInfo(&socketinfo, socklist->hSock))
				continue;
			iAddrLen = sizeof(clientAddr);
			getpeername(socklist->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
			strIP="";
			strIP = inet_ntoa(clientAddr.sin_addr);
			port = ntohs(clientAddr.sin_port);
			debug->Write("IP:[%s] PORT:[%d]접속해제", strIP, port);
			iocp->fnRemoveSocketInfo(socklist->hSock);
			continue;
		}
		//======================================================//
		//	받은 내용이 있을 때 처리	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			debug->Write("받기 오버랩을 진행 후..");
			_info->wType[0] = 0;
			//Recv 데이터 처리 함수 호출
			my->RecvHandling(_info,0);
		}
		//======================================================//
		//	보낸 데이터가 있을 때 처리	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			debug->Write("보내기 오버랩을 진행 후..");
			// 처리할 일이 있으면 처리
			_info->wType[1] = 0;
			//Send 데이터 처리 함수 호출
			my->SendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| 받은 데이터 처리 함수
//|
//---------------------------------------------------------------
void CServerMg::RecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	데이터 검증용 클래스 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	데이터 분류 	
	//=======================================================//
	baseDATA20* bsdt =(baseDATA20*) _info->data;
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	if(bsdt->dwBaseCode==0)
	{
		iocp->fnWSARecv(socklist->hSock);
		return;
	}
	SOCKADDR_IN clientAddr;
	int         iAddrLen;
	iAddrLen = sizeof(clientAddr);
	getpeername(socklist->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
	CString    strIP="";
	u_short		port=0;
	strIP = inet_ntoa(clientAddr.sin_addr);
	port = ntohs(clientAddr.sin_port);
	debug->Write("IP:[%s] PORT:[%d]", strIP, port);
	//======================================================//
	//	마이비 처리 결과 	
	//=======================================================//
	if(bsdt->dwBaseCode & RECVCODE_MYBISERVERRECV)
	{
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode^=RECVCODE_MYBISERVERRECV;
		//======================================================//
		//	카드 Enable 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
		{

		}
		//======================================================//
		//	사용자 변경(권종 변경) 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
		{

		}
		//======================================================//
		//	소득공제 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
		{

		}
		//======================================================//
		//	사용 내역 체크 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
		{

		}
		bsdt->dwBaseCode|=RECVCODE_VMCLIENTSEND;
		debug->Write("마이비 처리 결과");
		//받은 데이터 검증
		//Recv 데이터 축출
		DATA36* recvdata=(DATA36*)bsdt->data;
		debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d,데이타:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
		//데이터 검증 확인
		if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
			debug->Write("데이터 검증 OK");
		else
			debug->Write("데이터 검증 NO");
		SOCKETINFO*  socketinfo;
		if(iocp->fnGetSockInfo(&socketinfo, socklist->hlinkSock))
		{
			baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
			DATA36* senddata=(DATA36*)bsdtclt->data;
			SENDDATAINFO* sendinfo=(SENDDATAINFO*)&socketinfo->sendinfo;
			ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
			//받은 소켓 정보를 삭제하기 전에 데이터를 이동한다
			memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
			//보낼 데이터를 보낼 소켓 정보로 이동했으면 삭제한다
			iocp->fnRemoveSocketInfo(socklist->hSock);
			//크기를 구한다
			bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // 보낼 구조체 크기
			//보내야 할 데이터 크기를 저정한다
			sendinfo->nTotalSendBytes=bsdt->dwdatalen;
			//체크 데이터 입력
			crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
			//데이터 검증 확인
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("데이터 검증 OK");
			else
				debug->Write("데이터 검증 NO");
			iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
			//iocp->fnWSARecv(socketinfo->socklist.hSock);
		}
	}
	//======================================================//
	//	VM 처리 결과 	
	//=======================================================//
	else if(bsdt->dwBaseCode & RECVCODE_VMCLIENTRECV)
	{
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode^=RECVCODE_VMCLIENTRECV;
		//======================================================//
		//	카드 Enable 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
		{
			debug->Write("카드 Enable");
			//보내는 쪽 코드를 심는다(마이비 쪽으로)
			bsdt->dwBaseCode|=RECVCODE_MYBISERVERSEND;
			//받은 데이터 검증
			//Recv 데이터 축출
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d,데이타:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//체크 값이 0이 되는가 확인한다(체크값이 0이 되어야 정상)
			//데이터 검증 확인
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("데이터 검증 OK");
			else
				debug->Write("데이터 검증 NO");
			//======================================================//
			//	마이비 연결 소켓을 만들고 정보저장	
			//=======================================================//
			SOCKET sock;
			sock = MakeCompetionPortClient(socklist->hSock);
			SOCKETINFO*  socketinfo;
			if(iocp&&iocp->fnGetSockInfo(&socketinfo, sock))
			{
				baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
				DATA36* senddata=(DATA36*)bsdtclt->data;
				SENDDATAINFO* sendinfo=(SENDDATAINFO*)&socketinfo->sendinfo;
				ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
				memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
				//크기를 구한다
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // 보낼 구조체 크기
				//보내야 할 데이터 크기를 저정한다
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				//체크 데이터 입력
				crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
				//체크 값이 0이 되는가 확인한다(체크값이 0이 되어야 정상)
				//데이터 검증 확인
				if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
					debug->Write("데이터 검증 OK");
				else
					debug->Write("데이터 검증 NO");
				iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
				//iocp->fnWSARecv(socketinfo->socklist.hSock);

			}
		}
		//======================================================//
		//	사용자 변경(권종 변경) 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
		{
			debug->Write("권종 변경");
			//보내는 쪽 코드를 심는다(마이비 쪽으로)
			bsdt->dwBaseCode|=RECVCODE_MYBISERVERSEND;
			//받은 데이터 검증
			//Recv 데이터 축출
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d,데이타:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//체크 값이 0이 되는가 확인한다(체크값이 0이 되어야 정상)
			//데이터 검증 확인
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("데이터 검증 OK");
			else
				debug->Write("데이터 검증 NO");
			//======================================================//
			//	마이비 연결 소켓을 만들고 정보저장	
			//=======================================================//
			SOCKET sock;
			sock = MakeCompetionPortClient(socklist->hSock);
			SOCKETINFO*  socketinfo;
			if(iocp&&iocp->fnGetSockInfo(&socketinfo, sock))
			{
				baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
				DATA36* senddata=(DATA36*)bsdtclt->data;
				SENDDATAINFO* sendinfo=(SENDDATAINFO*)&socketinfo->sendinfo;
				ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
				memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
				//크기를 구한다
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // 보낼 구조체 크기
				//보내야 할 데이터 크기를 저정한다
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				//체크 데이터 입력
				crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
				//데이터 검증 확인
				if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
					debug->Write("데이터 검증 OK");
				else
					debug->Write("데이터 검증 NO");
				iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
				//iocp->fnWSARecv(socketinfo->socklist.hSock);

			}
		}
		//======================================================//
		//	소득공제 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
		{
			debug->Write("소득공제");
			//보내는 쪽 코드를 심는다(마이비 쪽으로)
			bsdt->dwBaseCode|=RECVCODE_MYBISERVERSEND;
			//받은 데이터 검증
			//Recv 데이터 축출
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d,데이타:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//체크 값이 0이 되는가 확인한다(체크값이 0이 되어야 정상)
			//데이터 검증 확인
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("데이터 검증 OK");
			else
				debug->Write("데이터 검증 NO");
			//======================================================//
			//	마이비 연결 소켓을 만들고 정보저장	
			//=======================================================//
			SOCKET sock;
			sock = MakeCompetionPortClient(socklist->hSock);
			SOCKETINFO*  socketinfo;
			if(iocp&&iocp->fnGetSockInfo(&socketinfo, sock))
			{
				baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
				DATA36* senddata=(DATA36*)bsdtclt->data;
				SENDDATAINFO* sendinfo=(SENDDATAINFO*)&socketinfo->sendinfo;
				ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
				memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
				//크기를 구한다
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // 보낼 구조체 크기
				//보내야 할 데이터 크기를 저정한다
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				//체크 데이터 입력
				crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
				//체크 값이 0이 되는가 확인한다(체크값이 0이 되어야 정상)
				//데이터 검증 확인
				if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
					debug->Write("데이터 검증 OK");
				else
					debug->Write("데이터 검증 NO");
				iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
				//iocp->fnWSARecv(socketinfo->socklist.hSock);

			}
		}
		//======================================================//
		//	사용 내역 체크 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
		{
			debug->Write("사용내역");
			//보내는 쪽 코드를 심는다(VM 쪽으로)
			bsdt->dwBaseCode|=RECVCODE_VMCLIENTSEND;
			//======================================================//
			//받은 데이터 검증
			//Recv 데이터 축출
			//======================================================//
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[받은 데이터] code:0x%08x,카드넘버:%s,데이터길이:%d,데이타:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//체크 값이 0이 되는가 확인한다(체크값이 0이 되어야 정상)
			//데이터 검증 확인
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("데이터 검증 OK");
			else
				debug->Write("데이터 검증 NO");
			//======================================================//
			//	사용내역 조회를 위해서 스래드풀을 하나 빌리고
			//  오브젝트를 생성해서 넣는다
			//=======================================================//
			IThreadPool* thdpool = obj->GetThreadPool();
			IThreadObjectBase* thpoolobj = new CDbDataSendPoolObj;
			thpoolobj->initThreadObject(obj);
			thpoolobj->SetDbDataInfo(socklist->hSock, bsdt->szCardNum);
			thdpool->Add(thpoolobj);
		}
	}
}
//--------------------------------------------------------------
//|
//| 받은 데이터 처리 함수
//|
//---------------------------------------------------------------
void CServerMg::SendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
{
	//--------------------------------------------------------------
	//|전송이 다 되었는지 확인한다. 다 전송되지 않았으면 아직 전송되지
	//|않은 데이터를 다시 보낸다. 다 전송되었으면 WSArecv를 호출해서 
	//|다시 받기 모드로 진입한다
	//---------------------------------------------------------------
	//======================================================//
	//	보낸 정보확인 	
	//=======================================================//
	SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
	sendinfo->nSendBytes+=dwTransfer;
	if(sendinfo->nTotalSendBytes>sendinfo->nSendBytes)  //다 전송되지 않았으면
	{
		//보낼 데이터가 있으면 보낸다
		debug->Write("보낼 데이터가 아직 남았다");
		iocp->fnWSASend(_info->socklist.hSock, _info->data+sendinfo->nSendBytes, sendinfo->nTotalSendBytes-sendinfo->nSendBytes);
		return;
	}
	else  // 데이터가 전부 전송된 경우
	{
		baseDATA20* bsdt =(baseDATA20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	마이비로 데이터 전송 후 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_MYBISERVERSEND)
		{
			//======================================================//
			//	카드 Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{

			}
			//======================================================//
			//	사용자 변경(권종 변경) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{

			}
			//======================================================//
			//	소득공제 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{

			}
			//======================================================//
			//	사용 내역 체크 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
			{

			}
		}
		//======================================================//
		//	VM으로 데이터 전송 후 	
		//=======================================================//
		else if(bsdt->dwBaseCode & RECVCODE_VMCLIENTSEND)
		{
			//======================================================//
			//	카드 Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{

			}
			//======================================================//
			//	사용자 변경(권종 변경) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{

			}
			//======================================================//
			//	소득공제 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{

			}
			//======================================================//
			//	사용 내역 체크 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("보내기 이벤트");
					}
				}
				else
				{
					SetEvent(sendinfo->hSendEvent);
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("오버랩 받기 상태");
				}
			}

		}
	}
}
//--------------------------------------------------------------
//|
//| CompletionPort 대기 함수(서버 Accept 함수)
//|
//---------------------------------------------------------------
DWORD WINAPI CServerMg::ProcessServer(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServer *iocp = obj->GetIocpServer();
	IServerMg* my = obj->GetServerMg();
	HANDLE  hResult;
	while(1)
	{
		hResult = iocp->CompletionPortServerSocket(my->GetCompletionPort());
		if(hResult == NULL)
		{
			return 0;
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| 서버 컴포트
//|
//---------------------------------------------------------------
HANDLE CServerMg::GetCompletionPort()
{
	return m_hIocp;
}
//--------------------------------------------------------------
//|
//| 클라이언트 컴포트
//|
//---------------------------------------------------------------
HANDLE CServerMg::GetClntCompletionPort()
{
	return m_hclntIocp;
}
//--------------------------------------------------------------
//|
//| 클라이언트 컴포트를 넘긴다
//|
//---------------------------------------------------------------
SOCKET CServerMg::MakeCompetionPortClient(SOCKET hSock)
{
	return iocp->CompletionPortClientSocket(GetClntCompletionPort(), hSock);
}