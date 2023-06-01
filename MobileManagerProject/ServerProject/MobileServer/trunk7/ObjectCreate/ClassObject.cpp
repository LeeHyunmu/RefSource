
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
//|������
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
void CClassObject::KillObject()
{
	////////////////////////////////////////////////////////////////
	//���� ������ ����
	mhKillEvent.Create();
	mKillthread.Create(KillProcess, (LPVOID)this);
	if(mhKillEvent.WaitForEvent(3000))
	{
		debug->Write("��� ��ü �޸� ����");
	}
	else
	{
		debug->Write("��� ��ü �޸� ���� ����");
	}
	//����� ��� â�� ���� ���߿� �����Ѵ�
	if(debug)
	{
		delete ((CTraceOutput*)debug);	debug=NULL;
	}
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//| �޸𸮸� �����ϱ� ���� ������
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
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CClassObject::Release()
{
	if(crc32)
	{
		crc32->Release();
		delete ((CCrc32*)crc32);	crc32=NULL;
		debug->Write("CCrc32 ����");
	}
	if(ServerMg)
	{
		ServerMg->Release();
		delete ((CServerMg*)ServerMg);	ServerMg=NULL;
		debug->Write("CServerMg ����");
	}
	if(DbServerMg)
	{
		DbServerMg->Release();
		delete ((CDbServerMg*)DbServerMg);	DbServerMg=NULL;
		debug->Write("CDbServerMg ����");
	}
	if(thpool)
	{
		thpool->Release();
		delete ((CThreadPool*)thpool);	thpool=NULL;
		debug->Write("CThreadPool ����");
	}
	//|=============================================
	//|������� ��ü�� �޸𸮸� ��� ���� ������
	//|==================================================
	mhKillEvent.SetEvent();
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
	//���� �޴���
	ServerMg=new CServerMg;
	if(ServerMg)
	{
		ServerMg->initServerMg(this);
	}
	//DB ���� �޴���
	DbServerMg=new CDbServerMg;
	if(DbServerMg)
	{
		DbServerMg->initServerMg(this);
	}

	//������ Ǯ ����
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
//|���� �޴���
//|
//---------------------------------------------------------------
IServerMg* CClassObject::GetServerMg()
{
	return ServerMg;
}
//--------------------------------------------------------------
//|
//|DB ���� �޴���
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
//|������ ���̽� Ŭ����
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
//|���Ӽ��� ���� �� ��ü�� �����ؼ� �ѱ��(����� ��ü�ϴ� �Լ� �ؿ� ����)
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|DB ���ڵ� Ŭ����
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
//|DB �Ķ���� Ŭ����
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
//|DB ���ڵ� Ŭ����
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
//|����� DB �޴���
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
//|������ Ǯ Ŭ����
//|
//---------------------------------------------------------------
IThreadPool* CClassObject::GetThreadPool()
{
	return thpool;
}
//--------------------------------------------------------------
//|
//| DB Send Pool Object ������ Ǯ ����� ��ü
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