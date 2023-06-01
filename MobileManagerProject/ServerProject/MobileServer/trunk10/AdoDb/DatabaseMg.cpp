
#include "stdAfx.h"
#include "DatabaseMg.h"

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDatabasebMg::CDatabasebMg():
obj(NULL),
debug(NULL),
adodb(NULL),
adorecord(NULL)
{

}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CDatabasebMg::~CDatabasebMg()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::initMobileDbMg(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutput();
	//=====================================================
	// Database  객체를 서버 클래스에서 생성한다
	//======================================================
	adodb=obj->CreateAdoDatabase();
	if(adodb==NULL)
	{
		debug->Write("Database 생성 실패"); return FALSE;
	}
	//=====================================================
	// 레코드를 얻어서 입력한다
	//======================================================
	adorecord = obj->CreateAdoRecordset();
	if(adorecord==NULL)
	{
		debug->Write("DB 레코드 클래스 생성 실패");
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CDatabasebMg::Release()
{
	obj->ReleaseObject(adodb);
}
//--------------------------------------------------------------
//|
//|Query 문
//|
//---------------------------------------------------------------
void CDatabasebMg::Trans(TCHAR* strQuery, TCHAR* format, ...)
{
	if(adodb->IsOpen()==FALSE)
		return;
	//가변인자로부터 문자열 얻기
	va_list args;
	TCHAR *buffer;
	//시작
	va_start(args, format);
	//가변인자로 이루어진 문자열의 크기를 구한다.
	INT len = _vsctprintf(format, args) + (sizeof(TCHAR)*2); //+1 널문자 추가
	// 위해서 구한 크기 +1 만큼 buffer에 메모리를 할당한다.
	buffer = new TCHAR[len];
	//문자열을 버퍼에 입력한다.
	_vstprintf_s(buffer, len, format, args);
	//끝
	va_end(args);
	TCHAR Query[256]={0,};
	sprintf_s(Query, "%s %s",strQuery, buffer);
	_bstr_t bstrQuery(Query);
	_variant_t vRecsAffected(0L);
	adodb->BeginTransaction();
	BOOL b = adodb->Execute(Query, adOptionUnspecified);//, &vRecsAffected, adOptionUnspecified);
	if(b)
	{
		HRESULT hr;
		hr = adodb->CommitTransaction();
		if(FAILED(hr))
			adodb->RollbackTransaction();
	}
	if(buffer)
	{
		delete [] buffer; buffer=NULL;
	}
}
//--------------------------------------------------------------
//|
//|Bi
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::BeginTn()
{
	HRESULT hr;
	if(adodb->IsOpen())
	{
		hr = adodb->BeginTransaction();
	}
	if(FAILED(hr))
		return FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//|Query 문
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::ActionQuery(TCHAR* strQuery, TCHAR* format, ...)
{
	BOOL b = FALSE;
	if(adodb->IsOpen()==FALSE)
		return b;
	//가변인자로부터 문자열 얻기
	va_list args;
	TCHAR *buffer;
	//시작
	va_start(args, format);
	//가변인자로 이루어진 문자열의 크기를 구한다.
	INT len = _vsctprintf(format, args) + (sizeof(TCHAR)*2); //+1 널문자 추가
	// 위해서 구한 크기 +1 만큼 buffer에 메모리를 할당한다.
	buffer = new TCHAR[len];
	//문자열을 버퍼에 입력한다.
	_vstprintf_s(buffer, len, format, args);
	//끝
	va_end(args);
	TCHAR Query[256]={0,};
	sprintf_s(Query, "%s %s",strQuery, buffer);
	_bstr_t bstrQuery(Query);
	_variant_t vRecsAffected(0L);
	b = adodb->Execute(Query);
	if(buffer)
	{
		delete [] buffer; buffer=NULL;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|commit
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::CommitTn()
{
	HRESULT hr;
	hr = adodb->CommitTransaction();
	if(FAILED(hr))
	{
		adodb->RollbackTransaction();
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|DB 컨넥션
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::DbConnect(CString strConnection)
{
	//CString strConnection = _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763");
	if(adodb->Open(strConnection))
	{	
		debug->Write("데이터베이스 연결 성공");
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|레코드 세팅
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::RecordsetSetting(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...)
{
	if(adodb->IsOpen()==FALSE)
	{
		debug->Write("데이터베이스 연결 끊겨 있다");
		return NULL;
	}
	va_list args;
	TCHAR *buffer;
	//시작
	va_start(args, format);
	//가변인자로 이루어진 문자열의 크기를 구한다.
	INT len = _vsctprintf(format, args) + (sizeof(TCHAR)*2); //+1 널문자 추가
	// 위해서 구한 크기 +1 만큼 buffer에 메모리를 할당한다.
	buffer = new TCHAR[len];
	//문자열을 버퍼에 입력한다.
	_vstprintf_s(buffer, len, format, args);
	//끝
	va_end(args);
	TCHAR Query[256]={0,};
	sprintf_s(Query, "%s %s",strQuery, buffer);
	if(adorecord->Open(adodb->GetActiveConnection(),Query, openUnknown))
	{
		adorecord->SetPageSize(pagsize);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|레코드 셋을 연결 한다
//|
//---------------------------------------------------------------
BOOL CDatabasebMg::RecordsetSetting(TCHAR* tablename)
{
	if(adodb->IsOpen()==FALSE)
	{
		debug->Write("데이터베이스 연결 끊겨 있다");
		return FALSE;
	}
	//======================================================//
	//	레코드 설정
	//=======================================================//
	if(adorecord->Open(adodb->GetActiveConnection(),tablename, openTable))
	{
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|레코드 포인터 넘긴다
//|
//---------------------------------------------------------------
IADORecordset* CDatabasebMg::GetRecordsetPoint()
{
	return adorecord;
}
//LRESULT CMainDlg::OnBnClickedBudbconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	//CString strConnection = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=dbTest.mdb");
//	CString strConnection = _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;uid=sa;pdw=gusan5763");
//	//CString strConnection = _T("Provider=sqloledb;Data Source=(local);Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763"); 
//	IADODatabase* adodb =  obj->GetAdoDatabase();
//	IADORecordset* adorecord = obj->GetAdoRecordset();
//	IOutput* debug = obj->GetOutput();
//	if(adodb->Open(strConnection))
//	{	
//		debug->Write("데이터베이스 연결 성공");
//		//	IDatabasebMg* dbmg = obj->GetMobileDbMg();
		//테이블 생성
//		//Trans("CREATE TABLE testdbcreate  ( did  integer PRIMARY KEY ,	name varchar(40) NOT NULL CHECK (name <> ''));","");
		//추가
//		//	dbmg->Trans("insert into dbo.traffic values","(%d,'%s', '%s', %d, '%s', '%s')",15, "병병", "11100", 400, "버스", "경기도남양주");
		//업데이트
//		//dbmg->Trans("Update dbo.traffic set","username='%s', useplace='%s' where usecode=%d","갑갑", "비행기", 10);	
		//삭제
//		//	dbmg->Trans("delete from dbo.traffic where","cardnum='%s'","true");		
//		if(adorecord->Open(adodb->GetActiveConnection(),"dbo.traffic", openTable))
//		{
			//레코드 추가
//			//	/*CString username=_T("이름정");
//			//	CString cardnum=_T("10022");
//			//	CString useplace=_T("버스");
//			//	CString adress=_T("경기도");
//			//	adorecord->AddNew();
//			//	adorecord->SetFieldValue("usecode", 14);
//			//	adorecord->SetFieldValue("usemoney", 500);
//			//	adorecord->SetFieldValue("username", username);
//			//	adorecord->SetFieldValue("cardnum", cardnum);
//			//	adorecord->SetFieldValue("useplace", useplace);
//			//	adorecord->SetFieldValue("adress", adress);
//			//	adorecord->Update();*/
			//select 된 데이터를 출력
//			adorecord->SetPageSize(5);
//			for(register int nPageIndex = 1; nPageIndex <= adorecord->GetPageCount(); 
//				nPageIndex++)
//			{
//				adorecord->SetAbsolutePage(nPageIndex);
//				for(register int nRecNumber = 0; nRecNumber < adorecord->GetPageSize(); 
//					nRecNumber++)
//				{
//					int lVal1,lVal2;
//					CString name, card, place, adress;
//					COleDateTime datetime;
//					CString dttm;
//					adorecord->GetFieldValue("usecode", lVal1);
//					adorecord->GetFieldValue("usemoney", lVal2);
//					adorecord->GetFieldValue("username", name);
//					adorecord->GetFieldValue("cardnum", card);
//					adorecord->GetFieldValue("useplace", place);
//					adorecord->GetFieldValue("usedate", datetime);
//					dttm.Format(_T("%d-%d-%d %d:%d,%d"),datetime.GetYear(), 
//						datetime.GetMonth(),
//						datetime.GetDay(),
//						datetime.GetHour(),
//						datetime.GetMinute(),
//						datetime.GetSecond());
//					adorecord->GetFieldValue("adress", adress);
//					name.TrimRight();
//					card.TrimRight();
//					place.TrimRight();
//					adress.TrimRight();
//					debug->Write("usecode:%d, usemoney:%d, username:%s, cardnum:%s, useplace:%s, usedate:%s, adress:%s",
//						lVal1, lVal2, name, card, place, dttm, adress);
//					adorecord->MoveNext();
//
//					if(adorecord->IsEof())
//						break;
//				}
//			}
//			adorecord->Close();
//		}
//
//	}
//	else
//	{
//		MessageBox("연결에러");
//	}
//	return 0;
//}