// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CContentsMallDlg : public CDialogImpl<CContentsMallDlg>, public CMessageFilter
{
	DWORD aniFlag;
	DWORD aniTime;
	HWND hWnd;
	BOOL bDestoryFlag;
	//
	CEvent RespEvent;
	//
	CThread NfcThread;
	//
	static int imgId[16];
	//
	int imgnum;
public:
	enum { IDD = IDD_CONTENTSMALL };

	BEGIN_MSG_MAP(CContentsMallDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//��� ���� ��ư
	CSuperButton contentsbtn1;
	CSuperButton contentsbtn2;
	CSuperButton contentsbtn3;
	CSuperButton contentsbtn4;
	int moveindex;
	//======================================================//
	//			
	//======================================================//
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	//������
	CContentsMallDlg();
	//�ʱⰪ �����Լ�
	void init(HWND _hWnd);
	//Dlg ���� �Լ�
	void DlgClose();
	//Dlg �ʱ�ȭ 
	//======================================================//
	//			�ʱ�ȭ
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			������ �ı��Լ�
	//======================================================//
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	BOOL PreTranslateMessage(MSG* pMsg);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	void OnMouseMove(UINT nFlags, CPoint point);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	void buttonAnimation();
	//
	//thread �Լ�
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	BOOL OnEraseBkgnd(CDCHandle dc);
};
