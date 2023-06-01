
#ifndef __SCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSCardTm : public ISCardTm
{
	struct SCARDINFOSTRUCT 
	{
		ISCardMsg* object;
		CString VP_id;
		CString USB_id;
	};
	//��Ʈ���� Ž���ϱ� ���� ����
	typedef vector<ISCardMsg*> SCARDOBJECTMSG;
	//��Ʈ���� Ž���ϱ� ���� ����
	typedef CStringElementTraits<CString> ATLELEMENTSCARD;
	//����Ʈī�� ����
	typedef CAtlMap<CString, SCARDINFOSTRUCT, ATLELEMENTSCARD> ATLMAPSCARD;
	//�˻����� ���� ��ü
	typedef CAtlMap<CString, SCARDINFOSTRUCT, ATLELEMENTSCARD>::CPair ATLPAIRSCARD;
	//����Ʈī���ǵ��� contex
	SCARDCONTEXT	 hContextHandle;
	//VID_PID�� �����Ͽ� ����
	ATLMAPSCARD SCardList;
	//��ġ ��ȭ�� ���� �޽��� �ޱ� ���� ��ü ����
	SCARDOBJECTMSG ObjectMsg;

	//Last Recv Code
	CString ErrorCode;
public:
	//������
	CSCardTm();
	//���� ������
	CSCardTm(CSCardTm &v);
	//�Ҹ���
	~CSCardTm();
	//=================������ �Լ�===========================//

	//=======================================================//
	//			���� ����
	//======================================================//

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
	//======================================================//
	//			������ ��ȯ �⺻ �Լ�
	//======================================================//
	//�Լ� ��Ʈ�� ���ڸ� HEX ������ ��ȯ�Ѵ�
	int ChangeHexCord(CString data, BYTE hex[]);
	//char�� hex��
	BOOL Changcharbyhex(CString pass, CString& repass);
	//BYTE�� hex��
	BOOL ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass);
	//
	void ChangStringtoChar(CString pass,  char repass[]);
	//======================================================//
	//			����Ʈī�� ����Ÿ �����м�
	//======================================================//
	BOOL Analysis(SCARDHANDLE scard, DWORD protocol,CString cord, CString& recv, int nCount, BOOL brecv);
	//======================================================//
	//			����Ʈī�� �ʱ�ȭ ���ؽ�Ʈ �ڵ��� ��´�
	//======================================================//
	bool SCardinit();
	//======================================================//
	//			���ؽ�Ʈ �ڵ��� �ݴ´�
	//======================================================//
	void ReleaseinitHandle(void);
	//======================================================//
	//			����Ʈī�� ���� ����Ʈ �б� �Լ�
	//======================================================//
	//�̹� ���ӵ� ����Ʈ��ġ�� �ε��ϱ� ���ؼ� PidVid���� _T("")�� �Ѵ�
	size_t ConnectSCardList(CString PidVid, CString Usbid);
	//======================================================//
	//		�������� ��� �Լ�(BYTE������ ������ ���)
	//======================================================//
	//T0 �������� ���
	int  CommandT0(SCARDHANDLE scard,LPBYTE sendbuffer, unsigned long dwtransLen, BYTE recvbuffer[],int len);
	//T1�������� ���
	int  CommandT1(SCARDHANDLE scard,LPBYTE sendbuffer, unsigned long dwtransLen, BYTE recvbuffer[],int len);

	//======================================================//
	//	������κ��� ����Ʈī�� ���� �޽����� �޾��� ��
	//======================================================//
	BOOL AddSCardList(CString PidVid, CString Usbid);
	BOOL RemoveSCardList(CString PidVid, CString Usbid);
	//======================================================//
	//	���α׷� ���� ���� ���ӵ� ����Ʈī��
	//======================================================//
	//����Ʈī�� �������� Vid Pid ã��
	CString GetDevicePidVid(CString DeviceName);

	//======================================================//
	//	Usbid�� �������� ������ ��
	//======================================================//
	//�����ϱ����ؼ� ���ӵ� ����Ʈ����� �����
	size_t GetSCardList(vecString& vecSCard);
	//Usbid�� ã�� ������ ���
	BOOL RemoveSCardList();
	//======================================================//
	//	��ϵ� Ŭ������ ��ȭ�� ���� ������ ������
	//======================================================//
	void SendSCardChangMsg();
	void SendSCardDisconnectMsg(POSITION pos);
	//=======================================================//
	//======================================================//
	//			����
	//======================================================//

	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release();
	//======================================================//
	//	 �ü���κ��� ����Ʈī�� ���� �޽��� ó���Լ�
	//======================================================//
	//======================================================//
	//			����̽� ���ؼ�(Smart Card ����)
	//======================================================//
	virtual BOOL DeviceConnect(LPARAM lParam, BOOL bConnect);
	//=======================================================//
	//======================================================//
	//		����Ʈī�� ����Ʈ ����� �ܺη� �ش�
	//======================================================//
	virtual BOOL GetSCardListinfo(CString VidPid, vecString& vecSCardName);
	virtual BOOL GetSCardListinfo(vecString& vecVidPid, vecString& vecUsbid, vecString& vecSCardName);
	//======================================================//
	//			����Ʈī�� ���� ó�� �Լ�
	//======================================================//
	//���� ���� ���(�̸����� �ؼ� �ڵ�� ������Ŭ�� ����)
	virtual BOOL ScardConnect(SCARDHANDLE& hCard, DWORD& ActiveProtocol, CString SCardName, ISCardMsg* _connectobj);
	//���� ���� ���(�ڵ��� �ݳ���)
	virtual BOOL ScardDisConnect(SCARDHANDLE hCard);
	//======================================================//
	//			�Ⱦ��� �Լ�
	//======================================================//
	//T0 ��ſ� �������̽� �Լ�
	virtual BOOL CommandT0(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//T1 ��ſ� �������̽� �Լ�
	virtual BOOL CommandT1(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//======================================================//
	//			��ſ� �Լ�
	//======================================================//
	//�������ݿ� ����  ����� ������ �������̽� �Լ�
	virtual BOOL CommandAll(SCARDHANDLE scard, DWORD protocol, CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//======================================================//
	//			Last Error Code
	//======================================================//
	//���� ������ ����� ���� �����ڵ带 �ѱ��
	virtual CString GetLastErrorCode();
	//======================================================//
	//			��ġ��ȭ�� ���� ���� ��ü ���
	//======================================================//
	virtual void SetSCardMsg(ISCardMsg* obj);
};
#endif// __SCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__
