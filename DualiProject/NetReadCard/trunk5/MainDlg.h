// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,public CDoubleBufferImpl<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_HANDLER(IDC_DUALICARDCONNECT, BN_CLICKED, OnBnClickedDualicardconnect)
		COMMAND_HANDLER(IDC_DEBUGPANE, BN_CLICKED, OnBnClickedDebugpane)
		COMMAND_HANDLER(IDC_BUT_0, BN_CLICKED, OnBnClickedBut0)
		COMMAND_HANDLER(IDC_BUT_1, BN_CLICKED, OnBnClickedBut1)
		COMMAND_HANDLER(IDC_BUT_2, BN_CLICKED, OnBnClickedBut2)
		COMMAND_HANDLER(IDC_BUT_3, BN_CLICKED, OnBnClickedBut3)
		COMMAND_HANDLER(IDC_BUT_4, BN_CLICKED, OnBnClickedBut4)
		COMMAND_HANDLER(IDC_BUT_5, BN_CLICKED, OnBnClickedBut5)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
	END_MSG_MAP()
	//======================================================//
	//			생성자
	//======================================================//
	CMainDlg();
	//======================================================//
	//			동적 생성 객체
	//======================================================//
	//===========Class 객체 생성 클래스=====================//
	IMyObject *obj;
	IGdiDlg* gdi;
	//======================================================//
	//			일반 함수
	//======================================================//
	//======================================================//
	//			변수
	//======================================================//

	//===========Class 객체 생성 함수=====================//
	BOOL CreateObject();
	//===========Class 객체 소멸=====================//
	void Release();
	//======================================================//
	//			메시지 함수
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);
	LRESULT OnBnClickedDualicardconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedDebugpane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBut1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBut2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBut3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBut4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBut5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBut0(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
};
