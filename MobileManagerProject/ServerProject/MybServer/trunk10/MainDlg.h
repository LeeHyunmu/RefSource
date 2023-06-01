// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
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
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_HANDLER(IDC_BUSTART, BN_CLICKED, OnBnClickedBustart)
		COMMAND_HANDLER(IDC_BUSTOP, BN_CLICKED, OnBnClickedBustop)
		COMMAND_HANDLER(IDC_BUSTARTDB, BN_CLICKED, OnBnClickedBustartdb)
		COMMAND_HANDLER(IDC_BUCONNECTCLIENT, BN_CLICKED, OnBnClickedBuconnectclient)
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
	//=======================================================//

	//==통합생성 객체 포인터(사용하고자하는 클래스 포인터)==//

	//=======================================================//

	//======================================================//
	//			일반 함수
	//======================================================//
	//===========Class 객체 생성 함수=====================//
	BOOL CreateObject();
	//===========Class 객체 소멸=====================//
	void Release();
	//===========종료 함수=====================//
	void CloseDialog(int nVal);
	//=======================================================//

	//======================================================//
	//			메시지 함수
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBustart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBustop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBustartdb(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBuconnectclient(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
