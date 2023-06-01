
#ifndef __ADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CADOParameter : public IADOParameter
{
public:
	//생성자
	CADOParameter();
	CADOParameter(int nType, long lSize = 0, int nDirection = paramInput, CString strName = _T(""));
	//소멸자
	~CADOParameter();

//======================================================//
//			일반함수 선언
//======================================================//
	BOOL SetValue(int nValue);
	BOOL SetValue(long lValue);
	BOOL SetValue(double dbValue);
	BOOL SetValue(CString strValue);
	BOOL SetValue(COleDateTime time);
	BOOL SetValue(_variant_t vtValue);
	BOOL GetValue(int& nValue);
	BOOL GetValue(long& lValue);
	BOOL GetValue(double& dbValue);
	BOOL GetValue(CString& strValue, CString strDateFormat = _T(""));
	BOOL GetValue(COleDateTime& time);
	BOOL GetValue(_variant_t& vtValue);
	void SetPrecision(int nPrecision);
	void SetScale(int nScale);
	void SetName(CString strName);
	CString GetName();
	int GetType();
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			변수 선언
//======================================================//
protected:
	void dump_com_error(_com_error &e);

protected:
	_ParameterPtr m_pParameter;
	CString m_strName;
	int m_nType;
	CString m_strLastError;
	DWORD m_dwLastError;

//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initAdoParameter(IMyObject* _obj);
	//
	virtual _ParameterPtr GetParameter();
//=======================================================//

};
#endif// __ADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__
