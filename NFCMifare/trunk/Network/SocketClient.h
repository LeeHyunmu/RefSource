
#ifndef __SOCKETECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SOCKETECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketClient
{
	HWND hWndp;
	//
	CEvent RespEvent;
	//
	CThread NfcThread;
public:
	//������
	CSocketClient();
	//�Ҹ���
	~CSocketClient();
//======================================================//
//		 ����Ʈ Ŭ���̾�Ʈ ���� ��
//======================================================//
	static const UINT DEFAULTPORT;
    static const CString DEFAULTIP;
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	int recvn(SOCKET s, char *buf, int len, int flags, BOOL bfull=FALSE);
	void ClientMain();
	//thread �Լ�
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
//=======================================================//
//			��ü ������ ����
//======================================================//

//=======================================================//
//			������� ����
//======================================================//
	SOCKET sock;
	//���� ����
	byte recvbuf[BUFFSIZE_4096];
	UINT recvbuflen;
	//�ۼ��� �ӽù���
	char tempbuf[BUFFSIZE_1024];
	UINT tempbuflen;
	//��Ʈ
	u_short port;
	//���� ���� �ּ�
	CString ip;
	//��볻�� ��ȸ ����ü
//======================================================//
//			
//======================================================//
	void ReleaseClient();
	BOOL initClient(HWND _hwnd);
	BOOL ServerConnect(u_short _port, CString _ip);
	BOOL SendData(int code, CString type, byte* data, UINT datalen);
//=======================================================//
	//����� ��� ȣ��
	void DebugEditCall(CString str);
};
#endif// __SOCKETECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
