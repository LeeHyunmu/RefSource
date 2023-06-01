
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
record(NULL),
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
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CDbDataSendPoolObj::Release()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CDbDataSendPoolObj::initThreadObject(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutput();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|db 데이터 축출 정보 설정
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
	ICrc* crc32 = obj->GetCrc32();
	if(sock&&record)
	{
		SOCKETINFO* sockinfo;
		iocp->fnGetSockInfo(&sockinfo, sock);
		//보낼 구조체 베이스 연결
		baseDB20*    bsdt=(baseDB20*)sockinfo->data;
		//======================================================//
		//	테이블 명 기록	
		//=======================================================//
		CHAR szSenddata[]="trafficinfo";
		memcpy_s(bsdt->TableName,sizeof(bsdt->TableName),szSenddata, sizeof(szSenddata));
		//실제 보내야할 구조체 연결
		DBdata162*    senddata=(DBdata162*)bsdt->data;
		//보내는 데이터 정보 입력
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
				//보내기가 끝냈을 때 다시 보내기 위한 이벤트
				WaitForSingleObject(hSendEvent, INFINITE);
				//보낼 구조체 크기를 구한다
				bsdt->dwdatalen=sizeof(baseDATA20)+sizeof(DATA162)-2;
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
				_tcscpy_s(senddata->usetime,31,times.GetBuffer(0));
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
		//보낼 구조체 크기를 구한다
		bsdt->dwdatalen=sizeof(baseDB20)+sizeof(DBdata162)-2;
		//보낼 데이터 구조체 초기화 
		memset(senddata,0,sizeof(DBdata162));
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
