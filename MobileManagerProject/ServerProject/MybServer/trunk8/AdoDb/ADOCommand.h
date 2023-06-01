
#ifndef __ADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CADOCommand : public IADOCommand
{
public:
	//생성자
	CADOCommand();
	CADOCommand(IADODatabase* pAdoDatabase, CString strCommandText = _T(""), int nCommandType = typeCmdStoredProc);
	//소멸자
	~CADOCommand();
//======================================================//
//			일반함수 선언
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
	_CommandPtr m_pCommand;
	int m_nCommandType;
	int m_nRecordsAffected;
	CString m_strCommandText;
	CString m_strLastError;
	DWORD m_dwLastError;

//======================================================//
//			인터페이스 선언 재 정의 함수
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
