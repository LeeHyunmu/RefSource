
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainGdiDlg
{
	static enum ButState{
		dlgpurchase=0x00,
		dlgpointcheck=0x01
	};
	//Ȱ��ȭ�� dlg
	ButState dlgState;
	//dlg ����
	vector<CWindow*> dlgWater;

	//�޸� DC
	CMemDC* m_memDC;
	//��� �̹���
	CBitmapHandle backimage;
	//ani image num
	int AniSelectNum;
	int AniNum;
	//Rgn
	CRgnHandle m_hRgn;

	CAQdeviceCmd test;
	//Atype Call back func
	static DWORD WINAPI AtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD AtypEventMessage(byte* eventdata, int eventdatalen);
	//Mtype Call back func
	static DWORD WINAPI MtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD MtypEventMessage(byte* eventdata, int eventdatalen);
	//disconnect Call back func
	static DWORD WINAPI DisConnectEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD DisConnectEventMessage(byte* eventdata, int eventdatalen);
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
	//======================================================//
	//			�������Լ�
	//======================================================//
	void ThreadMakeObject();
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	void MainThreadFunc();

	//=======================================================//
	//			��Ʈ�� ����
	//======================================================//

	//=======================================================//
	//			���� ����
	//======================================================//
	HWND hWnd;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
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

	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd);
	//��� �׸��� �Լ�
	virtual BOOL EraseBkgnd(CDCHandle dc);
	//Dlg �׸��� �Լ�
	virtual void DrawPaint(CDCHandle dc);
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
