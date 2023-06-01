// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once


class CCompleteDlg : public CDialogImpl<CCompleteDlg>, public CMessageFilter
{
	//Rgn
	CRgnHandle m_hRgn;
public:
	enum { IDD = IDD_COMPLETDLG };

	BEGIN_MSG_MAP(CCompleteDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	CCompleteDlg();
	//�̹���
	CSuperStatic m_image;
	BOOL bClose;
	UINT imgid;
	//======================================================//
	//			
	//======================================================//
	CMultiMediaTimer<CCompleteDlg> m_Timer;
// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	//�ʱⰪ �����Լ�
	void init(UINT id);
	//Dlg ���� �Լ�
	void DlgClose();
	//Dlg �ʱ�ȭ 
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
	void OnMMTimer(UINT uID);
	//�����Լ�
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
	BOOL GetClose();
};
