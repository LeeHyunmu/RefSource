
#ifndef __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//	
//======================================================//
class CMobileClient : public CThreadImpl<CMobileClient>, public IClient
{

public:
	//������
	CMobileClient();
	//�Ҹ���
	~CMobileClient();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	//������ ���Լ�
	DWORD Run();
	//Recv ����� ���� �Լ�
	int recvn(SOCKET s, CHAR *buf, int len, int flags, BOOL bfull=FALSE);
	//������ ���� ȣ�� �Լ�
	BOOL ClientMain();
	//������ ����
	void ThreadClose();
	//Recv Send �Ϸ� �̺�Ʈ �Լ�
	BOOL WaitForReadEvent(DWORD dwTimeout = INFINITE) const;
//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			������� ����
//======================================================//
	SOCKET sock;
	//Read Event
	HANDLE hReadEvent;
	//Wirte Event
	HANDLE hWriteEvent;
	//�ۼ��� ����
	CHAR buf[MAX_SOCKBUF];
	//������ ������
	DWORD dwSendSize;
	//�޴� ������
	DWORD dwRecvSize;
	//���� �÷���
	DWORD dwLoop;
	//��Ʈ
	u_short port;
	//���� ���� �ּ�
	CString ip;
	//��볻�� ��ȸ ����ü
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	//��ü �ʱ�ȭ
	virtual BOOL initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ReleaseClient();
	//���� ����(ip , port)
	virtual	BOOL ServerConnect(u_short _port, CString _ip);
	//������ ����
	virtual BOOL SendDatafn(DWORD _dwSendSize, void* data, DWORD dwTimeout = INFINITE);
	//������ �ޱ�
	virtual BOOL RecvDatafn(DWORD _dwRecvSize, void* data, DWORD dwTimeout = INFINITE);
	//Ŭ���̾�Ʈ�� ���ӻ����ΰ�?
	virtual BOOL ActiveClient();
//=======================================================//

};
#endif// __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
