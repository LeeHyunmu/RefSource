
#ifndef __ADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CADORecordset : public IADORecordset
{
public:
	//생성자
	CADORecordset();
	CADORecordset(IADODatabase* pAdoDatabase);
	//소멸자
	~CADORecordset();

//======================================================//
//			일반함수 선언
//======================================================//
	BOOL Clone(CADORecordset& pRs);

	BOOL SetFieldEmpty(int nIndex);
	BOOL SetFieldEmpty(LPCTSTR lpFieldName);

	void CancelUpdate();
	void Edit();

	BOOL Find(LPCTSTR lpFind, int nSearchDirection = CADORecordset::searchForward);
	BOOL FindFirst(LPCTSTR lpFind);
	BOOL FindNext();

	CString GetQuery(); 
	void SetQuery(LPCSTR strQuery); 
	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	DWORD GetRecordCount();
	BOOL IsOpen();

	BOOL OpenSchema(int nSchema, LPCTSTR SchemaID = _T(""));
	
	BOOL IsFieldNull(LPCTSTR lpFieldName);
	BOOL IsFieldNull(int nIndex);
	BOOL IsFieldEmpty(LPCTSTR lpFieldName);
	BOOL IsFieldEmpty(int nIndex);	

	long GetAbsolutePosition();
	void SetAbsolutePosition(int nPosition);

	CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows = 0);
	CString GetLastErrorString(); 
	DWORD GetLastError();
	void GetBookmark();
	BOOL SetBookmark();
	BOOL Delete();
	BOOL IsConnectionOpen();
	_RecordsetPtr GetRecordset();
	_ConnectionPtr GetActiveConnection(); 

	BOOL SetFilter(LPCTSTR strFilter);
	BOOL SetSort(LPCTSTR lpstrCriteria);
	BOOL SaveAsXML(LPCTSTR lpstrXMLFile);
	BOOL OpenXML(LPCTSTR lpstrXMLFile);
	BOOL Execute(IADOCommand* pCommand);
	BOOL Requery();
protected:
	BOOL PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld);
	BOOL PutFieldValue(_variant_t vtIndex, _variant_t vtFld);
	BOOL GetFieldInfo(FieldPtr pField, CADOFieldInfo* fldInfo);
	BOOL GetChunk(FieldPtr pField, CString& strValue);
	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			변수 선언
//======================================================//
public:
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCmd;

protected:
	_ConnectionPtr m_pConnection;
	int m_nSearchDirection;
	CString m_strFind;
	_variant_t m_varBookFind;
	_variant_t m_varBookmark;
	int m_nEditStatus;
	CString m_strLastError;
	DWORD m_dwLastError;
	void dump_com_error(_com_error &e);
	IADORecordBinding *m_pRecBinding;
	CString m_strQuery;
public:
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	//초기화
	virtual BOOL initAdoRecordset(IMyObject* _obj);
	//connectPtr 세팅 하면서 쿼리 
	virtual BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = openUnknown);
	//connectPtr이 이미 설정 되어 있을 때
	virtual BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = openUnknown);
	//필드의 값을 가져온다(데이타 타입에 따라서 오버라이딩)
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, int& nValue);
	virtual BOOL GetFieldValue(int nIndex, int& nValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, long& lValue);
	virtual BOOL GetFieldValue(int nIndex, long& lValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue);
	virtual BOOL GetFieldValue(int nIndex, unsigned long& ulValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, double& dbValue);
	virtual BOOL GetFieldValue(int nIndex, double& dbValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, CString& strValue, CString strDateFormat = _T(""));
	virtual BOOL GetFieldValue(int nIndex, CString& strValue, CString strDateFormat = _T(""));
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time);
	virtual BOOL GetFieldValue(int nIndex, COleDateTime& time);
	virtual BOOL GetFieldValue(int nIndex, bool& bValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, bool& bValue);
	virtual BOOL GetFieldValue(int nIndex, _variant_t& vtValue);
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue);
	//필드 값을 채운다(데이타 타입에 따라서 오버라이딩)
	virtual BOOL SetFieldValue(int nIndex, int nValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue);
	virtual BOOL SetFieldValue(int nIndex, long lValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
	virtual BOOL SetFieldValue(int nIndex, unsigned long lValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, unsigned long lValue);
	virtual BOOL SetFieldValue(int nIndex, double dblValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue);
	virtual BOOL SetFieldValue(int nIndex, CString strValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);
	virtual BOOL SetFieldValue(int nIndex, COleDateTime time);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time);
	virtual BOOL SetFieldValue(int nIndex, bool bValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, bool bValue);
	virtual BOOL SetFieldValue(int nIndex, _variant_t vtValue);
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue);
	//레코드 페이지 사이즈
	virtual long GetPageCount();
	virtual long GetPageSize();
	virtual void SetPageSize(int nSize);
	//레코드 카운트 리턴
	virtual long GetFieldCount();
	//필드 카운트
	virtual long GetAbsolutePage();
	virtual void SetAbsolutePage(int nPage);
	//레코드 셋 끝
	virtual BOOL IsEof();
	virtual BOOL IsEOF();
	//레코드 셋 시작
	virtual BOOL IsBof();
	virtual BOOL IsBOF();
	//처음 레코드
	virtual void MoveFirst();
	// 다음 레코드
	virtual void MoveNext(); 
	//전 레코드
	virtual void MovePrevious();
	//마지막 레코드
	virtual void MoveLast();
	//레코드 닫다
	virtual void Close();
	//
	virtual BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);
	virtual BOOL GetChunk(int nIndex, CString& strValue);
	virtual BOOL GetChunk(LPCTSTR lpFieldName, LPVOID pData);
	virtual BOOL GetChunk(int nIndex, LPVOID pData);
	//
	virtual BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes);
	virtual BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes);
	//필드 정보를 담아온다
	virtual BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	virtual BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	//필드 추가
	virtual BOOL AddNew();
	virtual BOOL AddNew(CADORecordBinding &pAdoRecordBinding);
	//필드 업데이트
	virtual BOOL Update();

//=======================================================//

};
#endif// __ADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__
