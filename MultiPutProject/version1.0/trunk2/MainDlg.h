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
		MESSAGE_HANDLER(WM_PAINT, OnPaint) // �޽������� �����ϵ�, ���� �Լ��� �������� �ʾƾ��Ѵ�.
		//MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
		//��Ʈ�ѷ� ���� �޽���
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	//======================================================//
	//			������
	//======================================================//
	CMainDlg();
	//======================================================//
	//			���� ���� ��ü
	//======================================================//
	//===========Class ��ü ���� Ŭ����=====================//
	IMyObject *obj;
	//���� gdi
	IMainGdiDlg* gdi;
	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	//======================================================//
	//			����
	//======================================================//

	//===========Class ��ü ���� �Լ�=====================//
	BOOL CreateObject();
	//===========Class ��ü �Ҹ�=====================//
	void Release();
	//======================================================//
	//			�޽��� �Լ�
	//======================================================//
	//======================================================//
	//			�ʱ�ȭ
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			������ �ı��Լ�
	//======================================================//
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			�����ư
	//======================================================//
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//======================================================//
	//			���� �Լ�
	//======================================================//
	void CloseDialog(int nVal);
	//======================================================//
	//			�׸��� �Լ�
	//======================================================//
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			��� �ٽ� �׸��� �Լ�
	//======================================================//
	BOOL OnEraseBkgnd(CDCHandle dc);
	//======================================================//
	//			��ư�� Ŭ�� �޽��� �޴´�
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//======================================================//
	//		���ʵ�ο찡 �ƴ� static Ŭ���� ��� ����
	//======================================================//
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
	//======================================================//
	//		���콺 ������
	//======================================================//
	void OnRButtonDown(UINT nFlags, CPoint point);
	//======================================================//
	//		���콺 ������
	//======================================================//
	void OnMouseMove(UINT nFlags, CPoint point);
	//======================================================//
	//		���콺 ĸ�� ���� �޽��� 
	//======================================================//
	void OnCaptureChanged(CWindow wnd);
	//======================================================//
	//		HotKey Event 
	//======================================================//
	void OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey);
	//======================================================//
	//		Ŀ�� �޽���
	//======================================================//
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	//======================================================//
	//		Ű���� �޽���(DOWN)
	//======================================================//
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//======================================================//
	//		Ű���� �޽���(UP)
	//======================================================//
	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
