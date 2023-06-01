
#include "stdAfx.h"
#include "MobileDbMg.h"

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMobileDbMg::CMobileDbMg():
obj(NULL),
debug(NULL),
adodb(NULL)
{

}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMobileDbMg::~CMobileDbMg()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CMobileDbMg::initMobileDbMg(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();
	adodb=obj->GetAdoDatabase();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CMobileDbMg::Release()
{
}
//--------------------------------------------------------------
//|
//|Query ��
//|
//---------------------------------------------------------------
void CMobileDbMg::Trans(TCHAR* strQuery, TCHAR* format, ...)
{
	if(adodb->IsOpen()==FALSE)
		return;
	_ConnectionPtr pConnection=adodb->GetActiveConnection();
	//�������ڷκ��� ���ڿ� ���
	va_list args;
	TCHAR *buffer;
	//����
	va_start(args, format);
	//�������ڷ� �̷���� ���ڿ��� ũ�⸦ ���Ѵ�.
	INT len = _vsctprintf(format, args) + (sizeof(TCHAR)*2); //+1 �ι��� �߰�
	// ���ؼ� ���� ũ�� +1 ��ŭ buffer�� �޸𸮸� �Ҵ��Ѵ�.
	buffer = new TCHAR[len];
	//���ڿ��� ���ۿ� �Է��Ѵ�.
	_vstprintf_s(buffer, len, format, args);
	//��
	va_end(args);
	TCHAR Query[256]={0,};
	sprintf_s(Query, "%s %s",strQuery, buffer);
	_bstr_t bstrQuery(Query);
	_variant_t vRecsAffected(0L);
	pConnection->BeginTrans();
	pConnection->Execute(Query, &vRecsAffected, adOptionUnspecified);
	pConnection->CommitTrans();
	if(buffer)
	{
		delete [] buffer; buffer=NULL;
	}
}
//--------------------------------------------------------------
//|
//|DB ���ؼ�
//|
//---------------------------------------------------------------
BOOL CMobileDbMg::DbConnect()
{
	CString strConnection = _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763");
	if(adodb->Open(strConnection))
	{	
		debug->Write("�����ͺ��̽� ���� ����");
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|VM ��û ����Ÿ�� DB�κ��� �����´�
//|
//---------------------------------------------------------------
IADORecordset* CMobileDbMg::GetDbRecordset(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...)
{
	if(adodb->IsOpen()==FALSE)
	{
		debug->Write("�����ͺ��̽� ���� ���� �ִ�");
		return NULL;
	}
	va_list args;
	TCHAR *buffer;
	//����
	va_start(args, format);
	//�������ڷ� �̷���� ���ڿ��� ũ�⸦ ���Ѵ�.
	INT len = _vsctprintf(format, args) + (sizeof(TCHAR)*2); //+1 �ι��� �߰�
	// ���ؼ� ���� ũ�� +1 ��ŭ buffer�� �޸𸮸� �Ҵ��Ѵ�.
	buffer = new TCHAR[len];
	//���ڿ��� ���ۿ� �Է��Ѵ�.
	_vstprintf_s(buffer, len, format, args);
	//��
	va_end(args);
	TCHAR Query[256]={0,};
	sprintf_s(Query, "%s %s",strQuery, buffer);
	IADORecordset *adorecord= obj->GetAdoRecordset();
	if(adorecord->Open(adodb->GetActiveConnection(),Query, openUnknown))
	{
		adorecord->SetPageSize(pagsize);
		return adorecord;
	}
	return NULL;
}

//LRESULT CMainDlg::OnBnClickedBudbconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//{
//	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	//CString strConnection = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=dbTest.mdb");
//	CString strConnection = _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;uid=sa;pdw=gusan5763");
//	//CString strConnection = _T("Provider=sqloledb;Data Source=(local);Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763"); 
//	IADODatabase* adodb =  obj->GetAdoDatabase();
//	IADORecordset* adorecord = obj->GetAdoRecordset();
//	IOutput* debug = obj->GetOutput();
//	if(adodb->Open(strConnection))
//	{	
//		debug->Write("�����ͺ��̽� ���� ����");
//		//	IMobileDbMg* dbmg = obj->GetMobileDbMg();
		//���̺� ����
//		//Trans("CREATE TABLE testdbcreate  ( did  integer PRIMARY KEY ,	name varchar(40) NOT NULL CHECK (name <> ''));","");
		//�߰�
//		//	dbmg->Trans("insert into dbo.traffic values","(%d,'%s', '%s', %d, '%s', '%s')",15, "����", "11100", 400, "����", "��⵵������");
		//������Ʈ
//		//dbmg->Trans("Update dbo.traffic set","username='%s', useplace='%s' where usecode=%d","����", "�����", 10);	
		//����
//		//	dbmg->Trans("delete from dbo.traffic where","cardnum='%s'","true");		
//		if(adorecord->Open(adodb->GetActiveConnection(),"dbo.traffic", openTable))
//		{
			//���ڵ� �߰�
//			//	/*CString username=_T("�̸���");
//			//	CString cardnum=_T("10022");
//			//	CString useplace=_T("����");
//			//	CString adress=_T("��⵵");
//			//	adorecord->AddNew();
//			//	adorecord->SetFieldValue("usecode", 14);
//			//	adorecord->SetFieldValue("usemoney", 500);
//			//	adorecord->SetFieldValue("username", username);
//			//	adorecord->SetFieldValue("cardnum", cardnum);
//			//	adorecord->SetFieldValue("useplace", useplace);
//			//	adorecord->SetFieldValue("adress", adress);
//			//	adorecord->Update();*/
			//select �� �����͸� ���
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
//		MessageBox("���ῡ��");
//	}
//	return 0;
//}