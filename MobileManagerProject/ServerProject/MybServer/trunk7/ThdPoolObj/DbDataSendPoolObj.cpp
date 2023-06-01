
#include "stdAfx.h"
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
dbmg(NULL),
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
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CDbDataSendPoolObj::initThreadObject(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();
	dbmg=obj->GetMobileDbMg();
	iocp=obj->GetIocpServer();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|db ������ ���� ���� ����
//|
//---------------------------------------------------------------
HANDLE CDbDataSendPoolObj::SetDbDataInfo(SOCKET _sock, CString _where)
{
	m_n = InterlockedIncrement(&s_lNext);
	InterlockedIncrement(&s_lCount);
	sock = _sock;
	hSendEvent=NULL;
	hSendEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	record = dbmg->GetDbRecordset(5, "select* from dbo.trafficinfo","where usecardnum='%s'", _where);
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
	IServer* iocp = obj->GetIocpServer();
	ICrc* crc32 = obj->GetCrc32();
	if(sock)
	{
		SOCKETINFO* sockinfo;
		iocp->fnGetSockInfo(&sockinfo, sock);
		//���� ����ü ���̽� ����
		baseDATA20*    bsdt=(baseDATA20*)sockinfo->data;
		//���� ����ü ũ�⸦ ���Ѵ�
		bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA162)-2;
		//���� �������� ����ü ����
		DATA162*    senddata=(DATA162*)bsdt->data;
		//������ ������ ���� �Է�
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&sockinfo->sendinfo;
		ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
		sendinfo->hSendEvent=hSendEvent;
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
				CString name, card, place, adress;
				COleDateTime datetime;
				TCHAR dttm[32]={0,};
				record->GetFieldValue("code", lVal1);
				record->GetFieldValue("usemoney", lVal2);
				record->GetFieldValue("usename", name);
				record->GetFieldValue("usecardnum", card);
				record->GetFieldValue("useplace", place);
				record->GetFieldValue("usetime", datetime);
				record->GetFieldValue("useadress", adress);
				sprintf_s(dttm,_T("%d-%d-%d %d:%d,%d"),datetime.GetYear(), 
					datetime.GetMonth(),
					datetime.GetDay(),
					datetime.GetHour(),
					datetime.GetMinute(),
					datetime.GetSecond());
				name.TrimRight();
				card.TrimRight();
				place.TrimRight();
				adress.TrimRight();
				debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
					lVal1, lVal2, name, card, place, dttm, adress);
				//�����Ⱑ ������ �� �ٽ� ������ ���� �̺�Ʈ
				WaitForSingleObject(hSendEvent, INFINITE);
				//���� ������ ����ü �ʱ�ȭ 
				memset(senddata,0,sizeof(DATA162));
				//������ ������ ũ�⸦ sendinfo�� ������ �д�
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				sendinfo->nSendBytes=0;
				sendinfo->bafterSenddata=TRUE;
				//������ �Է�
				senddata->dwnum=nPageIndex*(nRecNumber+1);
				senddata->dwcode=lVal1;
				senddata->dwusemoney=lVal2;
				_tcscpy_s(senddata->usename,20,name.GetBuffer(0));
				_tcscpy_s(senddata->usecardnum,10,card.GetBuffer(0));
				_tcscpy_s(senddata->useplace,20,place.GetBuffer(0));
				_tcscpy_s(senddata->usetime,31,dttm);
				_tcscpy_s(senddata->useadress,60,adress.GetBuffer(0));
				//üũ ������ �Է�
				crc32->GetResult(sockinfo->data,bsdt->dwdatalen-4);
				//������ ���� Ȯ��
				if(crc32->GetCheck(sockinfo->data,bsdt->dwdatalen))
					debug->Write("������ ���� OK");
				else
					debug->Write("������ ���� NO");
				iocp->fnWSASend(sock, sockinfo->data, bsdt->dwdatalen);
				record->MoveNext();
				BOOL bEof = record->IsEof();
				if(bEof)
					break;
			}
		}
		//�����͸� ��� ������ ���� �ڵ带 ������ ���� �κ�
		WaitForSingleObject(hSendEvent, INFINITE);
		//���� ������ ����ü �ʱ�ȭ 
		memset(senddata,0,sizeof(DATA162));
		//������ ������ ũ�⸦ sendinfo�� ������ �д�
		sendinfo->nTotalSendBytes=bsdt->dwdatalen;
		sendinfo->nSendBytes=0;
		sendinfo->bafterSenddata=FALSE;
		//������ �Է�
		senddata->dwnum=0;
		//üũ ������ �Է�
		crc32->GetResult(sockinfo->data,bsdt->dwdatalen-4);
		//������ ���� Ȯ��
		if(crc32->GetCheck(sockinfo->data,bsdt->dwdatalen))
			debug->Write("������ ���� OK");
		else
			debug->Write("������ ���� NO");
		//DB �������� �����Ͱ� �� �̻� ���ٴ� ���� �˸���
		iocp->fnWSASend(sock, sockinfo->data, bsdt->dwdatalen);
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
