
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


interface IImagingFactory;
interface IImage;

class CMainGdiDlg
{
	//�޸� DC
	CMemDC* m_memDC;
	//��� �̹���
	CBitmapHandle backimage;
	//Rgn
	CRgnHandle m_hRgn;
	//
	CSuperButton videochange;
	typedef vector<IImage*> MODEIMAGE;
	typedef vector<CString> vecApudString;

	CAQdeviceCmd test;
	//Atype Call back func
	static DWORD WINAPI AtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD CMainGdiDlg::AtypEventMessage(byte* eventdata, int eventdatalen);
	//Mtype Call back func
	static DWORD WINAPI MtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD CMainGdiDlg::MtypEventMessage(byte* eventdata, int eventdatalen);
	//disconnect Call back func
	static DWORD WINAPI DisConnectEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD CMainGdiDlg::DisConnectEventMessage(byte* eventdata, int eventdatalen);
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
	IImagingFactory* pImgFactory;

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
	virtual void DrawPaint();
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
