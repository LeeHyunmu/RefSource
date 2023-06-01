
#include "stdAfx.h"
#include "ObjectCreate.h"
#include "../Crc/Crc.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Server/Server.h"
#include "../Database/Database.h"
#include "../AdoDb/AdoDb.h"
#include "../ThdPoolObj/DbDataSendPoolObj.h"
#include "../ThdPool/ThdPool.h"


//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
debug(NULL),
ServerMg(NULL),
DbServerMg(NULL),
crc32(NULL),
mKillthread(NULL),
mhKillEvent(NULL)
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
void CClassObject::KillObject()
{
	////////////////////////////////////////////////////////////////
	//종료 스레드 생성
	mhKillEvent.Create();
	mKillthread.Create(KillProcess, (LPVOID)this);
	if(mhKillEvent.WaitForEvent(3000))
	{
		debug->Write("모든 객체 메모리 해제");
	}
	else
	{
		debug->Write("모든 객체 메모리 해제 실패");
	}
	//디버그 출력 창을 제일 나중에 종료한다
	if(debug)
	{
		delete ((CTraceOutput*)debug);	debug=NULL;
	}
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//| 메모리를 해제하기 위한 스래드
//|
//---------------------------------------------------------------
DWORD WINAPI CClassObject::KillProcess(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	obj->Release();
	return 0;
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
		debug->Write("CCrc32 해제");
	}
	if(ServerMg)
	{
		ServerMg->Release();
		delete ((CServerMg*)ServerMg);	ServerMg=NULL;
		debug->Write("CServerMg 해제");
	}
	if(DbServerMg)
	{
		DbServerMg->Release();
		delete ((CDbServerMg*)DbServerMg);	DbServerMg=NULL;
		debug->Write("CDbServerMg 해제");
	}
	if(thpool)
	{
		thpool->Release();
		delete ((CThreadPool*)thpool);	thpool=NULL;
		debug->Write("CThreadPool 해제");
	}
	//|=============================================
	//|사용중인 객체의 메모리를 모두 해제 했으면
	//|==================================================
	mhKillEvent.SetEvent();
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
	//서버 메니저
	ServerMg=new CServerMg;
	if(ServerMg)
	{
		ServerMg->initServerMg(this);
	}
	//DB 서버 메니저
	DbServerMg=new CDbServerMg;
	if(DbServerMg)
	{
		DbServerMg->initServerMg(this);
	}

	//스래드 풀 생성
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
IServer* CClassObject::CreateIocpServer()
{
	IServer* iocp=new CIocpServer;
	if(iocp)
		iocp->initServer(this);
	return iocp;
}
void CClassObject::ReleaseObject(IServer* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CIocpServer*)_obj);	_obj=NULL;
	}
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
//|DB 서버 메니저
//|
//---------------------------------------------------------------
IServerMg* CClassObject::GetDbServerMg()
{
	return DbServerMg;
}
//--------------------------------------------------------------
//|
//|mysql
//|
//---------------------------------------------------------------
IMysqlDb* CClassObject::CreateMysqlDb()
{
	IMysqlDb* Mysql=new CMysqlDb;
	if(Mysql)
		Mysql->initMysqlDb(this);
	return Mysql;
}
void CClassObject::ReleaseObject(IMysqlDb* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CMysqlDb*)_obj);	_obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|Mysql table
//|
//---------------------------------------------------------------
IMysqlDbtable* CClassObject::CreateMysqlDbtable()
{
	IMysqlDbtable* Mysqltable=new CMysqlDbtable;
	if(Mysqltable)
		Mysqltable->initMysqlDbtable(this);
	return Mysqltable;
}
void CClassObject::ReleaseObject(IMysqlDbtable* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CMysqlDbtable*)_obj);	_obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|데이터 베이스 클래스
//|
//---------------------------------------------------------------
IADODatabase* CClassObject::CreateAdoDatabase()
{
	IADODatabase* adodb=new CADODatabase;
	if(adodb)
		adodb->initAdoDatabase(this);
	return adodb;
}
void CClassObject::ReleaseObject(IADODatabase* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CADODatabase*)_obj);	_obj=NULL;
	}
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
IADOCommand* CClassObject::CreateAdoCommand()
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
IADOParameter* CClassObject::CreateAdoParameter()
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
IADORecordset* CClassObject::CreateAdoRecordset()
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
//|모바일 DB 메니저
//|
//---------------------------------------------------------------
IDatabasebMg* CClassObject::CreateDatabaseMg()
{
	IDatabasebMg* mdbmg=new CDatabasebMg;
	if(mdbmg)
		mdbmg->initMobileDbMg(this);
	return mdbmg;
}
void CClassObject::ReleaseObject(IDatabasebMg* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CDatabasebMg*)_obj);	_obj=NULL;
	}
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
//--------------------------------------------------------------
//|
//| DB Send Pool Object 스래드 풀 사용할 객체
//|
//---------------------------------------------------------------
IThreadObjectBase* CClassObject::CreateDbSendPoolObject()
{
	IThreadObjectBase* dbsendobj=new CDbDataSendPoolObj;
	if(dbsendobj)
		dbsendobj->initThreadObject(this);
	return dbsendobj;
}
void CClassObject::ReleaseObject(IThreadObjectBase* _obj)
{
	if(_obj)
	{
		_obj->Release();	delete ((CDbDataSendPoolObj*)_obj);	_obj=NULL;
	}
}