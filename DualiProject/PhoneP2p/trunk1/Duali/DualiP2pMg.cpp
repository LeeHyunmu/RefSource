
#include "stdAfx.h"
#include "DualiP2pMg.h"

#define LOOPFLAGE_ACTIVE	0x00000001 //���� ���� ����
#define LOOPFLAGE_END		0x00000002 //���� ���� ���� ����
#define LOOPFLAGE_CLOSE		0x00000004 //���� ���� ���� ����

#define SENDFLAGE_DATA		0x00000001 //����Ÿ�� ���� ��
#define SENDFLAGE_FILE		0x00000002 //������ ���� ��
#define SENDFLAGE_NONE		0x00000004 //�ƹ��͵� ������ ���� ��
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CDualiP2pMg::CDualiP2pMg():
debug(NULL),
bOk(FALSE),
dwSendflage(SENDFLAGE_NONE)
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CDualiP2pMg::~CDualiP2pMg()
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDualiP2pMg::P2pObject()
{
	////////////////////////////////////////////////////////////////
	//�̺�Ʈ ��ü ����
	//�Ϸ� ���� �޽���
	hReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	//Send, Recv ���� �޽���
	hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	// ������ ����
	p2pthread.Create(P2pProcess, (LPVOID)this);
	dwLoop=LOOPFLAGE_ACTIVE;
}
//--------------------------------------------------------------
//|
//|ī�� ���� �� ó�� �Լ�
//|
//---------------------------------------------------------------
void CDualiP2pMg::P2pFunc()
{
	Sleep(10);
	while(TRUE)
	{
		WaitForSingleObject(hWriteEvent, INFINITE);
		if(dwLoop&LOOPFLAGE_ACTIVE)
		{
			if(dwSendflage&SENDFLAGE_DATA)
			{
				bOk = dualicard->OneDataSend(SendData, RecvData);
			}
			else if(dwSendflage&SENDFLAGE_FILE)
			{
				bOk = dualicard->FileDataSend(SendData);
			}
		}
		else if(dwLoop&LOOPFLAGE_CLOSE)
		{
			break;
		}
		SetEvent(hReadEvent);
		debug->Write(_T("���� �غ����"));
	}
	dwLoop=LOOPFLAGE_END;
	SetEvent(hReadEvent);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CDualiP2pMg::P2pProcess(LPVOID lpArg)
{
	IDualiP2pMg *dualip2p = (IDualiP2pMg *)lpArg;
	dualip2p->P2pFunc();
	return 0;
}

//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CDualiP2pMg::Release()
{
	//|=============================================
	//|������ ���� �̺�Ʈ
	//|==================================================
	if(hWriteEvent)
	{
		dwLoop=LOOPFLAGE_CLOSE;
		SetEvent(hWriteEvent);
	}
	WaitForSingleObject(hReadEvent, INFINITE);
	CloseHandle(hReadEvent);
	CloseHandle(hWriteEvent);
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CDualiP2pMg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
	//========================================
	debug=obj->GetOutputObject();
	//========================================
	// ��� ���� ���̺귯�� Ŭ����
	//========================================
	dualicard = obj->GetDualiCardMgObject();
	//���� �ʱ�ȭ
	//�۽� ����Ÿ
	SendData="";
	//���� ����Ÿ
	RecvData="";
	P2pObject();
}
//--------------------------------------------------------------
//|
//|Send (���� �����Ͱ� ���� ��
//|
//---------------------------------------------------------------
BOOL CDualiP2pMg::SendDatafn(DWORD _dwSendflage, CString data, DWORD dwTimeout)
{
	bOk=FALSE;
	dwSendflage=_dwSendflage;
	SendData="";
	SendData=data;
	SetEvent(hWriteEvent);
	if(WaitForSingleObject(hReadEvent, dwTimeout)== WAIT_OBJECT_0)
	{
		data=RecvData;
	}
	dwSendflage=SENDFLAGE_NONE;
	SendData="";
	return bOk;
}
