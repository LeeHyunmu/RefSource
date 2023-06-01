
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
//======================================================//
//			�������̽� ������ 
//======================================================//

//=======================================================//
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL CreateObject(LPVOID _pross);
	virtual IOutput* GetOutput();
	virtual IServer* GetIocpServer();
	virtual IServerMg* GetServerMg();
//=======================================================//
};
#endif// __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
