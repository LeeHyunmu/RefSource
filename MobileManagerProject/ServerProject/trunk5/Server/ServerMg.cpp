
#include "stdAfx.h"
#include "ServerMg.h"
#include "../ThdPoolObj/DbDataSendPoolObj.h"
#define  IOCP_SERVERCOUNT 2
#define  IOCP_CLIENTCOUNT 2
//--------------------------------------------------------------
//|
//|������
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
	//������ ���� �÷���
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
//|���� ����
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
		MessageBox(NULL, "��Ʈ ��ȣ ����(5001 - 9999)", "����", MB_ICONINFORMATION);
		return FALSE;
	}
	if(iocp->ServerListen(nPort)==FALSE)
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
	//������ ������
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
	// ���� ������ ����
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);
	//Ŭ���̾�Ʈ�� ������
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
		//	����ó��	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(my->GetEndServer())
			return 0;
		//======================================================//
		//	���� ������ ������ �� ó��	
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
			debug->Write("IP:[%s] PORT:[%d]��������", strIP, port);
			iocp->fnRemoveSocketInfo(socklist->hSock);
			continue;
		}
		//======================================================//
		//	���� ������ ���� �� ó��	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			debug->Write("�ޱ� �������� ���� ��..");
			_info->wType[0] = 0;
			//Recv ������ ó�� �Լ� ȣ��
			my->RecvHandling(_info,0);
		}
		//======================================================//
		//	���� �����Ͱ� ���� �� ó��	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			debug->Write("������ �������� ���� ��..");
			// ó���� ���� ������ ó��
			_info->wType[1] = 0;
			//Send ������ ó�� �Լ� ȣ��
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
		//	����ó��	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(my->GetEndServer())
			return 0;
		//======================================================//
		//	���� ������ ������ �� ó��	
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
			debug->Write("IP:[%s] PORT:[%d]��������", strIP, port);
			iocp->fnRemoveSocketInfo(socklist->hSock);
			continue;
		}
		//======================================================//
		//	���� ������ ���� �� ó��	
		//=======================================================//
		if(_info->wType[0]&IO_TYPE_RECV)
		{
			debug->Write("�ޱ� �������� ���� ��..");
			_info->wType[0] = 0;
			//Recv ������ ó�� �Լ� ȣ��
			my->RecvHandling(_info,0);
		}
		//======================================================//
		//	���� �����Ͱ� ���� �� ó��	
		//=======================================================//
		else if(_info->wType[1]&IO_TYPE_SEND)
		{
			debug->Write("������ �������� ���� ��..");
			// ó���� ���� ������ ó��
			_info->wType[1] = 0;
			//Send ������ ó�� �Լ� ȣ��
			my->SendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| ���� ������ ó�� �Լ�
//|
//---------------------------------------------------------------
void CServerMg::RecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	������ ������ Ŭ���� 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	������ �з� 	
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
	//	���̺� ó�� ��� 	
	//=======================================================//
	if(bsdt->dwBaseCode & RECVCODE_MYBISERVERRECV)
	{
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode^=RECVCODE_MYBISERVERRECV;
		//======================================================//
		//	ī�� Enable 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
		{

		}
		//======================================================//
		//	����� ����(���� ����) 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
		{

		}
		//======================================================//
		//	�ҵ���� 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
		{

		}
		//======================================================//
		//	��� ���� üũ 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
		{

		}
		bsdt->dwBaseCode|=RECVCODE_VMCLIENTSEND;
		debug->Write("���̺� ó�� ���");
		//���� ������ ����
		//Recv ������ ����
		DATA36* recvdata=(DATA36*)bsdt->data;
		debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d,����Ÿ:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
		//������ ���� Ȯ��
		if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
			debug->Write("������ ���� OK");
		else
			debug->Write("������ ���� NO");
		SOCKETINFO*  socketinfo;
		if(iocp->fnGetSockInfo(&socketinfo, socklist->hlinkSock))
		{
			baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
			DATA36* senddata=(DATA36*)bsdtclt->data;
			SENDDATAINFO* sendinfo=(SENDDATAINFO*)&socketinfo->sendinfo;
			ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
			//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
			memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
			//���� �����͸� ���� ���� ������ �̵������� �����Ѵ�
			iocp->fnRemoveSocketInfo(socklist->hSock);
			//ũ�⸦ ���Ѵ�
			bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // ���� ����ü ũ��
			//������ �� ������ ũ�⸦ �����Ѵ�
			sendinfo->nTotalSendBytes=bsdt->dwdatalen;
			//üũ ������ �Է�
			crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
			//������ ���� Ȯ��
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("������ ���� OK");
			else
				debug->Write("������ ���� NO");
			iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
			//iocp->fnWSARecv(socketinfo->socklist.hSock);
		}
	}
	//======================================================//
	//	VM ó�� ��� 	
	//=======================================================//
	else if(bsdt->dwBaseCode & RECVCODE_VMCLIENTRECV)
	{
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode^=RECVCODE_VMCLIENTRECV;
		//======================================================//
		//	ī�� Enable 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
		{
			debug->Write("ī�� Enable");
			//������ �� �ڵ带 �ɴ´�(���̺� ������)
			bsdt->dwBaseCode|=RECVCODE_MYBISERVERSEND;
			//���� ������ ����
			//Recv ������ ����
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d,����Ÿ:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//üũ ���� 0�� �Ǵ°� Ȯ���Ѵ�(üũ���� 0�� �Ǿ�� ����)
			//������ ���� Ȯ��
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("������ ���� OK");
			else
				debug->Write("������ ���� NO");
			//======================================================//
			//	���̺� ���� ������ ����� ��������	
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
				//ũ�⸦ ���Ѵ�
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // ���� ����ü ũ��
				//������ �� ������ ũ�⸦ �����Ѵ�
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				//üũ ������ �Է�
				crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
				//üũ ���� 0�� �Ǵ°� Ȯ���Ѵ�(üũ���� 0�� �Ǿ�� ����)
				//������ ���� Ȯ��
				if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
					debug->Write("������ ���� OK");
				else
					debug->Write("������ ���� NO");
				iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
				//iocp->fnWSARecv(socketinfo->socklist.hSock);

			}
		}
		//======================================================//
		//	����� ����(���� ����) 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
		{
			debug->Write("���� ����");
			//������ �� �ڵ带 �ɴ´�(���̺� ������)
			bsdt->dwBaseCode|=RECVCODE_MYBISERVERSEND;
			//���� ������ ����
			//Recv ������ ����
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d,����Ÿ:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//üũ ���� 0�� �Ǵ°� Ȯ���Ѵ�(üũ���� 0�� �Ǿ�� ����)
			//������ ���� Ȯ��
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("������ ���� OK");
			else
				debug->Write("������ ���� NO");
			//======================================================//
			//	���̺� ���� ������ ����� ��������	
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
				//ũ�⸦ ���Ѵ�
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // ���� ����ü ũ��
				//������ �� ������ ũ�⸦ �����Ѵ�
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				//üũ ������ �Է�
				crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
				//������ ���� Ȯ��
				if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
					debug->Write("������ ���� OK");
				else
					debug->Write("������ ���� NO");
				iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
				//iocp->fnWSARecv(socketinfo->socklist.hSock);

			}
		}
		//======================================================//
		//	�ҵ���� 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
		{
			debug->Write("�ҵ����");
			//������ �� �ڵ带 �ɴ´�(���̺� ������)
			bsdt->dwBaseCode|=RECVCODE_MYBISERVERSEND;
			//���� ������ ����
			//Recv ������ ����
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d,����Ÿ:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//üũ ���� 0�� �Ǵ°� Ȯ���Ѵ�(üũ���� 0�� �Ǿ�� ����)
			//������ ���� Ȯ��
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("������ ���� OK");
			else
				debug->Write("������ ���� NO");
			//======================================================//
			//	���̺� ���� ������ ����� ��������	
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
				//ũ�⸦ ���Ѵ�
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA36)-2; // ���� ����ü ũ��
				//������ �� ������ ũ�⸦ �����Ѵ�
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				//üũ ������ �Է�
				crc32->GetResult(socketinfo->data,bsdtclt->dwdatalen-4);
				//üũ ���� 0�� �Ǵ°� Ȯ���Ѵ�(üũ���� 0�� �Ǿ�� ����)
				//������ ���� Ȯ��
				if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
					debug->Write("������ ���� OK");
				else
					debug->Write("������ ���� NO");
				iocp->fnWSASend(socketinfo->socklist.hSock, socketinfo->data, bsdt->dwdatalen);
				//iocp->fnWSARecv(socketinfo->socklist.hSock);

			}
		}
		//======================================================//
		//	��� ���� üũ 	
		//=======================================================//
		else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
		{
			debug->Write("��볻��");
			//������ �� �ڵ带 �ɴ´�(VM ������)
			bsdt->dwBaseCode|=RECVCODE_VMCLIENTSEND;
			//======================================================//
			//���� ������ ����
			//Recv ������ ����
			//======================================================//
			DATA36* recvdata=(DATA36*)bsdt->data;
			debug->Write("[���� ������] code:0x%08x,ī��ѹ�:%s,�����ͱ���:%d,����Ÿ:%s",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen,recvdata->info);
			//üũ ���� 0�� �Ǵ°� Ȯ���Ѵ�(üũ���� 0�� �Ǿ�� ����)
			//������ ���� Ȯ��
			if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
				debug->Write("������ ���� OK");
			else
				debug->Write("������ ���� NO");
			//======================================================//
			//	��볻�� ��ȸ�� ���ؼ� ������Ǯ�� �ϳ� ������
			//  ������Ʈ�� �����ؼ� �ִ´�
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
//| ���� ������ ó�� �Լ�
//|
//---------------------------------------------------------------
void CServerMg::SendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
{
	//--------------------------------------------------------------
	//|������ �� �Ǿ����� Ȯ���Ѵ�. �� ���۵��� �ʾ����� ���� ���۵���
	//|���� �����͸� �ٽ� ������. �� ���۵Ǿ����� WSArecv�� ȣ���ؼ� 
	//|�ٽ� �ޱ� ���� �����Ѵ�
	//---------------------------------------------------------------
	//======================================================//
	//	���� ����Ȯ�� 	
	//=======================================================//
	SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
	sendinfo->nSendBytes+=dwTransfer;
	if(sendinfo->nTotalSendBytes>sendinfo->nSendBytes)  //�� ���۵��� �ʾ�����
	{
		//���� �����Ͱ� ������ ������
		debug->Write("���� �����Ͱ� ���� ���Ҵ�");
		iocp->fnWSASend(_info->socklist.hSock, _info->data+sendinfo->nSendBytes, sendinfo->nTotalSendBytes-sendinfo->nSendBytes);
		return;
	}
	else  // �����Ͱ� ���� ���۵� ���
	{
		baseDATA20* bsdt =(baseDATA20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	���̺�� ������ ���� �� 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_MYBISERVERSEND)
		{
			//======================================================//
			//	ī�� Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{

			}
			//======================================================//
			//	����� ����(���� ����) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{

			}
			//======================================================//
			//	�ҵ���� 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{

			}
			//======================================================//
			//	��� ���� üũ 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
			{

			}
		}
		//======================================================//
		//	VM���� ������ ���� �� 	
		//=======================================================//
		else if(bsdt->dwBaseCode & RECVCODE_VMCLIENTSEND)
		{
			//======================================================//
			//	ī�� Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{

			}
			//======================================================//
			//	����� ����(���� ����) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{

			}
			//======================================================//
			//	�ҵ���� 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{

			}
			//======================================================//
			//	��� ���� üũ 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USECHECK)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
				}
				else
				{
					SetEvent(sendinfo->hSendEvent);
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}
			}

		}
	}
}
//--------------------------------------------------------------
//|
//| CompletionPort ��� �Լ�(���� Accept �Լ�)
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
//| ���� ����Ʈ
//|
//---------------------------------------------------------------
HANDLE CServerMg::GetCompletionPort()
{
	return m_hIocp;
}
//--------------------------------------------------------------
//|
//| Ŭ���̾�Ʈ ����Ʈ
//|
//---------------------------------------------------------------
HANDLE CServerMg::GetClntCompletionPort()
{
	return m_hclntIocp;
}
//--------------------------------------------------------------
//|
//| Ŭ���̾�Ʈ ����Ʈ�� �ѱ��
//|
//---------------------------------------------------------------
SOCKET CServerMg::MakeCompetionPortClient(SOCKET hSock)
{
	return iocp->CompletionPortClientSocket(GetClntCompletionPort(), hSock);
}