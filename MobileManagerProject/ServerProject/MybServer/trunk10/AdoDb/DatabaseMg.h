
#ifndef __DATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CDatabasebMg : public IDatabasebMg
{
public:
	//생성자
	CDatabasebMg();
	//소멸자
	~CDatabasebMg();

//======================================================//
//			일반함수 선언
//======================================================//

//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IADODatabase* adodb;
//=======================================================//
//			변수 선언
//======================================================//

//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initMobileDbMg(IMyObject* _obj);
	virtual void Trans(TCHAR* strQuery, TCHAR* format, ...);
	//DB 연결
	virtual BOOL DbConnect(CString strConnection);
	//레코드 셋에 쿼리 정보를 담아서 넘긴다
	virtual IADORecordset* GetDbRecordset(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...);
	//트랜젝션 사용 쿼리
	//시작
	virtual BOOL BeginTn();
	//Query
	virtual BOOL ActionQuery(TCHAR* strQuery, TCHAR* format, ...);
	//commit
	virtual BOOL CommitTn();

//=======================================================//

};
#endif// __DATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__
