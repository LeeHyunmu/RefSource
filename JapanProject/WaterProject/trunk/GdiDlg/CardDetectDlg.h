// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CCardDetectDlg : public CDialogImpl<CCardDetectDlg>, public CMessageFilter
{
	//��� �̹���
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
	//������
	CCardDetectDlg();
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
	void AnimationFunc();
	//
	//thread �Լ�
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
