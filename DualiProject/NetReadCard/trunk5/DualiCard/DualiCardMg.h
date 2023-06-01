
#ifndef __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDualiCardMg : public IDualiCardMg
{
public:
	//������
	CDualiCardMg();
	//�Ҹ���
	~CDualiCardMg();
//=================������ �Լ�===========================//
	static DWORD WINAPI DetectProcess(LPVOID lpArg);
//=======================================================//
//			���� ����
//======================================================//
public:
	CThread  Detectthread;
	CEvent  DetectEvent;
//======================================================//
//			�������̽� ��ü ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//======================================================//
//			�������̽� ������ 
//======================================================//

//======================================================//
//			�Ϲ� �Լ�
//======================================================//
	//display ���� ���� �Լ�
	CString ShowString(int len,BYTE* data,BYTE flag);
	//display ���� ���� �Լ�
	CString ShowString(CString data,BYTE flag);
	//ī�� �浹 ������ ī�� UI�� �����´�
	CString Anticol();
	//ī�� ����
	void Select(BYTE* Recv, int len=4);
	//��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�(���̸� �ѱ��)
	int  ChangeHexCord(CString data, BYTE hex[]);
	//
	void DetectObject();
//=======================================================//
//======================================================//
//			����
//======================================================//
	BOOL m_bConnect;
	int  m_nPort;
	BYTE  nSlotno;
	//ī��ID��
	CString cardID;
	//0:�ڵ��� ���� �б� ���, 1:�ڵ������� ����(ī�޶���) 2:�ڵ������� ����(���Ҹ����)
	int Mode;
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void initInterfaceObject(IMyObject* _obj);
	virtual void Release();
	//ī�� ���ؼ�
	virtual void CardConnent(CString nPort);
	//
	virtual BOOL IdleReqa();
	//ī�� ���� �б�
	virtual BOOL DeviceInfoRead();
	//======================================================//
	//	apdu ���� Ŀ���
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//ī�� ã��
	virtual BOOL FindCard();
	//======================================================//
	//	SAM ī��
	//======================================================//
	//off
	virtual void SAMPwoff();
	//on
	virtual void SAMPwon(int _nSlotno=0);
	//�� Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//ī�� ���� �� ó�� �Լ�
	virtual void CardDetectFunc();
	//��弳���Լ�
	virtual void SetControlMode(int _Mode=0);
//=======================================================//
};
#endif// __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__