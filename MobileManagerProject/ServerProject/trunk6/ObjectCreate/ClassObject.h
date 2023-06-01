
#ifndef __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClassObject : public IMyObject
{
public:
	//������
	CClassObject();
	//�Ҹ���
	~CClassObject();
//======================================================//
//			�������̽� ��ü ����
//======================================================//
	LPVOID  mPross;
	IOutput* debug;
	IServer* IocpServer;
	IServerMg* ServerMg;
	IMysqlDb*  Mysql;
	IMysqlDbtable* Mysqltable;
	//ADO DB �������̽�
	IADODatabase* adodatabase;
	IMobileDbMg* dbmg;
	IThreadPool* thpool;
	ICrc*  crc32;
//======================================================//
//			�������̽� ������ 
//======================================================//

//=======================================================//
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL CreateObject(LPVOID _pross);
	//Crc32 
	virtual ICrc* GetCrc32();
	//����� ���
	virtual IOutput* GetOutput();
	//����
	virtual IServer* GetIocpServer();
	virtual IServerMg* GetServerMg();
	//MySQL
	virtual IMysqlDb* GetMysqlDb();
	virtual IMysqlDbtable* GetMysqlDbtable();
	//ADO 
	//����Ÿ���̽� ����
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
	//DB ����Ÿ ���� �б� ���� Ŭ����
	virtual IMobileDbMg* GetMobileDbMg();
	//Thread Pool Ŭ����
	virtual IThreadPool* GetThreadPool();
//=======================================================//
};
#endif// __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
