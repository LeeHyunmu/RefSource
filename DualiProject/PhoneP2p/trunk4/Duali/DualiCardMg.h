
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
	CEvent  CloseEvent;
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
	CString Anticol(BOOL bDebug=TRUE);
	//ī�� ����
	void Select(BYTE* Recv, int len=4, BOOL bDebug=TRUE);
	//��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�(���̸� �ѱ��)
	int  ChangeHexCord(CString data, BYTE hex[]);
	//��� ����Ʈ
	void DetectObject();
	//ĳ���͸� �����
	CString Changcharbyhex(CString pass);
	//���� ��Ʈ������ ��ȯ
	BOOL ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass);
	//
	void SendFile();
//=======================================================//
//======================================================//
//			����
//======================================================//
	BOOL m_bConnect;
	int  m_nPort;
	BYTE  nSlotno;
	//ī��ID��
	CString cardID;
	BOOL bSAMOn;

	//initiator Mode, Target Mode ����
	UINT uLoopMode;

	//����Ÿ ������ �������� ������ �����Ѵ�
	//����Ÿ�� ��� ������ ī�尡 �ִ� ���� �����Ѵ�
	//������ Flage
	UINT uThreadLoop;
	//�ÿ� ���
	UINT uShowMode;
	//
	LPBYTE l_pbuf;
	DWORD read_size;
	CString repass;
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void initInterfaceObject(IMyObject* _obj);
	virtual void Release();
	//ī�� ���ؼ�
	virtual void CardConnent(CString nPort);
	//���̵� ����
	virtual BOOL IdleReqa(BOOL bDebug=TRUE);
	//���� ����
	//initiator
	virtual BOOL PollingDetectInitiator(BOOL bDebug=TRUE);
	//Target
	virtual BOOL PollingDetectTarget(BOOL bDebug=TRUE);
	//������� �б�
	virtual BOOL DeviceInfoRead(BOOL bDebug=TRUE);
	//======================================================//
	//	apud Ŀ��Ʈ
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE);
	//ī�� ã��
	virtual BOOL FindCard(BOOL bDebug=TRUE);
	//======================================================//
	//	SAM ī��
	//======================================================//
	//off
	virtual void SAMPwoff(BOOL bDebug=TRUE);
	//on
	virtual void SAMPwon(int _nSlotno=0, BOOL bDebug=TRUE);
	//�� Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE);
	//ī�� ���� �� ó�� �Լ�
	virtual void CardDetectFunc();
	//���� Send �Լ�
	virtual BOOL PollingSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE);
	//���� ����Ÿ ����
	virtual BOOL  FileDataSend();
	//üũ ����Ÿ(����Ÿ ���� �Լ�)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//������ ���� ���� �Լ�
	virtual void  SetThreadState(UINT uThreadState, UINT uModeState);
	// ��� ü���� �Լ�(�ÿ� ���� ���)
	virtual void  ChangeModeShow(UINT _uShowMode);
//=======================================================//
};
#endif// __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__