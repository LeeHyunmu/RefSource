
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Server/Server.h"
#include "../Database/Database.h"
#include "../AdoDb/AdoDb.h"
#include "../ThdPool/ThdPool.h"
#include "../Crc/Crc.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
debug(NULL),
ServerMg(NULL),
IocpServer(NULL),
Mysql(NULL),
Mysqltable(NULL),
adodatabase(NULL),
dbmg(NULL),
crc32(NULL)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CClassObject::~CClassObject()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CClassObject::Release()
{
	if(crc32)
	{
		crc32->Release();
		delete ((CCrc32*)crc32);	crc32=NULL;
	}
	if(IocpServer)
	{
		IocpServer->Release();
		delete ((CIocpServer*)IocpServer);	IocpServer=NULL;
	}
	if(ServerMg)
	{
		ServerMg->Release();
		delete ((CServerMg*)ServerMg);	ServerMg=NULL;
	}
	if(Mysql)
	{
		Mysql->Release();
		delete ((CMysqlDb*)Mysql);	Mysql=NULL;
	}
	if(Mysqltable)
	{
		Mysqltable->Release();
		delete ((CMysqlDbtable*)Mysqltable);	Mysqltable=NULL;
	}
	if(adodatabase)
	{
		adodatabase->Release();
		delete ((CADODatabase*)adodatabase);	adodatabase=NULL;
	}
	if(dbmg)
	{
		dbmg->Release();
		delete ((CMobileDbMg*)dbmg);	dbmg=NULL;
	}
	if(thpool)
	{
		thpool->Release();
		delete ((CThreadPool*)thpool);	thpool=NULL;
	}
	if(debug)
	{
		delete ((CTraceOutput*)debug);	debug=NULL;

	}
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
BOOL CClassObject::CreateObject(LPVOID _pross)
{
	mPross=_pross;
	//디버그 출력 생성
#ifdef _DEBUG
	debug = new CTraceOutput(CTraceOutput::CONSOLE,_T("MobileDebug"));
#else
	debug = new CTraceOutput(CTraceOutput::FILE,_T("MobileDebug"));
#endif
	//Crc32 생성
	crc32=new CCrc32;
	if(crc32)
		crc32->initCrc(this);
	//서버 생성
	IocpServer=new CIocpServer;
	if(IocpServer)
		IocpServer->initServer(this);
	ServerMg=new CServerMg;
	if(ServerMg)
		ServerMg->initServerMg(this);

	//DB 생성
	Mysql=new CMysqlDb;
	if(Mysql)
		Mysql->initMysqlDb(this);
	Mysqltable=new CMysqlDbtable;
	if(Mysqltable)
		Mysqltable->initMysqlDbtable(this);

	//ADO 생성
	adodatabase=new CADODatabase;
	if(adodatabase)
		adodatabase->initAdoDatabase(this);
	dbmg=new CMobileDbMg;
	if(dbmg)
		dbmg->initMobileDbMg(this);
	thpool=new CThreadPool;
	if(thpool)
	{
		thpool->initThreadPool(this);
		thpool->Init(INITNUMOFTHREAD , MAXNUMOFTHREAD);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|생성된 클래스 객체 포인터를 넘긴다
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|Crc32 데이터 검증 클래스
//|
//---------------------------------------------------------------
ICrc* CClassObject::GetCrc32()
{
	return crc32;
}
//--------------------------------------------------------------
//|
//|디버그 출력 클래스
//|
//---------------------------------------------------------------
IOutput* CClassObject::GetOutput()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|IOCP 서버 클래스
//|
//---------------------------------------------------------------
IServer* CClassObject::GetIocpServer()
{
	return IocpServer;
}
//--------------------------------------------------------------
//|
//|서버 메니저
//|
//---------------------------------------------------------------
IServerMg* CClassObject::GetServerMg()
{
	return ServerMg;
}
//--------------------------------------------------------------
//|
//|mysql
//|
//---------------------------------------------------------------
IMysqlDb* CClassObject::GetMysqlDb()
{
	return Mysql;
}
//--------------------------------------------------------------
//|
//|Mysql table
//|
//---------------------------------------------------------------
IMysqlDbtable* CClassObject::GetMysqlDbtable()
{
	return Mysqltable;
}
//--------------------------------------------------------------
//|
//|데이터 베이스 클래스
//|
//---------------------------------------------------------------
IADODatabase* CClassObject::GetAdoDatabase()
{
	return adodatabase;
}
//--------------------------------------------------------------
//|
//|쓰임세가 있을 때 객체를 생성해서 넘긴다(사용후 해체하는 함수 밑에 있음)
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|DB 레코드 클래스
//|
//---------------------------------------------------------------
IADOCommand* CClassObject::GetAdoCommand()
{
	IADOCommand* adocomd=new CADOCommand;
	if(adocomd)
		adocomd->initAdoCommand(this);
	return adocomd;
}
void CClassObject::ReleaseObject(IADOCommand* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CADOCommand*)_obj);	_obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|DB 파라메터 클래스
//|
//---------------------------------------------------------------
IADOParameter* CClassObject::GetAdoParameter()
{
	IADOParameter *adoparam=new CADOParameter;
	if(adoparam)
		adoparam->initAdoParameter(this);
	return adoparam;
}
void CClassObject::ReleaseObject(IADOParameter* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((IADOParameter*)_obj);	_obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|DB 레코드 클래스
//|
//---------------------------------------------------------------
IADORecordset* CClassObject::GetAdoRecordset()
{
	IADORecordset *adorecord=new CADORecordset;
	if(adorecord)
		adorecord->initAdoRecordset(this);
	return adorecord;
}
void CClassObject::ReleaseObject(IADORecordset* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((IADORecordset*)_obj);	_obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|스래드 풀 메니저
//|
//---------------------------------------------------------------
IMobileDbMg* CClassObject::GetMobileDbMg()
{
	return dbmg;
}
//--------------------------------------------------------------
//|
//|스래드 풀 클래스
//|
//---------------------------------------------------------------
IThreadPool* CClassObject::GetThreadPool()
{
	return thpool;
}