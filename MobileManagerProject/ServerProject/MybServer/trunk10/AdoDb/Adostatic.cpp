
#include "stdAfx.h"
#include "Adostatic.h"

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CJetEngine::CCompactDatabase(CString strDatabaseSource, CString strDatabaseDestination)
{
	try
	{
		::CoInitialize(NULL);
		IJetEnginePtr jet(__uuidof(JetEngine));		
		HRESULT hr = jet->CompactDatabase(_bstr_t(strDatabaseSource.GetBuffer(0)), _bstr_t(strDatabaseDestination.GetBuffer(0)));
		::CoUninitialize();
		return hr == S_OK;
	}
	catch(_com_error &e) 
	{  
		::CoUninitialize();
		CString ErrorStr;
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		ErrorStr.Format( "CADOCommand Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n",
			e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
		::MessageBox(NULL, ErrorStr, "확인", MB_OK);
		return FALSE;
	} 
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CAdostaticApi::IntToStr(int nVal)
{
	CString strRet;
	char buff[10];

	_itoa_s(nVal, buff, 10);
	strRet = buff;
	return strRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CAdostaticApi::LongToStr(long lVal)
{
	CString strRet;
	char buff[20];

	_ltoa_s(lVal, buff, 10);
	strRet = buff;
	return strRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CAdostaticApi::ULongToStr(unsigned long ulVal)
{
	CString strRet;
	char buff[20];

	_ultoa_s(ulVal, buff, 10);
	strRet = buff;
	return strRet;

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CAdostaticApi::DblToStr(double dblVal, int ndigits)
{
	CString strRet;
	char buff[50];

	_gcvt_s(buff,sizeof(buff),dblVal, ndigits);
	strRet = buff;
	return strRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CAdostaticApi::DblToStr(float fltVal)
{
	CString strRet = _T("");
	char buff[50];

	_gcvt_s(buff,sizeof(buff),fltVal, 10);
	strRet = buff;
	return strRet;
}
//--------------------------------------------------------------
//|
//|VARIANT 형으로 변환
//|
//---------------------------------------------------------------
void CAdostaticApi::BlobToVariant(VARIANT& varArray, void* data, long size)
{
	BYTE* pByte;
	try
	{
		SAFEARRAY FAR* psa;
		SAFEARRAYBOUND rgsabound[1];
		rgsabound[0].lLbound=0;
		rgsabound[0].cElements=size;

		psa = SafeArrayCreate(VT_I1, 1, rgsabound);
		if(SafeArrayAccessData(psa, (void**)&pByte)==NOERROR)
		{
			memcpy_s((LPVOID)pByte, size, (LPVOID)data, size);
		}
		SafeArrayUnaccessData(psa);
		varArray.vt=VT_ARRAY|VT_UI1;
		varArray.parray=psa;
	}
	catch (_com_error& e)
	{
		CString ErrorStr;
		ErrorStr.Format("Error = %s",(LPSTR)(LPCTSTR)e.Description());
		::MessageBox(NULL, ErrorStr, "확인", MB_OK);
	}
}