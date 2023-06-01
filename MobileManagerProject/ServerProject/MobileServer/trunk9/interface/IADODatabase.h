#ifndef __IADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
enum cadoConnectModeEnum
{	
	connectModeUnknown = adModeUnknown,
	connectModeRead = adModeRead,
	connectModeWrite = adModeWrite,
	connectModeReadWrite = adModeReadWrite,
	connectModeShareDenyRead = adModeShareDenyRead,
	connectModeShareDenyWrite = adModeShareDenyWrite,
	connectModeShareExclusive = adModeShareExclusive,
	connectModeShareDenyNone = adModeShareDenyNone
};
//--------------------------------------------------------------
//|
//|IADODatabase 인터페이스
//|
//---------------------------------------------------------------
interface IADODatabase
{
	virtual void Release()=0;
	//서버메니저 초기화
	virtual BOOL initAdoDatabase(IMyObject* _obj)=0;
	//컨넥션이 맺어진 상태 파악
	virtual BOOL IsOpen()=0;
	//
	virtual _ConnectionPtr GetActiveConnection()=0; 
	//DB 연결
	virtual BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""))=0;
	//트랜젝션
	//시작
	virtual HRESULT BeginTransaction()=0; 
	//쿼리 입력
	virtual BOOL Execute(LPCTSTR lpstrExec, long Option=adExecuteNoRecords)=0;
	// 커밋
	virtual HRESULT CommitTransaction()=0; 
	// 롤백
	virtual HRESULT RollbackTransaction()=0; 
};

#endif //__IADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__