
#include "stdAfx.h"
#include "MysqlDbtable.h"
#include <conio.h>

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMysqlDbtable::CMysqlDbtable():
obj(NULL),
debug(NULL),
m_Rec(NULL)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMysqlDbtable::~CMysqlDbtable()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CMysqlDbtable::initMysqlDbtable(IMyObject* _obj)
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
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CMysqlDbtable::Release()
{
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CMysqlDbtable::ISEOF()
{
	int rs;
	if(m_Rec==NULL)
	{
		debug->Write("Invalid Record");
		return -1;
	}
	try{
		rs=m_Rec->EndOfFile;
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_ISEOF",e);
		m_Rec=NULL;
		return -2;
	}
	return rs;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDbtable::GetFieldValue(char* FieldName, char* FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();
		debug->Write("%s",(LPCSTR)((_bstr_t)vtValue.bstrVal));
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_GetFieldValue",e);
		return 0;
	}
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDbtable::GetFieldValue(char* FieldName,int& FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();
		FieldValue=vtValue.intVal;
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_GetFieldValue",e);
		return 0;
	}
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDbtable::GetFieldValue(char* FieldName,float& FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();
		FieldValue=vtValue.fltVal;
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_GetFieldValue",e);
		return 0;
	}
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CMysqlDbtable::GetFieldValue(char* FieldName,double& FieldValue)
{
	try
	{
		_variant_t  vtValue;
		vtValue = m_Rec->Fields->GetItem(FieldName)->GetValue();
		FieldValue=vtValue.dblVal;
		//GetDec(vtValue,FieldValue,3);
	}
	catch(_com_error &e){
		debug->ErrorHandler("mysql_GetFieldValue",e);
		return 0;
	}
	return 1;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
HRESULT CMysqlDbtable::MoveNext()
{
	HRESULT hr;
	try
	{
		hr=m_Rec->MoveNext();
	}
	catch(_com_error &e)
	{
		debug->ErrorHandler("mysql_MoveeNext",e);
		//m_Rec=NULL;
		return -2;
	}
	return hr;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
HRESULT CMysqlDbtable::MovePrevious()
{
	HRESULT hr;
	try
	{
		hr=m_Rec->MovePrevious();
	}
	catch(_com_error &e)
	{
		debug->ErrorHandler("mysql_MovePrevious",e);
		//m_Rec=NULL;
		return -2;
	}
	return hr;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
HRESULT CMysqlDbtable::MoveFirst()
{
	HRESULT hr;
	try
	{
		hr=m_Rec->MoveFirst();
	}
	catch(_com_error &e)
	{
		debug->ErrorHandler("mysql_MoveFirst",e);
		//m_Rec=NULL;
		return -2;
	}
	return hr;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
HRESULT CMysqlDbtable::MoveLast()
{
	HRESULT hr;
	try
	{
		hr=m_Rec->MoveLast();
	}
	catch(_com_error &e)
	{
		debug->ErrorHandler("mysql_<MoveLast",e);
		//m_Rec=NULL;
		return -2;
	}
	return hr;
}
//--------------------------------------------------------------
//|
//| MySQL ���ڵ� ������ ����
//|
//---------------------------------------------------------------
 void CMysqlDbtable::SetRecordPtr(RecPtr _rec)
{
	m_Rec=_rec;
}