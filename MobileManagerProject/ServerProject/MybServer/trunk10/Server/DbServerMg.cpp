
#include "stdAfx.h"
#include "DbServerMgCode.h"
#include "DbServerMg.h"

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CDbServerMg::~CDbServerMg()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CDbServerMg::initServerMg(IMyObject* _obj)
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
void CDbServerMg::Release()
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
BOOL CDbServerMg::GetEndServer()
{
	return bEndServer;
}

//--------------------------------------------------------------
//|
//| ������ �����Ѵ�
//|
//---------------------------------------------------------------
BOOL CDbServerMg::ServerStart(u_short nPort)
{
	if(bServerStart)
	{
		debug->Write("�̹� DB�ۼ��� ������ ���۵Ǿ��ִ�");
		return FALSE;
	}
	if((mdbmg==NULL)||(iocp==NULL))
	{
		debug->Write("���� ���� ����");
		return FALSE;
	}
	//=====================================================
	// DB ����
	//======================================================
	if(mdbmg->DbConnect(MYBIDBCONNECT_STR)==FALSE)
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
	// IOCP Server ó�� �����带 �����Ѵ�.
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
	// IOCP Server���� Ŭ���̾�Ʈ ������ �ޱ� ���� ������ ����
	//======================================================
	DWORD dwThreadId2;
	mthread = CreateThread(NULL, 0, ProcessServer, (LPVOID)obj, 0, &dwThreadId2);
	//=====================================================
	// IOCP Client ó�� �����带 �����Ѵ�.
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
	debug->Write("DB ��������");
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
	debug->Write("��������");
}
//--------------------------------------------------------------
//|
//| IOCP Server Worker Thread
//|
//---------------------------------------------------------------
DWORD WINAPI CDbServerMg::WorkerThread(LPVOID lpArg)
{
	//======================================================//
	//	������ �Լ����� �ʿ��� Ŭ���� �����͸� �����Ѵ�	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* dbservermg = obj->GetDbServerMg();
	//======================================================//
	//	IOCP Client �� �ڵ�
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
		//	����ó��	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(dbservermg->GetEndServer())
			return 0;
		//======================================================//
		//	���� ������ ������ �� ó��	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//������ ������ �� ó�� �Լ�
			dbservermg->DisConnectHandling(_info,0);
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
			dbservermg->IocpServerRecvHandling(_info,0);
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
	//	������ �Լ����� �ʿ��� Ŭ���� �����͸� �����Ѵ�	
	//=======================================================//
	IMyObject *obj = (IMyObject *)lpArg;
	IOutput * debug = obj->GetOutput();
	IServerMg* dbservermg = obj->GetDbServerMg();
	//======================================================//
	//	IOCP Client �� �ڵ�
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
		//	����ó��	
		//=======================================================//
		if(_info==NULL)
			return 0;
		if(dbservermg->GetEndServer())
			return 0;
		//======================================================//
		//	���� ������ ������ �� ó��	
		//=======================================================//
		if(iRetval == 0 || dwTransfer == 0) 
		{
			if(iRetval == FALSE)
				debug->err_display("WSAGetOverlappedResult()");
			//������ ������ �� ó�� �Լ�
			dbservermg->DisConnectHandling(_info,0);
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
			dbservermg->IocpClientRecvHandling(_info,0);
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
			dbservermg->IocpClientSendHandling(_info,dwTransfer,0);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| ���� ������ ������ �� ó��	
//|
//---------------------------------------------------------------
BOOL CDbServerMg::DisConnectHandling(SOCKETINFO* _info, DWORD flag)
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
//| ���� ������ ó�� �Լ�
//|
//---------------------------------------------------------------
void CDbServerMg::IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	������ ������ Ŭ���� 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	������ �з� 	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) _info->data;
	//======================================================//
	//	���� ������ ���� 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//�⺻ ������
		debug->Write("[�⺻ ������] code:0x%08x,���̺��:%s,�����ͱ���:%d",bsdt->dwBaseCode, bsdt->TableName,bsdt->dwdatalen);
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
	if((bsdt->dwBaseCode & DATABASECOMM_ERROR))
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
	//	������ ���̽� �����͸� ����	
	//=======================================================//
	if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATARECV)
	{
		debug->Write("ó�� ����");
	}
	//======================================================//
	//	������ �̿� ��û ������ ���� ��	
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYRECV)
	{
		debug->Write("ó�� ����");
	}
	//======================================================//
	//	�� �� ���� ��û ������ ��
	//=======================================================//
	else
	{
		debug->Write("��û ������ �� �� ����");
		debug->Write("ó�� ����");
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode=0;
		//======================================================//
		//	���� ������ ������ ���� ����
		//=======================================================//
		DBError36* recvdata=(DBError36*)bsdt->data;
		//======================================================//
		//	���� �޽��� ���	
		//=======================================================//
		CHAR szSenddata[]="�ڵ� ��";
		memcpy_s(bsdt->TableName,sizeof(bsdt->TableName),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDB20)+sizeof(DBError36)-2, DATABASECOMM_ERROR);
	}
}
//--------------------------------------------------------------
//|
//| ������ ������ �� ó�� �κ�
//|
//---------------------------------------------------------------
void CDbServerMg::IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
		debug->Write("���� �����Ͱ� 0�̴�");
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
		baseDB20* bsdt =(baseDB20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//======================================================//
		//	 	
		//=======================================================//
		if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATASEND)
		{
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
			debug->Write("ó�� ����");
		}
		//======================================================//
		//	��û ���׿� ���ؼ� ó�� �� �۽�	
		//=======================================================//
		else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYSEND)
		{
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
			debug->Write("ó�� ����");
		}
		//======================================================//
		//	������ �۽� 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & DATABASECOMM_ERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���� �� �����ڵ�");
				debug->Write("������ �ޱ� ����");
				debug->Write("ó�� ����");
			}
		}
	}
}
//--------------------------------------------------------------
//|
//| ���� ������ ó�� �Լ�
//|
//---------------------------------------------------------------
void CDbServerMg::IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag)
{
	//======================================================//
	//	������ ������ Ŭ���� 	
	//=======================================================//
	ICrc* crc32 = obj->GetCrc32();
	//======================================================//
	//	������ �з� 	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) _info->data;
	//======================================================//
	//	���� ������ ���� 	
	//=======================================================//
	if(crc32->GetCheck(_info->data,bsdt->dwdatalen))
	{
		//�⺻ ������
		debug->Write("[�⺻ ������] code:0x%08x,���̺��:%s,�����ͱ���:%d",bsdt->dwBaseCode, bsdt->TableName,bsdt->dwdatalen);
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
	//	������������ MYBI ������ ���� �޽��� ����
	//=======================================================//
	if((bsdt->dwBaseCode & DATABASECOMM_ERROR))
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
	//	������������ ������ ��û
	//=======================================================//
	if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATASEND)
	{
		//======================================================//
		//	���� �ڵ� ����	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_DATASEND;
		//	��û ���� ���
		//=======================================================//
		DBdata36* recvdata=(DBdata36*)bsdt->data;
		//======================================================//
		// ������ ���
		//======================================================//
		debug->Write("[������] %s",recvdata->info);
		//======================================================//
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
		if(sendinfo->bafterSenddata)
		{
			if(sendinfo->hSendEvent)
			{
				SetEvent(sendinfo->hSendEvent);
				debug->Write("������ �̺�Ʈ");
			}
			else
			{
				iocp->fnRemoveSocketInfo(socklist->hSock);
			}
		}
		else
		{
			debug->Write("���� �Ϸ� ���� ����");
			iocp->fnRemoveSocketInfo(socklist->hSock);
		}
	}
	//======================================================//
	//	������������ ���� �غ� �Ǿ��ٴ� �ڵ�
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_STARTDATARECV)
	{
		//======================================================//
		//	���� �ڵ� ����	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_STARTDATARECV;
		//	��û ���� ���
		//=======================================================//
		DBdata36* recvdata=(DBdata36*)bsdt->data;
		//======================================================//
		// ������ ���
		//======================================================//
		debug->Write("[������] %s",recvdata->info);
		//======================================================//
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
		if(sendinfo->hSendEvent)
		{
			SetEvent(sendinfo->hSendEvent);
			debug->Write("������ �̺�Ʈ");
		}
		else
		{
			iocp->fnRemoveSocketInfo(socklist->hSock);
		}
	}
	//======================================================//
	//	���� �ڵ带 �� �� ����
	//=======================================================//
	else
	{
		debug->Write("��û ������ �� �� ����");
		debug->Write("ó�� ����");
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode=0;
		//======================================================//
		//	���� ������ ������ ���� ����
		//=======================================================//
		DBError36* senderr=(DBError36*)bsdt->data;
		//======================================================//
		//	���� �޽��� ���	
		//=======================================================//
		CHAR szSenddata[]="�ڵ� ��";
		memcpy_s(senderr->err,sizeof(senderr->err),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDB20)+sizeof(DBError36)-2, DATABASECOMM_ERROR);
	}
}
//--------------------------------------------------------------
//|
//| ������ ������ �� ó�� �κ�
//|
//---------------------------------------------------------------
void CDbServerMg::IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)
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
		baseDB20* bsdt =(baseDB20*) _info->data;
		SOCKLIST* socklist = (SOCKLIST*)&_info->socklist;
		//������ ������ ���� �Է�
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&_info->sendinfo;
		//======================================================//
		//		
		//=======================================================//
		if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATARECV)
		{
			//======================================================//
			//	���� �ڵ� ����	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_DATARECV;
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
			if(sendinfo->bafterSenddata==FALSE)
			{
				SetEvent(sendinfo->hSendEvent);
				debug->Write("Ǯ���� �̺�Ʈ");
			}
		}
		//======================================================//
		//	���� �޽��� �������� ��� 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & DATABASECOMM_ERROR)
			{
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���� �� �����ڵ�");
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
DWORD WINAPI CDbServerMg::ProcessServer(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	IServerMg* dbservermg = obj->GetDbServerMg();
	SOCKET  hResult;
	while(1)
	{
		hResult = dbservermg->MakeCompetionPortServer();
		if(hResult == NULL)
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
SOCKET CDbServerMg::MakeCompetionPortServer()
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
BOOL CDbServerMg::RequestSendData(SOCKETINFO* info, DWORD totalstructlen, DWORD code, HANDLE hEvent, BOOL bafterdata)
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
HANDLE CDbServerMg::GetCompletionPort()
{
	return m_hIocp;
}
//--------------------------------------------------------------
//|
//| Ŭ���̾�Ʈ ����Ʈ
//|
//---------------------------------------------------------------
HANDLE CDbServerMg::GetClntCompletionPort()
{
	return m_hclntIocp;
}
//--------------------------------------------------------------
//|
//| IOCP Ŭ���̾�Ʈ ����
//|
//---------------------------------------------------------------
SOCKET CDbServerMg::MakeCompetionPortClient(SOCKET hSock)
{
	return iocp->CompletionPortClientSocket(GetClntCompletionPort(), hSock,MOBILESERVER_IP, MOBILESERVER_PORT);
}
//--------------------------------------------------------------
//|
//|IOCP ����� �ѱ��
//|
//---------------------------------------------------------------
IServer* CDbServerMg::GetIocpServer()
{
	return iocp;
}
//--------------------------------------------------------------
//|
//|DatabasebMg ����� �ѱ��
//|
//---------------------------------------------------------------
IDatabasebMg* CDbServerMg::GetDatabasebMg()
{
	return mdbmg;
}
//--------------------------------------------------------------
//|
//|DB ������ �����ϱ� ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CDbServerMg::DbDataSend()
{
	//======================================================//
	//	������ ������ �����Ѵ�	
	//=======================================================//
	SOCKET sock;
	sock = MakeCompetionPortClient(0);
	SOCKETINFO*  socketinfo;
	if(iocp&&iocp->fnGetSockInfo(&socketinfo, sock))
	{
		//======================================================//
		//	���̽� ����ü ������ ����	
		//=======================================================//
		baseDB20* bsdtclt =(baseDB20*) socketinfo->data;
		debug->Write("DB ������ ����");
		ZeroMemory(bsdtclt,sizeof(baseDB20));
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
				dbsendobj->SetDbDataInfo(this, sock);
				//������Ǯ�� �߰�
				if(thdpool->Add(dbsendobj)==S_OK)
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}