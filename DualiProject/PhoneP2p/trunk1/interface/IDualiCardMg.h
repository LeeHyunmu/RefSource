
#ifndef __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IDualiCardMg
{
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	virtual void Release()=0;
	//ī�� ���ؼ�
	virtual void CardConnent(CString nPort)=0;
	//
	virtual BOOL IdleReqa()=0;
	//���� ����
	virtual BOOL PollingDetect()=0;
	//������� �б�
	virtual BOOL DeviceInfoRead()=0;
	//======================================================//
	//	apud Ŀ��Ʈ
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//ī�� ã��
	virtual BOOL FindCard()=0;
	//======================================================//
	//	SAM ī��
	//======================================================//
	//off
	virtual void SAMPwoff()=0;
	//on
	virtual void SAMPwon(int _nSlotno=0)=0;
	//�� Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//ī�� ���� �� ó�� �Լ�
	virtual void CardDetectFunc()=0;
	//���� Send �Լ�
	virtual BOOL PollingSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//���� ����Ÿ ����
	virtual BOOL  FileDataSend(CString FileName)=0;
	//üũ ����Ÿ(����Ÿ ���� �Լ�)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
};

#endif //__IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__