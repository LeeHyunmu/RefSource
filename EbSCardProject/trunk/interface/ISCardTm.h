
#ifndef __ISCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|ISCardMsg �������̽�
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|Flag ��
//|---------------------------------------------------------------
//
#define ISCARDMSG_NEW 1
//
#define ISCARDMSG_DISCONNECT 2

interface ISCardMsg
{
	//======================================================//
	//SCard	Msg  ���� ������ �Լ�
	//======================================================//
	//��� ���� Ŭ�������� ����Ʈī�� ��ġ�� ��ȭ�� �˰� ���� ��
	//flag���� ���� ó�� ����� �ٸ��� ����
	virtual void DeviceListChangeMsg(int flag, CString SCardName=_T(""))=0;
};

//--------------------------------------------------------------
//|
//|ISCardTm �������̽�
//|
//---------------------------------------------------------------
interface ISCardTm
{
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release()=0;
	//======================================================//
	//	 �ü���κ��� ����Ʈī�� ���� �޽��� ó���Լ�
	//======================================================//
	//======================================================//
	//			����̽� ���ؼ�(Smart Card ����)
	//======================================================//
	virtual BOOL DeviceConnect(LPARAM lParam, BOOL bConnect)=0;
	//=======================================================//
	//======================================================//
	//		����Ʈī�� ����Ʈ ����� �ܺη� �ش�
	//======================================================//
	virtual BOOL GetSCardListinfo(CString VidPid, vecString& vecSCardName)=0;
	virtual BOOL GetSCardListinfo(vecString& vecVidPid, vecString& vecUsbid, vecString& vecSCardName)=0;
	//======================================================//
	//			����Ʈī�� ���� ó�� �Լ�
	//======================================================//
	//���� ���� ���(�̸����� �ؼ� �ڵ�� ������Ŭ�� ����)
	virtual BOOL ScardConnect(SCARDHANDLE& hCard, DWORD& ActiveProtocol, CString SCardName, ISCardMsg* _connectobj)=0;
	//���� ���� ���(�ڵ��� �ݳ���)
	virtual BOOL ScardDisConnect(SCARDHANDLE hCard)=0;
	//======================================================//
	//			�Ⱦ��� �Լ�
	//======================================================//
	//T0 ��ſ� �������̽� �Լ�
	virtual BOOL CommandT0(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//T1 ��ſ� �������̽� �Լ�
	virtual BOOL CommandT1(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//======================================================//
	//			��ſ� �Լ�
	//======================================================//
	//�������ݿ� ����  ����� ������ �������̽� �Լ�
	virtual BOOL CommandAll(SCARDHANDLE scard, DWORD protocol, CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//======================================================//
	//			Last Error Code
	//======================================================//
	//���� ������ ����� ���� �����ڵ带 �ѱ��
	virtual CString GetLastErrorCode()=0;
	//======================================================//
	//			��ġ��ȭ�� ���� ���� ��ü ���
	//======================================================//
	virtual void SetSCardMsg(ISCardMsg* obj)=0;
};

#endif //__ISCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__