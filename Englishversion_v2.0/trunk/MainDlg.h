// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
	IOutput* debug;
	CSuperButton CloseBut;
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_USERTIMER, OnUserTimer)
		MESSAGE_HANDLER(WM_NFCEVENTCALL, OnNfcEventCall)
		MESSAGE_HANDLER(WM_DEVICECMDCALLMESSAGE, OnDeviceCmdCallMessage)
		MESSAGE_HANDLER(WM_NFCAQDEVICEMESSAGE, OnNfcAqDeviceMessage)
		MESSAGE_HANDLER(WM_COUPONCLICKMSG, OnCouponClickMessage)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		MSG_WM_TIMER(OnTimer)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MESSAGE_HANDLER(WM_PAINT, OnPaint) // 메시지맵은 정의하되, 실제 함수는 선언하지 않아야한다.
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
	//			버튼의 클릭 메시지 받는다
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//======================================================//
	//			
	//======================================================//
	void OnTimer(UINT_PTR nIDEvent);
	//======================================================//
	//			
	//======================================================//
	void OnMouseMove(UINT nFlags, CPoint point);
	//======================================================//
	//			
	//======================================================//
	LRESULT OnUserTimer(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			
	//======================================================//
	LRESULT OnNfcEventCall(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			
	//======================================================//
	LRESULT OnDeviceCmdCallMessage(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			
	//======================================================//
	LRESULT OnNfcAqDeviceMessage(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			
	//======================================================//
	LRESULT OnCouponClickMessage(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			
	//======================================================//

	CMultiMediaTimer<CMainDlg> m_Timer;
	//======================================================//
	//			
	//======================================================//
	void OnMMTimer(UINT uID); 
	//======================================================//
	//			
	//======================================================//
	BOOL OnEraseBkgnd(CDCHandle dc);
};
