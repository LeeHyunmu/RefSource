
#include "stdAfx.h"
#include "DbServerMgCode.h"
#include "DbServerMg.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDbServerMg::CDbServerMg():
obj(NULL),
debug(NULL),
mdbmg(NULL),
adorecord(NULL),
iocp(NULL),
m_hIocp(NULL),
m_hclntIocp(NULL),
mthread(NULL),
bEndServer(FALSE),
bServerStart(FALSE)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CDbServerMg::~CDbServerMg()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CDbServerMg::initServerMg(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutput();
	//=====================================================
	// IOCP  객체를 서버 클래스에서 생성한다
	//======================================================
	iocp=obj->CreateIocpServer();
	if(iocp==NULL)
	{
		debug->Write("IOCP 서버 생성 실패");
	}
	//=====================================================
	// DB 연결 메니저 오브젝을 서버 클래스에서 생성한다
	//======================================================
	mdbmg = obj->CreateDatabaseMg();
	if(mdbmg==NULL)
	{
		debug->Write("DB 메니저 생성 실패");
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CDbServerMg::Release()
{
	//=====================================================
	// 스래드 종료 플래그
	//======================================================
	bEndServer=TRUE;
	//=====================================================
	// IOCP 핸들
	//======================================================
	if(m_hIocp)
	{
		for (DWORD i = 0; i<IOCP_DbSERVERCOUNT; i++)
			::PostQueuedCompletionStatus(m_hIocp, 0, 0, (OVERLAPPED*)NULL);
		WaitForSingleObject(m_hIocp, INFINITE);
		CloseHandle(m_hIocp); m_hIocp=NULL;
	}
	if(m_hclntIocp)
	{
		for (DWORD i = 0; i<IOCP_DbCLIENTCOUNT; i++)
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
	//=====================================================
	// IOCP  객체 메모리 제거
	//======================================================
	obj->ReleaseObject(iocp);
	//=====================================================
	// DB 생셩  객체 메모리제거
	//======================================================
	obj->ReleaseObject(mdbmg);
	bServerStart=FALSE;
}
//--------------------------------------------------------------
//|
//|서버 종료
//|
//---------------------------------------------------------------
BOOL CDbServerMg::GetEndServer()
{
	return bEndServer;
}
//--------------------------------------------------------------
//|
//| 서버를 생성한다
//|
//---------------------------------------------------------------
BOOL CDbServerMg::ServerStart(u_short nPort)
{
	if(bServerStart)
	{
		debug->Write("이미 DB송수신 서버가 시작되어있다");
		return FALSE;
	}
	if((mdbmg==NULL)||(iocp==NULL))
	{
		debug->Write("서버 시작 실패");
		return FALSE;
	}
	//=====================================================
	// DB 연동
	//======================================================
	if(mdbmg->DbConnect(DBCONNECT_STR)==FALSE)
	{
		MessageBox(NULL, "DB연결","데이터베이스 연결에러",MB_OK);
		return FALSE;
	}
	if(nPort < 5000 || nPort > 10000)
	{
		MessageBox(NULL, "포트 번호 에러(5001 - 9999)", "오류", MB_ICONINFORMATION);
		return FALSE;
	}
	if(iocp->ServerListen(nPort)==FALSE)
		return FALSE;
	//=====================================================
	// 서버용 IOCP 핸들 생성한다
	//======================================================
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIocp == NULL )
	{
		MessageBox(NULL, "IOCP 생성 실패", "알림", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}
	//=====================================================
	// 클라이언트용 IOCP 핸들 생성한다
	//======================================================
	m_hclntIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hclntIocp == NULL )
	{
		MessageBox(NULL, "IOCP 생성 실패", "알림", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}
	//=====================================================
	// IOCP Server 처리 스레드를 생성한다.
	//======================================================
	for(DWORD i = 0 ; i < IOCP_DbSERVERCOUNT; i ++ )
	{
		HANDLE hThread=NULL;
		DWORD dwThreadId1;
		hThread = CreateThread(NULL, 0, WorkerThread, (LPVOID)obj, 0, &dwThreadId1);
		if(hThread != NULL)
		{
			workerthread.push_back(hThread);
		}
	}
	//=====================================================
	// IOCP Server에서 클라이언트 접속을 받기 위한 스래드 생성
	//======================================================
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);
	//=====================================================
	// IOCP Client 처리 스레드를 생성한다.
	//======================================================
	for(DWORD i = 0 ; i < IOCP_DbCLIENTCOUNT; i ++ )
	{
		HANDLE hThread=NULL;
		DWORD dwThreadId1;
		hThread = CreateThread(NULL, 0, WorkerClntThread, (LPVOID)obj, 0, &dwThreadId1);
		if(hThread != NULL)
		{
			workerthread.push_back(hThread);
		}
	}
	debug->Write("DB 서버시작");
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CDbServerMg::ServerStop()
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
//| IOCP Server Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CDbServerMg::WorkerThread(LPVOID lpArg)
{
	//======================================================//
	//	스래드 함수에서 필요한 클래스 포인터를 연결한다	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* dbservermg = obj->GetDbServerMg();
	//======================================================//
	//	IOCP Client 용 핸들
	//=======================================================//
	HANDLE hIocp=dbservermg->GetCompletionPort();

	SOCKET      client_socket;
	SOCKETINFO*  _info=NULL;
	int         iRetval;
	DWORD       dwTransfer;
	while(1)
	{
		iRetval = GetQueuedCompletionStatus(hIocp, &dwTransfer, (LPDWORD)&client_socket, (LPOVERLAPPED*)&_info, INFINITE );
		//======================================================//
		//	종료처리	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(dbservermg->GetEndServer())
			return 0;
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//연결이 끊겼을 때 처리 함수
			dbservermg->DisConnectHandling(_info,0);
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
			dbservermg->IocpServerRecvHandling(_info,0);
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
			dbservermg->IocpServerSendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| IOCP Client Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CDbServerMg::WorkerClntThread(LPVOID lpArg)
{
	//======================================================//
	//	스래드 함수에서 필요한 클래스 포인터를 연결한다	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* dbservermg = obj->GetDbServerMg();
	//======================================================//
	//	IOCP Client 용 핸들
	//=======================================================//
	HANDLE hIocp=dbservermg->GetClntCompletionPort();

	SOCKET      client_socket;
	SOCKETINFO*  _info=NULL;
	int         iRetval;
	DWORD       dwTransfer;
	while(1)
	{
		iRetval = GetQueuedCompletionStatus(hIocp, &dwTransfer, (LPDWORD)&client_socket, (LPOVERLAPPED*)&_info, INFINITE );
		//======================================================//
		//	종료처리	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(dbservermg->GetEndServer())
			return 0;
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//연결이 끊겼을 때 처리 함수
			dbservermg->DisConnectHandling(_info,0);
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
			dbservermg->IocpClientRecvHandling(_info,0);
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
			dbservermg->IocpClientSendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| 소켓 연결이 끊겼을 때 처리	
//|
//---------------------------------------------------------------
BOOL CDbServerMg::DisConnectHandling(SOCKETINFO* _info, DWORD flag)
{
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	SOCKETINFO*  socketinfo;
	if(iocp->fnGetSockInfo(&socketinfo, socklist->hSock))
	{
		//======================================================//
		//	소켓 정보 출력
		//=======================================================//
		Sockinfodisplay(socklist->hSock);
		//======================================================//
		//	소켓 정보 삭제
		//=======================================================//
		iocp->fnRemoveSocketInfo(socklist->hSock);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 출력
//|
//---------------------------------------------------------------
void CDbServerMg::Sockinfodisplay(SOCKET _sock)
{
	CString    strIP="";
	u_short		port=0;
	int         iAddrLen;
	SOCKADDR_IN  sockAddr;
	iAddrLen = sizeof(sockAddr);
	getpeername(_sock, (SOCKADDR *)&sockAddr, &iAddrLen);
	strIP = inet_ntoa(sockAddr.sin_addr);
	port = ntohs(sockAddr.sin_port);
	debug->Write("IP:[%s] PORT:[%d]", strIP, port);
}
//--------------------------------------------------------------
//|
//| 받은 데이터 처리 함수
//|
//---------------------------------------------------------------
void CDbServerMg::IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	데이터 검증용 클래스 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	데이터 분류 	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) _info->data;
	//======================================================//
	//	받은 데이터 검증 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//기본 데이터
		debug->Write("[기본 데이터] code:0x%08x,테이블명:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->TableName,bsdt->dwdatalen);
	}
	//======================================================//
	//	소켓 리스트	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	if(socklist->hSock==0)
	{
		debug->Write("통신 소켓이 없다");
		return;
	}
	//======================================================//
	//	보낸 데이터에 에러가 있다는 코드를 보내왔을 때	
	//=======================================================//
	if((bsdt->dwBaseCode & DATABASECOMM_ERROR))
	{
		debug->Write("에러 상황 판단해서 처리");
		iocp->fnWSARecv(socklist->hSock);
		return;
	}
	//======================================================//
	//	받은 소켓 정보 출력 	
	//=======================================================//
	Sockinfodisplay(socklist->hSock);
	//======================================================//
	//	데이터 베이스 데이터를 수신	
	//=======================================================//
	if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATARECV)
	{
		debug->Write("code:%d",bsdt->dwBaseCode);
		//======================================================//
		//	전송 코드 빼기	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_DATARECV;
		if(adorecord)
		{
			//사용내역 조회 받을 구조체 연결
			DBdata162* recvdata=(DBdata162*)bsdt->data;
			if(recvdata->dwcode)
			{
				debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
					recvdata->dwcode, recvdata->dwusemoney, recvdata->usename, recvdata->usecardnum,
					recvdata->useplace, recvdata->usetime, recvdata->useadress);
				CString usename = recvdata->usename;
				int usemoney = recvdata->dwusemoney;
				CString usecardnum = recvdata->usecardnum;
				CString useplace = recvdata->useplace;
				CString useadress = recvdata->useadress;
				adorecord->AddNew();
				adorecord->SetFieldValue("usename",(CString)recvdata->usename);
				adorecord->SetFieldValue("usemoney", (int)recvdata->dwusemoney);
				adorecord->SetFieldValue("usecardnum", (CString)recvdata->usecardnum);
				adorecord->SetFieldValue("useplace", (CString)recvdata->useplace);
				adorecord->SetFieldValue("useadress", (CString)recvdata->useadress);
				adorecord->SetFieldValue("usetime", (CString)recvdata->usetime);
			}
			else
			{
					adorecord->Update();
				debug->Write("데이터 전송 완료 코드 받았다");
			}
			debug->Write("데이터를 잘 받았다");
			debug->Write("다음 데이터 요청");
		}
		//======================================================//
		//	데이터 요청하기위해 연결
		//=======================================================//
		DBdata36* senddata=(DBdata36*)bsdt->data;
		//======================================================//
		//	메지시 기록	
		//=======================================================//
		CHAR szSenddata[]="데이터 양호";
		memcpy_s(senddata->info,sizeof(senddata->info),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	요청 코드 보내기 함수(함수로 복잡도를 제거)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDB20)+sizeof(DBdata36)-2, MYBIDATABASECOMM_DATASEND);
	}
	//======================================================//
	//	데이터 이외 요청 사항이 있을 때	
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYRECV)
	{
		//======================================================//
		//	전송 코드 빼기	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_IDENTIFYRECV;
		debug->Write("code:%d",bsdt->dwBaseCode);
	}
	//======================================================//
	//	데이터 이외 요청 사항이 있을 때	
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_STARTDATARECV)
	{
		//======================================================//
		//	전송 코드 빼기	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_STARTDATARECV;
		debug->Write("code:%d",bsdt->dwBaseCode);
	}
	//======================================================//
	//	알 수 없는 요청 사항일 때
	//=======================================================//
	else
	{
		debug->Write("code:%d",bsdt->dwBaseCode);
		debug->Write("요청 사항을 알 수 없다");
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode=0;
		//======================================================//
		//	에러 데이터 보내기 위해 연결
		//=======================================================//
		DBError36* senderr=(DBError36*)bsdt->data;
		//======================================================//
		//	에러 메시지 기록	
		//=======================================================//
		CHAR szSenddata[]="무슨 요청인가?";
		memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	요청 코드 보내기 함수(함수로 복잡도를 제거)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDB20)+sizeof(DBError36)-2, DATABASECOMM_ERROR);
	}
}
//--------------------------------------------------------------
//|
//| 보내기 오버랩 후 처리 부분
//|
//---------------------------------------------------------------
void CDbServerMg::IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
	if(sendinfo->nTotalSendBytes==0)
	{
		debug->Write("보낸 데이터가 0이다");
		return;
	}
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
		//======================================================//
		//	보내는 정보가 정상적으로 보내졌으면 정보를 초기화 해서 다시 들어오지 않게 한다 	
		//=======================================================//
		sendinfo->nTotalSendBytes=0;
		sendinfo->nSendBytes=0;
		//======================================================//
		//	보낸 데이터 정보에 구조체를 연결한다	
		//=======================================================//
		baseDB20* bsdt =(baseDB20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	데이터 베이스 데이터를 정상적으로 수신했을 때 	
		//=======================================================//
		if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATASEND)
		{
			//======================================================//
			//	보낸 코드를 삭제	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_DATASEND;
			//======================================================//
			//	받기 대기 상태	
			//=======================================================//
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("오버랩 받기 상태");
		}
		//======================================================//
		//	받을 준비가 되었다는 메시지를 보냈을 때
		//=======================================================//
		else if(bsdt->dwBaseCode & MYBIDATABASECOMM_STARTDATARECV)
		{
			//======================================================//
			//	보낸 코드를 삭제	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_STARTDATARECV;
			//======================================================//
			//	받기 대기 상태	
			//=======================================================//
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("오버랩 받기 상태");
		}
		//======================================================//
		//	요청 사항에 대해서 처리 후 송신	
		//=======================================================//
		else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYSEND)
		{
			//======================================================//
			//	보낸 코드를 삭제	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_IDENTIFYSEND;
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("오버랩 받기 상태");
		}
		//======================================================//
		//	에러를 송신 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & DATABASECOMM_ERROR)
			{
				//======================================================//
				//	보낸 코드를 삭제	
				//=======================================================//
				bsdt->dwBaseCode=DATABASECOMM_ERROR;
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("보낸 곳 에러코드");
				debug->Write("오버랩 받기 상태");
			}
		}
	}
}
//--------------------------------------------------------------
//|
//| 받은 데이터 처리 함수
//|
//---------------------------------------------------------------
void CDbServerMg::IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	데이터 검증용 클래스 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	데이터 분류 	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) _info->data;
	//======================================================//
	//	받은 데이터 검증 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//기본 데이터
		debug->Write("[기본 데이터] code:0x%08x,테이블명:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->TableName,bsdt->dwdatalen);
	}
	//======================================================//
	//	소켓 리스트	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	if(socklist->hSock==0)
	{
		debug->Write("통신 소켓이 없다");
		return;
	}
	//======================================================//
	//	관리서버에서 MYBI 서버에 요청사항이 있을 때	
	//=======================================================//
	if((bsdt->dwBaseCode & DATABASECOMM_ERROR))
	{
		debug->Write("에러 상황 판단해서 처리");
		iocp->fnWSARecv(socklist->hSock);
		return;
	}
	//======================================================//
	//	받은 소켓 정보 출력 	
	//=======================================================//
	Sockinfodisplay(socklist->hSock);
	//======================================================//
	//	관리서버에서 MYBI 서버에 요청사항에 대한 응답
	//=======================================================//
	if(bsdt->dwBaseCode & MOBILEDATABASECOMM_IDENTIFYRECV)
	{
		//======================================================//
		//	보낸 코드를 삭제	
		//=======================================================//
		bsdt->dwBaseCode^=MOBILEDATABASECOMM_IDENTIFYRECV;
	}
	//======================================================//
	//	응답 코드를 알 수 없다
	//=======================================================//
	else
	{
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode=0;
		debug->Write("요청에 대한 처리 코드를 알 수 없다");
		//======================================================//
		//	에러 데이터 보내기 위해 연결
		//=======================================================//
		DBError36* senderr=(DBError36*)bsdt->data;
		//======================================================//
		//	에러 메시지 기록	
		//=======================================================//
		CHAR szSenddata[]="무슨 요청인가?";
		memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	요청 코드 보내기 함수(함수로 복잡도를 제거)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDB20)+sizeof(DBError36)-2, DATABASECOMM_ERROR);
	}
}
//--------------------------------------------------------------
//|
//| 보내기 오버랩 후 처리 부분
//|
//---------------------------------------------------------------
void CDbServerMg::IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
	if(sendinfo->nTotalSendBytes==0)
	{
		return;
	}
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
		//======================================================//
		//	보내는 정보가 정상적으로 보내졌으면 정보를 초기화 해서 다시 들어오지 않게 한다 	
		//=======================================================//
		sendinfo->nTotalSendBytes=0;
		sendinfo->nSendBytes=0;
		//======================================================//
		//	보낸 데이터 정보에 구조체를 연결한다	
		//=======================================================//
		baseDB20* bsdt =(baseDB20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	뭔가를 요청 했을 때	
		//=======================================================//
		if(bsdt->dwBaseCode & MOBILEDATABASECOMM_IDENTIFYSEND)
		{
			//======================================================//
			//	보낸 코드를 삭제	
			//=======================================================//
			bsdt->dwBaseCode^=MOBILEDATABASECOMM_IDENTIFYSEND;
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("오버랩 받기 상태");
		}
		//======================================================//
		//	에러 메시지 전송했을 경우 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & DATABASECOMM_ERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("보낸 곳 에러코드");
				debug->Write("오버랩 받기 상태");

			}
			//======================================================//
			//	보낸 코드를 삭제	
			//=======================================================//
			bsdt->dwBaseCode=0;
		}
	}
}
//--------------------------------------------------------------
//|
//| CompletionPort 대기 스래드 함수
//|
//---------------------------------------------------------------
DWORD WINAPI CDbServerMg::ProcessServer(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServerMg* dbservermg = obj->GetDbServerMg();
	SOCKET  hResult;
	while(1)
	{
		hResult = dbservermg->MakeCompetionPortServer();
		if(hResult == 0)
		{
			return 0;
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| CompletionPort 대기 함수(서버 Accept 함수)
//|
//---------------------------------------------------------------
//데이블이 있는가를 확인하는 쿼리
 #define  TABLENAMEFIND "select 'Name' = o.name,'Owner' = user_name(uid),'Object_type' = substring(v.name,5,31) \
 from sysobjects o, master.dbo.spt_values v where o.xtype = substring(v.name,1,2) and v.type = 'O9T'\
 and v.name like 'U%' order by Object_type desc, Name asc"
SOCKET CDbServerMg::MakeCompetionPortServer()
{
	//======================================================//
	// 클라이언트 접속 및 정보를 기록하는 함수	
	//=======================================================//
	SOCKET hResult = iocp->CompletionPortServerSocket(GetCompletionPort());
	//======================================================//
	// 소켓 정보 포인터를 얻는다
	//=======================================================//
	SOCKETINFO* info;
	iocp->fnGetSockInfo(&info,hResult);
	//======================================================//
	//	데이터 분류 	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) info->data;
	//======================================================//
	//	소켓 리스트	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&info->socklist;
	//======================================================//
	//	테이블 생성(날짜별 생성) 이름 얻기 위해서 
	//=======================================================//
	COleDateTime datetime = COleDateTime::GetCurrentTime();
	memset(tablename,0,sizeof(tablename));
	sprintf_s(tablename,_T("traffic_%d%02d%02d"),  datetime.GetYear(), datetime.GetMonth(),	datetime.GetDay());
	//======================================================//
	//	테이블을 검색해서 같은 이름이 있는가 알아본다 
	//=======================================================//
	adorecord=NULL;
	if(mdbmg->RecordsetSetting(5, TABLENAMEFIND,""))
		adorecord = mdbmg->GetRecordsetPoint();
	register int nPageIndex = 1;
	BOOL bIstable=FALSE;
	if(adorecord)
	{
		int pagecount=adorecord->GetPageCount();
		while(nPageIndex<=pagecount)
		{
			adorecord->SetAbsolutePage(nPageIndex++);
			int pagesize = adorecord->GetPageSize();
			for(register int nRecNumber = 0; nRecNumber < pagesize;nRecNumber++)
			{
				CString name, owner, object_type;
				adorecord->GetFieldValue("NAME", name);
				adorecord->GetFieldValue("Owner", owner);
				adorecord->GetFieldValue("Object_type", object_type);
				name.TrimRight();
				owner.TrimRight();
				object_type.TrimRight();
				debug->Write("테이블 확인 => name:%s, owner:%s, object_type:%s", name, owner, object_type);
				if(name.CompareNoCase(tablename)==0)
				{
					bIstable=TRUE;
					//break;
				}
				adorecord->MoveNext();
				BOOL bEof = adorecord->IsEof();
				if(bEof)
					break;
			}
			if(bIstable)
				break;
		}
	}
	// 같은 테이블 이름이 없으면 추가 생성
	if(bIstable==FALSE)
		mdbmg->Trans("select* into ","%s from trafficinfo_base", tablename);
	//======================================================//
	//	테이블 레코드 설정
	//=======================================================//
	adorecord=NULL;
	if(mdbmg->RecordsetSetting(tablename))
		adorecord = mdbmg->GetRecordsetPoint();

	debug->Write("데이터 요청");
	//======================================================//
	//	데이터 요청하기위해 연결
	//=======================================================//
	DBdata36* senddata=(DBdata36*)bsdt->data;
	//======================================================//
	//	메지시 기록	
	//=======================================================//
	CHAR szSenddata[]="수신 준비 완료";
	memcpy_s(senddata->info,sizeof(senddata->info),szSenddata, sizeof(szSenddata));
	//======================================================//
	//	요청 코드 보내기 함수(함수로 복잡도를 제거)
	//=======================================================//
	RequestSendData(info,sizeof(baseDB20)+sizeof(DBdata36)-2, MYBIDATABASECOMM_STARTDATARECV);
	return hResult;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CDbServerMg::RequestSendData(SOCKETINFO* info, DWORD totalstructlen, DWORD code, HANDLE hEvent, BOOL bafterdata)
{
	//======================================================//
	//	데이터 검증용 클래스 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	기본 데이터 클래스 연결	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) info->data;
	//======================================================//
	//	소켓 리스트	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&info->socklist;

	//======================================================//
	//	보낼 구조체 총 크기
	//=======================================================//
	bsdt->dwdatalen=totalstructlen; // 보낼 구조체 크기
	//======================================================//
	//	다음 데이터 요청 코드	| 연산
	//=======================================================//
	bsdt->dwBaseCode|=code;
	//======================================================//
	//	보내기 전에 정보를 백업한다(오버랩 상태에서 쓰일 정보)
	//=======================================================//
	//보내는 정보를 기록하기 위한 구조체
	SENDDATAINFO* sendinfo=(SENDDATAINFO*)&info->sendinfo;
	ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
	//보내야 할 데이터 크기를 저정한다
	//보낼 총 데이터 크기
	sendinfo->nTotalSendBytes=totalstructlen;
	//보내고 남은 데이터(초기 세팅에서 0으로)
	sendinfo->nSendBytes=0;
	//보내고 다시 보낼 데이터가 있는가?(없으면 FALSE)
	sendinfo->bafterSenddata=bafterdata;
	//이벤트 세팅
	sendinfo->hSendEvent=hEvent;
	//======================================================//
	//	검증 데이터 입력
	//=======================================================//
	crc32->SetCheckResult(info->data,bsdt->dwdatalen-4);
	//데이터 검증 확인
	crc32->GetCheck(info->data,bsdt->dwdatalen);
	//보내온 곳으로 다시 보낸다
	iocp->fnWSASend(socklist->hSock, info->data, bsdt->dwdatalen);
	return TRUE;
}

//--------------------------------------------------------------
//|
//| 서버 컴포트
//|
//---------------------------------------------------------------
HANDLE CDbServerMg::GetCompletionPort()
{
	return m_hIocp;
}
//--------------------------------------------------------------
//|
//| 클라이언트 컴포트
//|
//---------------------------------------------------------------
HANDLE CDbServerMg::GetClntCompletionPort()
{
	return m_hclntIocp;
}
//--------------------------------------------------------------
//|
//| IOCP 클라이언트 접속
//|
//---------------------------------------------------------------
SOCKET CDbServerMg::MakeCompetionPortClient(SOCKET hSock)
{
	return iocp->CompletionPortClientSocket(GetClntCompletionPort(), hSock,DBSERVER_IP, DBSERVER_PORT);
}
//--------------------------------------------------------------
//|
//|IOCP 멤버를 넘긴다
//|
//---------------------------------------------------------------
IServer* CDbServerMg::GetIocpServer()
{
	return iocp;
}
//--------------------------------------------------------------
//|
//|DatabasebMg 멤버를 넘긴다
//|
//---------------------------------------------------------------
IDatabasebMg* CDbServerMg::GetDatabasebMg()
{
	return mdbmg;
}