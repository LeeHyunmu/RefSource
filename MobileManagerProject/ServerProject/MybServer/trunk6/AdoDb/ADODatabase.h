
#ifndef __ADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CADODatabase : public IADODatabase
{
public:
	//������
	CADODatabase();
	//�Ҹ���
	~CADODatabase();

//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	BOOL Execute(LPCTSTR lpstrExec);
	int GetRecordsAffected();
	DWORD GetRecordCount(_RecordsetPtr m_pRs);
	long BeginTransaction(); 
	long CommitTransaction(); 
	long RollbackTransaction(); 
	void Close();
	void SetConnectionMode(cadoConnectModeEnum nMode);
	void SetConnectionString(LPCTSTR lpstrConnection);
	CString GetConnectionString();
	CString GetLastErrorString(); 
	DWORD GetLastError();
	CString GetErrorDescription(); 
	void SetConnectionTimeout(long nConnectionTimeout = 30);
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
public:
	_ConnectionPtr m_pConnection;
protected:
	CString m_strConnection;
	CString m_strLastError;
	CString m_strErrorDescription;
	DWORD m_dwLastError;
	int m_nRecordsAffected;
	long m_nConnectionTimeout;
	BOOL bDbConnect;
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL initAdoDatabase(IMyObject* _obj);
	//���ؼ��� �ξ��� ���� �ľ�
	virtual BOOL IsOpen();
	//���ؼ� ������
	virtual _ConnectionPtr GetActiveConnection(); 
	//DB����
	virtual BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""));

//=======================================================//

};
#endif// __ADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__
