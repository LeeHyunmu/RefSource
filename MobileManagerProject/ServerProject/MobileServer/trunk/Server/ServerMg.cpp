
#include "stdAfx.h"
#include "ServerMg.h"

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CServerMg::~CServerMg()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
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
//|������� ��ü�� �޸𸮿��� �����Ѵ�
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
		MessageBox(NULL, "��Ʈ ��ȣ ����(5001 - 9999)", "����", MB_ICONINFORMATION);
		return FALSE;
	}
	if(iocp->Initialize(nPort)==FALSE)
		return FALSE;
	// IOCP�� �����Ѵ�.
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIocp == NULL )
	{
		MessageBox(NULL, "IOCP ���� ����", "�˸�", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}

	m_hclntIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hclntIocp == NULL )
	{
		MessageBox(NULL, "IOCP ���� ����", "�˸�", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}
	////////////////////////////////////////////////////////////////
	// IOCP ó�� �����带 �����Ѵ�.
	SYSTEM_INFO si;
	GetSystemInfo( &si ); // CPU ���� Ȯ��
	// CPU * 2 ���� �۾� ������ ����
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
	// ���� ������ ����
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);

	DWORD dwThreadId3;
	mclntthread = CreateThread(NULL, 0, WorkerClntThread, (LPVOID)obj, 0, &dwThreadId3);

	debug->Write("��������");
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
	debug->Write("��������");
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
		//	���� ������ ������ �� ó��	
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
			debug->Write("IP:[%s] PORT:[%d]��������", strIP, port);
			iocp->fnRemoveSocketInfo(_info->hSock);
			continue;
		}
		//======================================================//
		//	���� ������ ���� �� ó��	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			_info->wType[0] = 0;
			if(_info->data.wCode==0)
			{
				iocp->fnWSARecv(_info->hSock);
				continue;
			}
			_tcscpy_s(_info->szCardNum,20,_info->data.szCardNum);
			iAddrLen = sizeof(clientAddr);
			getpeername(_info->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
			strIP="";
			port=0;
			strIP = inet_ntoa(clientAddr.sin_addr);
			port = ntohs(clientAddr.sin_port);
			debug->Write("IP:[%s] PORT:[%d]", strIP, port);
			debug->Write("%d, %s, %s", _info->data.wCode, _info->data.szCardNum, _info->data.szData);
			//Recv ������ ó�� �Լ� ȣ��
			my->RecvHandling(_info,0);
			// �ٽ� ���� �� �ְ� I/O ��û�� �Ѵ�.
			iocp->fnWSARecv(_info->hSock);
		}
		//======================================================//
		//	���� �����Ͱ� ���� �� ó��	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			// ó���� ���� ������ ó��
			_info->wType[1] = 0;
			debug->Write("������ �Ϸ�");
		}
	}
	return 0;
}
DWORD WINAPI CServerMg::WorkerClntThread(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServer *iocp = obj->GetIocpServer();
	IOutput * debug = obj->GetOutput();
	IServerMg* my = obj->GetServerMg();
	HANDLE hIocp=my->GetClntCompletionPort();

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
		//	���� ������ ������ �� ó��	
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
			debug->Write("IP:[%s] PORT:[%d]��������", strIP, port);
			iocp->fnRemoveSocketInfo(_info->hSock);
			continue;
		}
		//======================================================//
		//	���� ������ ���� �� ó��	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			_info->wType[0] = 0;
			if(_info->data.wCode==0)
			{
				iocp->fnWSARecv(_info->hSock);
				continue;
			}
			_tcscpy_s(_info->szCardNum,20,_info->data.szCardNum);
			iAddrLen = sizeof(clientAddr);
			getpeername(_info->hSock, (SOCKADDR *)&clientAddr, &iAddrLen);
			strIP="";
			port=0;
			strIP = inet_ntoa(clientAddr.sin_addr);
			port = ntohs(clientAddr.sin_port);
			debug->Write("IP:[%s] PORT:[%d]", strIP, port);
			debug->Write("%d, %s, %s", _info->data.wCode, _info->data.szCardNum, _info->data.szData);
			//Recv ������ ó�� �Լ� ȣ��
			my->RecvHandling(_info,0);
			// �ٽ� ���� �� �ְ� I/O ��û�� �Ѵ�.
		//	iocp->fnWSARecv(_info->hSock);
		}
		//======================================================//
		//	���� �����Ͱ� ���� �� ó��	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			// ó���� ���� ������ ó��
			_info->wType[1] = 0;
			debug->Write("������ �Ϸ�");
		}
	}
	return 0;
}
void CServerMg::RecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	���̺� ó�� ��� 	
	//=======================================================//
	if(_info->data.wCode & RECVCODE_CONNECTSERVER)
	{
		debug->Write("���̺� ó�� ���");
		SOCKETINFO*  socketinfo;
		if(iocp->fnGetSockInfo(&socketinfo, _info->hSock,_info->data.szCardNum))
		{
			iocp->fnWSASend(socketinfo->hSock, &_info->data, sizeof(DATA));
			iocp->fnRemoveSocketInfo(_info->hSock);
		}
	}
	else
	{
		//======================================================//
		//	ī�� Enable 	
		//=======================================================//
		if(_info->data.wCode & RECVCODE_CARDENABLE)
		{
			SOCKET sock;
			sock = MakeCompetionPortClient(_info->data.szCardNum);
			debug->Write("ī�� Enable");
			if(iocp)
			{
				iocp->fnWSASend(sock, &_info->data, sizeof(DATA));
				iocp->fnWSARecv(sock);
			}
		}
		//======================================================//
		//	����� ����(���� ����) 	
		//=======================================================//
		else	if(_info->data.wCode & RECVCODE_USERCHANGE)
		{
			SOCKET sock;
			sock = MakeCompetionPortClient(_info->data.szCardNum);
			debug->Write("���� ����");
			if(iocp)
			{
				iocp->fnWSASend(sock, &_info->data, sizeof(DATA));
				iocp->fnWSARecv(sock);
			}
		}
		//======================================================//
		//	�ҵ���� 	
		//=======================================================//
		else	if(_info->data.wCode & RECVCODE_DEDUCTION)
		{
			SOCKET sock;
			sock = MakeCompetionPortClient(_info->data.szCardNum);
			debug->Write("�ҵ����");
			if(iocp)
			{
				iocp->fnWSASend(sock, &_info->data, sizeof(DATA));
				iocp->fnWSARecv(sock);
			}
		}
		//======================================================//
		//	��� ���� üũ 	
		//=======================================================//
		else	if(_info->data.wCode & RECVCODE_USECHECK)
		{
			SOCKET sock;
			sock = MakeCompetionPortClient(_info->data.szCardNum);
			debug->Write("��볻��");
			if(iocp)
			{
				iocp->fnWSASend(sock, &_info->data, sizeof(DATA));
				iocp->fnWSARecv(sock);
			}
		}
	}
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
		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		hResult = CreateIoCompletionPort((HANDLE)socket, my->GetCompletionPort(), (DWORD)socket , 0);
		if(hResult == NULL)
			return -1;
		// �񵿱� ����� ����
		iocp->fnAddSocketInfo(socket);
		iocp->fnWSARecv(socket);
	}
	return 0;
}
HANDLE CServerMg::GetCompletionPort()
{
	return m_hIocp;
}
HANDLE CServerMg::GetClntCompletionPort()
{
	return m_hclntIocp;
}
#define  MB_IP "127.0.0.1"
#define  MB_PORT 9001
SOCKET CServerMg::MakeCompetionPortClient(TCHAR* _szCardName)
{
	SOCKET sock;
	HANDLE  hResult;
	SOCKETINFO*  socketinfo=NULL;
	if(iocp->CompletionPortClientSocket(MB_IP, MB_PORT, sock))
	{
		// ���ϰ� ����� �Ϸ� ��Ʈ ����
		hResult = CreateIoCompletionPort((HANDLE)sock, GetClntCompletionPort(), (DWORD)sock , 0);
		if(hResult == NULL)
			return 0;
		// �񵿱� ����� ���� ����
		iocp->fnAddSocketInfo(sock);
		SOCKETINFO*  socketinfo;
		if(iocp->fnGetSockInfo(&socketinfo, sock))
		{
			_tcscpy_s(socketinfo->szCardNum,20,_szCardName);
		}
	}
	return sock;
}