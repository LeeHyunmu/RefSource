
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
// #define  TABLENAMEFIND "select 'Name' = o.name,'Owner' = user_name(uid),'Object_type' = substring(v.name,5,31) \
// from sysobjects o, master.dbo.spt_values v where o.xtype = substring(v.name,1,2) and v.type = 'O9T'\
// and v.name like 'U%' order by Object_type desc, Name asc"
// record->GetFieldValue("NAME", name);
// record->GetFieldValue("Owner", owner);
// record->GetFieldValue("Object_type", object_type);

#define  TABAESELECT "select* from trafficinfo"
HANDLE CDbDataSendPoolObj::SetDbDataInfo(IServerMg* servermg, SOCKET _sock, CString _where)
{
	m_n = InterlockedIncrement(&s_lNext);
	InterlockedIncrement(&s_lCount);
	sock = _sock;
	hSendEvent=NULL;
	hSendEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);
	iocp=servermg->GetIocpServer();
	record = servermg->GetDatabasebMg()->GetDbRecordset(5, TABAESELECT,"");
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
	ICrc* crc32 = obj->GetCrc32();
	if(sock&&record)
	{
		SOCKETINFO* sockinfo;
		iocp->fnGetSockInfo(&sockinfo, sock);
		//���� ����ü ���̽� ����
		baseDB20*    bsdt=(baseDB20*)sockinfo->data;
		//======================================================//
		//	���̺� �� ���	
		//=======================================================//
		CHAR szSenddata[]="trafficinfo";
		memcpy_s(bsdt->TableName,sizeof(bsdt->TableName),szSenddata, sizeof(szSenddata));
		//���� �������� ����ü ����
		DBdata162*    senddata=(DBdata162*)bsdt->data;
		//������ ������ ���� �Է�
		SENDDATAINFO* sendinfo=(SENDDATAINFO*)&sockinfo->sendinfo;
		ZeroMemory(sendinfo,sizeof(SENDDATAINFO));
		sendinfo->hSendEvent=hSendEvent;
		int recordcount = record->GetFieldCount();
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

				name.TrimRight();
				card.TrimRight();
				place.TrimRight();
				adress.TrimRight();
				times.TrimRight();
				debug->Write("code:%d, usemoney:%d, usename:%s, usecardnum:%s, useplace:%s, usetime:%s, useadress:%s",
					lVal1, lVal2, name, card, place, times, adress);
				//�����Ⱑ ������ �� �ٽ� ������ ���� �̺�Ʈ
				WaitForSingleObject(hSendEvent, INFINITE);
				//���� ����ü ũ�⸦ ���Ѵ�
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA162)-2;
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
				_tcscpy_s(senddata->usetime,31,times.GetBuffer(0));
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
		//���� ����ü ũ�⸦ ���Ѵ�
		bsdt->dwdatalen=sizeof(baseDB20)+sizeof(DBdata162)-2;
		//���� ������ ����ü �ʱ�ȭ 
		memset(senddata,0,sizeof(DBdata162));
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
