
#include "stdAfx.h"
#include "MysqlDb.h"
#include <conio.h>

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
ADODB::_RecordsetPtr rec1=NULL;
_variant_t  vtMissing1(DISP_E_PARAMNOTFOUND, VT_ERROR); 
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMysqlDb::CMysqlDb():
obj(NULL),
debug(NULL),
mysqltable(NULL),
m_Cnn(NULL)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CMysqlDb::~CMysqlDb()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CMysqlDb::initMysqlDb(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();
	mysqltable=obj->GetMysqlDbtable();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CMysqlDb::Release()
{
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDb::Open(char* UserName, char* Pwd,char* CnnStr)
{
	//cnn->Open(strCnn,"sa","sa",NULL);
	try
	{
		HRESULT hr;
		hr    = m_Cnn.CreateInstance( __uuidof( ADODB::Connection ) );
		m_Cnn->Open(CnnStr, UserName, Pwd, NULL);
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_open",e);
			m_Cnn=NULL;
			return 0;
			}
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDb::OpenTbl(int Mode, char* CmdStr)
{
	if(m_Cnn==NULL)
	{
		mysqltable->SetRecordPtr(NULL);
		return 0;
	}
	RecPtr t_Rec=NULL;
	try
	{
		//t_Rec->putref_ActiveConnection(m_Cnn);
		//vtMissing<<-->>_variant_t((IDispatch *) m_Cnn, true)
		t_Rec.CreateInstance( __uuidof( ADODB::Recordset ) );
		t_Rec->Open(CmdStr,_variant_t((IDispatch *) m_Cnn, true),ADODB::adOpenStatic,ADODB::adLockOptimistic,Mode);
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_openTbl",e);
		mysqltable->SetRecordPtr(NULL);
		return 0;
	}
	mysqltable->SetRecordPtr(t_Rec);
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDb::Execute(char* CmdStr)
{
	try
	{
		m_Cnn->Execute(CmdStr,NULL,1);
	}
	catch(_com_error &e)
	{
		debug->ErrorHandler("mysql_Execute",e);
		return 0;
	}
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
//BOOL CMysqlDb::Execute(char* CmdStr)
//{
//	RecPtr t_Rec=NULL;
//	try
//	{
//		t_Rec=m_Cnn->Execute(CmdStr,NULL,1);
//	}
//	catch(_com_error &e){}
//	//CATCHERROR(Tbl.m_Rec,0)
//	//	sprintf(m_ErrStr,"Success");
//	mysqltable->SetRecordPtr(t_Rec);
//	//sprintf(m_ErrStr,"Success");
//	return TRUE;
//}