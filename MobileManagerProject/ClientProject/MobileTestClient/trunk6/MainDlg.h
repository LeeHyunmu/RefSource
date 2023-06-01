// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
	//======================================================//
	//			정적 생성 객체
	//======================================================//
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUCONNECT, BN_CLICKED, OnBnClickedBuconnect)
		COMMAND_HANDLER(IDC_BUSEND, BN_CLICKED, OnBnClickedBusend)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
	END_MSG_MAP()
	//======================================================//
	//			생성자
	//======================================================//
	CMainDlg();
	//======================================================//
	//			컨트롤 변수
	//======================================================//
	CComboBox typecode;
	//======================================================//
	//			동적 생성 객체
	//======================================================//
	//===========Class 객체 생성 클래스=====================//
	IMyObject *obj;
	//=======================================================//

	//==통합생성 객체 포인터(사용하고자하는 클래스 포인터)==//
	IClient *client;
	//=======================================================//

	//======================================================//
	//			일반 함수
	//======================================================//
	//===========Class 객체 생성 함수=====================//
	BOOL CreateObject();
	//===========Class 객체 소멸=====================//
	void Release();
	//=======================================================//

	//======================================================//
	//			메시지 함수
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);
	LRESULT OnBnClickedBuconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedBusend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};
