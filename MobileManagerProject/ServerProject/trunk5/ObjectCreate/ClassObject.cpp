
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Server/Server.h"
#include "../Database/Database.h"
#include "../AdoDb/AdoDb.h"
#include "../ThdPool/ThdPool.h"
#include "../Crc/Crc.h"

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CClassObject::~CClassObject()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
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
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
BOOL CClassObject::CreateObject(LPVOID _pross)
{
	mPross=_pross;
	//����� ��� ����
#ifdef _DEBUG
	debug = new CTraceOutput(CTraceOutput::CONSOLE,_T("MobileDebug"));
#else
	debug = new CTraceOutput(CTraceOutput::FILE,_T("MobileDebug"));
#endif
	//Crc32 ����
	crc32=new CCrc32;
	if(crc32)
		crc32->initCrc(this);
	//���� ����
	IocpServer=new CIocpServer;
	if(IocpServer)
		IocpServer->initServer(this);
	ServerMg=new CServerMg;
	if(ServerMg)
		ServerMg->initServerMg(this);

	//DB ����
	Mysql=new CMysqlDb;
	if(Mysql)
		Mysql->initMysqlDb(this);
	Mysqltable=new CMysqlDbtable;
	if(Mysqltable)
		Mysqltable->initMysqlDbtable(this);

	//ADO ����
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
//|������ Ŭ���� ��ü �����͸� �ѱ��
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|Crc32 ������ ���� Ŭ����
//|
//---------------------------------------------------------------
ICrc* CClassObject::GetCrc32()
{
	return crc32;
}
//--------------------------------------------------------------
//|
//|����� ��� Ŭ����
//|
//---------------------------------------------------------------
IOutput* CClassObject::GetOutput()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|IOCP ���� Ŭ����
//|
//---------------------------------------------------------------
IServer* CClassObject::GetIocpServer()
{
	return IocpServer;
}
//--------------------------------------------------------------
//|
//|���� �޴���
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
//|������ ���̽� Ŭ����
//|
//---------------------------------------------------------------
IADODatabase* CClassObject::GetAdoDatabase()
{
	return adodatabase;
}
//--------------------------------------------------------------
//|
//|���Ӽ��� ���� �� ��ü�� �����ؼ� �ѱ��(����� ��ü�ϴ� �Լ� �ؿ� ����)
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|DB ���ڵ� Ŭ����
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
//|DB �Ķ���� Ŭ����
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
//|DB ���ڵ� Ŭ����
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
//|������ Ǯ �޴���
//|
//---------------------------------------------------------------
IMobileDbMg* CClassObject::GetMobileDbMg()
{
	return dbmg;
}
//--------------------------------------------------------------
//|
//|������ Ǯ Ŭ����
//|
//---------------------------------------------------------------
IThreadPool* CClassObject::GetThreadPool()
{
	return thpool;
}