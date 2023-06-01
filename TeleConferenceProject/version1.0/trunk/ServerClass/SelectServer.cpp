
#include "stdAfx.h"
#include "ServerClass.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSelectServer::CSelectServer():
obj(NULL),
debug(NULL),
nTotalSockets(0),
bTcpClose(TRUE),
bUdpClose(TRUE),
bTcpClientClose(TRUE),
bUdpClientClose(TRUE),
TcpServerCall(NULL),
UdpServerCall(NULL),
TcpClientCall(NULL),
UdpClientCall(NULL),
TcpServerlpArg(NULL),
UdpServerlpArg(NULL),
TcpClientlpArg(NULL),
UdpClientlpArg(NULL)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSelectServer::~CSelectServer()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CSelectServer::initInterfaceObject(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutputObject();
	//=====================================================
	// 기초 클래스 서버 초기화
	//======================================================
	CServerBase::initServerBase(_obj);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CSelectServer::Release()
{
	//=====================================================
	// 서버 스래드 종료(TCP)
	//======================================================
	if(!bTcpClose)
	{
		bTcpClose=TRUE;
		// 종료 이벤트 생성
		CloseSelectTcpServerEvent.Create();
		//스래드 종료 이벤트 대기
		CloseSelectTcpServerEvent.WaitForEvent();
	}
	//=====================================================
	// 서버 스래드 종료(UDP)
	//======================================================
	if(!bUdpClose)
	{
		bUdpClose=TRUE;
		// 종료 이벤트 생성
		CloseSelectUdpServerEvent.Create();
		//스래드 종료 이벤트 대기
		CloseSelectUdpServerEvent.WaitForEvent();
	}
	//=====================================================
	// 클라이언트 스래드 종료(TCP)
	//======================================================
	if(!bTcpClientClose)
	{
		bTcpClientClose=TRUE;
		// 종료 이벤트 생성
		CloseSelectTcpClientEvent.Create();
		//스래드 종료 이벤트 대기
		CloseSelectTcpClientEvent.WaitForEvent();
	}
	//=====================================================
	// 클라이언트 스래드 종료(UDP)
	//======================================================
	if(!bUdpClientClose)
	{
		bUdpClientClose=TRUE;
		// 종료 이벤트 생성
		CloseSelectUdpClientEvent.Create();
		//스래드 종료 이벤트 대기
		CloseSelectUdpClientEvent.WaitForEvent();
	}
	//=====================================================
	// 기초 클래스 메모리 해제
	//======================================================
	CServerBase::ReleaseBase();
}
//--------------------------------------------------------------
//|
//| 서버 생성(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateTcpServer(u_short _nport, TCPRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// 데이타 처리 함수
	//======================================================
	if(callFunc==NULL) return FALSE;
	TcpServerCall = callFunc;
	//=====================================================
	// 데이타 처리  클래스 객체
	//======================================================
	if(lpArg==NULL) return FALSE;
	TcpServerlpArg = lpArg;
	//=====================================================
	// 기초 클래스 서버 생성
	//======================================================
	if(!CServerBase::CreateServerBaseTcp(_nport)) return FALSE;
	////////////////////////////////////////////////////////////////
	// 스래드 생성
	SelectTcpServerthread.Create(SelectTcpServerProcess, (LPVOID)this);
	bTcpClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 서버 생성(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateUdpServer(u_short _nport, UDPRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// 데이타 처리 함수
	//======================================================
	if(callFunc==NULL) return FALSE;
	UdpServerCall = callFunc;
	//=====================================================
	// 데이타 처리  클래스 객체
	//======================================================
	if(lpArg==NULL) return FALSE;
	UdpServerlpArg = lpArg;
	//=====================================================
	// 기초 클래스 서버 생성
	//======================================================
	if(!CServerBase::CreateServerBaseUdp(_nport)) return FALSE;
	////////////////////////////////////////////////////////////////
	// 스래드 생성
	SelectUdpServerthread.Create(SelectUdpServerProcess, (LPVOID)this);
	bUdpClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 서버 생성(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateTcpClient(u_short _port, CString _ip, TCPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// 데이타 처리 함수
	//======================================================
	if(callFunc==NULL) return FALSE;
	TcpClientCall = callFunc;
	//=====================================================
	// 데이타 처리  클래스 객체
	//======================================================
	if(lpArg==NULL) return FALSE;
	TcpClientlpArg = lpArg;
	//=====================================================
	// 기초 클래스 클라이언트 생성
	//======================================================
	if(!CServerBase::CreateServerBaseTcpClient(_port, _ip)) return FALSE;
	////////////////////////////////////////////////////////////////
	// 스래드 생성
	SelectTcpClientthread.Create(SelectTcpClientProcess, (LPVOID)this);
	bTcpClientClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 서버 생성(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateUdpClient(u_short _port, CString _ip, UDPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// 데이타 처리 함수
	//======================================================
	if(callFunc==NULL) return FALSE;
	UdpClientCall = callFunc;
	//=====================================================
	// 데이타 처리  클래스 객체
	//======================================================
	if(lpArg==NULL) return FALSE;
	UdpClientlpArg = lpArg;
	//=====================================================
	// 기초 클래스 서버 생성
	//======================================================
	if(!CServerBase::CreateServerBaseUdpClient(_port, _ip)) return FALSE;
	////////////////////////////////////////////////////////////////
	// 스래드 생성
	SelectUdpClientthread.Create(SelectUdpClientProcess, (LPVOID)this);
	bUdpClientClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Broadcast (UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::Sendbroadcast(u_short _port, char SendrecvMsg[], UINT& sendrecvlen, UINT totalrecv)
{
	//=====================================================
	// 기초 클래스 서버 생성
	//======================================================
	SOCKET sock;
	if(!CServerBase::CreateServerBaseUdpbroadcast(sock, _port)) return FALSE;
	//=====================================================
	//  broadcast
	//======================================================
	int retval, addrlen;
	SOCKADDR_IN bcastaddr, addrRemote;
	memset(&bcastaddr, 0, sizeof(bcastaddr));
	bcastaddr.sin_family = AF_INET;
	bcastaddr.sin_addr.s_addr = INADDR_BROADCAST;
	bcastaddr.sin_port = htons(_port);
	retval = sendto(sock,SendrecvMsg, sendrecvlen, 0, 
		(SOCKADDR *)&bcastaddr, sizeof(bcastaddr));
	if(retval == SOCKET_ERROR)
	{
		err_display("sendto()"); return FALSE;
	}
	// 소켓 셋 초기화
	FD_SET rset;
	struct timeval tv; 
	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	// 약 2초간 기다린다.  
	tv.tv_sec = 3; 
	tv.tv_usec = 0; 

	// select()
	retval = select(0, &rset, NULL, NULL, &tv);
	if(retval == SOCKET_ERROR) 
	{
		err_display("select()"); return FALSE;
	}
	else if(retval == 0)
	{
		return FALSE;
	}
	// UDP는 접속과 데이타가 같이 온다
	if(FD_ISSET(sock, &rset))
	{
		// 데이터 받기
		addrlen = sizeof(addrRemote);
		retval = recvfrom(sock,	SendrecvMsg,	totalrecv,
			0,	(PSOCKADDR) &addrRemote,	&addrlen);		
		if(retval == SOCKET_ERROR)
		{
			err_display("recvfrom()"); return FALSE;
		}
		SendrecvMsg[retval]='\0';
		sendrecvlen=retval;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| Select 루프(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::SelectTcpLoop(CSelectServer *server)
{
	//=====================================================
	// Select Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// 데이터 통신에 사용할 변수
	FD_SET rset;
	FD_SET wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//서버 소켓 객체(TCP)
	SOCKET  hTcpSocket = server->GetTCPSocket();
	while(1)
	{
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(hTcpSocket, &rset);
		for(int i=0; i<nTotalSockets; i++)
		{
			if(SocketInfoArray[i]->totalsend)
				FD_SET(SocketInfoArray[i]->sock, &wset);
			else
				FD_SET(SocketInfoArray[i]->sock, &rset);
		}
		// 약 1초간 기다린다.  
		tv.tv_sec = 1; 
		tv.tv_usec = 0; 

		// select()
		retval = select(0, &rset, &wset, NULL, &tv);
		if(retval == SOCKET_ERROR) 
		{
			err_display("select()");
		}
		else if(retval == 0||server->bTcpClose)
		{
			if(server->bTcpClose)
			{
				if(debug)
					debug->Write("TCP서버 종료");
				break;
			}
			continue;
		}
		
		// 소켓 셋 검사(1): 클라이언트 접속 수용
		if(FD_ISSET(hTcpSocket, &rset))
		{
			addrlen = sizeof(clientaddr);
			client_sock = fnAccept();
			if(client_sock)
			{
				if(AddSocketInfo(client_sock) == FALSE)
				{
					if(debug)
					{
						debug->Write("[TCP 서버] 클라이언트 접속을 해제합니다!");
					}
					closesocket(client_sock);
				}
			}
		}
		// 소켓 셋 검사(2): 데이터 통신
		for(int i=0; i<nTotalSockets; i++)
		{
			SOCKETINFOTCP *ptr = SocketInfoArray[i];
			if(FD_ISSET(ptr->sock, &rset))
			{
				// 데이터 받기
				if(ptr->maxrecv==0)
					ptr->maxrecv=SELECTTCPSERVERBUFSIZE;
				retval = recv(ptr->sock, ptr->buf+ptr->recvbytes, ptr->maxrecv-ptr->recvbytes, 0);
				if(retval == SOCKET_ERROR)
				{
					if(WSAGetLastError() != WSAEWOULDBLOCK)
					{
						err_display("recv()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				else if(retval == 0)
				{
					RemoveSocketInfo(i);
					continue;
				}
				ptr->recvbytes += retval;
				ptr->buf[ptr->recvbytes] = '\0';
			
// 				if(debug)
// 				{
// 					// 받은 클라이언트 정보
// 					addrlen = sizeof(clientaddr);
// 					getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
// 					debug->Write("[TCP/%s:%d]", inet_ntoa(clientaddr.sin_addr),
// 					ntohs(clientaddr.sin_port));
// 				}
				//데이타 처리 함수 호출
				ptr->totalsend=0;
				if(TcpServerCall)
				{
					TcpServerCall(TcpServerlpArg, ptr->sock, ptr->buf, ptr->recvbytes, ptr->maxrecv, ptr->sendbytes, ptr->totalsend);
				}
			}
			if(FD_ISSET(ptr->sock, &wset))
			{
				BOOL bsend;
				//데이타 처리 함수 호출
				if(TcpServerCall&&ptr->sendbytes == 0)
				{
					bsend = TcpServerCall(TcpServerlpArg, ptr->sock, ptr->buf, ptr->recvbytes, ptr->maxrecv, ptr->sendbytes, ptr->totalsend);
				}
				// 데이터 보내기
				retval = send(ptr->sock, ptr->buf + ptr->sendbytes, 
					ptr->totalsend - ptr->sendbytes, 0);
				if(retval == SOCKET_ERROR)
				{
					if(WSAGetLastError() != WSAEWOULDBLOCK)
					{
						err_display("send()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				ptr->sendbytes += retval;
				if(ptr->totalsend == ptr->sendbytes)
				{
					ptr->recvbytes = ptr->sendbytes = ptr->maxrecv = ptr->totalsend = 0;
					if(bsend)
						ptr->totalsend = SELECTTCPSERVERBUFSIZE;
				}
			}
		}
	}
	// 소켓 정보 삭제
	AllRemoveSocketInfo();
	server->bTcpClose=TRUE;
	if(server->CloseSelectTcpServerEvent.IsOpen())
		SetEvent(server->CloseSelectTcpServerEvent);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|  루프(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::UdpLoop(CSelectServer *server)
{
	//=====================================================
	// Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// 데이터 통신에 사용할 변수
	FD_SET rset;
	FD_SET wset;
	//서버 소켓 객체(UDP)
	SOCKET  hUdpSocket = server->GetUDPSocket();
	memset(&udpinfo,0,sizeof(udpinfo));
	while(1)
	{
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(udpinfo.totalsend)
			FD_SET(hUdpSocket, &wset);
		else
			FD_SET(hUdpSocket, &rset);
		// 약 1초간 기다린다.  
		tv.tv_sec = 1; 
		tv.tv_usec = 0; 

		// select()
		retval = select(0, &rset, &wset, NULL, &tv);
		if(retval == SOCKET_ERROR) 
		{
			err_display("select()");
		}
		else if(retval == 0||server->bUdpClose)
		{
			if(server->bUdpClose)
			{
				if(debug)
				{
					debug->Write("UD:서버 종료");
				}
				break;
			}
			continue;
		}
		// UDP는 접속과 데이타가 같이 온다
		if(FD_ISSET(hUdpSocket, &rset))
		{
			// 데이터 받기
			if(udpinfo.maxrecv==0)
				udpinfo.maxrecv=SELECTUDPSERVERBUFSIZE;
			int peerlen = sizeof(udpinfo.peeraddr);
			retval = recvfrom(hUdpSocket,udpinfo.buf+udpinfo.recvbytes, udpinfo.maxrecv-udpinfo.recvbytes, 0, 
				(SOCKADDR *)&udpinfo.peeraddr, &peerlen);
			if(retval == SOCKET_ERROR)
			{
				err_display("recvfrom()"); 
				continue;
			}
			udpinfo.recvbytes+=retval;
			udpinfo.buf[udpinfo.recvbytes]='\0';
// 			if(debug)
// 			{
// 				debug->Write("[UDP/%s:%d]", inet_ntoa(udpinfo.peeraddr.sin_addr),
// 				ntohs(udpinfo.peeraddr.sin_port));
// 			}
			//데이타 처리 함수 호출
			udpinfo.totalsend=0;
			if(UdpServerCall)
			{
				UdpServerCall(UdpServerlpArg, udpinfo.buf, udpinfo.recvbytes, udpinfo.maxrecv, udpinfo.sendbytes, udpinfo.totalsend);
			}
		}
		//데이타를 받을 결과가 있을 때
		if(FD_ISSET(hUdpSocket, &wset))
		{
			BOOL bsend;
			//데이타 처리 함수 호출
			if(UdpServerCall&&udpinfo.sendbytes == 0)
			{
				bsend = UdpServerCall(UdpServerlpArg, udpinfo.buf, udpinfo.recvbytes, udpinfo.maxrecv, udpinfo.sendbytes, udpinfo.totalsend);
			}
			// 데이터 보내기
			retval = sendto(hUdpSocket,udpinfo.buf + udpinfo.sendbytes, udpinfo.totalsend - udpinfo.sendbytes, 0, 
				 				(SOCKADDR *)&udpinfo.peeraddr, sizeof(udpinfo.peeraddr));
			if(retval == SOCKET_ERROR)
			{
				err_display("sendto()"); 
				continue;
			}
			udpinfo.sendbytes += retval;
			if(udpinfo.totalsend == udpinfo.sendbytes)
			{
				udpinfo.recvbytes = udpinfo.sendbytes = udpinfo.maxrecv = udpinfo.totalsend = 0;
				if(bsend)
					udpinfo.totalsend = SELECTUDPSERVERBUFSIZE;
			}
		}
	}
	memset(&udpinfo,0,sizeof(udpinfo));
	server->bUdpClose = TRUE;
	if(server->CloseSelectUdpServerEvent.IsOpen())
		SetEvent(server->CloseSelectUdpServerEvent);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|  Client 루프(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::TcpClientLoop(CSelectServer *server)
{
	//=====================================================
	// Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// 데이터 통신에 사용할 변수
	FD_SET rset;
	FD_SET wset;
	//서버 소켓 객체(UDP)
	SOCKET  hTcpClientSocket = server->GetTCPClientSocket();
	memset(&tcpclientinfo,0,sizeof(tcpclientinfo));
	//서버 소켓 정보
// 	SOCKADDR_IN serveraddr;
// 	int addrlen;
	while(1)
	{
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(tcpclientinfo.totalsend)
			FD_SET(hTcpClientSocket, &wset);
		else
			FD_SET(hTcpClientSocket, &rset);
		// 약 1초간 기다린다.  
		tv.tv_sec = 1; 
		tv.tv_usec = 0; 

		// select()
		retval = select(0, &rset, &wset, NULL, &tv);
		if(retval == SOCKET_ERROR) 
		{
			err_display("select()");
		}
		else if(retval == 0||server->bTcpClientClose)
		{
			if(server->bTcpClientClose)
			{
				if(debug)
					debug->Write("UDP 서버 종료");
				break;
			}
			continue;
		}
		// 클라이언트 접속 데이타를 받았을 때 
		if(FD_ISSET(hTcpClientSocket, &rset))
		{
			// 데이터 받기
			if(tcpclientinfo.maxrecv==0)
				tcpclientinfo.maxrecv=SELECTTCPCLIENTBUFSIZE;
			retval = recv(hTcpClientSocket, tcpclientinfo.buf+tcpclientinfo.recvbytes, tcpclientinfo.maxrecv-tcpclientinfo.recvbytes, 0);
			if(retval == SOCKET_ERROR)
			{
				if(WSAGetLastError() != WSAEWOULDBLOCK)
				{
					err_display("recv()");
					break;
				}
				continue;
			}
			else if(retval == 0)
			{
				break;
			}
			tcpclientinfo.recvbytes+=retval;
			tcpclientinfo.buf[tcpclientinfo.recvbytes]='\0';

// 			if(debug)
// 			{
// 				// 받은 데이터 출력
// 				addrlen = sizeof(serveraddr);
// 				getpeername(hTcpClientSocket, (SOCKADDR *)&serveraddr, &addrlen);
// 				debug->Write("[TCP/%s:%d]", inet_ntoa(serveraddr.sin_addr),
// 					ntohs(serveraddr.sin_port));
// 			}
			//데이타 처리 함수 호출
			tcpclientinfo.totalsend=0;
			if(TcpClientCall)
			{
				TcpClientCall(TcpClientlpArg, tcpclientinfo.buf, tcpclientinfo.recvbytes, tcpclientinfo.maxrecv, tcpclientinfo.sendbytes, tcpclientinfo.totalsend);
			}
		}
		// 보낼 데이타가 있을 때
		if(FD_ISSET(hTcpClientSocket, &wset))
		{
			BOOL bsend;
			//데이타 처리 함수 호출
			if(TcpClientCall&&tcpclientinfo.sendbytes == 0)
			{
				bsend = TcpClientCall(TcpClientlpArg, tcpclientinfo.buf, tcpclientinfo.recvbytes, tcpclientinfo.maxrecv, tcpclientinfo.sendbytes, tcpclientinfo.totalsend);
			}
			// 데이터 보내기
			retval = send(hTcpClientSocket, tcpclientinfo.buf + tcpclientinfo.sendbytes, 
				tcpclientinfo.totalsend - tcpclientinfo.sendbytes, 0);
			if(retval == SOCKET_ERROR)
			{
				if(WSAGetLastError() != WSAEWOULDBLOCK)
				{
					err_display("send()");
					break;
				}
				continue;
			}
			tcpclientinfo.sendbytes += retval;
			if(tcpclientinfo.totalsend == tcpclientinfo.sendbytes)
			{
				tcpclientinfo.recvbytes = tcpclientinfo.sendbytes = tcpclientinfo.maxrecv = tcpclientinfo.totalsend = 0;
				if(bsend)
					tcpclientinfo.totalsend = SELECTTCPCLIENTBUFSIZE;
			}
		}
	}
	memset(&tcpclientinfo,0,sizeof(tcpclientinfo));
	server->bTcpClientClose = TRUE;
	if(server->CloseSelectTcpClientEvent.IsOpen())
		SetEvent(server->CloseSelectTcpClientEvent);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|  Client 루프(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::UdpClientLoop(CSelectServer *server)
{
	//=====================================================
	// Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// 데이터 통신에 사용할 변수
	FD_SET rset;
	FD_SET wset;
	//서버 소켓 객체(UDP)
	SOCKET  hUdpClientSocket = server->GetUDPClientSocket();
	memset(&udpclientinfo,0,sizeof(udpclientinfo));
	//서버 소켓 정보
// 	SOCKADDR_IN serveraddr;
// 	int addrlen;
	while(1)
	{
		// 소켓 셋 초기화
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(udpclientinfo.totalsend)
			FD_SET(hUdpClientSocket, &wset);
		else
			FD_SET(hUdpClientSocket, &rset);
		// 약 1초간 기다린다.  
		tv.tv_sec = 1; 
		tv.tv_usec = 0; 

		// select()
		retval = select(0, &rset, &wset, NULL, &tv);
		if(retval == SOCKET_ERROR) 
		{
			err_display("select()");
		}
		else if(retval == 0||server->bUdpClientClose)
		{
			if(server->bUdpClientClose)
			{
				if(debug)
				{
					debug->Write("UDP 서버 종료");
				}
				break;
			}
			continue;
		}
		// 소켓 셋 검사(1): 클라이언트 접속 수용
		if(FD_ISSET(hUdpClientSocket, &rset))
		{
			// 데이터 받기
			if(udpclientinfo.maxrecv==0)
				udpclientinfo.maxrecv=SELECTUDPCLIENTBUFSIZE;
			retval = recv(hUdpClientSocket, udpclientinfo.buf+udpclientinfo.recvbytes, udpclientinfo.maxrecv-udpclientinfo.recvbytes, 0);
			if(retval == SOCKET_ERROR)
			{
				if(WSAGetLastError() != WSAEWOULDBLOCK)
				{
					err_display("recv()");
					break;
				}
				continue;
			}
			else if(retval == 0)
			{
				break;
			}
			udpclientinfo.recvbytes+=retval;
			udpclientinfo.buf[udpclientinfo.recvbytes]='\0';
// 			if(debug)
// 			{
// 				// 받은 데이터 출력
// 				addrlen = sizeof(serveraddr);
// 				getpeername(hUdpClientSocket, (SOCKADDR *)&serveraddr, &addrlen);
// 				debug->Write("[UDP/%s:%d]", inet_ntoa(serveraddr.sin_addr),
// 					ntohs(serveraddr.sin_port));
// 			}
			//데이타 처리 함수 호출
			udpclientinfo.totalsend=0;
			if(UdpClientCall)
			{
				UdpClientCall(UdpClientlpArg, udpclientinfo.buf, udpclientinfo.recvbytes, udpclientinfo.maxrecv, udpclientinfo.sendbytes, udpclientinfo.totalsend);
			}
		}
		if(FD_ISSET(hUdpClientSocket, &wset))
		{
			BOOL bsend;
			//데이타 처리 함수 호출
			if(UdpClientCall&&udpclientinfo.sendbytes == 0)
			{
				bsend = UdpClientCall(UdpClientlpArg, udpclientinfo.buf, udpclientinfo.recvbytes, udpclientinfo.maxrecv, udpclientinfo.sendbytes, udpclientinfo.totalsend);
			}
			// 데이터 보내기
			retval = send(hUdpClientSocket, udpclientinfo.buf + udpclientinfo.sendbytes, 
				udpclientinfo.totalsend - udpclientinfo.sendbytes, 0);
			if(retval == SOCKET_ERROR)
			{
				if(WSAGetLastError() != WSAEWOULDBLOCK)
				{
					err_display("send()");
					break;
				}
				continue;
			}
			udpclientinfo.sendbytes += retval;
			if(udpclientinfo.totalsend == udpclientinfo.sendbytes)
			{
				udpclientinfo.recvbytes = udpclientinfo.sendbytes = udpclientinfo.maxrecv = udpclientinfo.totalsend = 0;
				if(bsend)
					udpclientinfo.totalsend = SELECTUDPCLIENTBUFSIZE;
			}
		}
	}
	memset(&udpclientinfo,0,sizeof(udpclientinfo));
	server->bUdpClientClose = TRUE;
	if(server->CloseSelectUdpClientEvent.IsOpen())
		SetEvent(server->CloseSelectUdpClientEvent);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 추가
//|
//---------------------------------------------------------------
BOOL CSelectServer::AddSocketInfo(SOCKET sock)
{
	// FD_SETSIZE - 연결 대기 소켓
	if(nTotalSockets >= (FD_SETSIZE-1))
	{
		if(debug)
		{
			debug->Write("[오류] 소켓 정보를 추가할 수 없습니다!");
		}
		return FALSE;
	}

	SOCKETINFOTCP *ptr = new SOCKETINFOTCP;
	if(ptr == NULL)
	{
		if(debug)
		{
			debug->Write("[오류] 메모리가 부족합니다!");
		}
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->maxrecv = SELECTTCPSERVERBUFSIZE;
	ptr->sendbytes = 0;
	ptr->totalsend = 0;
	SocketInfoArray[nTotalSockets++] = ptr;

	return TRUE;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 삭제
//|
//---------------------------------------------------------------
void CSelectServer::RemoveSocketInfo(int nIndex)
{
	SOCKETINFOTCP *ptr = SocketInfoArray[nIndex];

	if(debug)
	{
		// 클라이언트 정보 얻기
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
		debug->Write("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d", 
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}

	closesocket(ptr->sock);
	delete ptr;

	for(int i=nIndex; i<nTotalSockets; i++)
	{
		SocketInfoArray[i] = SocketInfoArray[i+1];
	}
	nTotalSockets--;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 모두 삭제
//|
//---------------------------------------------------------------
void CSelectServer::AllRemoveSocketInfo()
{
	for(int i=0; i<nTotalSockets; i++)
	{
		SOCKETINFOTCP *ptr = SocketInfoArray[i];
		if(debug)
		{
			// 클라이언트 정보 얻기
			SOCKADDR_IN clientaddr;
			int addrlen = sizeof(clientaddr);
			getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
			debug->Write("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d", 
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		}

		closesocket(ptr->sock);
		delete ptr; ptr=NULL;
	}
	nTotalSockets = 0;
}
//--------------------------------------------------------------
//|
//| 스래드 함수(TCP)
//|
//---------------------------------------------------------------
DWORD WINAPI CSelectServer::SelectTcpServerProcess(LPVOID lpArg)
{
	CSelectServer *selectserver = (CSelectServer *)lpArg;
	if(selectserver)
		selectserver->SelectTcpLoop(selectserver);
	return 0;
}
//--------------------------------------------------------------
//|
//| 스래드 함수(UDP)
//|
//---------------------------------------------------------------
DWORD WINAPI CSelectServer::SelectUdpServerProcess(LPVOID lpArg)
{
	CSelectServer *selectserver = (CSelectServer *)lpArg;
	if(selectserver)
		selectserver->UdpLoop(selectserver);
	return 0;
}
//--------------------------------------------------------------
//|
//| Client 스래드 함수(TCP)
//|
//---------------------------------------------------------------
DWORD WINAPI CSelectServer::SelectTcpClientProcess(LPVOID lpArg)
{
	CSelectServer *selectserver = (CSelectServer *)lpArg;
	if(selectserver)
		selectserver->TcpClientLoop(selectserver);
	return 0;
}
//--------------------------------------------------------------
//|
//| Client 스래드 함수(UDP)
//|
//---------------------------------------------------------------
DWORD WINAPI CSelectServer::SelectUdpClientProcess(LPVOID lpArg)
{
	CSelectServer *selectserver = (CSelectServer *)lpArg;
	if(selectserver)
		selectserver->UdpClientLoop(selectserver);
	return 0;
}
//--------------------------------------------------------------
//|
//|서버 IP 주소
//|
//---------------------------------------------------------------
CString CSelectServer::GetLocalServerIp()
{
	return GetServerIp();
}
//--------------------------------------------------------------
//|
//| TCP SERVER INFO FIND
//|
//---------------------------------------------------------------
CSelectServer::SOCKETINFOTCP* CSelectServer::GetTcpServerInfo(SOCKET _sock)
{
	for(int i=0; i<nTotalSockets; i++)
	{
		SOCKETINFOTCP *ptr = SocketInfoArray[i];
		if(ptr->sock==_sock)
			return ptr;
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//| TCP SERVER DATA SEND
//|
//---------------------------------------------------------------
BOOL CSelectServer::TcpServerSend(SOCKET _sock)
{
	SOCKETINFOTCP *ptr = GetTcpServerInfo(_sock);
	if(!bTcpClose&&ptr&&ptr->maxrecv==0)
	{
		ptr->sendbytes = 0;
		ptr->totalsend=SELECTTCPSERVERBUFSIZE;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| UDP SERVER DATA SEND
//|
//---------------------------------------------------------------
BOOL CSelectServer::UdpServerSend()
{
	if(!bUdpClose&&udpinfo.maxrecv==0)
	{
		udpinfo.sendbytes = 0;
		udpinfo.totalsend=SELECTUDPSERVERBUFSIZE;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| TCP Client DATA SEND
//|
//---------------------------------------------------------------
BOOL CSelectServer::TcpClientSend()
{
	if(!bTcpClientClose&&tcpclientinfo.maxrecv==0)
	{
		tcpclientinfo.sendbytes = 0;
		tcpclientinfo.totalsend=SELECTTCPCLIENTBUFSIZE;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| UDP Client DATA SEND
//|
//---------------------------------------------------------------
BOOL CSelectServer::UdpClientSend()
{
	if(!bUdpClientClose&&udpclientinfo.maxrecv==0)
	{
		udpclientinfo.sendbytes = 0;
		udpclientinfo.totalsend=SELECTUDPCLIENTBUFSIZE;
		return TRUE;
	}
	return FALSE;
}
