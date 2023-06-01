
#ifndef __ADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CADOCommand : public IADOCommand
{
public:
	//������
	CADOCommand();
	CADOCommand(IADODatabase* pAdoDatabase, CString strCommandText = _T(""), int nCommandType = typeCmdStoredProc);
	//�Ҹ���
	~CADOCommand();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	void SetTimeout(long nTimeOut);
	void SetText(CString strCommandText);
	void SetType(int nCommandType);
	BOOL AddParameter(IADOParameter* pAdoParameter);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, int nValue);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, long lValue);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, double dblValue, int nPrecision = 0, int nScale = 0);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, CString strValue);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, COleDateTime time);
	BOOL AddParameter(CString strName, int nType, int nDirection, long lSize, _variant_t vtValue, int nPrecision = 0, int nScale = 0);
	BOOL Execute();
	int GetRecordsAffected();
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
	_CommandPtr m_pCommand;
	int m_nCommandType;
	int m_nRecordsAffected;
	CString m_strCommandText;
	CString m_strLastError;
	DWORD m_dwLastError;

//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();

	virtual BOOL initAdoCommand(IMyObject* _obj);
	//
	virtual CString GetText();
	//
	virtual	_CommandPtr GetCommand();
	//
	virtual int GetType();
//=======================================================//

};
#endif// __ADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__
