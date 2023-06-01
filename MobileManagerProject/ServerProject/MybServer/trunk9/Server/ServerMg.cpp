
#include "stdAfx.h"
#include "ServerMgCode.h"
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
m_hIocp(NULL),
mthread(NULL),
dwServerThreadCount(0),
bEndServer(FALSE),
bServerStart(FALSE)
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
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CServerMg::Release()
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
		for (DWORD i = 0; i<dwServerThreadCount; i++)
			::PostQueuedCompletionStatus(m_hIocp, 0, 0, (OVERLAPPED*)NULL);
		WaitForSingleObject(m_hIocp, INFINITE);
		CloseHandle(m_hIocp); m_hIocp=NULL;
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
	bServerStart=FALSE;
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
//| 서버를 생성한다
//|
//---------------------------------------------------------------
BOOL CServerMg::ServerStart(u_short nPort)
{
	if(bServerStart)
	{
		debug->Write("이미 서버가 시작되어있다");
		return TRUE;
	}
	if(iocp==NULL)
	{
		debug->Write("서버 시작 실패");
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
	// CPU 개수를 확인하여 스래드를 생성하기 위한 부분
	//======================================================
	SYSTEM_INFO si;
	GetSystemInfo( &si ); // CPU 개수 확인
	//=====================================================
	// IOCP Server 처리 스레드를 생성한다.
	//======================================================
	// CPU * IOCP_SERVERCOUNT 개의 작업 스레드 생성
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
	//=====================================================
	// IOCP Server에서 클라이언트 접속을 받기 위한 스래드 생성
	//======================================================
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);
	debug->Write("서버시작");
	bServerStart=TRUE;
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
	//=====================================================
	// IOCP 핸들
	//======================================================
	if(m_hIocp)
	{
		for (DWORD i = 0; i<dwServerThreadCount; i++)
			::PostQueuedCompletionStatus(m_hIocp, 0, 0, (OVERLAPPED*)NULL);
		WaitForSingleObject(m_hIocp, INFINITE);
		CloseHandle(m_hIocp); m_hIocp=NULL;
	}
	iocp->fnCloseSocket();
	debug->Write("서버정지");
	bServerStart=FALSE;
}
//--------------------------------------------------------------
//|
//| IOCP Server Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CServerMg::WorkerThread(LPVOID lpArg)
{
	//======================================================//
	//	스래드 함수에서 필요한 클래스 포인터를 연결한다	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* servermg = obj->GetServerMg();
	//======================================================//
	//	IOCP Client 용 핸들
	//=======================================================//
	HANDLE hIocp=servermg->GetCompletionPort();

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
		if(servermg->GetEndServer())
			return 0;
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//연결이 끊겼을 때 처리 함수
			servermg->DisConnectHandling(_info,0);
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
			servermg->IocpServerRecvHandling(_info,0);
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
			servermg->IocpServerSendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| IOCP Client Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CServerMg::WorkerClntThread(LPVOID lpArg)
{
	//======================================================//
	//	스래드 함수에서 필요한 클래스 포인터를 연결한다	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* servermg = obj->GetServerMg();
	//======================================================//
	//	IOCP Client 용 핸들
	//=======================================================//
	HANDLE hIocp=servermg->GetClntCompletionPort();

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
		if(servermg->GetEndServer())
			return 0;
		//======================================================//
		//	소켓 연결이 끊겼을 때 처리	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//연결이 끊겼을 때 처리 함수
			servermg->DisConnectHandling(_info,0);
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
			servermg->IocpClientRecvHandling(_info,0);
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
			servermg->IocpClientSendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| 소켓 연결이 끊겼을 때 처리	
//|
//---------------------------------------------------------------
BOOL CServerMg::DisConnectHandling(SOCKETINFO* _info, DWORD flag)
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
void CServerMg::Sockinfodisplay(SOCKET _sock)
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
void CServerMg::IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	데이터 검증용 클래스 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	데이터 분류 	
	//=======================================================//
	baseDATA20* bsdt =(baseDATA20*) _info->data;
	//======================================================//
	//	받은 데이터 검증 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//기본 데이터
		debug->Write("[기본 데이터] code:0x%08x,테이블명:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
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
	if((bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)||(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)||(bsdt->dwBaseCode & RECVCODE_DATAERROR)||(bsdt->dwBaseCode & RECVCODE_BASEERROR))
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
	//	VM에서 받은 데이터 처리 	
	//=======================================================//
	if(bsdt->dwBaseCode & RECVCODE_MYBISERVERSEND)
	{
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode^=RECVCODE_MYBISERVERSEND;
		if((bsdt->dwBaseCode & RECVCODE_CARDENABLE)||(bsdt->dwBaseCode & RECVCODE_USERCHANGE)||(bsdt->dwBaseCode & RECVCODE_DEDUCTION))
		{

			//======================================================//
			//	카드 Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{
				//받은 정보 출력
				DATA36* recvdata=(DATA36*)bsdt->data;
				//보내온 데이터
				debug->Write("[카드Enable 데이터] %s",recvdata->info);
				//보내온 데이터 내용을 바꾼다
				//======================================================//
				//	보내는 쪽 정보 저장소로 데이터를 변경하여 보낸다	
				//=======================================================//
				CHAR szSenddata[]="카드Enable 승인";
				memcpy_s(recvdata->info,sizeof(recvdata->info),szSenddata, sizeof(szSenddata));
				//======================================================//
				//	요청 코드 보내기 함수(함수로 복잡도를 제거)
				//=======================================================//
				RequestSendData(_info,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_MYBISERVERRECV);
			}
			//======================================================//
			//	사용자 변경(권종 변경) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{
				//받은 정보 출력
				DATA36* recvdata=(DATA36*)bsdt->data;
				//보내온 데이터
				debug->Write("[권종 변경 데이터] %s",recvdata->info);
				//보내온 데이터 내용을 바꾼다
				//======================================================//
				//	보내는 쪽 정보 저장소로 데이터를 변경하여 보낸다	
				//=======================================================//
				CHAR szSenddata[]="권종변경 승인";
				memcpy_s(recvdata->info,sizeof(recvdata->info),szSenddata, sizeof(szSenddata));
				//======================================================//
				//	요청 코드 보내기 함수(함수로 복잡도를 제거)
				//=======================================================//
				RequestSendData(_info,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_MYBISERVERRECV);
			}
			//======================================================//
			//	소득공제 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{
				//받은 정보 출력
				DATA36* recvdata=(DATA36*)bsdt->data;
				//보내온 데이터
				debug->Write("[소득공제 데이터] %s",recvdata->info);
				//보내온 데이터 내용을 바꾼다
				//======================================================//
				//	보내는 쪽 정보 저장소로 데이터를 변경하여 보낸다	
				//=======================================================//
				CHAR szSenddata[]="소득공제 등록완료";
				memcpy_s(recvdata->info,sizeof(recvdata->info),szSenddata, sizeof(szSenddata));
				//======================================================//
				//	요청 코드 보내기 함수(함수로 복잡도를 제거)
				//=======================================================//
				RequestSendData(_info,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_MYBISERVERRECV);
			}

		}
		else
		{

			//======================================================//
			//	알수 없는 코드를 받았을 때 	
			//=======================================================//
			debug->Write("데이터 처리 불분명");
			//======================================================//
			//	에러 데이터를 보내기 위해 연결 	
			//=======================================================//
			ERROR36* senddata=(ERROR36*)bsdt->data;
			//======================================================//
			//	에러 메시지를 기록한다	
			//=======================================================//
			CHAR szSenddata[]="데이터처리 코드 불분명";
			memcpy_s(senddata->err,sizeof(senddata->err),szSenddata, sizeof(szSenddata));
			//======================================================//
			//	요청 코드 보내기 함수(함수로 복잡도를 제거)
			//=======================================================//
			RequestSendData(_info,sizeof(baseDATA20)+sizeof(ERROR36)-2, RECVCODE_WHATDATAERROR);
		}
	}
	//======================================================//
	//		VM, MYBI 보내온 곳의 코드을 알수 없다
	//=======================================================//
	else
	{
		debug->Write("보낸 곳 코드 불분명 데이터 처리");
		//코드에서 보낸 쪽의 코드를 빼냄
		//다시 들어오지 못하도록 뺌
		bsdt->dwBaseCode=0;
		//======================================================//
		//	에러 데이터 보내기 위해 연결
		//=======================================================//
		ERROR36* senddata=(ERROR36*)bsdt->data;
		//======================================================//
		//	에러 메시지 기록	
		//=======================================================//
		CHAR szSenddata[]="보내온 곳 코드 불분명";
		memcpy_s(senddata->err,sizeof(senddata->err),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	요청 코드 보내기 함수(함수로 복잡도를 제거)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDATA20)+sizeof(ERROR36)-2, RECVCODE_WHERECORDERROR);
	}
}
//--------------------------------------------------------------
//|
//| 보내기 오버랩 후 처리 부분
//|
//---------------------------------------------------------------
void CServerMg::IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
		baseDATA20* bsdt =(baseDATA20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	관리서버로 데이터 전송 후 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_MYBISERVERRECV)
		{
			//======================================================//
			//	카드 Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("보내기 이벤트");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("오버랩 받기 상태");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("오버랩 받기 상태");
				}

			}
			//======================================================//
			//	사용자 변경(권종 변경) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("보내기 이벤트");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("오버랩 받기 상태");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("오버랩 받기 상태");
				}

			}
			//======================================================//
			//	소득공제 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("보내기 이벤트");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("오버랩 받기 상태");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("오버랩 받기 상태");
				}
			}
		}
		//======================================================//
		//	에러 메시지 전송했을 경우 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("보낸 곳 에러코드");
				debug->Write("오버랩 받기 상태");

			}
			else if(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("데이터 처리 에러코드");
				debug->Write("오버랩 받기 상태");
			}
			else if(bsdt->dwBaseCode & RECVCODE_DATAERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("받은데이터에 에러가 있다");
				debug->Write("오버랩 받기 상태");
			}
			else if(bsdt->dwBaseCode & RECVCODE_BASEERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("처리중 에러 발생");
				debug->Write("오버랩 받기 상태");
			}
			else
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("에러코드 알수 없음 여기로 들어오면 안된다");
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
void CServerMg::IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	데이터 검증용 클래스 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	데이터 분류 	
	//=======================================================//
	baseDATA20* bsdt =(baseDATA20*) _info->data;
	//======================================================//
	//	받은 데이터 검증 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//기본 데이터
		debug->Write("[기본 데이터] code:0x%08x,테이블명:%s,데이터길이:%d",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
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
	if((bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)||(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)||(bsdt->dwBaseCode & RECVCODE_DATAERROR)||(bsdt->dwBaseCode & RECVCODE_BASEERROR))
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
	//		
	//=======================================================//
	debug->Write("마이비서버에서 사용하지 않는 루틴");
}
//--------------------------------------------------------------
//|
//| 보내기 오버랩 후 처리 부분
//|
//---------------------------------------------------------------
void CServerMg::IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
		debug->Write("마이비 서버에서 사용하지 않는 루틴");
	}
}
//--------------------------------------------------------------
//|
//| CompletionPort 대기 스래드 함수
//|
//---------------------------------------------------------------
DWORD WINAPI CServerMg::ProcessServer(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServerMg* servermg = obj->GetServerMg();
	SOCKET  hResult;
	while(1)
	{
		hResult = servermg->MakeCompetionPortServer();
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
SOCKET CServerMg::MakeCompetionPortServer()
{
	//======================================================//
	//	클라이언트 접속시
	//=======================================================//
	SOCKET  hResult = iocp->CompletionPortServerSocket(GetCompletionPort());
	//======================================================//
	//	데이터 수신 상태로 만듬	
	//=======================================================//
	iocp->fnWSARecv(hResult);
	return hResult;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CServerMg::RequestSendData(SOCKETINFO* info, DWORD totalstructlen, DWORD code, HANDLE hEvent, BOOL bafterdata)
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
	return NULL;
}
//--------------------------------------------------------------
//|
//| IOCP 클라이언트로 접속
//|
//---------------------------------------------------------------
SOCKET CServerMg::MakeCompetionPortClient(SOCKET hSock)
{
	return 0;
}
//--------------------------------------------------------------
//|
//|IOCP 멤버를 넘긴다
//|
//---------------------------------------------------------------
IServer* CServerMg::GetIocpServer()
{
	return iocp;
}
//--------------------------------------------------------------
//|
//|DatabasebMg 멤버를 넘긴다
//|
//---------------------------------------------------------------
IDatabasebMg* CServerMg::GetDatabasebMg()
{
	return NULL;
}
//--------------------------------------------------------------
//|
//|DB 데이터 전송하기 위한 함수
//|
//---------------------------------------------------------------
BOOL CServerMg::DbDataSend()
{
	return FALSE;
}