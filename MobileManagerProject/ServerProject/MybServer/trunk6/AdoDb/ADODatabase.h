
#ifndef __ADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CADODatabase : public IADODatabase
{
public:
	//생성자
	CADODatabase();
	//소멸자
	~CADODatabase();

//======================================================//
//			일반함수 선언
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
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initAdoDatabase(IMyObject* _obj);
	//컨넥션이 맺어진 상태 파악
	virtual BOOL IsOpen();
	//컨넥션 포인터
	virtual _ConnectionPtr GetActiveConnection(); 
	//DB열기
	virtual BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""));

//=======================================================//

};
#endif// __ADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__
