
#ifndef __ADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CADOParameter : public IADOParameter
{
public:
	//������
	CADOParameter();
	CADOParameter(int nType, long lSize = 0, int nDirection = paramInput, CString strName = _T(""));
	//�Ҹ���
	~CADOParameter();

//======================================================//
//			�Ϲ��Լ� ����
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
//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			���� ����
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
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL initAdoParameter(IMyObject* _obj);
	//
	virtual _ParameterPtr GetParameter();
//=======================================================//

};
#endif// __ADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__
