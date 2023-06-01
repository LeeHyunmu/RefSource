
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
	IClient* client;
	IOutput* debug;
	ICrc*  crc32;
//======================================================//
//			�������̽� ������ 
//======================================================//

//=======================================================//
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL CreateObject();
	//Crc32 
	virtual ICrc* GetCrc32();
	virtual IOutput* GetOutput();
	virtual IClient* GetClient();
//=======================================================//
};
#endif// __CLASSOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
