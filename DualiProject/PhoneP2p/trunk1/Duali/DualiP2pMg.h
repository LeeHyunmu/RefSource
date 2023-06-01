
#ifndef __DUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_P2PBUF 64
class CDualiP2pMg : public IDualiP2pMg
{
public:
	//������
	CDualiP2pMg();
	//�Ҹ���
	~CDualiP2pMg();
//=================������ �Լ�===========================//
	static DWORD WINAPI P2pProcess(LPVOID lpArg);
//=======================================================//
//			���� ����
//======================================================//
public:
	CThread  p2pthread;
	CEvent  p2pEvent;
//======================================================//
//			�������̽� ��ü ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IDualiCardMg* dualicard;
//======================================================//
	//=======================================================//
	//			������� ����
	//======================================================//
	//Read Event
	HANDLE hReadEvent;
	//Wirte Event
	HANDLE hWriteEvent;
	//�۽� ����Ÿ
	CString SendData;
	//���� ����Ÿ
	CString RecvData;
	//������ ������
	DWORD dwSendflage;
	//�޴� ������
	DWORD dwRecvSize;
	//���� �÷���
	DWORD dwLoop;
	//�ۼ����� �� �Ǿ��°��� �Ǵ��Ѵ�
	BOOL bOk;

//======================================================//
//			�Ϲ� �Լ�
//======================================================//
	void P2pObject();
//=======================================================//
//======================================================//
//			����
//======================================================//

//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void initInterfaceObject(IMyObject* _obj);
	virtual void Release();
	//�����忡�� ȣ��Ǵ� �Լ�
	virtual void P2pFunc();
	//������ ����
	virtual BOOL SendDatafn(DWORD _dwSendflage, CString data, DWORD dwTimeout = INFINITE);
//=======================================================//
};
#endif// __DUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__
