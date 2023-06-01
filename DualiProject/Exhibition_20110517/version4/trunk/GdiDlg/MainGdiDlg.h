
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


interface IImagingFactory;
interface IImage;

class CMainGdiDlg : public IMainGdiDlg
{
	typedef vector<IImage*> MODEIMAGE;
	typedef vector<CString> vecApudString;
public:
	//������
	CMainGdiDlg();
	//�Ҹ���
	~CMainGdiDlg();
	//=================������ �Լ�===========================//
	//======================================================//
	//			�����庯��
	//======================================================//
	CThread  maingdithread;
	CEvent   maingdiEvent;
	CEvent   CloseEvent;
	MODEIMAGE modeimage;
	//======================================================//
	//			�������Լ�
	//======================================================//
	void ThreadMakeObject();
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	void MainThreadFunc();

	//=======================================================//
	//			��Ʈ�� ����
	//======================================================//
	//CSuperButton but1;
	//=======================================================//
	//			���� ����
	//======================================================//
	HWND hWnd;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//IDualiP2pMg*  dualip2p;
	IDualiCardMg* dualicard;
	//======================================================//
	//			�������̽� ������ 
	//======================================================//

	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	void DrawWhiteText(CDC* pDC, CRect textrc, CString text);
	//=======================================================//
	//======================================================//
	//			����
	//======================================================//
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//
	UINT MgNum;
	//
	UINT MgMode;
	//
	IImagingFactory* pImgFactory;
	//��� ���� ��ư
	CSuperButton ModeChangeBut;
	//�����ư
	CSuperButton CloseBut;
	//��ư
	CSuperButton HospitalBut_1;
	//��ư
	CSuperButton HospitalBut_2;
	//��ư
	CSuperButton HospitalBut_3;
	//
	UINT hospitalnum;
	//
	UINT ButShow;
	UINT backButShow;
	//��� 3���� ��ȭ��ȣ�� �ؽ�Ʈ�� ����ϴ� ��
	CString strMode3;
	CRect   rcMode3;
	//�̹��� ��� ��ȯ
	int ImageNumber;
	int imgbackup;
	BOOL bCardConnection;
	DWORD changeTime;
	//������ ��
	int loopcount;
	//APDU String
	vecApudString apdu;
	//���� ���
	CString soundfile;
	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd);
	//Dlg �׸��� �Լ�
	virtual void DrawPaint();
	//�ı�
	virtual void  ControlDestroy();
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//���3���� �ؽ�Ʈ ����Լ�
	virtual void SetMode3Text(CString text, CRect* rc);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
