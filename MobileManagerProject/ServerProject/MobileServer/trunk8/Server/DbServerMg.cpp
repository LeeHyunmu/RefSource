
#include "stdAfx.h"
#include "DbServerMg.h"
//=====================================================
// 스래드 생성 개수
//======================================================
#define  IOCP_DbSERVERCOUNT 1 // IOCP_DbSERVERCOUNT 개수
#define  IOCP_DbCLIENTCOUNT 1 // IOCP_DbCLIENTCOUNT 개수
//=====================================================
// Mybi 서버 IP, Port
//======================================================
#define  DBSERVER_IP "127.0.0.1"
#define  DBSERVER_PORT 9102
//=====================================================
// MS SQL ADO 연동 스트링(Database=Mobile에 접속)
//======================================================
#define DBCONNECT_STR  _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763")

//--------------------------------------------------------------
//|
//|Recv Send Code
//|
//---------------------------------------------------------------
//MYBI가 클라이언트로 붙을 때
//데이터 수신
#define MYBIDATABASECOMM_DATARECV              0x00000001
//데이터 요청
#define MYBIDATABASECOMM_DATASEND              0x00000002
//확인 요청
#define MYBIDATABASECOMM_IDENTIFYRECV          0x00000004
//확인 요청 처리
#define MYBIDATABASECOMM_IDENTIFYSEND          0x00000008

//관리서버에서 마이비로 요청이 필요할 때
//확인 요청
#define MOBILEDATABASECOMM_IDENTIFYRECV          0x00000040
//확인 요청 처리
#define MOBILEDATABASECOMM_IDENTIFYSEND          0x00000080

//에러
//CODE가 에러가 났다
#define DATABASECOMM_ERROR    	   	      0x10000000


//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDbServerMg::CDbServerMg():
obj(NULL),
debug(NULL),
mdbmg(NULL),
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
	//받은 데이터 검증
	//데이터 검증 확인
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		debug->Write("데이터 검증 OK");
		//기본 데이터
		debug->Write("[기본 데이터] code:0x%08x,테이블명:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->TableName,bsdt->dwdatalen);
	}
	else
	{
		debug->Write("데이터 검증 NO");// return;
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
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode=0;
		//사용내역 조회 받을 구조체 연결
		DBdata162*    recvdata=(DBdata162*)bsdt->data;
		if(recvdata->dwcode)
		{
			debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
						recvdata->dwcode, recvdata->dwusemoney, recvdata->usename, recvdata->usecardnum,
						recvdata->useplace, recvdata->usetime, recvdata->useadress);
		    BOOL b = mdbmg->ActionQuery(insertquery,"values('%s','%s', %d, '%s', '%s', '%s')",recvdata->usename, recvdata->usecardnum, recvdata->dwusemoney,
				recvdata->useplace, recvdata->useadress, recvdata->usetime);
		}
		else
		{
			debug->Write("데이터 전송 완료 코드 받았다");
			mdbmg->CommitTn();
		}

		debug->Write("데이터를 잘 받았다");
		debug->Write("다음 데이터 요청");
		//======================================================//
		//	데이터 요청하기위해 연결
		//=======================================================//
		DBdata36* senddata=(DBdata36*)bsdt->data;
		//======================================================//
		//	메지시 기록	
		//=======================================================//
		CHAR szSenddata[]="데이터 양호";
		memcpy_s(senddata->info,sizeof(senddata->info),szSenddata, sizeof(szSenddata));
		//크기를 구한다
		bsdt->dwdatalen=sizeof(baseDB20)+sizeof(DBdata36)-2; // 보낼 구조체 크기
		//======================================================//
		//	다음 데이터 요청 코드	
		//=======================================================//
		bsdt->dwBaseCode|=MYBIDATABASECOMM_DATASEND;
		//======================================================//
		//	보내기 전에 정보를 백업한다(오버랩 상태에서 쓰일 정보)
		//=======================================================//
		//보내는 정보를 기록하기 위한 구조체
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
		ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
		//보내야 할 데이터 크기를 저정한다
		//보낼 총 데이터 크기
		sendinfo->nTotalSendBytes=bsdt->dwdatalen;
		//보내고 남은 데이터(초기 세팅에서 0으로)
		sendinfo->nSendBytes=0;
		//보내고 다시 보낼 데이터가 있는가?(없으면 FALSE)
		sendinfo->bafterSenddata=FALSE;
		//======================================================//
		//	검증 데이터 입력
		//=======================================================//
		crc32->GetResult(_info->data,bsdt->dwdatalen-4);
		//데이터 검증 확인
		if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
			debug->Write("데이터 검증 OK");
		else
			debug->Write("데이터 검증 NO");
		//보내온 곳으로 다시 보낸다
		iocp->fnWSASend(socklist->hSock, _info->data, bsdt->dwdatalen);

	}
	//======================================================//
	//	데이터 이외 요청 사항이 있을 때	
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYRECV)
	{

	}
	//======================================================//
	//	알 수 없는 요청 사항일 때
	//=======================================================//
	else
	{
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
		//크기를 구한다
		bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DBError36)-2; // 보낼 구조체 크기
		//======================================================//
		//	에러코드 어디서 온 데이터냐? 보낸곳이 어디냐?	
		//=======================================================//
		bsdt->dwBaseCode|=DATABASECOMM_ERROR;
		//======================================================//
		//	보내기 전에 정보를 백업한다(오버랩 상태에서 쓰일 정보)
		//=======================================================//
		//보내는 정보를 기록하기 위한 구조체
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
		ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
		//보내야 할 데이터 크기를 저정한다
		//보낼 총 데이터 크기
		sendinfo->nTotalSendBytes=bsdt->dwdatalen;
		//보내고 남은 데이터(초기 세팅에서 0으로)
		sendinfo->nSendBytes=0;
		//보내고 다시 보낼 데이터가 있는가?(없으면 FALSE)
		sendinfo->bafterSenddata=FALSE;
		//======================================================//
		//	검증 데이터 입력
		//=======================================================//
		crc32->GetResult(_info->data,bsdt->dwdatalen-4);
		//데이터 검증 확인
		if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
			debug->Write("데이터 검증 OK");
		else
			debug->Write("데이터 검증 NO");
		//보내온 곳으로 다시 보낸다
		iocp->fnWSASend(socklist->hSock, _info->data, bsdt->dwdatalen);
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
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("오버랩 받기 상태");
		}
		//======================================================//
		//	요청 사항에 대해서 처리 후 송신	
		//=======================================================//
		else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYSEND)
		{
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
	//받은 데이터 검증
	//데이터 검증 확인
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		debug->Write("데이터 검증 OK");
		//기본 데이터
		debug->Write("[기본 데이터] code:0x%08x,테이블명:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->TableName,bsdt->dwdatalen);
	}
	else
	{
		debug->Write("데이터 검증 NO");// return;
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
		//크기를 구한다
		bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DBError36)-2; // 보낼 구조체 크기
		//======================================================//
		//	에러코드 어디서 온 데이터냐? 보낸곳이 어디냐?	
		//=======================================================//
		bsdt->dwBaseCode|=DATABASECOMM_ERROR;
		//======================================================//
		//	보내기 전에 정보를 백업한다(오버랩 상태에서 쓰일 정보)
		//=======================================================//
		//보내는 정보를 기록하기 위한 구조체
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
		ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
		//보내야 할 데이터 크기를 저정한다
		//보낼 총 데이터 크기
		sendinfo->nTotalSendBytes=bsdt->dwdatalen;
		//보내고 남은 데이터(초기 세팅에서 0으로)
		sendinfo->nSendBytes=0;
		//보내고 다시 보낼 데이터가 있는가?(없으면 FALSE)
		sendinfo->bafterSenddata=FALSE;
		//======================================================//
		//	검증 데이터 입력
		//=======================================================//
		crc32->GetResult(_info->data,bsdt->dwdatalen-4);
		//데이터 검증 확인
		if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
			debug->Write("데이터 검증 OK");
		else
			debug->Write("데이터 검증 NO");
		//보내온 곳으로 다시 보낸다
		iocp->fnWSASend(socklist->hSock, _info->data, bsdt->dwdatalen);
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
 #define  TABLENAMEFIND "select 'Name' = o.name,'Owner' = user_name(uid),'Object_type' = substring(v.name,5,31) \
 from sysobjects o, master.dbo.spt_values v where o.xtype = substring(v.name,1,2) and v.type = 'O9T'\
 and v.name like 'U%' order by Object_type desc, Name asc"
// record->GetFieldValue("NAME", name);
// record->GetFieldValue("Owner", owner);
// record->GetFieldValue("Object_type", object_type);

SOCKET CDbServerMg::MakeCompetionPortServer()
{
	SOCKET hResult = iocp->CompletionPortServerSocket(GetCompletionPort());
	COleDateTime datetime = COleDateTime::GetCurrentTime();
	memset(tablename,0,sizeof(tablename));
	sprintf_s(tablename,_T("traffic_%d%02d%02d"),  datetime.GetYear(), datetime.GetMonth(),	datetime.GetDay());

	IADORecordset* record = mdbmg->GetDbRecordset(5, TABLENAMEFIND,"");
	register int nPageIndex = 1;
	BOOL bIstable=FALSE;
	if(record)
	{
		int pagecount=record->GetPageCount();
		while(nPageIndex<=pagecount)
		{
			record->SetAbsolutePage(nPageIndex++);
			int pagesize = record->GetPageSize();
			for(register int nRecNumber = 0; nRecNumber < pagesize;nRecNumber++)
			{
				CString name, owner, object_type;
				record->GetFieldValue("NAME", name);
				record->GetFieldValue("Owner", owner);
				record->GetFieldValue("Object_type", object_type);
				name.TrimRight();
				owner.TrimRight();
				object_type.TrimRight();
				debug->Write("테이블 확인 => name:%s, owner:%s, object_type:%s", name, owner, object_type);
				if(name.CompareNoCase(tablename)==0)
				{
					bIstable=TRUE;
					break;
				}
				record->MoveNext();
				BOOL bEof = record->IsEof();
				if(bEof)
					break;
			}
			if(bIstable)
				break;
		}
		//생성한 레코드 포인터를 해제한다
		if(obj)
			obj->ReleaseObject(record);
	}
	// 데이블 추가 생성
	if(bIstable==FALSE)
		mdbmg->Trans("select* into ","%s from trafficinfo_base", tablename);
	memset(insertquery,0,sizeof(insertquery));
	sprintf_s(insertquery,_T("insert into %s (usename,usecardnum,usemoney,useplace,useadress,usetime)"), tablename);
	mdbmg->BeginTn();
	return hResult;
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