
#ifndef __ADODSTATIC_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADODSTATIC_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 선언
//======================================================//

//=======================================================//
class CJetEngine
{
public:
	CJetEngine(){}
	virtual ~CJetEngine(){}
	static BOOL CCompactDatabase(CString strDatabaseSource, CString strDatabaseDestination);
};

class CAdostaticApi
{
public:
	CAdostaticApi(){}
	virtual ~CAdostaticApi(){}
	//
	static CString IntToStr(int nVal);
	static CString LongToStr(long lVal);
	static CString ULongToStr(unsigned long ulVal);
	static CString DblToStr(double dblVal, int ndigits);
	static CString DblToStr(float fltVal);
	static void BlobToVariant(VARIANT& varArray, void* data, long size);
};

#endif// __ADODSTATIC_74F736E25AB94b5dA789E4730F58F2A1_H__
