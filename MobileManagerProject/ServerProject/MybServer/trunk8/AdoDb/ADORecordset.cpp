
#include "stdAfx.h"
#include "ADORecordset.h"
#include "Adostatic.h"

#define ChunkSize 100
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CADORecordset::CADORecordset(IADODatabase* pAdoDatabase)
{
	m_pRecordset = NULL;
	m_pCmd = NULL;
	m_strQuery = _T("");
	m_strLastError = _T("");
	m_dwLastError = 0;
	m_pRecBinding = NULL;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	m_pCmd.CreateInstance(__uuidof(Command));
	m_nEditStatus = dbEditNone;
	m_nSearchDirection = searchForward;

	m_pConnection = pAdoDatabase->GetActiveConnection();
}
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CADORecordset::CADORecordset():
obj(NULL),
debug(NULL)
{
	m_pRecordset = NULL;
	m_pCmd = NULL;
	m_strQuery = _T("");
	m_strLastError = _T("");
	m_dwLastError = 0;
	m_pRecBinding = NULL;
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	m_pCmd.CreateInstance(__uuidof(Command));
	m_nEditStatus = dbEditNone;
	m_nSearchDirection = searchForward;
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CADORecordset::~CADORecordset()
{
	Close();
	if(m_pRecordset)
	{
		m_pRecordset.Release(); m_pRecordset = NULL;
	}
	if(m_pCmd)
	{
		m_pCmd.Release();	m_pCmd = NULL;
	}
	if(m_pRecBinding)
	{
		m_pRecBinding = NULL;
	}
	m_strQuery = _T("");
	m_strLastError = _T("");
	m_dwLastError = 0;
	m_nEditStatus = dbEditNone;
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CADORecordset::initAdoRecordset(IMyObject* _obj)
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
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CADORecordset::Release()
{
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADORecordset::GetQuery() 
{
	return m_strQuery;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::SetQuery(LPCSTR strQuery) 
{
	m_strQuery = strQuery;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADORecordset::GetFieldCount()
{
	return m_pRecordset->Fields->GetCount();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsEof()
{
	return m_pRecordset->EndOfFile == VARIANT_TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsEOF()
{
	return m_pRecordset->EndOfFile == VARIANT_TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsBof()
{
	return m_pRecordset->BOF == VARIANT_TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsBOF()
{
	return m_pRecordset->BOF == VARIANT_TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::MoveFirst() 
{
	m_pRecordset->MoveFirst();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::MoveNext() 
{
	m_pRecordset->MoveNext();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::MovePrevious() 
{
	m_pRecordset->MovePrevious();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::MoveLast() 
{
	m_pRecordset->MoveLast();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADORecordset::GetAbsolutePage()
{
	return m_pRecordset->GetAbsolutePage();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::SetAbsolutePage(int nPage)
{
	m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADORecordset::GetPageCount()
{
	return m_pRecordset->GetPageCount();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADORecordset::GetPageSize()
{
	return m_pRecordset->GetPageSize();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::SetPageSize(int nSize)
{
	m_pRecordset->PutPageSize(nSize);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADORecordset::GetAbsolutePosition()
{
	return m_pRecordset->GetAbsolutePosition();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::SetAbsolutePosition(int nPosition)
{
	m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADORecordset::GetLastErrorString() 
{
	return m_strLastError;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD CADORecordset::GetLastError()
{
	return m_dwLastError;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::GetBookmark()
{
	m_varBookmark = m_pRecordset->Bookmark;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsConnectionOpen()
{
	return m_pConnection != NULL && m_pConnection->GetState() != adStateClosed;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
_RecordsetPtr CADORecordset::GetRecordset()
{
	return m_pRecordset;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
_ConnectionPtr CADORecordset::GetActiveConnection() 
{
	return m_pConnection;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec, int nOption)
{	
	Close();

	if(strcmp(lpstrExec, _T("")) != 0)
		m_strQuery = lpstrExec;

	ATLASSERT(!m_strQuery.IsEmpty());

	if(m_pConnection == NULL)
		m_pConnection = mpdb;

	m_strQuery.TrimLeft();
	BOOL bIsSelect = m_strQuery.Mid(0, strlen("Select ")).CompareNoCase("select ") == 0 && nOption == openUnknown;

	try
	{
		m_pRecordset->CursorType = adOpenStatic;
		m_pRecordset->CursorLocation = adUseClient;
		if(bIsSelect || nOption == openQuery || nOption == openUnknown)
			m_pRecordset->Open((LPCSTR)m_strQuery, _variant_t((IDispatch*)mpdb, TRUE), 
			adOpenStatic, adLockOptimistic, adCmdUnknown);
		else if(nOption == openTable)
			m_pRecordset->Open((LPCSTR)m_strQuery, _variant_t((IDispatch*)mpdb, TRUE), 
			adOpenKeyset, adLockOptimistic, adCmdTable);
		else if(nOption == openStoredProc)
		{
			m_pCmd->ActiveConnection = mpdb;
			m_pCmd->CommandText = _bstr_t(m_strQuery);
			m_pCmd->CommandType = adCmdStoredProc;
			m_pConnection->CursorLocation = adUseClient;

			m_pRecordset = m_pCmd->Execute(NULL, NULL, adCmdText);
		}
		else
		{
			//TRACE( "Unknown parameter. %d", nOption);
			return FALSE;
		}
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}

	return m_pRecordset != NULL;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Open(LPCTSTR lpstrExec, int nOption)
{
	ATLASSERT(m_pConnection != NULL);
	ATLASSERT(m_pConnection->GetState() != adStateClosed);
	return Open(m_pConnection, lpstrExec, nOption);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::OpenSchema(int nSchema, LPCTSTR SchemaID)
{
	try
	{
		_variant_t vtSchemaID = vtMissing;

		if(strlen(SchemaID) != 0)
			vtSchemaID = SchemaID;

		m_pRecordset = m_pConnection->OpenSchema((enum SchemaEnum)nSchema, vtMissing, vtSchemaID);
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Requery()
{
	if(IsOpen())
	{
		try
		{
			m_pRecordset->Requery(adExecuteRecord);
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, double& dbValue)
{	
	double val = (double)NULL;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:
			val = vtFld.fltVal;
			break;
		case VT_R8:
			val = vtFld.dblVal;
			break;
		case VT_DECIMAL:
			//Corrected by Jos?Carlos Mart?ez Gal?
			val = vtFld.decVal.Lo32;
			val *= (vtFld.decVal.sign == 128)? -1 : 1;
			val /= pow(10.f, vtFld.decVal.scale); 
			break;
		case VT_UI1:
			val = vtFld.iVal;
			break;
		case VT_I2:
		case VT_I4:
			val = vtFld.lVal;
			break;
		case VT_INT:
			val = vtFld.intVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			val = 0;
			break;
		default:
			val = vtFld.dblVal;
		}
		dbValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, double& dbValue)
{	
	double val = (double)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		switch(vtFld.vt)
		{
		case VT_R4:
			val = vtFld.fltVal;
			break;
		case VT_R8:
			val = vtFld.dblVal;
			break;
		case VT_DECIMAL:
			//Corrected by Jos?Carlos Mart?ez Gal?
			val = vtFld.decVal.Lo32;
			val *= (vtFld.decVal.sign == 128)? -1 : 1;
			val /= pow(10.f, vtFld.decVal.scale); 
			break;
		case VT_UI1:
			val = vtFld.iVal;
			break;
		case VT_I2:
		case VT_I4:
			val = vtFld.lVal;
			break;
		case VT_INT:
			val = vtFld.intVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			val = 0;
			break;
		default:
			val = 0;
		}
		dbValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, long& lValue)
{
	long val = (long)NULL;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			val = vtFld.lVal;
		lValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, long& lValue)
{
	long val = (long)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			val = vtFld.lVal;
		lValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue)
{
	long val = (long)NULL;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			val = vtFld.ulVal;
		ulValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, unsigned long& ulValue)
{
	long val = (long)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			val = vtFld.ulVal;
		ulValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, int& nValue)
{
	int val = NULL;
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			val = vtFld.boolVal;
			break;
		case VT_I2:
		case VT_UI1:
			val = vtFld.iVal;
			break;
		case VT_INT:
			val = vtFld.intVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			val = 0;
			break;
		default:
			val = vtFld.iVal;
		}	
		nValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, int& nValue)
{
	int val = (int)NULL;
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		switch(vtFld.vt)
		{
		case VT_BOOL:
			val = vtFld.boolVal;
			break;
		case VT_I2:
		case VT_UI1:
			val = vtFld.iVal;
			break;
		case VT_INT:
			val = vtFld.intVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			val = 0;
			break;
		default:
			val = vtFld.iVal;
		}	
		nValue = val;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, CString& strValue, CString strDateFormat)
{
	CString str = _T("");
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_R4:
			str = CAdostaticApi::DblToStr((float)vtFld.fltVal);
			break;
		case VT_R8:
			str = CAdostaticApi::DblToStr((float)vtFld.dblVal);
			break;
		case VT_BSTR:
			str = vtFld.bstrVal;
			break;
		case VT_I2:
		case VT_UI1:
			str = CAdostaticApi::IntToStr(vtFld.iVal);
			break;
		case VT_INT:
			str = CAdostaticApi::IntToStr(vtFld.intVal);
			break;
		case VT_I4:
			str = CAdostaticApi::LongToStr(vtFld.lVal);
			break;
		case VT_UI4:
			str = CAdostaticApi::ULongToStr(vtFld.ulVal);
			break;
		case VT_DECIMAL:
			{
				//Corrected by Jos?Carlos Mart?ez Gal?
				double val = vtFld.decVal.Lo32;
				val *= (vtFld.decVal.sign == 128)? -1 : 1;
				val /= pow(10.0, vtFld.decVal.scale); 
				str = CAdostaticApi::DblToStr((float)val);
			}
			break;
		case VT_DATE:
			{
				COleDateTime dt(vtFld);

				if(strDateFormat.IsEmpty())
					strDateFormat = _T("%Y-%m-%d %H:%M:%S");
				str = dt.Format(strDateFormat);
			}
			break;
		case VT_EMPTY:
		case VT_NULL:
			str.Empty();
			break;
		case VT_BOOL:
			str = vtFld.boolVal == VARIANT_TRUE? 'T':'F';
			break;
		default:
			str.Empty();
			return FALSE;
		}
		strValue = str;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, CString& strValue, CString strDateFormat)
{
	CString str = _T("");
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		switch(vtFld.vt) 
		{
		case VT_R4:
			str = CAdostaticApi::DblToStr((float)vtFld.fltVal);
			break;
		case VT_R8:
			str = CAdostaticApi::DblToStr((float)vtFld.dblVal);
			break;
		case VT_BSTR:
			str = vtFld.bstrVal;
			break;
		case VT_I2:
		case VT_UI1:
			str = CAdostaticApi::IntToStr(vtFld.iVal);
			break;
		case VT_INT:
			str = CAdostaticApi::IntToStr(vtFld.intVal);
			break;
		case VT_I4:
			str = CAdostaticApi::LongToStr(vtFld.lVal);
			break;
		case VT_UI4:
			str = CAdostaticApi::ULongToStr(vtFld.ulVal);
			break;
		case VT_DECIMAL:
			{
				//Corrected by Jos?Carlos Mart?ez Gal?
				double val = vtFld.decVal.Lo32;
				val *= (vtFld.decVal.sign == 128)? -1 : 1;
				val /= pow(10.0, vtFld.decVal.scale); 
				str = CAdostaticApi::DblToStr((float)val);
			}
			break;
		case VT_DATE:
			{
				COleDateTime dt(vtFld);

				if(strDateFormat.IsEmpty())
					strDateFormat = _T("%Y-%m-%d %H:%M:%S");
				str = dt.Format(strDateFormat);
			}
			break;
		case VT_BOOL:
			str = vtFld.boolVal == VARIANT_TRUE? 'T':'F';
			break;
		case VT_EMPTY:
		case VT_NULL:
			str.Empty();
			break;
		default:
			str.Empty();
			return FALSE;
		}
		strValue = str;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time)
{
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
			{
				COleDateTime dt(vtFld);
				time = dt;
			}
			break;
		case VT_EMPTY:
		case VT_NULL:
			time.SetStatus(COleDateTime::null);
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, COleDateTime& time)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		switch(vtFld.vt) 
		{
		case VT_DATE:
			{
				COleDateTime dt(vtFld);
				time = dt;
			}
			break;
		case VT_EMPTY:
		case VT_NULL:
			time.SetStatus(COleDateTime::null);
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, bool& bValue)
{
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		switch(vtFld.vt) 
		{
		case VT_BOOL:
			bValue = vtFld.boolVal == VARIANT_TRUE? true: false;
			break;
		case VT_EMPTY:
		case VT_NULL:
			bValue = false;
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, bool& bValue)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		switch(vtFld.vt) 
		{
		case VT_BOOL:
			bValue = vtFld.boolVal == VARIANT_TRUE? true: false;
			break;
		case VT_EMPTY:
		case VT_NULL:
			bValue = false;
			break;
		default:
			return FALSE;
		}
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue)
{
	try
	{
		vtValue = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldValue(int nIndex, _variant_t& vtValue)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtValue = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsFieldNull(LPCTSTR lpFieldName)
{
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsFieldNull(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsFieldEmpty(LPCTSTR lpFieldName)
{
	_variant_t vtFld;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
		return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	try
	{
		vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
		return vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldEmpty(LPCTSTR lpFieldName)
{
	_variant_t vtFld;
	vtFld.vt = VT_EMPTY;

	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldEmpty(int nIndex)
{
	_variant_t vtFld;
	vtFld.vt = VT_EMPTY;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD CADORecordset::GetRecordCount()
{
	DWORD nRows = 0;

	nRows = m_pRecordset->GetRecordCount();

	if(nRows == -1)
	{
		nRows = 0;
		if(m_pRecordset->EndOfFile != VARIANT_TRUE)
			m_pRecordset->MoveFirst();

		while(m_pRecordset->EndOfFile != VARIANT_TRUE)
		{
			nRows++;
			m_pRecordset->MoveNext();
		}
		if(nRows > 0)
			m_pRecordset->MoveFirst();
	}

	return nRows;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::IsOpen()
{
	if(m_pRecordset != NULL && IsConnectionOpen())
		return m_pRecordset->GetState() != adStateClosed;
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::Close()
{
	if(IsOpen())
	{
		if (m_nEditStatus != dbEditNone)
			CancelUpdate();
		if(m_pRecordset)
		{
			m_pRecordset->PutSort(_T(""));
			m_pRecordset->Close();	
		}
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::RecordBinding(CADORecordBinding &pAdoRecordBinding)
{
	HRESULT hr;
	m_pRecBinding = NULL;

	//Open the binding interface.
	if(FAILED(hr = m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&m_pRecBinding )))
	{
		_com_issue_error(hr);
		return FALSE;
	}

	//Bind the recordset to class
	if(FAILED(hr = m_pRecBinding->BindToRecordset(&pAdoRecordBinding)))
	{
		_com_issue_error(hr);
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)
{
	FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

	return GetFieldInfo(pField, fldInfo);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

	return GetFieldInfo(pField, fldInfo);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo)
{
	memset(fldInfo, 0, sizeof(CADOFieldInfo));

	strcpy_s(fldInfo->m_strName, sizeof(fldInfo->m_strName), (LPCTSTR)pField->GetName());
	fldInfo->m_lDefinedSize = pField->GetDefinedSize();
	fldInfo->m_nType = pField->GetType();
	fldInfo->m_lAttributes = pField->GetAttributes();
	if(!IsEof())
		fldInfo->m_lSize = pField->GetActualSize();
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetChunk(LPCTSTR lpFieldName, CString& strValue)
{
	FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

	return GetChunk(pField, strValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetChunk(int nIndex, CString& strValue)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

	return GetChunk(pField, strValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetChunk(FieldPtr pField, CString& strValue)
{
	CString str = _T("");
	long lngSize, lngOffSet = 0;
	_variant_t varChunk;

	lngSize = pField->ActualSize;

	str.Empty();
	while(lngOffSet < lngSize)
	{ 
		try
		{
			varChunk = pField->GetChunk(ChunkSize);

			str += varChunk.bstrVal;
			lngOffSet += ChunkSize;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	lngOffSet = 0;
	strValue = str;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetChunk(LPCTSTR lpFieldName, LPVOID lpData)
{
	FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

	return GetChunk(pField, lpData);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetChunk(int nIndex, LPVOID lpData)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

	return GetChunk(pField, lpData);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::GetChunk(FieldPtr pField, LPVOID lpData)
{
	long lngSize, lngOffSet = 0;
	_variant_t varChunk;    
	UCHAR chData;
	HRESULT hr;
	long lBytesCopied = 0;

	lngSize = pField->ActualSize;

	while(lngOffSet < lngSize)
	{ 
		try
		{
			varChunk = pField->GetChunk(ChunkSize);

			//Copy the data only upto the Actual Size of Field.  
			for(long lIndex = 0; lIndex <= (ChunkSize - 1); lIndex++)
			{
				hr= SafeArrayGetElement(varChunk.parray, &lIndex, &chData);
				if(SUCCEEDED(hr))
				{
					//Take BYTE by BYTE and advance Memory Location
					//hr = SafeArrayPutElement((SAFEARRAY FAR*)lpData, &lBytesCopied ,&chData); 
					((UCHAR*)lpData)[lBytesCopied] = chData;
					lBytesCopied++;
				}
				else
					break;
			}
			lngOffSet += ChunkSize;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	lngOffSet = 0;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes)
{

	FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

	return AppendChunk(pField, lpData, nBytes);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::AppendChunk(int nIndex, LPVOID lpData, UINT nBytes)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

	return AppendChunk(pField, lpData, nBytes);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
{
	HRESULT hr;
	_variant_t varChunk;
	long lngOffset = 0;
	UCHAR chData;
	SAFEARRAY FAR *psa = NULL;
	SAFEARRAYBOUND rgsabound[1];

	try
	{
		//Create a safe array to store the array of BYTES 
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = nBytes;
		psa = SafeArrayCreate(VT_UI1,1,rgsabound);

		while(lngOffset < (long)nBytes)
		{
			chData	= ((UCHAR*)lpData)[lngOffset];
			hr = SafeArrayPutElement(psa, &lngOffset, &chData);

			if(FAILED(hr))
				return FALSE;

			lngOffset++;
		}
		lngOffset = 0;

		//Assign the Safe array  to a variant. 
		varChunk.vt = VT_ARRAY|VT_UI1;
		varChunk.parray = psa;

		hr = pField->AppendChunk(varChunk);

		if(SUCCEEDED(hr)) return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}

	return FALSE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADORecordset::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
{
	_bstr_t varOutput;
	_bstr_t varNull("");
	_bstr_t varCols("\t");
	_bstr_t varRows("\r");

	if(strlen(lpCols) != 0)
		varCols = _bstr_t(lpCols);

	if(strlen(lpRows) != 0)
		varRows = _bstr_t(lpRows);

	if(numRows == 0)
		numRows =(long)GetRecordCount();			

	varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

	return (LPCTSTR)varOutput;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::Edit()
{
	m_nEditStatus = dbEdit;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::AddNew()
{
	m_nEditStatus = dbEditNone;
	if(m_pRecordset->AddNew() != S_OK)
		return FALSE;

	m_nEditStatus = dbEditNew;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::AddNew(CADORecordBinding &pAdoRecordBinding)
{
	try
	{
		if(m_pRecBinding->AddNew(&pAdoRecordBinding) != S_OK)
		{
			return FALSE;
		}
		else
		{
			m_pRecBinding->Update(&pAdoRecordBinding);
			return TRUE;
		}

	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}	
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Update()
{
	BOOL bret = TRUE;

	if(m_nEditStatus != dbEditNone)
	{

		try
		{
			if(m_pRecordset->Update() != S_OK)
				bret = FALSE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			bret = FALSE;
		}

		if(!bret)
			m_pRecordset->CancelUpdate();
		m_nEditStatus = dbEditNone;
	}
	return bret;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::CancelUpdate()
{
	m_pRecordset->CancelUpdate();
	m_nEditStatus = dbEditNone;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, CString strValue)
{
	_variant_t vtFld;
	_variant_t vtIndex;	

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	if(!strValue.IsEmpty())
		vtFld.vt = VT_BSTR;
	else
		vtFld.vt = VT_NULL;

	//Corrected by Giles Forster 10/03/2001
	vtFld.bstrVal = strValue.AllocSysString();

	return PutFieldValue(vtIndex, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, CString strValue)
{
	_variant_t vtFld;

	if(!strValue.IsEmpty())
		vtFld.vt = VT_BSTR;
	else
		vtFld.vt = VT_NULL;

	//Corrected by Giles Forster 10/03/2001
	vtFld.bstrVal = strValue.AllocSysString();

	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, int nValue)
{
	_variant_t vtFld;

	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, int nValue)
{
	_variant_t vtFld;

	vtFld.vt = VT_I2;
	vtFld.iVal = nValue;


	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);

}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, long lValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_I4;
	vtFld.lVal = lValue;

	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, unsigned long ulValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_UI4;
	vtFld.ulVal = ulValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);

}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, unsigned long ulValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_UI4;
	vtFld.ulVal = ulValue;

	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, double dblValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_R8;
	vtFld.dblVal = dblValue;

	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, COleDateTime time)
{
	_variant_t vtFld;
	vtFld.vt = VT_DATE;
	vtFld.date = time;

	return PutFieldValue(lpFieldName, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, bool bValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BOOL;
	vtFld.boolVal = bValue;

	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtFld);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, bool bValue)
{
	_variant_t vtFld;
	vtFld.vt = VT_BOOL;
	vtFld.boolVal = bValue;

	return PutFieldValue(lpFieldName, vtFld);
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(int nIndex, _variant_t vtValue)
{
	_variant_t vtIndex;

	vtIndex.vt = VT_I2;
	vtIndex.iVal = nIndex;

	return PutFieldValue(vtIndex, vtValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue)
{	
	return PutFieldValue(lpFieldName, vtValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetBookmark()
{
	if(m_varBookmark.vt != VT_EMPTY)
	{
		m_pRecordset->Bookmark = m_varBookmark;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Delete()
{
	if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
		return FALSE;

	if(m_pRecordset->Update() != S_OK)
		return FALSE;

	m_nEditStatus = dbEditNone;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Find(LPCTSTR lpFind, int nSearchDirection)
{

	m_strFind = lpFind;
	m_nSearchDirection = nSearchDirection;

	ATLASSERT(!m_strFind.IsEmpty());

	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchForward, "");
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return TRUE;
		}
	}
	else if(m_nSearchDirection == searchBackward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchBackward, "");
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return TRUE;
		}
	}
	else
	{
//		TRACE("Unknown parameter. %d", nSearchDirection);
		m_nSearchDirection = searchForward;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::FindFirst(LPCTSTR lpFind)
{
	m_pRecordset->MoveFirst();
	return Find(lpFind);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::FindNext()
{
	if(m_nSearchDirection == searchForward)
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchForward, m_varBookFind);
		if(!IsEof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return TRUE;
		}
	}
	else
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchBackward, m_varBookFind);
		if(!IsBof())
		{
			m_varBookFind = m_pRecordset->Bookmark;
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 필드 이름에 값을 적용
//|
//---------------------------------------------------------------
BOOL CADORecordset::PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld)
{
	if(m_nEditStatus == dbEditNone)
		return FALSE;

	try
	{
		m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld; 
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;	
	}
}
//--------------------------------------------------------------
//|
//| 인덱스 필드에 값을 넣는다
//|
//---------------------------------------------------------------
BOOL CADORecordset::PutFieldValue(_variant_t vtIndex, _variant_t vtFld)
{
	if(m_nEditStatus == dbEditNone)
		return FALSE;

	try
	{
		m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Clone(CADORecordset &pRs)
{
	try
	{
		pRs.m_pRecordset = m_pRecordset->Clone(adLockUnspecified);
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetFilter(LPCTSTR strFilter)
{
	ATLASSERT(IsOpen());

	try
	{
		m_pRecordset->PutFilter(strFilter);
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SetSort(LPCTSTR strCriteria)
{
	ATLASSERT(IsOpen());

	try
	{
		m_pRecordset->PutSort(strCriteria);
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::SaveAsXML(LPCTSTR lpstrXMLFile)
{
	HRESULT hr;

	ATLASSERT(IsOpen());

	try
	{
		hr = m_pRecordset->Save(lpstrXMLFile, adPersistXML);
		return hr == S_OK;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::OpenXML(LPCTSTR lpstrXMLFile)
{
	HRESULT hr = S_OK;

	if(IsOpen())
		Close();

	try
	{
		hr = m_pRecordset->Open(lpstrXMLFile, "Provider=MSPersist;", adOpenForwardOnly, adLockOptimistic, adCmdFile);
		return hr == S_OK;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADORecordset::Execute(IADOCommand* pAdoCommand)
{
	if(IsOpen())
		Close();

	ATLASSERT(!pAdoCommand->GetText().IsEmpty());
	try
	{
		m_pConnection->CursorLocation = adUseClient;
		m_pRecordset = pAdoCommand->GetCommand()->Execute(NULL, NULL, pAdoCommand->GetType());
		return TRUE;
	}
	catch(_com_error &e)
	{
		dump_com_error(e);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADORecordset::dump_com_error(_com_error &e)
{
	CString ErrorStr;


	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CADORecordset Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = _T("Query = " + GetQuery() + '\n' + ErrorStr);
	m_dwLastError = e.Error();

	debug->Write(ErrorStr.GetBuffer(0));
}
