
#include "stdAfx.h"
#include "ServerClass.h"

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CSelectServer::~CSelectServer()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CSelectServer::initInterfaceObject(IMyObject* _obj)
{
	//=====================================================
	// �� ��ü ���� Ŭ���� ������Ʈ
	//======================================================
	obj=_obj;
	//=====================================================
	// ����� ��� Ŭ����
	//======================================================
	debug=obj->GetOutputObject();
	//=====================================================
	// ���� Ŭ���� ���� �ʱ�ȭ
	//======================================================
	CServerBase::initServerBase(_obj);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CSelectServer::Release()
{
	//=====================================================
	// ���� ������ ����(TCP)
	//======================================================
	if(!bTcpClose)
	{
		bTcpClose=TRUE;
		// ���� �̺�Ʈ ����
		CloseSelectTcpServerEvent.Create();
		//������ ���� �̺�Ʈ ���
		CloseSelectTcpServerEvent.WaitForEvent();
	}
	//=====================================================
	// ���� ������ ����(UDP)
	//======================================================
	if(!bUdpClose)
	{
		bUdpClose=TRUE;
		// ���� �̺�Ʈ ����
		CloseSelectUdpServerEvent.Create();
		//������ ���� �̺�Ʈ ���
		CloseSelectUdpServerEvent.WaitForEvent();
	}
	//=====================================================
	// Ŭ���̾�Ʈ ������ ����(TCP)
	//======================================================
	if(!bTcpClientClose)
	{
		bTcpClientClose=TRUE;
		// ���� �̺�Ʈ ����
		CloseSelectTcpClientEvent.Create();
		//������ ���� �̺�Ʈ ���
		CloseSelectTcpClientEvent.WaitForEvent();
	}
	//=====================================================
	// Ŭ���̾�Ʈ ������ ����(UDP)
	//======================================================
	if(!bUdpClientClose)
	{
		bUdpClientClose=TRUE;
		// ���� �̺�Ʈ ����
		CloseSelectUdpClientEvent.Create();
		//������ ���� �̺�Ʈ ���
		CloseSelectUdpClientEvent.WaitForEvent();
	}
	//=====================================================
	// ���� Ŭ���� �޸� ����
	//======================================================
	CServerBase::ReleaseBase();
}
//--------------------------------------------------------------
//|
//| ���� ����(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateTcpServer(u_short _nport, TCPRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// ����Ÿ ó�� �Լ�
	//======================================================
	if(callFunc==NULL) return FALSE;
	TcpServerCall = callFunc;
	//=====================================================
	// ����Ÿ ó��  Ŭ���� ��ü
	//======================================================
	if(lpArg==NULL) return FALSE;
	TcpServerlpArg = lpArg;
	//=====================================================
	// ���� Ŭ���� ���� ����
	//======================================================
	if(!CServerBase::CreateServerBaseTcp(_nport)) return FALSE;
	////////////////////////////////////////////////////////////////
	// ������ ����
	SelectTcpServerthread.Create(SelectTcpServerProcess, (LPVOID)this);
	bTcpClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| ���� ����(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateUdpServer(u_short _nport, UDPRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// ����Ÿ ó�� �Լ�
	//======================================================
	if(callFunc==NULL) return FALSE;
	UdpServerCall = callFunc;
	//=====================================================
	// ����Ÿ ó��  Ŭ���� ��ü
	//======================================================
	if(lpArg==NULL) return FALSE;
	UdpServerlpArg = lpArg;
	//=====================================================
	// ���� Ŭ���� ���� ����
	//======================================================
	if(!CServerBase::CreateServerBaseUdp(_nport)) return FALSE;
	////////////////////////////////////////////////////////////////
	// ������ ����
	SelectUdpServerthread.Create(SelectUdpServerProcess, (LPVOID)this);
	bUdpClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| ���� ����(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateTcpClient(u_short _port, CString _ip, TCPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// ����Ÿ ó�� �Լ�
	//======================================================
	if(callFunc==NULL) return FALSE;
	TcpClientCall = callFunc;
	//=====================================================
	// ����Ÿ ó��  Ŭ���� ��ü
	//======================================================
	if(lpArg==NULL) return FALSE;
	TcpClientlpArg = lpArg;
	//=====================================================
	// ���� Ŭ���� Ŭ���̾�Ʈ ����
	//======================================================
	if(!CServerBase::CreateServerBaseTcpClient(_port, _ip)) return FALSE;
	////////////////////////////////////////////////////////////////
	// ������ ����
	SelectTcpClientthread.Create(SelectTcpClientProcess, (LPVOID)this);
	bTcpClientClose=FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| ���� ����(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::CreateUdpClient(u_short _port, CString _ip, UDPCLIENTRECVCALLBACK callFunc, LPVOID lpArg)
{
	//=====================================================
	// ����Ÿ ó�� �Լ�
	//======================================================
	if(callFunc==NULL) return FALSE;
	UdpClientCall = callFunc;
	//=====================================================
	// ����Ÿ ó��  Ŭ���� ��ü
	//======================================================
	if(lpArg==NULL) return FALSE;
	UdpClientlpArg = lpArg;
	//=====================================================
	// ���� Ŭ���� ���� ����
	//======================================================
	if(!CServerBase::CreateServerBaseUdpClient(_port, _ip)) return FALSE;
	////////////////////////////////////////////////////////////////
	// ������ ����
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
	// ���� Ŭ���� ���� ����
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
	// ���� �� �ʱ�ȭ
	FD_SET rset;
	struct timeval tv; 
	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	// �� 2�ʰ� ��ٸ���.  
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
	// UDP�� ���Ӱ� ����Ÿ�� ���� �´�
	if(FD_ISSET(sock, &rset))
	{
		// ������ �ޱ�
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
//| Select ����(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::SelectTcpLoop(CSelectServer *server)
{
	//=====================================================
	// Select Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// ������ ��ſ� ����� ����
	FD_SET rset;
	FD_SET wset;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	//���� ���� ��ü(TCP)
	SOCKET  hTcpSocket = server->GetTCPSocket();
	while(1)
	{
		// ���� �� �ʱ�ȭ
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
		// �� 1�ʰ� ��ٸ���.  
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
					debug->Write("TCP���� ����");
				break;
			}
			continue;
		}
		
		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
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
						debug->Write("[TCP ����] Ŭ���̾�Ʈ ������ �����մϴ�!");
					}
					closesocket(client_sock);
				}
			}
		}
		// ���� �� �˻�(2): ������ ���
		for(int i=0; i<nTotalSockets; i++)
		{
			SOCKETINFOTCP *ptr = SocketInfoArray[i];
			if(FD_ISSET(ptr->sock, &rset))
			{
				// ������ �ޱ�
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
// 					// ���� Ŭ���̾�Ʈ ����
// 					addrlen = sizeof(clientaddr);
// 					getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
// 					debug->Write("[TCP/%s:%d]", inet_ntoa(clientaddr.sin_addr),
// 					ntohs(clientaddr.sin_port));
// 				}
				//����Ÿ ó�� �Լ� ȣ��
				ptr->totalsend=0;
				if(TcpServerCall)
				{
					TcpServerCall(TcpServerlpArg, ptr->sock, ptr->buf, ptr->recvbytes, ptr->maxrecv, ptr->sendbytes, ptr->totalsend);
				}
			}
			if(FD_ISSET(ptr->sock, &wset))
			{
				BOOL bsend;
				//����Ÿ ó�� �Լ� ȣ��
				if(TcpServerCall&&ptr->sendbytes == 0)
				{
					bsend = TcpServerCall(TcpServerlpArg, ptr->sock, ptr->buf, ptr->recvbytes, ptr->maxrecv, ptr->sendbytes, ptr->totalsend);
				}
				// ������ ������
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
	// ���� ���� ����
	AllRemoveSocketInfo();
	server->bTcpClose=TRUE;
	if(server->CloseSelectTcpServerEvent.IsOpen())
		SetEvent(server->CloseSelectTcpServerEvent);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|  ����(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::UdpLoop(CSelectServer *server)
{
	//=====================================================
	// Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// ������ ��ſ� ����� ����
	FD_SET rset;
	FD_SET wset;
	//���� ���� ��ü(UDP)
	SOCKET  hUdpSocket = server->GetUDPSocket();
	memset(&udpinfo,0,sizeof(udpinfo));
	while(1)
	{
		// ���� �� �ʱ�ȭ
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(udpinfo.totalsend)
			FD_SET(hUdpSocket, &wset);
		else
			FD_SET(hUdpSocket, &rset);
		// �� 1�ʰ� ��ٸ���.  
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
					debug->Write("UD:���� ����");
				}
				break;
			}
			continue;
		}
		// UDP�� ���Ӱ� ����Ÿ�� ���� �´�
		if(FD_ISSET(hUdpSocket, &rset))
		{
			// ������ �ޱ�
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
			//����Ÿ ó�� �Լ� ȣ��
			udpinfo.totalsend=0;
			if(UdpServerCall)
			{
				UdpServerCall(UdpServerlpArg, udpinfo.buf, udpinfo.recvbytes, udpinfo.maxrecv, udpinfo.sendbytes, udpinfo.totalsend);
			}
		}
		//����Ÿ�� ���� ����� ���� ��
		if(FD_ISSET(hUdpSocket, &wset))
		{
			BOOL bsend;
			//����Ÿ ó�� �Լ� ȣ��
			if(UdpServerCall&&udpinfo.sendbytes == 0)
			{
				bsend = UdpServerCall(UdpServerlpArg, udpinfo.buf, udpinfo.recvbytes, udpinfo.maxrecv, udpinfo.sendbytes, udpinfo.totalsend);
			}
			// ������ ������
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
//|  Client ����(TCP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::TcpClientLoop(CSelectServer *server)
{
	//=====================================================
	// Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// ������ ��ſ� ����� ����
	FD_SET rset;
	FD_SET wset;
	//���� ���� ��ü(UDP)
	SOCKET  hTcpClientSocket = server->GetTCPClientSocket();
	memset(&tcpclientinfo,0,sizeof(tcpclientinfo));
	//���� ���� ����
// 	SOCKADDR_IN serveraddr;
// 	int addrlen;
	while(1)
	{
		// ���� �� �ʱ�ȭ
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(tcpclientinfo.totalsend)
			FD_SET(hTcpClientSocket, &wset);
		else
			FD_SET(hTcpClientSocket, &rset);
		// �� 1�ʰ� ��ٸ���.  
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
					debug->Write("UDP ���� ����");
				break;
			}
			continue;
		}
		// Ŭ���̾�Ʈ ���� ����Ÿ�� �޾��� �� 
		if(FD_ISSET(hTcpClientSocket, &rset))
		{
			// ������ �ޱ�
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
// 				// ���� ������ ���
// 				addrlen = sizeof(serveraddr);
// 				getpeername(hTcpClientSocket, (SOCKADDR *)&serveraddr, &addrlen);
// 				debug->Write("[TCP/%s:%d]", inet_ntoa(serveraddr.sin_addr),
// 					ntohs(serveraddr.sin_port));
// 			}
			//����Ÿ ó�� �Լ� ȣ��
			tcpclientinfo.totalsend=0;
			if(TcpClientCall)
			{
				TcpClientCall(TcpClientlpArg, tcpclientinfo.buf, tcpclientinfo.recvbytes, tcpclientinfo.maxrecv, tcpclientinfo.sendbytes, tcpclientinfo.totalsend);
			}
		}
		// ���� ����Ÿ�� ���� ��
		if(FD_ISSET(hTcpClientSocket, &wset))
		{
			BOOL bsend;
			//����Ÿ ó�� �Լ� ȣ��
			if(TcpClientCall&&tcpclientinfo.sendbytes == 0)
			{
				bsend = TcpClientCall(TcpClientlpArg, tcpclientinfo.buf, tcpclientinfo.recvbytes, tcpclientinfo.maxrecv, tcpclientinfo.sendbytes, tcpclientinfo.totalsend);
			}
			// ������ ������
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
//|  Client ����(UDP)
//|
//---------------------------------------------------------------
BOOL CSelectServer::UdpClientLoop(CSelectServer *server)
{
	//=====================================================
	// Loop 
	//======================================================
	struct timeval tv; 
	int retval;
	// ������ ��ſ� ����� ����
	FD_SET rset;
	FD_SET wset;
	//���� ���� ��ü(UDP)
	SOCKET  hUdpClientSocket = server->GetUDPClientSocket();
	memset(&udpclientinfo,0,sizeof(udpclientinfo));
	//���� ���� ����
// 	SOCKADDR_IN serveraddr;
// 	int addrlen;
	while(1)
	{
		// ���� �� �ʱ�ȭ
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		if(udpclientinfo.totalsend)
			FD_SET(hUdpClientSocket, &wset);
		else
			FD_SET(hUdpClientSocket, &rset);
		// �� 1�ʰ� ��ٸ���.  
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
					debug->Write("UDP ���� ����");
				}
				break;
			}
			continue;
		}
		// ���� �� �˻�(1): Ŭ���̾�Ʈ ���� ����
		if(FD_ISSET(hUdpClientSocket, &rset))
		{
			// ������ �ޱ�
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
// 				// ���� ������ ���
// 				addrlen = sizeof(serveraddr);
// 				getpeername(hUdpClientSocket, (SOCKADDR *)&serveraddr, &addrlen);
// 				debug->Write("[UDP/%s:%d]", inet_ntoa(serveraddr.sin_addr),
// 					ntohs(serveraddr.sin_port));
// 			}
			//����Ÿ ó�� �Լ� ȣ��
			udpclientinfo.totalsend=0;
			if(UdpClientCall)
			{
				UdpClientCall(UdpClientlpArg, udpclientinfo.buf, udpclientinfo.recvbytes, udpclientinfo.maxrecv, udpclientinfo.sendbytes, udpclientinfo.totalsend);
			}
		}
		if(FD_ISSET(hUdpClientSocket, &wset))
		{
			BOOL bsend;
			//����Ÿ ó�� �Լ� ȣ��
			if(UdpClientCall&&udpclientinfo.sendbytes == 0)
			{
				bsend = UdpClientCall(UdpClientlpArg, udpclientinfo.buf, udpclientinfo.recvbytes, udpclientinfo.maxrecv, udpclientinfo.sendbytes, udpclientinfo.totalsend);
			}
			// ������ ������
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
//| ���� ���� �߰�
//|
//---------------------------------------------------------------
BOOL CSelectServer::AddSocketInfo(SOCKET sock)
{
	// FD_SETSIZE - ���� ��� ����
	if(nTotalSockets >= (FD_SETSIZE-1))
	{
		if(debug)
		{
			debug->Write("[����] ���� ������ �߰��� �� �����ϴ�!");
		}
		return FALSE;
	}

	SOCKETINFOTCP *ptr = new SOCKETINFOTCP;
	if(ptr == NULL)
	{
		if(debug)
		{
			debug->Write("[����] �޸𸮰� �����մϴ�!");
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
//| ���� ���� ����
//|
//---------------------------------------------------------------
void CSelectServer::RemoveSocketInfo(int nIndex)
{
	SOCKETINFOTCP *ptr = SocketInfoArray[nIndex];

	if(debug)
	{
		// Ŭ���̾�Ʈ ���� ���
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
		debug->Write("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d", 
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
//| ���� ���� ��� ����
//|
//---------------------------------------------------------------
void CSelectServer::AllRemoveSocketInfo()
{
	for(int i=0; i<nTotalSockets; i++)
	{
		SOCKETINFOTCP *ptr = SocketInfoArray[i];
		if(debug)
		{
			// Ŭ���̾�Ʈ ���� ���
			SOCKADDR_IN clientaddr;
			int addrlen = sizeof(clientaddr);
			getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
			debug->Write("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d", 
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		}

		closesocket(ptr->sock);
		delete ptr; ptr=NULL;
	}
	nTotalSockets = 0;
}
//--------------------------------------------------------------
//|
//| ������ �Լ�(TCP)
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
//| ������ �Լ�(UDP)
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
//| Client ������ �Լ�(TCP)
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
//| Client ������ �Լ�(UDP)
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
//|���� IP �ּ�
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
