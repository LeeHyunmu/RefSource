
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
		debug->Write("code:%d",bsdt->dwBaseCode);
		//======================================================//
		//	���� �ڵ� ����	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_DATARECV;
		if(adorecord)
		{
			//��볻�� ��ȸ ���� ����ü ����
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
				debug->Write("������ ���� �Ϸ� �ڵ� �޾Ҵ�");
			}
			debug->Write("�����͸� �� �޾Ҵ�");
			debug->Write("���� ������ ��û");
		}
		//======================================================//
		//	������ ��û�ϱ����� ����
		//=======================================================//
		DBdata36* senddata=(DBdata36*)bsdt->data;
		//======================================================//
		//	������ ���	
		//=======================================================//
		CHAR szSenddata[]="������ ��ȣ";
		memcpy_s(senddata->info,sizeof(senddata->info),szSenddata, sizeof(szSenddata));
		//======================================================//
		//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
		//=======================================================//
		RequestSendData(_info,sizeof(baseDB20)+sizeof(DBdata36)-2, MYBIDATABASECOMM_DATASEND);
	}
	//======================================================//
	//	������ �̿� ��û ������ ���� ��	
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYRECV)
	{
		//======================================================//
		//	���� �ڵ� ����	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_IDENTIFYRECV;
		debug->Write("code:%d",bsdt->dwBaseCode);
	}
	//======================================================//
	//	������ �̿� ��û ������ ���� ��	
	//=======================================================//
	else if(bsdt->dwBaseCode & MYBIDATABASECOMM_STARTDATARECV)
	{
		//======================================================//
		//	���� �ڵ� ����	
		//=======================================================//
		bsdt->dwBaseCode^=MYBIDATABASECOMM_STARTDATARECV;
		debug->Write("code:%d",bsdt->dwBaseCode);
	}
	//======================================================//
	//	�� �� ���� ��û ������ ��
	//=======================================================//
	else
	{
		debug->Write("code:%d",bsdt->dwBaseCode);
		debug->Write("��û ������ �� �� ����");
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
		CHAR szSenddata[]="���� ��û�ΰ�?";
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
		//	������ ���̽� �����͸� ���������� �������� �� 	
		//=======================================================//
		if(bsdt->dwBaseCode & MYBIDATABASECOMM_DATASEND)
		{
			//======================================================//
			//	���� �ڵ带 ����	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_DATASEND;
			//======================================================//
			//	�ޱ� ��� ����	
			//=======================================================//
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
		}
		//======================================================//
		//	���� �غ� �Ǿ��ٴ� �޽����� ������ ��
		//=======================================================//
		else if(bsdt->dwBaseCode & MYBIDATABASECOMM_STARTDATARECV)
		{
			//======================================================//
			//	���� �ڵ带 ����	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_STARTDATARECV;
			//======================================================//
			//	�ޱ� ��� ����	
			//=======================================================//
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
		}
		//======================================================//
		//	��û ���׿� ���ؼ� ó�� �� �۽�	
		//=======================================================//
		else if(bsdt->dwBaseCode & MYBIDATABASECOMM_IDENTIFYSEND)
		{
			//======================================================//
			//	���� �ڵ带 ����	
			//=======================================================//
			bsdt->dwBaseCode^=MYBIDATABASECOMM_IDENTIFYSEND;
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
		}
		//======================================================//
		//	������ �۽� 	
		//=======================================================//
		else
		{
			if(bsdt->dwBaseCode & DATABASECOMM_ERROR)
			{
				//======================================================//
				//	���� �ڵ带 ����	
				//=======================================================//
				bsdt->dwBaseCode=DATABASECOMM_ERROR;
				iocp->fnWSARecv(socklist->hSock);
				debug->Write("���� �� �����ڵ�");
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
	//	������������ MYBI ������ ��û������ ���� ��	
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
	//	������������ MYBI ������ ��û���׿� ���� ����
	//=======================================================//
	if(bsdt->dwBaseCode & MOBILEDATABASECOMM_IDENTIFYRECV)
	{
		//======================================================//
		//	���� �ڵ带 ����	
		//=======================================================//
		bsdt->dwBaseCode^=MOBILEDATABASECOMM_IDENTIFYRECV;
	}
	//======================================================//
	//	���� �ڵ带 �� �� ����
	//=======================================================//
	else
	{
		//�ڵ忡�� ���� ���� �ڵ带 ����
		//�ٽ� ������ ���ϵ��� ��
		bsdt->dwBaseCode=0;
		debug->Write("��û�� ���� ó�� �ڵ带 �� �� ����");
		//======================================================//
		//	���� ������ ������ ���� ����
		//=======================================================//
		DBError36* senderr=(DBError36*)bsdt->data;
		//======================================================//
		//	���� �޽��� ���	
		//=======================================================//
		CHAR szSenddata[]="���� ��û�ΰ�?";
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
		//======================================================//
		//	������ ��û ���� ��	
		//=======================================================//
		if(bsdt->dwBaseCode & MOBILEDATABASECOMM_IDENTIFYSEND)
		{
			//======================================================//
			//	���� �ڵ带 ����	
			//=======================================================//
			bsdt->dwBaseCode^=MOBILEDATABASECOMM_IDENTIFYSEND;
			iocp->fnWSARecv(socklist->hSock);
			debug->Write("������ �ޱ� ����");
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
			//======================================================//
			//	���� �ڵ带 ����	
			//=======================================================//
			bsdt->dwBaseCode=0;
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
//���̺��� �ִ°��� Ȯ���ϴ� ����
 #define  TABLENAMEFIND "select 'Name' = o.name,'Owner' = user_name(uid),'Object_type' = substring(v.name,5,31) \
 from sysobjects o, master.dbo.spt_values v where o.xtype = substring(v.name,1,2) and v.type = 'O9T'\
 and v.name like 'U%' order by Object_type desc, Name asc"
SOCKET CDbServerMg::MakeCompetionPortServer()
{
	//======================================================//
	// Ŭ���̾�Ʈ ���� �� ������ ����ϴ� �Լ�	
	//=======================================================//
	SOCKET hResult = iocp->CompletionPortServerSocket(GetCompletionPort());
	//======================================================//
	// ���� ���� �����͸� ��´�
	//=======================================================//
	SOCKETINFO* info;
	iocp->fnGetSockInfo(&info,hResult);
	//======================================================//
	//	������ �з� 	
	//=======================================================//
	baseDB20* bsdt =(baseDB20*) info->data;
	//======================================================//
	//	���� ����Ʈ	
	//=======================================================//
	SOCKLIST* socklist = (SOCKLIST*)&info->socklist;
	//======================================================//
	//	���̺� ����(��¥�� ����) �̸� ��� ���ؼ� 
	//=======================================================//
	COleDateTime datetime = COleDateTime::GetCurrentTime();
	memset(tablename,0,sizeof(tablename));
	sprintf_s(tablename,_T("traffic_%d%02d%02d"),  datetime.GetYear(), datetime.GetMonth(),	datetime.GetDay());
	//======================================================//
	//	���̺��� �˻��ؼ� ���� �̸��� �ִ°� �˾ƺ��� 
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
				debug->Write("���̺� Ȯ�� => name:%s, owner:%s, object_type:%s", name, owner, object_type);
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
	// ���� ���̺� �̸��� ������ �߰� ����
	if(bIstable==FALSE)
		mdbmg->Trans("select* into ","%s from trafficinfo_base", tablename);
	//======================================================//
	//	���̺� ���ڵ� ����
	//=======================================================//
	adorecord=NULL;
	if(mdbmg->RecordsetSetting(tablename))
		adorecord = mdbmg->GetRecordsetPoint();

	debug->Write("������ ��û");
	//======================================================//
	//	������ ��û�ϱ����� ����
	//=======================================================//
	DBdata36* senddata=(DBdata36*)bsdt->data;
	//======================================================//
	//	������ ���	
	//=======================================================//
	CHAR szSenddata[]="���� �غ� �Ϸ�";
	memcpy_s(senddata->info,sizeof(senddata->info),szSenddata, sizeof(szSenddata));
	//======================================================//
	//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
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
	return iocp->CompletionPortClientSocket(GetClntCompletionPort(), hSock,DBSERVER_IP, DBSERVER_PORT);
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