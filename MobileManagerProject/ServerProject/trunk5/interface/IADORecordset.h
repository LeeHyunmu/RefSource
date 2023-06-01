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
//|IADORecordset 인터페이스
//|
//---------------------------------------------------------------
interface IADORecordset
{
	virtual void Release()=0;
	//서버메니저 초기화
	virtual BOOL initAdoRecordset(IMyObject* _obj)=0;
	virtual BOOL Open(_ConnectionPtr mpdb, LPCTSTR lpstrExec = _T(""), int nOption = openUnknown)=0;
	//connectPtr이 이미 설정 되어 있을 때
	virtual BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = openUnknown)=0;
	//필드의 값을 가져온다(데이타 타입에 따라서 오버라이딩)
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
	//필드 값을 채운다(데이타 타입에 따라서 오버라이딩)
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
	//레코드 페이지 사이즈
	virtual long GetPageCount()=0;
	virtual long GetPageSize()=0;
	virtual void SetPageSize(int nSize)=0;
	//필드 카운트
	virtual long GetAbsolutePage()=0;
	virtual void SetAbsolutePage(int nPage)=0;
	//레코드 셋 끝
	virtual BOOL IsEof()=0;
	virtual BOOL IsEOF()=0;
	//레코드 셋 시작
	virtual BOOL IsBof()=0;
	virtual BOOL IsBOF()=0;
	//처음 레코드
	virtual void MoveFirst()=0;
	// 다음 레코드
	virtual void MoveNext()=0; 
	//전 레코드
	virtual void MovePrevious()=0;
	//마지막 레코드
	virtual void MoveLast()=0; 
	//레코드 닫다
	virtual void Close()=0;
	//
	virtual BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue)=0;
	virtual BOOL GetChunk(int nIndex, CString& strValue)=0;
	virtual BOOL GetChunk(LPCTSTR lpFieldName, LPVOID pData)=0;
	virtual BOOL GetChunk(int nIndex, LPVOID pData)=0;
	//
	virtual BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes)=0;
	virtual BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes)=0;
	//필드 정보를 담아온다
	virtual BOOL GetFieldInfo(LPCTSTR lpFieldName, CADOFieldInfo* fldInfo)=0;
	virtual BOOL GetFieldInfo(int nIndex, CADOFieldInfo* fldInfo)=0;
	//필드 추가
	virtual BOOL AddNew()=0;
	virtual BOOL AddNew(CADORecordBinding &pAdoRecordBinding)=0;
	//필드 업데이트
	virtual BOOL Update()=0;
};

#endif //__IADORECORDSET_74F736E25AB94b5dA789E4730F58F2A1_H__