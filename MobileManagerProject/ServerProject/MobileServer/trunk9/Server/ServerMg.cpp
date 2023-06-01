
#include "stdAfx.h"
#include "ServerMgCode.h"
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
mdbmg(NULL),
m_hIocp(NULL),
mthread(NULL),
m_hclntIocp(NULL),
dwServerThreadCount(0),
dwClientThreadCount(0),
bEndServer(FALSE),
bServerStart(FALSE)
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
	//=====================================================
	// �� ��ü ���� Ŭ���� ������Ʈ
	//======================================================
	obj=_obj;
	//=====================================================
	// ����� ��� Ŭ����
	//======================================================
	debug=obj->GetOutput();
	//=====================================================
	// IOCP  ��ü�� ���� Ŭ�������� �����Ѵ�
	//======================================================
	iocp=obj->CreateIocpServer();
	if(iocp==NULL)
	{
		debug->Write("IOCP ���� ���� ����");
	}
	//=====================================================
	// DB ���� �޴��� �������� ���� Ŭ�������� �����Ѵ�
	//======================================================
	mdbmg = obj->CreateDatabaseMg();
	if(mdbmg==NULL)
	{
		debug->Write("DB �޴��� ���� ����");
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CServerMg::Release()
{
	//=====================================================
	// ������ ���� �÷���
	//======================================================
	bEndServer=TRUE;
	//=====================================================
	// IOCP �ڵ�
	//======================================================
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
	//=====================================================
	// IOCP  ��ü �޸� ����
	//======================================================
	obj->ReleaseObject(iocp);
	//=====================================================
	// DB ����  ��ü �޸�����
	//======================================================
	obj->ReleaseObject(mdbmg);
	bServerStart=FALSE;
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
//| ������ �����Ѵ�
//|
//---------------------------------------------------------------
BOOL CServerMg::ServerStart(u_short nPort)
{
	if(bServerStart)
	{
		debug->Write("�̹� ������ ���۵Ǿ��ִ�");
		return TRUE;
	}
	if((mdbmg==NULL)||(iocp==NULL))
	{
		debug->Write("���� ���� ����");
		return FALSE;
	}
	//=====================================================
	// DB ����
	//======================================================
	if(mdbmg->DbConnect(DBCONNECT_STR)==FALSE)
	{
		MessageBox(NULL, "DB����","�����ͺ��̽� ���ῡ��",MB_OK);
		return FALSE;
	}
	if(nPort < 5000 || nPort > 10000)
	{
		MessageBox(NULL, "��Ʈ ��ȣ ����(5001 - 9999)", "����", MB_ICONINFORMATION);
		return FALSE;
	}
	if(iocp->ServerListen(nPort)==FALSE)
		return FALSE;
	//=====================================================
	// ������ IOCP �ڵ� �����Ѵ�
	//======================================================
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hIocp == NULL )
	{
		MessageBox(NULL, "IOCP ���� ����", "�˸�", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}
	//=====================================================
	// Ŭ���̾�Ʈ�� IOCP �ڵ� �����Ѵ�
	//======================================================
	m_hclntIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	if( m_hclntIocp == NULL )
	{
		MessageBox(NULL, "IOCP ���� ����", "�˸�", MB_OK );
		iocp->fnCloseSocket();
		return FALSE;
	}
	//=====================================================
	// CPU ������ Ȯ���Ͽ� �����带 �����ϱ� ���� �κ�
	//======================================================
	SYSTEM_INFO si;
	GetSystemInfo( &si ); // CPU ���� Ȯ��
	//=====================================================
	// IOCP Server ó�� �����带 �����Ѵ�.
	//======================================================
	// CPU * IOCP_SERVERCOUNT ���� �۾� ������ ����
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
	// IOCP Server���� Ŭ���̾�Ʈ ������ �ޱ� ���� ������ ����
	//======================================================
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);
	//=====================================================
	// IOCP Client ó�� �����带 �����Ѵ�.
	//======================================================
	// CPU * IOCP_CLIENTCOUNT ���� �۾� ������ ����
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
	::CloseHandle(m_hIocp); 
	m_hIocp = INVALID_HANDLE_VALUE; 
	iocp->fnCloseSocket();
	debug->Write("��������");
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
	//	������ �Լ����� �ʿ��� Ŭ���� �����͸� �����Ѵ�	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* servermg = obj->GetServerMg();
	//======================================================//
	//	IOCP Client �� �ڵ�
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
		//	����ó��	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(servermg->GetEndServer())
			return 0;
		//======================================================//
		//	���� ������ ������ �� ó��	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//������ ������ �� ó�� �Լ�
			servermg->DisConnectHandling(_info,0);
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
			servermg->IocpServerRecvHandling(_info,0);
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
	//	������ �Լ����� �ʿ��� Ŭ���� �����͸� �����Ѵ�	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* servermg = obj->GetServerMg();
	//======================================================//
	//	IOCP Client �� �ڵ�
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
		//	����ó��	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(servermg->GetEndServer())
			return 0;
		//======================================================//
		//	���� ������ ������ �� ó��	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//������ ������ �� ó�� �Լ�
			servermg->DisConnectHandling(_info,0);
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
			servermg->IocpClientRecvHandling(_info,0);
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
			servermg->IocpClientSendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| ���� ������ ������ �� ó��	
//|
//---------------------------------------------------------------
BOOL CServerMg::DisConnectHandling(SOCKETINFO* _info, DWORD flag)
{
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	SOCKETINFO*  socketinfo;
	if(iocp->fnGetSockInfo(&socketinfo, socklist->hSock))
	{
		//======================================================//
		//	���� ���� ���
		//=======================================================//
		Sockinfodisplay(socklist->hSock);
		//======================================================//
		//	���� ���� ����
		//=======================================================//
		iocp->fnRemoveSocketInfo(socklist->hSock);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| ���� ���� ���
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
//| ���� ������ ó�� �Լ�
//|
//---------------------------------------------------------------
void CServerMg::IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	������ ������ Ŭ���� 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	������ �з� 	
	//=======================================================//
	baseDATA20* bsdt =(baseDATA20*) _info->data;
	//======================================================//
	//	���� ������ ���� 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//�⺻ ������
		debug->Write("[�⺻ ������] code:0x%08x,���̺��:%s,�����ͱ���:%d",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
	}
	//======================================================//
	//	���� ����Ʈ	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	if(socklist->hSock==0)
	{
		debug->Write("��� ������ ����");
		return;
	}
	//======================================================//
	//	���� �����Ϳ� ������ �ִٴ� �ڵ带 �������� ��	
	//=======================================================//
	if((bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)||(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)||(bsdt->dwBaseCode & RECVCODE_DATAERROR)||(bsdt->dwBaseCode & RECVCODE_BASEERROR))
	{
		debug->Write("���� ��Ȳ �Ǵ��ؼ� ó��");
		iocp->fnWSARecv(socklist->hSock);
		return;
	}
	//======================================================//
	//	���� ���� ���� ��� 	
	//=======================================================//
	Sockinfodisplay(socklist->hSock);
	//======================================================//
	//	VM���� ���� ������ ó�� 	
	//=======================================================//
	if(bsdt->dwBaseCode & RECVCODE_VMCLIENTRECV)
	{
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode^=RECVCODE_VMCLIENTRECV;
		if((bsdt->dwBaseCode & RECVCODE_CARDENABLE)||(bsdt->dwBaseCode & RECVCODE_USERCHANGE)||(bsdt->dwBaseCode & RECVCODE_DEDUCTION))
		{
			//======================================================//
			//	���̺� ���� ������ ����� ��������	
			//=======================================================//
			SOCKET sock;
			sock = MakeCompetionPortClient(socklist->hSock);
			SOCKETINFO*  socketinfo;
			if(iocp&&iocp->fnGetSockInfo(&socketinfo, sock))
			{
				//���̽� ����ü ������ ����
				baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
				//======================================================//
				//	ī�� Enable 	
				//=======================================================//
				if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
				{
					//���� ���� ���
					DATA36* recvdata=(DATA36*)bsdt->data;
					//������ ������
					debug->Write("[ī��Enable ������] %s",recvdata->info);
					//������ ������ ����ü ����
					DATA36* senddata=(DATA36*)bsdtclt->data;
					//======================================================//
					//	������ �� ���� ����ҷ� ������ �ű��(MYBI�� �����͸� VM���� �ű��) 	
					//=======================================================//
					//MYBI �����͸� VM���� �ű��
					//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
					//�̺κп��� ����ü�� Ʋ�� ��� �ٸ��� ó���� �� �ֵ��� ����
					memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
					//======================================================//
					//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
					//=======================================================//
					RequestSendData(socketinfo,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_MYBISERVERSEND);
				}
				//======================================================//
				//	����� ����(���� ����) 	
				//=======================================================//
				else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
				{
					//���� ���� ���
					DATA36* recvdata=(DATA36*)bsdt->data;
					//������ ������
					debug->Write("[�������� ������] %s",recvdata->info);
					//������ ������ ����ü ����
					DATA36* senddata=(DATA36*)bsdtclt->data;
					//======================================================//
					//	������ �� ���� ����ҷ� ������ �ű��(MYBI�� �����͸� VM���� �ű��) 	
					//=======================================================//
					//MYBI �����͸� VM���� �ű��
					//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
					//�̺κп��� ����ü�� Ʋ�� ��� �ٸ��� ó���� �� �ֵ��� ����
					memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
					//======================================================//
					//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
					//=======================================================//
					RequestSendData(socketinfo,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_MYBISERVERSEND);
				}
				//======================================================//
				//	�ҵ���� 	
				//=======================================================//
				else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
				{
					//���� ���� ���
					DATA36* recvdata=(DATA36*)bsdt->data;
					//������ ������
					debug->Write("[�ҵ���� ������] %s",recvdata->info);
					//������ ������ ����ü ����
					DATA36* senddata=(DATA36*)bsdtclt->data;
					//======================================================//
					//	������ �� ���� ����ҷ� ������ �ű��(MYBI�� �����͸� VM���� �ű��) 	
					//=======================================================//
					//MYBI �����͸� VM���� �ű��
					//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
					//�̺κп��� ����ü�� Ʋ�� ��� �ٸ��� ó���� �� �ֵ��� ����
					memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
					//======================================================//
					//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
					//=======================================================//
					RequestSendData(socketinfo,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_MYBISERVERSEND);
				}
			}
		}
		else
		{

			//======================================================//
			//	��� ���� üũ 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_USECHECK)
			{
				debug->Write("��볻��");
				//======================================================//
				//Recv ������ ����
				//======================================================//
				DATA36* recvdata=(DATA36*)bsdt->data;
				//������ ������
				debug->Write("[��볻�� ��ȸ ������] %s",recvdata->info);
				//======================================================//
				//	��볻�� ��ȸ�� ���ؼ� ������Ǯ�� �ϳ� ������
				//  ������Ʈ�� �����ؼ� �ִ´�
				//=======================================================//
				IThreadPool* thdpool = obj->GetThreadPool();
				if(thdpool)
				{
					//������Ǯ ��� ������Ʈ(��� ���� ��ȸ ������ ���� ������Ʈ)
					IThreadObjectBase* dbsendobj = obj->CreateDbSendPoolObject();
					if(dbsendobj)
					{
						//���巡�� ��� ������Ʈ �ʱ�ȭ
						dbsendobj->initThreadObject(obj);
						//�������� ���� ���� ������ ����
						dbsendobj->SetDbDataInfo(this, socklist->hSock, bsdt->szCardNum);
						//������Ǯ�� �߰�
						if(thdpool->Add(dbsendobj)==S_OK)
						{
							return;
						}
					}
				}
				debug->Write("��� ���� ������ ���� ����");
				//======================================================//
				//	���� �����͸� ������ ���� ���� 	
				//=======================================================//
				ERROR36* senderr=(ERROR36*)bsdt->data;
				//======================================================//
				//	���� �޽����� ����Ѵ�	
				//=======================================================//
				CHAR szSenddata[]="������ Ǯ ��� ����";
				memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
				//======================================================//
				//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
				//=======================================================//
				RequestSendData(_info,sizeof(baseDATA20)+sizeof(ERROR36)-2, RECVCODE_BASEERROR);
			}
			else
			{
				debug->Write("������ ó�� �Һи�");
				//======================================================//
				//	���� �����͸� ������ ���� ���� 	
				//=======================================================//
				ERROR36* senderr=(ERROR36*)bsdt->data;
				//======================================================//
				//	���� �޽����� ����Ѵ�	
				//=======================================================//
				CHAR szSenddata[]="������ó�� �ڵ� �Һи�";
				memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
				//======================================================//
				//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
				//=======================================================//
				RequestSendData(_info,sizeof(baseDATA20)+sizeof(ERROR36)-2, RECVCODE_WHATDATAERROR);
			}

		}
	}
	//======================================================//
	//	VM ������ ���� �ڵ��� �˼� ����
	//=======================================================//
	else
	{
		debug->Write("���� �� �ڵ� �Һи� ������ ó��");
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode=0;
		//======================================================//
		//	���� ������ ������ ���� ����
		//=======================================================//
		ERROR36* senderr=(ERROR36*)bsdt->data;
		//======================================================//
		//	���� �޽��� ���	
		//=======================================================//
		CHAR szSenddata[]="������ �� �ڵ� �Һи�";
		memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDATA20)+sizeof(ERROR36)-2, RECVCODE_WHERECORDERROR);
	}
}
//--------------------------------------------------------------
//|
//| ������ ������ �� ó�� �κ�
//|
//---------------------------------------------------------------
void CServerMg::IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
	if(sendinfo->nTotalSendBytes==0)
	{
		return;
	}
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
		//======================================================//
		//	������ ������ ���������� ���������� ������ �ʱ�ȭ �ؼ� �ٽ� ������ �ʰ� �Ѵ� 	
		//=======================================================//
		sendinfo->nTotalSendBytes=0;
		sendinfo->nSendBytes=0;
		//======================================================//
		//	���� ������ ������ ����ü�� �����Ѵ�	
		//=======================================================//
		baseDATA20* bsdt =(baseDATA20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	VM���� ������ ���� �� 	
		//=======================================================//
		if(bsdt->dwBaseCode & RECVCODE_VMCLIENTSEND)
		{
			//======================================================//
			//	ī�� Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("������ �ޱ� ����");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}

			}
			//======================================================//
			//	����� ����(���� ����) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("������ �ޱ� ����");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}

			}
			//======================================================//
			//	�ҵ���� 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("������ �ޱ� ����");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}
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
		//======================================================//
		//	���� �޽��� �������� ��� 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���� �� �����ڵ�");
				debug->Write("������ �ޱ� ����");

			}
			else if(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("������ ó�� �����ڵ�");
				debug->Write("������ �ޱ� ����");
			}
			else if(bsdt->dwBaseCode & RECVCODE_DATAERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���������Ϳ� ������ �ִ�");
				debug->Write("������ �ޱ� ����");
			}
			else if(bsdt->dwBaseCode & RECVCODE_BASEERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("ó���� ���� �߻�");
				debug->Write("������ �ޱ� ����");
			}
			else
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("�����ڵ� �˼� ���� ����� ������ �ȵȴ�");
				debug->Write("������ �ޱ� ����");
			}
		}
	}
}
//--------------------------------------------------------------
//|
//| ���� ������ ó�� �Լ�
//|
//---------------------------------------------------------------
void CServerMg::IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	������ ������ Ŭ���� 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	������ �з� 	
	//=======================================================//
	baseDATA20* bsdt =(baseDATA20*) _info->data;
	//======================================================//
	//	���� ������ ���� 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//�⺻ ������
		debug->Write("[�⺻ ������] code:0x%08x,���̺��:%s,�����ͱ���:%d",bsdt->dwBaseCode, bsdt->szCardNum,bsdt->dwdatalen);
	}
	//======================================================//
	//	���� ����Ʈ	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
	if(socklist->hSock==0)
	{
		debug->Write("��� ������ ����");
		return;
	}
	//======================================================//
	//	���� �����Ϳ� ������ �ִٴ� �ڵ带 �������� ��	
	//=======================================================//
	if((bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)||(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)||(bsdt->dwBaseCode & RECVCODE_DATAERROR)||(bsdt->dwBaseCode & RECVCODE_BASEERROR))
	{
		debug->Write("���� ��Ȳ �Ǵ��ؼ� ó��");
		iocp->fnWSARecv(socklist->hSock);
		return;
	}
	//======================================================//
	//	���� ���� ���� ��� 	
	//=======================================================//
	Sockinfodisplay(socklist->hSock);
	//======================================================//
	//	���̺񿡼� ���� ������ ó��	
	//=======================================================//
	if(bsdt->dwBaseCode & RECVCODE_MYBISERVERRECV)
	{
		debug->Write("���̺� ó�� ���");
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode^=RECVCODE_MYBISERVERRECV;
		//MYBI�� ó�� ����� VM���� ������ ���ؼ� ������ ���������� �����´�
		//���� ������ ���� �������� ���� ã�´�
		SOCKETINFO*  socketinfo;
		if(iocp->fnGetSockInfo(&socketinfo, socklist->hlinkSock))
		{
			//���̽� ����ü ������ ����
			baseDATA20* bsdtclt =(baseDATA20*) socketinfo->data;
			//======================================================//
			//	ī�� Enable 	
			//=======================================================//
			if(bsdt->dwBaseCode & RECVCODE_CARDENABLE)
			{
				//���� ���� ���
				DATA36* recvdata=(DATA36*)bsdt->data;
				//������ ������
				debug->Write("[ī��Enable ������] %s",recvdata->info);
				//������ ������ ����ü ����
				DATA36* senddata=(DATA36*)bsdtclt->data;
				//======================================================//
				//	������ �� ���� ����ҷ� ������ �ű��(MYBI�� �����͸� VM���� �ű��) 	
				//=======================================================//
				//MYBI �����͸� VM���� �ű��
				//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
				//�̺κп��� ����ü�� Ʋ�� ��� �ٸ��� ó���� �� �ֵ��� ����
				memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
				//======================================================//
				//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
				//=======================================================//
				RequestSendData(socketinfo,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_VMCLIENTSEND);
			}
			//======================================================//
			//	����� ����(���� ����) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{
				//���� ���� ���
				DATA36* recvdata=(DATA36*)bsdt->data;
				//������ ������
				debug->Write("[�������� ������] %s",recvdata->info);
				//������ ������ ����ü ����
				DATA36* senddata=(DATA36*)bsdtclt->data;
				//======================================================//
				//	������ �� ���� ����ҷ� ������ �ű��(MYBI�� �����͸� VM���� �ű��) 	
				//=======================================================//
				//MYBI �����͸� VM���� �ű��
				//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
				//�̺κп��� ����ü�� Ʋ�� ��� �ٸ��� ó���� �� �ֵ��� ����
				memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
				//======================================================//
				//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
				//=======================================================//
				RequestSendData(socketinfo,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_VMCLIENTSEND);
			}
			//======================================================//
			//	�ҵ���� 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{
				//���� ���� ���
				DATA36* recvdata=(DATA36*)bsdt->data;
				//������ ������
				debug->Write("[�ҵ���� ������] %s",recvdata->info);
				//������ ������ ����ü ����
				DATA36* senddata=(DATA36*)bsdtclt->data;
				//======================================================//
				//	������ �� ���� ����ҷ� ������ �ű��(MYBI�� �����͸� VM���� �ű��) 	
				//=======================================================//
				//MYBI �����͸� VM���� �ű��
				//���� ���� ������ �����ϱ� ���� �����͸� �̵��Ѵ�
				//�̺κп��� ����ü�� Ʋ�� ��� �ٸ��� ó���� �� �ֵ��� ����
				memcpy_s(socketinfo->data,sizeof(socketinfo->data),&_info->data, sizeof(_info->data));
				//======================================================//
				//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
				//=======================================================//
				RequestSendData(socketinfo,sizeof(baseDATA20)+sizeof(DATA36)-2, RECVCODE_VMCLIENTSEND);
			}
			//======================================================//
			//	MYBI ������ ���� �Ѵ�(��û ó���� ��������)	
			//=======================================================//
			//���� �����͸� ���� ���� ������ �̵������� �����Ѵ�(MYBI ���� ����)
			iocp->fnRemoveSocketInfo(socklist->hSock);
		}
	}
	//======================================================//
	//	MYBI ������ ���� �ڵ��� �˼� ����
	//=======================================================//
	else
	{
		debug->Write("���� �� �ڵ� �Һи� ������ ó��");
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode=0;
		//======================================================//
		//	���� ������ ������ ���� ����
		//=======================================================//
		ERROR36* senderr=(ERROR36*)bsdt->data;
		//======================================================//
		//	���� �޽��� ���	
		//=======================================================//
		CHAR szSenddata[]="������ �� �ڵ� �Һи�";
		memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDATA20)+sizeof(ERROR36)-2, RECVCODE_WHERECORDERROR);
	}
}
//--------------------------------------------------------------
//|
//| ������ ������ �� ó�� �κ�
//|
//---------------------------------------------------------------
void CServerMg::IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
	if(sendinfo->nTotalSendBytes==0)
	{
		return;
	}
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
		//======================================================//
		//	������ ������ ���������� ���������� ������ �ʱ�ȭ �ؼ� �ٽ� ������ �ʰ� �Ѵ� 	
		//=======================================================//
		sendinfo->nTotalSendBytes=0;
		sendinfo->nSendBytes=0;
		//======================================================//
		//	���� ������ ������ ����ü�� �����Ѵ�	
		//=======================================================//
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
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("������ �ޱ� ����");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}

			}
			//======================================================//
			//	����� ����(���� ����) 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_USERCHANGE)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("������ �ޱ� ����");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}

			}
			//======================================================//
			//	�ҵ���� 	
			//=======================================================//
			else	if(bsdt->dwBaseCode & RECVCODE_DEDUCTION)
			{
				if(sendinfo->bafterSenddata)
				{
					if(sendinfo->hSendEvent)
					{
						SetEvent(sendinfo->hSendEvent);
						debug->Write("������ �̺�Ʈ");
					}
					else
					{
						iocp->fnWSARecv(socklist->hSock);
						debug->Write("������ �ޱ� ����");
					}
				}
				else
				{
					iocp->fnWSARecv(socklist->hSock);
					debug->Write("������ �ޱ� ����");
				}

			}
		}
		//======================================================//
		//	���� �޽��� �������� ��� 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & RECVCODE_WHERECORDERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���� �� �����ڵ�");
				debug->Write("������ �ޱ� ����");

			}
			else if(bsdt->dwBaseCode & RECVCODE_WHATDATAERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("������ ó�� �����ڵ�");
				debug->Write("������ �ޱ� ����");
			}
			else if(bsdt->dwBaseCode & RECVCODE_DATAERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���������Ϳ� ������ �ִ�");
				debug->Write("������ �ޱ� ����");
			}
			else if(bsdt->dwBaseCode & RECVCODE_BASEERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("ó���� ���� �߻�");
				debug->Write("������ �ޱ� ����");
			}
			else
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("�����ڵ� �˼� ���� ����� ������ �ȵȴ�");
				debug->Write("������ �ޱ� ����");
			}
		}
	}
}
//--------------------------------------------------------------
//|
//| CompletionPort ��� ������ �Լ�
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
//| CompletionPort ��� �Լ�(���� Accept �Լ�)
//|
//---------------------------------------------------------------
SOCKET CServerMg::MakeCompetionPortServer()
{
	//======================================================//
	//	Ŭ���̾�Ʈ ���ӽ�
	//=======================================================//
	SOCKET  hResult = iocp->CompletionPortServerSocket(GetCompletionPort());
	//======================================================//
	//	������ ���� ���·� ����	
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
	//	������ ������ Ŭ���� 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	�⺻ ������ Ŭ���� ����	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) info->data;
	//======================================================//
	//	���� ����Ʈ	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&info->socklist;

	//======================================================//
	//	���� ����ü �� ũ��
	//=======================================================//
	bsdt->dwdatalen=totalstructlen; // ���� ����ü ũ��
	//======================================================//
	//	���� ������ ��û �ڵ�	| ����
	//=======================================================//
	bsdt->dwBaseCode|=code;
	//======================================================//
	//	������ ���� ������ ����Ѵ�(������ ���¿��� ���� ����)
	//=======================================================//
	//������ ������ ����ϱ� ���� ����ü
	SENDDATAINFO* sendinfo=(SENDDATAINFO*)&info->sendinfo;
	ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
	//������ �� ������ ũ�⸦ �����Ѵ�
	//���� �� ������ ũ��
	sendinfo->nTotalSendBytes=totalstructlen;
	//������ ���� ������(�ʱ� ���ÿ��� 0����)
	sendinfo->nSendBytes=0;
	//������ �ٽ� ���� �����Ͱ� �ִ°�?(������ FALSE)
	sendinfo->bafterSenddata=bafterdata;
	//�̺�Ʈ ����
	sendinfo->hSendEvent=hEvent;
	//======================================================//
	//	���� ������ �Է�
	//=======================================================//
	crc32->SetCheckResult(info->data,bsdt->dwdatalen-4);
	//������ ���� Ȯ��
	crc32->GetCheck(info->data,bsdt->dwdatalen);
	//������ ������ �ٽ� ������
	iocp->fnWSASend(socklist->hSock, info->data, bsdt->dwdatalen);
	return TRUE;
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
//| IOCP Ŭ���̾�Ʈ�� ����
//|
//---------------------------------------------------------------
SOCKET CServerMg::MakeCompetionPortClient(SOCKET hSock)
{
	return iocp->CompletionPortClientSocket(GetClntCompletionPort(), hSock,MB_IP, MB_PORT);
}
//--------------------------------------------------------------
//|
//|IOCP ����� �ѱ��
//|
//---------------------------------------------------------------
IServer* CServerMg::GetIocpServer()
{
	return iocp;
}
//--------------------------------------------------------------
//|
//|DatabasebMg ����� �ѱ��
//|
//---------------------------------------------------------------
IDatabasebMg* CServerMg::GetDatabasebMg()
{
	return mdbmg;
}