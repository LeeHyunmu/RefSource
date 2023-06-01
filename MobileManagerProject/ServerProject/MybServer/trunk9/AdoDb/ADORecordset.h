
#ifndef __ADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CADORecordset : public IADORecordset
{
public:
	//������
	CADORecordset();
	CADORecordset(IADODatabase* pAdoDatabase);
	//�Ҹ���
	~CADORecordset();

//======================================================//
//			�Ϲ��Լ� ����
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
//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			���� ����
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
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	//�ʱ�ȭ
	virtual BOOL initAdoRecordset(IMyObject* _obj);
	//connectPtr ���� �ϸ鼭 ���� 
	virtual BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = openUnknown);
	//connectPtr�� �̹� ���� �Ǿ� ���� ��
	virtual BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = openUnknown);
	//�ʵ��� ���� �����´�(����Ÿ Ÿ�Կ� ���� �������̵�)
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
	//�ʵ� ���� ä���(����Ÿ Ÿ�Կ� ���� �������̵�)
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
	//���ڵ� ������ ������
	virtual long GetPageCount();
	virtual long GetPageSize();
	virtual void SetPageSize(int nSize);
	//���ڵ� ī��Ʈ ����
	virtual long GetFieldCount();
	//�ʵ� ī��Ʈ
	virtual long GetAbsolutePage();
	virtual void SetAbsolutePage(int nPage);
	//���ڵ� �� ��
	virtual BOOL IsEof();
	virtual BOOL IsEOF();
	//���ڵ� �� ����
	virtual BOOL IsBof();
	virtual BOOL IsBOF();
	//ó�� ���ڵ�
	virtual void MoveFirst();
	// ���� ���ڵ�
	virtual void MoveNext(); 
	//�� ���ڵ�
	virtual void MovePrevious();
	//������ ���ڵ�
	virtual void MoveLast();
	//���ڵ� �ݴ�
	virtual void Close();
	//
	virtual BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);
	virtual BOOL GetChunk(int nIndex, CString& strValue);
	virtual BOOL GetChunk(LPCTSTR lpFieldName, LPVOID pData);
	virtual BOOL GetChunk(int nIndex, LPVOID pData);
	//
	virtual BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes);
	virtual BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes);
	//�ʵ� ������ ��ƿ´�
	virtual BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo);
	virtual BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo);
	//�ʵ� �߰�
	virtual BOOL AddNew();
	virtual BOOL AddNew(CADORecordBinding &pAdoRecordBinding);
	//�ʵ� ������Ʈ
	virtual BOOL Update();

//=======================================================//

};
#endif// __ADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__
