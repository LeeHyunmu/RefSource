// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CCardDetectDlg : public CDialogImpl<CCardDetectDlg>, public CMessageFilter
{
	//배경 이미지
	vector<CBitmapHandle> backimage;
	//ani image num
	int AniSelectNum;
	//Rgn
	CRgnHandle m_hRgn;

	//close
	CSuperButton close;
	//dlg next
	CSuperButton dlgnext;
	//
	CSuperButton pointcheck;
	CSuperButton purchase;
	//
	CThread AniThread;
	//
	BOOL bDestoryFlag;
	//
	HWND hWnd;
public:
	enum { IDD = IDD_CARDDETECTDLG };

	BEGIN_MSG_MAP(CCardDetectDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_ACTIVATE(OnActivate)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//======================================================//
	//			
	//======================================================//
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	//생성자
	CCardDetectDlg();
	//초기값 설정함수
	void init(HWND _hWnd);
	//Dlg 종료 함수
	void DlgClose();
	//Dlg 초기화 
	//======================================================//
	//			초기화
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			윈도우 파괴함수
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
	void AnimationFunc();
	//
	//thread 함수
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	BOOL OnEraseBkgnd(CDCHandle dc);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	void OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther);
};
