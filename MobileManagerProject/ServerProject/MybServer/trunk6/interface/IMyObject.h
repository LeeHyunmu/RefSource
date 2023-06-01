
#ifndef __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IServer �������̽�
//|
//---------------------------------------------------------------
interface IMyObject
{
	//��ü�� �����Ѵ�
	virtual BOOL CreateObject(LPVOID _pross)=0;
	//������Ʈ ��ü�� �޸� �����Ѵ�
	virtual void KillObject()=0;
	//�޸� ���� ȣ��
	virtual void Release()=0;
	//����� ��� ���
	virtual IOutput* GetOutput()=0;
	//Crc32 
	virtual ICrc* GetCrc32()=0;
	//IOCP ����
	virtual IServer* GetIocpServer()=0;
	//���� �޴���
	virtual IServerMg* GetServerMg()=0;

	virtual IMysqlDb* GetMysqlDb()=0;
	virtual IMysqlDbtable* GetMysqlDbtable()=0;
	//ADO ��ü
	virtual IADODatabase* GetAdoDatabase()=0;
	//Command
	virtual IADOCommand* GetAdoCommand()=0;
	virtual void ReleaseObject(IADOCommand* _obj)=0;
	//Parameter
	virtual IADOParameter* GetAdoParameter()=0;
	virtual void ReleaseObject(IADOParameter* _obj)=0;
	//Recordset
	virtual IADORecordset* GetAdoRecordset()=0;
	virtual void ReleaseObject(IADORecordset* _obj)=0;
	//db ����� Ŭ����
	virtual IMobileDbMg* GetMobileDbMg()=0;
	//������ Ǯ
	virtual IThreadPool* GetThreadPool()=0;
};

#endif //__IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__