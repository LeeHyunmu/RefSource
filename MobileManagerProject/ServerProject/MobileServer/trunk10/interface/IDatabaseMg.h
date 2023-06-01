#ifndef __IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|IDatabasebMg 인터페이스
//|
//---------------------------------------------------------------
interface IDatabasebMg
{
	virtual void Release()=0;
	//서버메니저 초기화
	virtual BOOL initMobileDbMg(IMyObject* _obj)=0;
	//insert
	virtual void Trans(TCHAR* strQuery, TCHAR* format, ...)=0;
	//DB 연결
	virtual BOOL DbConnect(CString strConnection)=0;
	//레코드 셋 설정(특정 값 가져 오기)
	virtual BOOL RecordsetSetting(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...)=0;
	//레코드 셋 설정(테이블 전체)
	virtual BOOL RecordsetSetting(TCHAR* tablename)=0;
	//레코드 셋 포인터를 넘긴다
	virtual IADORecordset* GetRecordsetPoint()=0;
	//트랜젝션 사용 쿼리
	//시작
	virtual BOOL BeginTn()=0;
	//Query
	virtual BOOL ActionQuery(TCHAR* strQuery, TCHAR* format, ...)=0;
	//commit
	virtual BOOL CommitTn()=0;
};

#endif //__IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__