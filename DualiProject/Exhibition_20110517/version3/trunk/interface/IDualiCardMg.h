
#ifndef __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|������ ���� ���� �÷���
//|
//---------------------------------------------------------------
// Mode �÷���
#define DEVICEMODE_INITIATOR	0x00000001 //initiator Mode
#define DEVICEMODE_TARGET		0x00000002 //Target Mode
#define DEVICEMODE_NONE			0x00000004 //None

// Thread �÷���
#define THREADLOOP_IDLING		0x00000001 //idling
#define THREADLOOP_DETECTING	0x00000002 //detecting
#define THREADLOOP_NONE			0x00000004 //None
//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IDualiCardMg
{
	virtual ~IDualiCardMg()=0;
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	virtual void Release()=0;
	//ī�� ���ؼ�
	virtual void CardConnent(CString nPort)=0;
	//
	virtual BOOL IdleReqa(BOOL bDebug=TRUE)=0;
	//���� ����
	//initiator
	virtual BOOL PollingDetectInitiator(BOOL bDebug=TRUE)=0;
	//Target
	virtual BOOL PollingDetectTarget(BOOL bDebug=TRUE)=0;
	//Release
	virtual BOOL PollingDetectRelease(BOOL bDebug=TRUE)=0;
	//������� �б�
	virtual BOOL DeviceInfoRead(BOOL bDebug=TRUE)=0;
	//======================================================//
	//	apud Ŀ��Ʈ
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE)=0;
	//ī�� ã��
	virtual BOOL FindCard(BOOL bDebug=TRUE)=0;
	//======================================================//
	//	SAM ī��
	//======================================================//
	//off
	virtual void SAMPwoff(BOOL bDebug=TRUE)=0;
	//on
	virtual void SAMPwon(int _nSlotno=0, BOOL bDebug=TRUE)=0;
	//�� Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE)=0;
	//���� Send �Լ�
	virtual BOOL PollingSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE)=0;
	//���� ����Ÿ ����
	virtual BOOL  FileDataSend()=0;
	//üũ ����Ÿ(����Ÿ ���� �Լ�)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	// card findding loop
	virtual BOOL  CardFinddingLoop()=0;
	// ���ӵ� ī�尡 ���� �����ϴ°� ����
	virtual BOOL  isCardid()=0;
};

#endif //__IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__