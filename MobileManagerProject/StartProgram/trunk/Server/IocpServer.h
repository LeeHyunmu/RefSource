
#ifndef __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CIocpServer : public IServer
{
public:
	//������
	CIocpServer();
	//�Ҹ���
	~CIocpServer();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
//=================������ �Լ�===========================//
	static unsigned int __stdcall CompletionThread(void* pComPort);
//=================���� ���� ���� �Լ�===================//
	BOOL CreateServerSocket();
//=======================================================//
//			��ü ����
//======================================================//

//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL initServer();
//=======================================================//

};
#endif// __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__