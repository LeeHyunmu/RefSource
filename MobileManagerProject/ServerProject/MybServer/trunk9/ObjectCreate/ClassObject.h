
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
//			�Ϲ��Լ� ����
//======================================================//

//=================������ �Լ�===========================//
	static DWORD WINAPI KillProcess(LPVOID lpArg);
//=======================================================//
//			���� ����
//======================================================//
public:
	CThread  mKillthread;
    CEvent  mhKillEvent;
//======================================================//
//======================================================//
//			�������̽� ��ü ����
//======================================================//
	LPVOID  mPross;
	//��� â
	IOutput* debug;
	//VM ���� �޴���
	IServerMg* ServerMg;
	//MYBI DB ���� �޴���
	IServerMg* DbServerMg;
	//������ Ǯ
	IThreadPool* thpool;
	//���� ������ �ڵ� ������ ���� Ȯ��
	ICrc*  crc32;
//======================================================//
//			�������̽� ������ 
//======================================================//

//=======================================================//
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	//������Ʈ ��ü�� �޸� �����Ѵ�
	virtual void KillObject();
	//�޸� ���� 
	virtual void Release();
	//��ü ����
	virtual BOOL CreateObject(LPVOID _pross);
	//======================================================//
	//	Crc32 ���� ������
	//======================================================//
	virtual ICrc* GetCrc32();
	//======================================================//
	//	����� ���
	//======================================================//
	//����� ���
	virtual IOutput* GetOutput();
	//======================================================//
	//	IOCP ���� ���� ���� ���� Ŭ����
	//======================================================//
	//����(IOCP) �����ѱ��
	virtual IServer* CreateIocpServer();
	virtual void ReleaseObject(IServer* _obj);
	//======================================================//
	//	VM ������ ���� ���� �޴���
	//======================================================//
	virtual IServerMg* GetServerMg();
	//======================================================//
	//	DB ���� �޴���
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
	//����Ÿ���̽� ����
	virtual IADODatabase* CreateAdoDatabase();
	virtual void ReleaseObject(IADODatabase* _obj);
	//Command �����ؼ� �ѱ�
	virtual IADOCommand* CreateAdoCommand();
	//Command �ı�
	virtual void ReleaseObject(IADOCommand* _obj);
	//Parameter �����ؼ� �ѱ�
	virtual IADOParameter* CreateAdoParameter();
	//Parameter �ı�
	virtual void ReleaseObject(IADOParameter* _obj);
	//Recordset �����ؼ� �ѱ�
	virtual IADORecordset* CreateAdoRecordset();
	//Recordset �ı�
	virtual void ReleaseObject(IADORecordset* _obj);
	//======================================================//
	//	DB ����Ÿ ���� �б� ���� Ŭ����
	//======================================================//
	//DB ����Ÿ ���� �б� ���� Ŭ���� ����
	virtual IDatabasebMg* CreateDatabaseMg();
	virtual void ReleaseObject(IDatabasebMg* _obj);
	//======================================================//
	//	Thread Pool Ŭ����
	//======================================================//
	virtual IThreadPool* GetThreadPool();
	//======================================================//
	// ������ Ǯ�� ����� DB Send ������Ʈ
	//======================================================//
	virtual IThreadObjectBase* CreateDbSendPoolObject();
	virtual void ReleaseObject(IThreadObjectBase* _obj);
//=======================================================//
};
#endif// __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
