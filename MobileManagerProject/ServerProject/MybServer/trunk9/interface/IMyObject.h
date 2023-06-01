
#ifndef __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IMyObject 인터페이스
//|
//---------------------------------------------------------------
interface IMyObject
{
	//오브젝트 객체를 메모리 해제한다
	virtual void KillObject()=0;
	//메모리 해제 
	virtual void Release()=0;
	//객체 생성
	virtual BOOL CreateObject(LPVOID _pross)=0;
	//======================================================//
	//	Crc32 파일 검증용
	//======================================================//
	virtual ICrc* GetCrc32()=0;
	//======================================================//
	//	디버그 출력
	//======================================================//
	//디버그 출력
	virtual IOutput* GetOutput()=0;
	//======================================================//
	//	IOCP 서버 구성 정보 저장 클래스
	//======================================================//
	//서버(IOCP) 생성넘긴다
	virtual IServer* CreateIocpServer()=0;
	virtual void ReleaseObject(IServer* _obj)=0;
	//======================================================//
	//	VM 유저을 위한 서버 메니저
	//======================================================//
	virtual IServerMg* GetServerMg()=0;
	//======================================================//
	//	DB 서버 메니저
	//======================================================//
	virtual IServerMg* GetDbServerMg()=0;
	//======================================================//
	//	MySQL
	//======================================================//
	virtual IMysqlDb* CreateMysqlDb()=0;
	virtual void ReleaseObject(IMysqlDb* _obj)=0;
	virtual IMysqlDbtable* CreateMysqlDbtable()=0;
	virtual void ReleaseObject(IMysqlDbtable* _obj)=0;
	//======================================================//
	//	ADO
	//======================================================//
	//데이타베이스 접속
	virtual IADODatabase* CreateAdoDatabase()=0;
	virtual void ReleaseObject(IADODatabase* _obj)=0;
	//Command 생성해서 넘김
	virtual IADOCommand* CreateAdoCommand()=0;
	//Command 파괴
	virtual void ReleaseObject(IADOCommand* _obj)=0;
	//Parameter 생성해서 넘김
	virtual IADOParameter* CreateAdoParameter()=0;
	//Parameter 파괴
	virtual void ReleaseObject(IADOParameter* _obj)=0;
	//Recordset 생성해서 넘김
	virtual IADORecordset* CreateAdoRecordset()=0;
	//Recordset 파괴
	virtual void ReleaseObject(IADORecordset* _obj)=0;
	//======================================================//
	//	DB 데이타 쓰고 읽기 위한 클래스
	//======================================================//
	//DB 데이타 쓰고 읽기 위한 클래스 생성
	virtual IDatabasebMg* CreateDatabaseMg()=0;
	virtual void ReleaseObject(IDatabasebMg* _obj)=0;
	//======================================================//
	//	Thread Pool 클래스
	//======================================================//
	virtual IThreadPool* GetThreadPool()=0;
	//======================================================//
	// 스래드 풀에 사용할 DB Send 오브젝트
	//======================================================//
	virtual IThreadObjectBase* CreateDbSendPoolObject()=0;
	virtual void ReleaseObject(IThreadObjectBase* _obj)=0;
};

#endif //__IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__