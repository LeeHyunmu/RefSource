
#include "stdAfx.h"
#include "ADOCommand.h"
#include  <crtdbg.h>

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CADOCommand::CADOCommand():
obj(NULL),
debug(NULL)
{
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CADOCommand::CADOCommand(IADODatabase* pAdoDatabase, CString strCommandText, int nCommandType)
{
	m_pCommand = NULL;
	m_pCommand.CreateInstance(__uuidof(Command));
	m_strCommandText = strCommandText;
	m_pCommand->CommandText = m_strCommandText.AllocSysString();
	m_nCommandType = nCommandType;
	m_pCommand->CommandType = (CommandTypeEnum)m_nCommandType;
	m_pCommand->ActiveConnection = pAdoDatabase->GetActiveConnection();	
	m_nRecordsAffected = 0;
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CADOCommand::~CADOCommand()
{
	if(m_pCommand)
	{
		m_pCommand.Release();
		m_pCommand = NULL;
	}
	m_strCommandText = _T("");
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CADOCommand::initAdoCommand(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();

	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CADOCommand::Release()
{
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADOCommand::SetTimeout(long nTimeOut)
{
	m_pCommand->PutCommandTimeout(nTimeOut);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CADOCommand::GetType()
{
	return m_nCommandType;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADOCommand::GetText()
{
	return m_strCommandText;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CADOCommand::GetRecordsAffected()
{
	return m_nRecordsAffected;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
_CommandPtr CADOCommand::GetCommand()
{
	return m_pCommand;
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::AddParameter(IADOParameter* pAdoParameter)
{
	ATLASSERT(pAdoParameter->GetParameter() != NULL);

	try
	{
		m_pCommand->Parameters->Append(pAdoParameter->GetParameter());
		return TRUE;
	}
	catch(_com_error& e)
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
BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, int nValue)
{

	_variant_t vtValue;

	vtValue.vt = VT_I2;
	vtValue.iVal = nValue;

	return AddParameter(strName, nType, nDirection, lSize, vtValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, long lValue)
{

	_variant_t vtValue;

	vtValue.vt = VT_I4;
	vtValue.lVal = lValue;

	return AddParameter(strName, nType, nDirection, lSize, vtValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, double dblValue, int nPrecision, int nScale)
{

	_variant_t vtValue;

	vtValue.vt = VT_R8;
	vtValue.dblVal = dblValue;

	return AddParameter(strName, nType, nDirection, lSize, vtValue, nPrecision, nScale);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, CString strValue)
{

	_variant_t vtValue;

	vtValue.vt = VT_BSTR;
	vtValue.bstrVal = strValue.AllocSysString();

	return AddParameter(strName, nType, nDirection, lSize, vtValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, COleDateTime time)
{

	_variant_t vtValue;

	vtValue.vt = VT_DATE;
	vtValue.date = time;

	return AddParameter(strName, nType, nDirection, lSize, vtValue);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::AddParameter(CString strName, int nType, int nDirection, long lSize, _variant_t vtValue, int nPrecision, int nScale)
{
	try
	{
		_ParameterPtr pParam = m_pCommand->CreateParameter(strName.AllocSysString(), (DataTypeEnum)nType, (ParameterDirectionEnum)nDirection, lSize, vtValue);
		pParam->PutPrecision(nPrecision);
		pParam->PutNumericScale(nScale);
		m_pCommand->Parameters->Append(pParam);

		return TRUE;
	}
	catch(_com_error& e)
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
void CADOCommand::SetText(CString strCommandText)
{
	ATLASSERT(!strCommandText.IsEmpty());

	m_strCommandText = strCommandText;
	m_pCommand->CommandText = m_strCommandText.AllocSysString();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADOCommand::SetType(int nCommandType)
{
	m_nCommandType = nCommandType;
	m_pCommand->CommandType = (CommandTypeEnum)m_nCommandType;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOCommand::Execute()
{
	_variant_t vRecords;
	m_nRecordsAffected = 0;
	try
	{
		m_pCommand->Execute(&vRecords, NULL, adCmdStoredProc);
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
void CADOCommand::dump_com_error(_com_error &e)
{
	CString ErrorStr;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CADOCommand Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = ErrorStr;
	m_dwLastError = e.Error();
	debug->Write(ErrorStr.GetBuffer(0));
}