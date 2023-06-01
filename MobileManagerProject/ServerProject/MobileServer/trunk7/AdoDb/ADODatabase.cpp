
#include "stdAfx.h"
#include "ADODatabase.h"

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
CADODatabase::CADODatabase():
obj(NULL),
debug(NULL),
bDbConnect(0)
{
	m_pConnection = NULL;
	m_strConnection = _T("");
	m_strLastError = _T("");
	m_dwLastError = 0;
	m_nRecordsAffected = 0;
	m_nConnectionTimeout = 0;
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CADODatabase::~CADODatabase()
{
	if(bDbConnect)
	{
		Close(); bDbConnect=FALSE;
	}
	if(m_pConnection)
	{
		m_pConnection.Release();
		m_pConnection = NULL;
	}
	m_strConnection = _T("");
	m_strLastError = _T("");
	m_dwLastError = 0;
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CADODatabase::initAdoDatabase(IMyObject* _obj)
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
void CADODatabase::Release()
{
	if(bDbConnect)
	{
		Close(); bDbConnect=FALSE;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
_ConnectionPtr CADODatabase::GetActiveConnection() 
{
	return m_pConnection;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CADODatabase::GetRecordsAffected()
{
	return m_nRecordsAffected;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADODatabase::BeginTransaction() 
{
	return m_pConnection->BeginTrans();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADODatabase::CommitTransaction() 
{
	return m_pConnection->CommitTrans();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
long CADODatabase::RollbackTransaction() 
{
	return m_pConnection->RollbackTrans();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADODatabase::SetConnectionMode(cadoConnectModeEnum nMode)
{
	m_pConnection->PutMode((enum ConnectModeEnum)nMode);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADODatabase::SetConnectionString(LPCTSTR lpstrConnection)
{
	m_strConnection = lpstrConnection;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADODatabase::GetConnectionString()
{
	return m_strConnection;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADODatabase::GetLastErrorString() 
{
	return m_strLastError;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD CADODatabase::GetLastError()
{
	return m_dwLastError;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADODatabase::GetErrorDescription() 
{
	return m_strErrorDescription;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADODatabase::SetConnectionTimeout(long nConnectionTimeout)
{
	m_nConnectionTimeout = nConnectionTimeout;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD CADODatabase::GetRecordCount(_RecordsetPtr m_pRs)
{
	DWORD numRows = 0;

	numRows = m_pRs->GetRecordCount();

	if(numRows == -1)
	{
		if(m_pRs->EndOfFile != VARIANT_TRUE)
			m_pRs->MoveFirst();

		while(m_pRs->EndOfFile != VARIANT_TRUE)
		{
			numRows++;
			m_pRs->MoveNext();
		}
		if(numRows > 0)
			m_pRs->MoveFirst();
	}
	return numRows;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADODatabase::Execute(LPCTSTR lpstrExec)
{
	ATLASSERT(m_pConnection != NULL);
	ATLASSERT(strcmp(lpstrExec, _T("")) != 0);
	_variant_t vRecords;

	m_nRecordsAffected = 0;

	try
	{
		m_pConnection->CursorLocation = adUseClient;
		m_pConnection->Execute(_bstr_t(lpstrExec), &vRecords, adExecuteNoRecords);
		m_nRecordsAffected = vRecords.iVal;
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
BOOL CADODatabase::Open(LPCTSTR lpstrConnection, LPCTSTR lpstrUserID, LPCTSTR lpstrPassword)
{
	HRESULT hr = S_OK;

	if(IsOpen())
		Close();
	m_pConnection.CreateInstance(__uuidof(Connection));
	if(strcmp(lpstrConnection, _T("")) != 0)
		m_strConnection = lpstrConnection;

	ATLASSERT(!m_strConnection.IsEmpty());

	try
	{
		if(m_nConnectionTimeout != 0)
			m_pConnection->PutConnectionTimeout(m_nConnectionTimeout);
		hr = m_pConnection->Open(_bstr_t(m_strConnection), _bstr_t(lpstrUserID), _bstr_t(lpstrPassword), NULL);
		if(hr == S_OK)
			bDbConnect=TRUE;
		return bDbConnect;
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
void CADODatabase::dump_com_error(_com_error &e)
{
	CString ErrorStr;

	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CADODataBase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);
	m_strErrorDescription = (LPCSTR)bstrDescription ;
	m_strLastError = _T("Connection String = " + GetConnectionString() + '\n' + ErrorStr);
	m_dwLastError = e.Error(); 

	debug->Write(ErrorStr.GetBuffer(0));
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADODatabase::IsOpen()
{
	if(m_pConnection )
		return m_pConnection->GetState() != adStateClosed;
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADODatabase::Close()
{
	if(IsOpen())
		m_pConnection->Close();
	m_pConnection=NULL;
}