
#include "stdAfx.h"
#include "ADOParameter.h"
#include "Adostatic.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CADOParameter::CADOParameter():
obj(NULL),
debug(NULL)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CADOParameter::~CADOParameter()
{
	if(m_pParameter)
	{
		m_pParameter.Release();
		m_pParameter = NULL;
	}
	m_strName = _T("");
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CADOParameter::initAdoParameter(IMyObject* _obj)
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
void CADOParameter::Release()
{
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADOParameter::SetPrecision(int nPrecision)
{
	m_pParameter->PutPrecision(nPrecision);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADOParameter::SetScale(int nScale)
{
	m_pParameter->PutNumericScale(nScale);
};
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CADOParameter::SetName(CString strName)
{
	m_strName = strName;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CString CADOParameter::GetName()
{
	return m_strName;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CADOParameter::GetType()
{
	return m_nType;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
_ParameterPtr CADOParameter::GetParameter()
{
	return m_pParameter;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
CADOParameter::CADOParameter(int nType, long lSize, int nDirection, CString strName)
{
	m_pParameter = NULL;
	m_pParameter.CreateInstance(__uuidof(Parameter));
	m_strName = _T("");
	m_pParameter->Direction = (ParameterDirectionEnum)nDirection;
	m_strName = strName;
	m_pParameter->Name = m_strName.AllocSysString();
	m_pParameter->Type = (DataTypeEnum)nType;
	m_pParameter->Size = lSize;
	m_nType = nType;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
BOOL CADOParameter::SetValue(int nValue)
{
	_variant_t vtVal;

	ATLASSERT(m_pParameter != NULL);

	vtVal.vt = VT_I2;
	vtVal.iVal = nValue;

	try
	{
		if(m_pParameter->Size == 0)
			m_pParameter->Size = sizeof(int);

		m_pParameter->Value = vtVal;
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
BOOL CADOParameter::SetValue(long lValue)
{
	_variant_t vtVal;

	ATLASSERT(m_pParameter != NULL);

	vtVal.vt = VT_I4;
	vtVal.lVal = lValue;

	try
	{
		if(m_pParameter->Size == 0)
			m_pParameter->Size = sizeof(long);

		m_pParameter->Value = vtVal;
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
BOOL CADOParameter::SetValue(double dblValue)
{
	_variant_t vtVal;

	ATLASSERT(m_pParameter != NULL);

	vtVal.vt = VT_R8;
	vtVal.dblVal = dblValue;

	try
	{
		if(m_pParameter->Size == 0)
			m_pParameter->Size = sizeof(double);

		m_pParameter->Value = vtVal;
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
BOOL CADOParameter::SetValue(CString strValue)
{
	_variant_t vtVal;

	ATLASSERT(m_pParameter != NULL);

	if(!strValue.IsEmpty())
		vtVal.vt = VT_BSTR;
	else
		vtVal.vt = VT_NULL;

	//Corrected by Giles Forster 10/03/2001
	vtVal.bstrVal = strValue.AllocSysString();

	try
	{
		if(m_pParameter->Size == 0)
			m_pParameter->Size = sizeof(char) * strValue.GetLength();

		m_pParameter->Value = vtVal;
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
BOOL CADOParameter::SetValue(COleDateTime time)
{
	_variant_t vtVal;

	ATLASSERT(m_pParameter != NULL);

	vtVal.vt = VT_DATE;
	vtVal.date = time;

	try
	{
		if(m_pParameter->Size == 0)
			m_pParameter->Size = sizeof(DATE);

		m_pParameter->Value = vtVal;
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
BOOL CADOParameter::SetValue(_variant_t vtValue)
{

	ATLASSERT(m_pParameter != NULL);

	try
	{
		if(m_pParameter->Size == 0)
			m_pParameter->Size = sizeof(VARIANT);

		m_pParameter->Value = vtValue;
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
BOOL CADOParameter::GetValue(int& nValue)
{
	_variant_t vtVal;
	int nVal = 0;

	try
	{
		vtVal = m_pParameter->Value;

		switch(vtVal.vt)
		{
		case VT_BOOL:
			nVal = vtVal.boolVal;
			break;
		case VT_I2:
		case VT_UI1:
			nVal = vtVal.iVal;
			break;
		case VT_INT:
			nVal = vtVal.intVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			nVal = 0;
			break;
		default:
			nVal = vtVal.iVal;
		}	
		nValue = nVal;
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
BOOL CADOParameter::GetValue(long& lValue)
{
	_variant_t vtVal;
	long lVal = 0;

	try
	{
		vtVal = m_pParameter->Value;
		if(vtVal.vt != VT_NULL && vtVal.vt != VT_EMPTY)
			lVal = vtVal.lVal;
		lValue = lVal;
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
BOOL CADOParameter::GetValue(double& dbValue)
{
	_variant_t vtVal;
	double dblVal;
	try
	{
		vtVal = m_pParameter->Value;
		switch(vtVal.vt)
		{
		case VT_R4:
			dblVal = vtVal.fltVal;
			break;
		case VT_R8:
			dblVal = vtVal.dblVal;
			break;
		case VT_DECIMAL:
			//Corrected by Jos?Carlos Mart?ez Gal?
			dblVal = vtVal.decVal.Lo32;
			dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
			dblVal /= pow(10.f, vtVal.decVal.scale); 
			break;
		case VT_UI1:
			dblVal = vtVal.iVal;
			break;
		case VT_I2:
		case VT_I4:
			dblVal = vtVal.lVal;
			break;
		case VT_INT:
			dblVal = vtVal.intVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			dblVal = 0;
			break;
		default:
			dblVal = 0;
		}
		dbValue = dblVal;
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
BOOL CADOParameter::GetValue(CString& strValue, CString strDateFormat)
{
	_variant_t vtVal;
	CString strVal = _T("");

	try
	{
		vtVal = m_pParameter->Value;
		switch(vtVal.vt) 
		{
		case VT_R4:
			strVal = CAdostaticApi::DblToStr((float)vtVal.fltVal);
			break;
		case VT_R8:
			strVal = CAdostaticApi::DblToStr((float)vtVal.dblVal);
			break;
		case VT_BSTR:
			strVal = vtVal.bstrVal;
			break;
		case VT_I2:
		case VT_UI1:
			strVal = CAdostaticApi::IntToStr(vtVal.iVal);
			break;
		case VT_INT:
			strVal = CAdostaticApi::IntToStr(vtVal.intVal);
			break;
		case VT_I4:
			strVal = CAdostaticApi::LongToStr(vtVal.lVal);
			break;
		case VT_DECIMAL:
			{
				//Corrected by Jos?Carlos Mart?ez Gal?
				double val = vtVal.decVal.Lo32;
				val *= (vtVal.decVal.sign == 128)? -1 : 1;
				val /= pow(10.f, vtVal.decVal.scale); 
				strVal = CAdostaticApi::DblToStr((float)val);
			}
			break;
		case VT_DATE:
			{
				COleDateTime dt(vtVal);

				if(strDateFormat.IsEmpty())
					strDateFormat = _T("%Y-%m-%d %H:%M:%S");
				strVal = dt.Format(strDateFormat);
			}
			break;
		case VT_EMPTY:
		case VT_NULL:
			strVal.Empty();
			break;
		default:
			strVal.Empty();
			return FALSE;
		}
		strValue = strVal;
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
BOOL CADOParameter::GetValue(COleDateTime& time)
{
	_variant_t vtVal;

	try
	{
		vtVal = m_pParameter->Value;
		switch(vtVal.vt) 
		{
		case VT_DATE:
			{
				COleDateTime dt(vtVal);
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
BOOL CADOParameter::GetValue(_variant_t& vtValue)
{
	try
	{
		vtValue = m_pParameter->Value;
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
void CADOParameter::dump_com_error(_com_error &e)
{
	CString ErrorStr;


	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format( "CADOParameter Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
		e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
	m_strLastError = ErrorStr;
	m_dwLastError = e.Error();

	debug->Write(ErrorStr.GetBuffer(0));
}