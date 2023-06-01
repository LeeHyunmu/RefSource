
#ifndef __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClassObject : public IMyObject
{
public:
	//생성자
	CClassObject();
	//소멸자
	~CClassObject();
//======================================================//
//			일반함수 선언
//======================================================//

//=================스래드 함수===========================//
	static DWORD WINAPI KillProcess(LPVOID lpArg);
//=======================================================//
//			변수 선언
//======================================================//
public:
	CThread  mKillthread;
    CEvent  mhKillEvent;
//======================================================//
//======================================================//
//			인터페이스 객체 선언
//======================================================//
	LPVOID  mPross;
	//출력 창
	IOutput* debug;
	//VM 서버 메니저
	IServerMg* ServerMg;
	//MYBI DB 서버 메니저
	IServerMg* DbServerMg;
	//스래드 풀
	IThreadPool* thpool;
	//파일 검증용 코드 생성및 검증 확인
	ICrc*  crc32;
//======================================================//
//			인터페이스 포인터 
//======================================================//

//=======================================================//
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	//오브젝트 객체를 메모리 해제한다
	virtual void KillObject();
	//메모리 해제 
	virtual void Release();
	//객체 생성
	virtual BOOL CreateObject(LPVOID _pross);
	//======================================================//
	//	Crc32 파일 검증용
	//======================================================//
	virtual ICrc* GetCrc32();
	//======================================================//
	//	디버그 출력
	//======================================================//
	//디버그 출력
	virtual IOutput* GetOutput();
	//======================================================//
	//	IOCP 서버 구성 정보 저장 클래스
	//======================================================//
	//서버(IOCP) 생성넘긴다
	virtual IServer* CreateIocpServer();
	virtual void ReleaseObject(IServer* _obj);
	//======================================================//
	//	VM 유저을 위한 서버 메니저
	//======================================================//
	virtual IServerMg* GetServerMg();
	//======================================================//
	//	DB 서버 메니저
	//======================================================//
	virtual IServerMg* GetDbServerMg();
	//======================================================//
	//	MySQL
	//======================================================//
	virtual IMysqlDb* CreateMysqlDb();
	virtual void ReleaseObject(IMysqlDb* _obj);
	virtual IMysqlDbtable* CreateMysqlDbtable();
	virtual void ReleaseObject(IMysqlDbtable* _obj);
	//======================================================//
	//	ADO
	//======================================================//
	//데이타베이스 접속
	virtual IADODatabase* CreateAdoDatabase();
	virtual void ReleaseObject(IADODatabase* _obj);
	//Command 생성해서 넘김
	virtual IADOCommand* CreateAdoCommand();
	//Command 파괴
	virtual void ReleaseObject(IADOCommand* _obj);
	//Parameter 생성해서 넘김
	virtual IADOParameter* CreateAdoParameter();
	//Parameter 파괴
	virtual void ReleaseObject(IADOParameter* _obj);
	//Recordset 생성해서 넘김
	virtual IADORecordset* CreateAdoRecordset();
	//Recordset 파괴
	virtual void ReleaseObject(IADORecordset* _obj);
	//======================================================//
	//	DB 데이타 쓰고 읽기 위한 클래스
	//======================================================//
	//DB 데이타 쓰고 읽기 위한 클래스 생성
	virtual IDatabasebMg* CreateDatabaseMg();
	virtual void ReleaseObject(IDatabasebMg* _obj);
	//======================================================//
	//	Thread Pool 클래스
	//======================================================//
	virtual IThreadPool* GetThreadPool();
	//======================================================//
	// 스래드 풀에 사용할 DB Send 오브젝트
	//======================================================//
	virtual IThreadObjectBase* CreateDbSendPoolObject();
	virtual void ReleaseObject(IThreadObjectBase* _obj);
//=======================================================//
};
#endif// __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
