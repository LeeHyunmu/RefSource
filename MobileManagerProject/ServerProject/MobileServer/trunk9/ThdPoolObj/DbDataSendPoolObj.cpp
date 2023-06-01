
#include "stdAfx.h"
#include "../Server/ServerMgCode.h"
#include "DbDataSendPoolObj.h"

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

LONG CDbDataSendPoolObj::s_lNext = 0;
LONG CDbDataSendPoolObj::s_lCount = 0;
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CDbDataSendPoolObj::CDbDataSendPoolObj():
obj(NULL),
debug(NULL),
record(NULL),
iocp(NULL),
m_n(0),
thdid(0),
hSendEvent(NULL)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CDbDataSendPoolObj::~CDbDataSendPoolObj()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CDbDataSendPoolObj::Release()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CDbDataSendPoolObj::initThreadObject(IMyObject* _obj)
{
	//=====================================================
	// �� ��ü ���� Ŭ���� ������Ʈ
	//======================================================
	obj=_obj;
	//=====================================================
	// ����� ��� Ŭ����
	//======================================================
	debug=obj->GetOutput();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|db ������ ���� ���� ����
//|
//---------------------------------------------------------------
HANDLE CDbDataSendPoolObj::SetDbDataInfo(IServerMg* servermg, SOCKET _sock, CString _where)
{
	m_n = InterlockedIncrement(&s_lNext);
	InterlockedIncrement(&s_lCount);
	sock = _sock;
	hSendEvent=NULL;
	hSendEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	iocp=servermg->GetIocpServer();
	record=NULL;
	if(servermg->GetDatabasebMg()->RecordsetSetting(5, "select* from dbo.trafficinfo","where usecardnum='%s'", _where))
		record = servermg->GetDatabasebMg()->GetRecordsetPoint();
	if(record==NULL)
	{
		CloseHandle(hSendEvent); hSendEvent=NULL;
		debug->Write("Db ���� ���� ����");
	}
	return hSendEvent;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDbDataSendPoolObj::Run(IThreadPoolThreadCallback &pool)
{
	thdid = GetCurrentThreadId();
	debug->Write("������ID:%d", thdid);
	register int nPageIndex = 1;
	if(sock)
	{
		SOCKETINFO* sockinfo;
		iocp->fnGetSockInfo(&sockinfo, sock);
		//======================================================//
		//	���� ����ü ���̽� ����	
		//=======================================================//
		baseDATA20*    bsdt=(baseDATA20*)sockinfo->data;
		//=======================================================//
		//���� �������� ����ü ����
		//=======================================================//
		DBdata162*    senddata=(DBdata162*)bsdt->data;
		//======================================================//
		//	���̺�  ī��Ʈ	
		//=======================================================//
		int recordcount = record->GetFieldCount();
		debug->Write("���̺� ī��Ʈ:%d", recordcount);
		int pagecount=record->GetPageCount();
		while(nPageIndex<=pagecount)
		{
			if (!pool.CanContinue())
				return;
			record->SetAbsolutePage(nPageIndex++);
			int pagesize = record->GetPageSize();
			for(register int nRecNumber = 0; nRecNumber < pagesize;nRecNumber++)
			{
				int lVal1,lVal2;
				CString name, card, place, adress,times;
				record->GetFieldValue("code", lVal1);
				record->GetFieldValue("usemoney", lVal2);
				record->GetFieldValue("usename", name);
				record->GetFieldValue("usecardnum", card);
				record->GetFieldValue("useplace", place);
				record->GetFieldValue("usetime", times);
				record->GetFieldValue("useadress", adress);

				//COleDateTime datetime;
				//TCHAR dttm[32]={0,};
				//record->GetFieldValue("usetime", datetime);
				//sprintf_s(dttm,_T("%d-%d-%d %d:%d:%d"),
				//  datetime.GetYear(), 
				//	datetime.GetMonth(),
				//	datetime.GetDay(),
				//	datetime.GetHour(),
				//	datetime.GetMinute(),
				//	datetime.GetSecond());

				//======================================================//
				// �����̽� ������ ���δ�
				//=======================================================//
				name.TrimRight();
				card.TrimRight();
				place.TrimRight();
				adress.TrimRight();
				times.TrimRight();
				//======================================================//
				//	�����Ⱑ ������ �� �ٽ� ������ ���� �̺�Ʈ
				//=======================================================//
				WaitForSingleObject(hSendEvent, INFINITE);
				debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
					lVal1, lVal2, name, card, place, times, adress);
				//======================================================//
				//	���� ���ڵ� �����͸� ����ü(����)�� ��´�
				//=======================================================//
				//======================================================//
				//	���� ������ ����ü �ʱ�ȭ 
				//=======================================================//
				memset(senddata,0,sizeof(DBdata162));
				//======================================================//
				//	������ �Է� 
				//=======================================================//
				senddata->dwnum=nPageIndex*(nRecNumber+1);
				senddata->dwcode=lVal1;
				senddata->dwusemoney=lVal2;
				_tcscpy_s(senddata->usename,20,name.GetBuffer(0));
				_tcscpy_s(senddata->usecardnum,10,card.GetBuffer(0));
				_tcscpy_s(senddata->useplace,20,place.GetBuffer(0));
				_tcscpy_s(senddata->usetime,31,times.GetBuffer(0));
				_tcscpy_s(senddata->useadress,60,adress.GetBuffer(0));

				//======================================================//
				//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
				//=======================================================//
				RequestSendData(sockinfo,sizeof(baseDATA20)+sizeof(DATA162)-2, RECVCODE_VMCLIENTSEND,hSendEvent, TRUE);
				//======================================================//
				//	���� ���ڵ� �̵�
				//=======================================================//
				record->MoveNext();
				BOOL bEof = record->IsEof();
				if(bEof)
					break;
			}
		}
		//�����͸� ��� ������ ���� �ڵ带 ������ ���� �κ�
		WaitForSingleObject(hSendEvent, INFINITE);
		//======================================================//
		//	���� ���ڵ� �����͸� ����ü(����)�� ��´�
		//=======================================================//
		//======================================================//
		//	���� ������ ����ü �ʱ�ȭ
		//=======================================================//
		memset(senddata,0,sizeof(DATA162));
		//======================================================//
		//	������ �۽� ���� �ڵ�
		//=======================================================//
		senddata->dwnum=0;
		//======================================================//
		//	��û �ڵ� ������ �Լ�(�Լ��� ���⵵�� ����)
		//=======================================================//
		RequestSendData(sockinfo,sizeof(baseDATA20)+sizeof(DATA162)-2, RECVCODE_VMCLIENTSEND,hSendEvent);
		//======================================================//
		//	DB ������ ���� ����
		//=======================================================//
		debug->Write("DB������ ������ ����");
	}
}

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDbDataSendPoolObj::Done()
{
	InterlockedDecrement(&s_lCount);
	if(sock)
	{
		//�����ڵ带 ��� ������ �� �� ����
		WaitForSingleObject(hSendEvent, INFINITE);
		//�̺�Ʈ�� Close �Ѵ�
		if(hSendEvent)
		{
			CloseHandle(hSendEvent);hSendEvent=NULL;
		}
		//������ ���ڵ� �����͸� �����Ѵ�
		if(obj)
			obj->ReleaseObject(record);
		debug->Write("������ID:%d:���巡 ����", thdid);
	}
	delete this;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CDbDataSendPoolObj::RequestSendData(SOCKETINFO* info, DWORD totalstructlen, DWORD code, HANDLE hEvent, BOOL bafterdata)
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