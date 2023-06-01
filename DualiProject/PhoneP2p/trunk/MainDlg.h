// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CMessageFilter
{
	CSuperButton CloseBut;
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		REFLECT_NOTIFICATIONS()
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
	//메인 gdi
	IMainGdiDlg* gdi;
	//===========Class 객체 생성 함수=====================//
	BOOL CreateObject();
	//===========Class 객체 초기화 함수=====================//
	BOOL initObject();
	//===========Class 객체 소멸=====================//
	void Release();
	//======================================================//
	//			초기화 
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			윈도우 파괴
	//======================================================//
	void CloseDialog(int nVal);
	//======================================================//
	//			종료버튼
	//======================================================//
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//======================================================//
	//======================================================//
	//			윈도우 파괴함수
	//======================================================//
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
};
