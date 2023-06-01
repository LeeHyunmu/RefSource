
#ifndef __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IMyObject �������̽�
//|
//---------------------------------------------------------------
interface IMyObject
{
	//������Ʈ ��ü�� �޸� �����Ѵ�
	virtual void KillObject()=0;
	//�޸� ���� 
	virtual void Release()=0;
	//��ü ����
	virtual BOOL CreateObject(LPVOID _pross)=0;
	//======================================================//
	//	Crc32 ���� ������
	//======================================================//
	virtual ICrc* GetCrc32()=0;
	//======================================================//
	//	����� ���
	//======================================================//
	//����� ���
	virtual IOutput* GetOutput()=0;
	//======================================================//
	//	IOCP ���� ���� ���� ���� Ŭ����
	//======================================================//
	//����(IOCP) �����ѱ��
	virtual IServer* CreateIocpServer()=0;
	virtual void ReleaseObject(IServer* _obj)=0;
	//======================================================//
	//	VM ������ ���� ���� �޴���
	//======================================================//
	virtual IServerMg* GetServerMg()=0;
	//======================================================//
	//	DB ���� �޴���
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
	//����Ÿ���̽� ����
	virtual IADODatabase* CreateAdoDatabase()=0;
	virtual void ReleaseObject(IADODatabase* _obj)=0;
	//Command �����ؼ� �ѱ�
	virtual IADOCommand* CreateAdoCommand()=0;
	//Command �ı�
	virtual void ReleaseObject(IADOCommand* _obj)=0;
	//Parameter �����ؼ� �ѱ�
	virtual IADOParameter* CreateAdoParameter()=0;
	//Parameter �ı�
	virtual void ReleaseObject(IADOParameter* _obj)=0;
	//Recordset �����ؼ� �ѱ�
	virtual IADORecordset* CreateAdoRecordset()=0;
	//Recordset �ı�
	virtual void ReleaseObject(IADORecordset* _obj)=0;
	//======================================================//
	//	DB ����Ÿ ���� �б� ���� Ŭ����
	//======================================================//
	//DB ����Ÿ ���� �б� ���� Ŭ���� ����
	virtual IDatabasebMg* CreateDatabaseMg()=0;
	virtual void ReleaseObject(IDatabasebMg* _obj)=0;
	//======================================================//
	//	Thread Pool Ŭ����
	//======================================================//
	virtual IThreadPool* GetThreadPool()=0;
	//======================================================//
	// ������ Ǯ�� ����� DB Send ������Ʈ
	//======================================================//
	virtual IThreadObjectBase* CreateDbSendPoolObject()=0;
	virtual void ReleaseObject(IThreadObjectBase* _obj)=0;
};

#endif //__IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__