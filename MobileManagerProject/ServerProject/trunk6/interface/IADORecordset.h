#ifndef __IADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

enum cadoOpenEnum
{
	openUnknown = 0,
	openQuery = 1,
	openTable = 2,
	openStoredProc = 3
};
enum cadoEditEnum
{
	dbEditNone = 0,
	dbEditNew = 1,
	dbEdit = 2
};
enum cadoPositionEnum
{

	positionUnknown = -1,
	positionBOF = -2,
	positionEOF = -3
};
enum cadoSearchEnum
{	
	searchForward = 1,
	searchBackward = -1
};
enum cadoDataType
{
	typeEmpty = ADODB::adEmpty,
	typeTinyInt = ADODB::adTinyInt,
	typeSmallInt = ADODB::adSmallInt,
	typeInteger = ADODB::adInteger,
	typeBigInt = ADODB::adBigInt,
	typeUnsignedTinyInt = ADODB::adUnsignedTinyInt,
	typeUnsignedSmallInt = ADODB::adUnsignedSmallInt,
	typeUnsignedInt = ADODB::adUnsignedInt,
	typeUnsignedBigInt = ADODB::adUnsignedBigInt,
	typeSingle = ADODB::adSingle,
	typeDouble = ADODB::adDouble,
	typeCurrency = ADODB::adCurrency,
	typeDecimal = ADODB::adDecimal,
	typeNumeric = ADODB::adNumeric,
	typeBoolean = ADODB::adBoolean,
	typeError = ADODB::adError,
	typeUserDefined = ADODB::adUserDefined,
	typeVariant = ADODB::adVariant,
	typeIDispatch = ADODB::adIDispatch,
	typeIUnknown = ADODB::adIUnknown,
	typeGUID = ADODB::adGUID,
	typeDate = ADODB::adDate,
	typeDBDate = ADODB::adDBDate,
	typeDBTime = ADODB::adDBTime,
	typeDBTimeStamp = ADODB::adDBTimeStamp,
	typeBSTR = ADODB::adBSTR,
	typeChar = ADODB::adChar,
	typeVarChar = ADODB::adVarChar,
	typeLongVarChar = ADODB::adLongVarChar,
	typeWChar = ADODB::adWChar,
	typeVarWChar = ADODB::adVarWChar,
	typeLongVarWChar = ADODB::adLongVarWChar,
	typeBinary = ADODB::adBinary,
	typeVarBinary = ADODB::adVarBinary,
	typeLongVarBinary = ADODB::adLongVarBinary,
	typeChapter = ADODB::adChapter,
	typeFileTime = ADODB::adFileTime,
	typePropVariant = ADODB::adPropVariant,
	typeVarNumeric = ADODB::adVarNumeric,
	typeArray = ADODB::adVariant
};
enum cadoSchemaType 
{
	schemaSpecific = adSchemaProviderSpecific,	
	schemaAsserts = adSchemaAsserts,
	schemaCatalog = adSchemaCatalogs,
	schemaCharacterSet = adSchemaCharacterSets,
	schemaCollections = adSchemaCollations,
	schemaColumns = adSchemaColumns,
	schemaConstraints = adSchemaCheckConstraints,
	schemaConstraintColumnUsage = adSchemaConstraintColumnUsage,
	schemaConstraintTableUsage  = adSchemaConstraintTableUsage,
	shemaKeyColumnUsage = adSchemaKeyColumnUsage,
	schemaTableConstraints = adSchemaTableConstraints,
	schemaColumnsDomainUsage = adSchemaColumnsDomainUsage,
	schemaIndexes = adSchemaIndexes,
	schemaColumnPrivileges = adSchemaColumnPrivileges,
	schemaTablePrivileges = adSchemaTablePrivileges,
	schemaUsagePrivileges = adSchemaUsagePrivileges,
	schemaProcedures = adSchemaProcedures,
	schemaTables =adSchemaTables,
	schemaProviderTypes = adSchemaProviderTypes,
	schemaViews = adSchemaViews,
	schemaProcedureParameters = adSchemaProcedureParameters,
	schemaForeignKeys = adSchemaForeignKeys,
	schemaPrimaryKeys = adSchemaPrimaryKeys,
	schemaProcedureColumns = adSchemaProcedureColumns,
	schemaDBInfoKeywords = adSchemaDBInfoKeywords,
	schemaDBInfoLiterals = adSchemaDBInfoLiterals,
	schemaCubes = adSchemaCubes,
	schemaDimensions = adSchemaDimensions,
	schemaHierarchies  = adSchemaHierarchies, 
	schemaLevels = adSchemaLevels,
	schemaMeasures = adSchemaMeasures,
	schemaProperties = adSchemaProperties,
	schemaMembers = adSchemaMembers,
};

struct CADOFieldInfo
{
	char m_strName[30]; 
	short m_nType;
	long m_lSize; 
	long m_lDefinedSize;
	long m_lAttributes;
	short m_nOrdinalPosition;
	BOOL m_bRequired;   
	BOOL m_bAllowZeroLength; 
	long m_lCollatingOrder;  
};
//--------------------------------------------------------------
//|
//|IADORecordset �������̽�
//|
//---------------------------------------------------------------
interface IADORecordset
{
	virtual void Release()=0;
	//�����޴��� �ʱ�ȭ
	virtual BOOL initAdoRecordset(IMyObject* _obj)=0;
	virtual BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = openUnknown)=0;
	//connectPtr�� �̹� ���� �Ǿ� ���� ��
	virtual BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = openUnknown)=0;
	//�ʵ��� ���� �����´�(����Ÿ Ÿ�Կ� ���� �������̵�)
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, int& nValue)=0;
	virtual BOOL GetFieldValue(int nIndex, int& nValue)=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, long& lValue)=0;
	virtual BOOL GetFieldValue(int nIndex, long& lValue)=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue)=0;
	virtual BOOL GetFieldValue(int nIndex, unsigned long& ulValue)=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, double& dbValue)=0;
	virtual BOOL GetFieldValue(int nIndex, double& dbValue)=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, CString& strValue, CString strDateFormat = _T(""))=0;
	virtual BOOL GetFieldValue(int nIndex, CString& strValue, CString strDateFormat = _T(""))=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime& time)=0;
	virtual BOOL GetFieldValue(int nIndex, COleDateTime& time)=0;
	virtual BOOL GetFieldValue(int nIndex, bool& bValue)=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, bool& bValue)=0;
	virtual BOOL GetFieldValue(int nIndex, _variant_t& vtValue)=0;
	virtual BOOL GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue)=0;
	//�ʵ� ���� ä���(����Ÿ Ÿ�Կ� ���� �������̵�)
	virtual BOOL SetFieldValue(int nIndex, int nValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, int nValue)=0;
	virtual BOOL SetFieldValue(int nIndex, long lValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue)=0;
	virtual BOOL SetFieldValue(int nIndex, unsigned long lValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, unsigned long lValue)=0;
	virtual BOOL SetFieldValue(int nIndex, double dblValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, double dblValue)=0;
	virtual BOOL SetFieldValue(int nIndex, CString strValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue)=0;
	virtual BOOL SetFieldValue(int nIndex, COleDateTime time)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime time)=0;
	virtual BOOL SetFieldValue(int nIndex, bool bValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, bool bValue)=0;
	virtual BOOL SetFieldValue(int nIndex, _variant_t vtValue)=0;
	virtual BOOL SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue)=0;
	//���ڵ� ������ ������
	virtual long GetPageCount()=0;
	virtual long GetPageSize()=0;
	virtual void SetPageSize(int nSize)=0;
	//�ʵ� ī��Ʈ
	virtual long GetAbsolutePage()=0;
	virtual void SetAbsolutePage(int nPage)=0;
	//���ڵ� �� ��
	virtual BOOL IsEof()=0;
	virtual BOOL IsEOF()=0;
	//���ڵ� �� ����
	virtual BOOL IsBof()=0;
	virtual BOOL IsBOF()=0;
	//ó�� ���ڵ�
	virtual void MoveFirst()=0;
	// ���� ���ڵ�
	virtual void MoveNext()=0; 
	//�� ���ڵ�
	virtual void MovePrevious()=0;
	//������ ���ڵ�
	virtual void MoveLast()=0; 
	//���ڵ� �ݴ�
	virtual void Close()=0;
	//
	virtual BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue)=0;
	virtual BOOL GetChunk(int nIndex, CString& strValue)=0;
	virtual BOOL GetChunk(LPCTSTR lpFieldName, LPVOID pData)=0;
	virtual BOOL GetChunk(int nIndex, LPVOID pData)=0;
	//
	virtual BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes)=0;
	virtual BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes)=0;
	//�ʵ� ������ ��ƿ´�
	virtual BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)=0;
	virtual BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)=0;
	//�ʵ� �߰�
	virtual BOOL AddNew()=0;
	virtual BOOL AddNew(CADORecordBinding &pAdoRecordBinding)=0;
	//�ʵ� ������Ʈ
	virtual BOOL Update()=0;
};

#endif //__IADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__