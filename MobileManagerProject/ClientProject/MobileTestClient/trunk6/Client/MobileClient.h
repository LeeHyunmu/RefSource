
#ifndef __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//
#define  MAX_SOCKBUF	8192
//--------------------------------------------------------------
//|
//| �������� ������ ����ü
//|
//---------------------------------------------------------------
#pragma pack(push,1)
struct baseDATA20
{
	DWORD dwBaseCode;
	CHAR szCardNum[12];
	DWORD dwdatalen;
	CHAR  data[1];
};
struct DATA36
{
	CHAR  info[32];
	DWORD dwcheckorlen;
	CHAR  afterdata[1];
};
struct DATA162
{
	DWORD  dwnum;
	DWORD  dwcode;
	DWORD  dwusemoney;
	CHAR usename[21];
	CHAR usecardnum[11];
	CHAR useplace[21];
	CHAR useadress[61];
	CHAR usetime[32];
	DWORD dwcheckorlen;
	CHAR  afterdata[1];
};
#pragma pack(pop)
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
	DWORD Run();
	int recvn(SOCKET s, char *buf, int len, int flags, BOOL bfull=FALSE);
	BOOL ClientMain();
	void ThreadClose();
	int findBluetooth();
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
	TCHAR buf[MAX_SOCKBUF];
	//���� �÷���
	BOOL bEnd;
	//��Ʈ
	u_short port;
	//���� ���� �ּ�
	CString ip;
	//������ �ڵ�(������ �ڵ忡�� ��볻�� ��ȸ�� ����ü�� Ʋ������)
	DWORD wsendCode;
	//��볻�� ��ȸ ����ü
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void ReleaseClient();
	virtual BOOL initClient(IMyObject* _obj);
	virtual	BOOL ServerConnect(u_short _port, CString _ip);
	virtual void SendData(int code, TCHAR* CardNum, TCHAR* data);
//=======================================================//

};
#endif// __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
