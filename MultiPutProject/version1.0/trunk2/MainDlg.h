// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
	CSuperButton CloseBut;
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_RBUTTONDOWN(OnRButtonDown)
		MSG_WM_CAPTURECHANGED(OnCaptureChanged)
		MSG_WM_HOTKEY(OnHotKey)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_KEYUP(OnKeyUp)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MESSAGE_HANDLER(WM_PAINT, OnPaint) // 메시지맵은 정의하되, 실제 함수는 선언하지 않아야한다.
		//MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
		//컨트롤로 통지 메시지
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
	//======================================================//
	//			초기화
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			윈도우 파괴함수
	//======================================================//
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			종료버튼
	//======================================================//
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//======================================================//
	//			종료 함수
	//======================================================//
	void CloseDialog(int nVal);
	//======================================================//
	//			그리기 함수
	//======================================================//
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			배경 다시 그리기 함수
	//======================================================//
	BOOL OnEraseBkgnd(CDCHandle dc);
	//======================================================//
	//			버튼의 클릭 메시지 받는다
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//======================================================//
	//		오너드로우가 아닌 static 클래스 배경 투명
	//======================================================//
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
	//======================================================//
	//		마우스 오른쪽
	//======================================================//
	void OnRButtonDown(UINT nFlags, CPoint point);
	//======================================================//
	//		마우스 움직임
	//======================================================//
	void OnMouseMove(UINT nFlags, CPoint point);
	//======================================================//
	//		마우스 캡쳐 변경 메시지 
	//======================================================//
	void OnCaptureChanged(CWindow wnd);
	//======================================================//
	//		HotKey Event 
	//======================================================//
	void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey);
	//======================================================//
	//		커서 메시지
	//======================================================//
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	//======================================================//
	//		키보드 메시지(DOWN)
	//======================================================//
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//======================================================//
	//		키보드 메시지(UP)
	//======================================================//
	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
