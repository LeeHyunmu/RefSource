// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define COUPONDLGTHREAD_LOOPTIME 50
class CCouponDlg : public CDialogImpl<CCouponDlg>, public CMessageFilter
{
	DWORD aniFlag;
	DWORD aniTime;
	HWND hWnd;
	BOOL bDestoryFlag;
	//
	CEvent RespEvent;
	//
	CThread NfcThread;
public:
	enum { IDD = IDD_COUPONPOP };

	BEGIN_MSG_MAP(CCouponDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//��� ���� ��ư
	CSuperButton couponbtn1;
	CSuperButton couponbtn2;
	CSuperButton couponbtn3;
	CSuperButton couponbtn4;
	CSuperButton couponbtn5;
	CSuperButton couponbtn6;
	CSuperButton couponbtn7;
	CSuperButton couponbtn8;

	vector<CSuperButton*> move1;
	vector<CSuperButton*> move2;
	int moveindex;
	//======================================================//
	//			
	//======================================================//
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	//������
	CCouponDlg();
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
};
