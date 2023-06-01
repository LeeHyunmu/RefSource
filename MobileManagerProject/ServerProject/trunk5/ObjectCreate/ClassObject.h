
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
//			인터페이스 객체 선언
//======================================================//
	LPVOID  mPross;
	IOutput* debug;
	IServer* IocpServer;
	IServerMg* ServerMg;
	IMysqlDb*  Mysql;
	IMysqlDbtable* Mysqltable;
	//ADO DB 인터페이스
	IADODatabase* adodatabase;
	IMobileDbMg* dbmg;
	IThreadPool* thpool;
	ICrc*  crc32;
//======================================================//
//			인터페이스 포인터 
//======================================================//

//=======================================================//
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL CreateObject(LPVOID _pross);
	//Crc32 
	virtual ICrc* GetCrc32();
	//디버그 출력
	virtual IOutput* GetOutput();
	//서버
	virtual IServer* GetIocpServer();
	virtual IServerMg* GetServerMg();
	//MySQL
	virtual IMysqlDb* GetMysqlDb();
	virtual IMysqlDbtable* GetMysqlDbtable();
	//ADO 
	//데이타베이스 접속
	virtual IADODatabase* GetAdoDatabase();
	//Command
	virtual IADOCommand* GetAdoCommand();
	virtual void ReleaseObject(IADOCommand* _obj);
	//Parameter
	virtual IADOParameter* GetAdoParameter();
	virtual void ReleaseObject(IADOParameter* _obj);
	//Recordset
	virtual IADORecordset* GetAdoRecordset();
	virtual void ReleaseObject(IADORecordset* _obj);
	//DB 데이타 쓰고 읽기 위한 클래스
	virtual IMobileDbMg* GetMobileDbMg();
	//Thread Pool 클래스
	virtual IThreadPool* GetThreadPool();
//=======================================================//
};
#endif// __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
