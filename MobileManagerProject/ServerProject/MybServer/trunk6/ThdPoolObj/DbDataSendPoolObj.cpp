
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
//|생성자
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
//|소멸자
//|
//---------------------------------------------------------------
CDbDataSendPoolObj::~CDbDataSendPoolObj()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
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
//|db 데이터 축출 정보 설정
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
		debug->Write("Db 정보 세팅 에러");
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
	debug->Write("스래드ID:%d", thdid);
	register int nPageIndex = 1;
	IServer* iocp = obj->GetIocpServer();
	ICrc* crc32 = obj->GetCrc32();
	if(sock)
	{
		SOCKETINFO* sockinfo;
		iocp->fnGetSockInfo(&sockinfo, sock);
		//보낼 구조체 베이스 연결
		baseDATA20*    bsdt=(baseDATA20*)sockinfo->data;
		//보낼 구조체 크기를 구한다
		bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA162)-2;
		//실제 보내야할 구조체 연결
		DATA162*    senddata=(DATA162*)bsdt->data;
		//보내는 데이터 정보 입력
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
				//보내기가 끝냈을 때 다시 보내기 위한 이벤트
				WaitForSingleObject(hSendEvent, INFINITE);
				//보낼 데이터 구조체 초기화 
				memset(senddata,0,sizeof(DATA162));
				//보내는 데이터 크기를 sendinfo에 저정해 둔다
				sendinfo->nTotalSendBytes=bsdt->dwdatalen;
				sendinfo->nSendBytes=0;
				sendinfo->bafterSenddata=TRUE;
				//데이터 입력
				senddata->dwnum=nPageIndex*(nRecNumber+1);
				senddata->dwcode=lVal1;
				senddata->dwusemoney=lVal2;
				_tcscpy_s(senddata->usename,20,name.GetBuffer(0));
				_tcscpy_s(senddata->usecardnum,10,card.GetBuffer(0));
				_tcscpy_s(senddata->useplace,20,place.GetBuffer(0));
				_tcscpy_s(senddata->usetime,31,dttm);
				_tcscpy_s(senddata->useadress,60,adress.GetBuffer(0));
				//체크 데이터 입력
				crc32->GetResult(sockinfo->data,bsdt->dwdatalen-4);
				//데이터 검증 확인
				if(crc32->GetCheck(sockinfo->data,bsdt->dwdatalen))
					debug->Write("데이터 검증 OK");
				else
					debug->Write("데이터 검증 NO");
				iocp->fnWSASend(sock, sockinfo->data, bsdt->dwdatalen);
				record->MoveNext();
				BOOL bEof = record->IsEof();
				if(bEof)
					break;
			}
		}
		//데이터를 모두 보내고 종료 코드를 보내기 위한 부분
		WaitForSingleObject(hSendEvent, INFINITE);
		//보낼 데이터 구조체 초기화 
		memset(senddata,0,sizeof(DATA162));
		//보내는 데이터 크기를 sendinfo에 저정해 둔다
		sendinfo->nTotalSendBytes=bsdt->dwdatalen;
		sendinfo->nSendBytes=0;
		sendinfo->bafterSenddata=FALSE;
		//데이터 입력
		senddata->dwnum=0;
		//체크 데이터 입력
		crc32->GetResult(sockinfo->data,bsdt->dwdatalen-4);
		//데이터 검증 확인
		if(crc32->GetCheck(sockinfo->data,bsdt->dwdatalen))
			debug->Write("데이터 검증 OK");
		else
			debug->Write("데이터 검증 NO");
		//DB 보내야할 데이터가 더 이상 없다는 것을 알린다
		iocp->fnWSASend(sock, sockinfo->data, bsdt->dwdatalen);
		debug->Write("DB데이터 보내기 종료");
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
		//종료코드를 모두 보내고 난 후 종료
		WaitForSingleObject(hSendEvent, INFINITE);
		//이벤트를 Close 한다
		if(hSendEvent)
		{
			CloseHandle(hSendEvent);hSendEvent=NULL;
		}
		//생성한 레코드 포인터를 해제한다
		if(obj)
			obj->ReleaseObject(record);
		debug->Write("스래드ID:%d:스드래 종료", thdid);
	}
	delete this;
}
