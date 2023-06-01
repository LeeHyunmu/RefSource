// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CDialogImpl<CMainDlg>, public CDoubleBufferImpl<CMainDlg>,public CMessageFilter
{
	CSuperButton CloseBut;
public:
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MSG_WM_TIMER(OnTimer)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
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
	//===========Class ��ü ���� �Լ�=====================//
	BOOL CreateObject();
	//===========Class ��ü �ʱ�ȭ �Լ�=====================//
	BOOL initObject();
	//===========Class ��ü �Ҹ�=====================//
	void Release();
	//======================================================//
	//			�ʱ�ȭ 
	//======================================================//
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			������ �ı�
	//======================================================//
	void CloseDialog(int nVal);
	//======================================================//
	//			�����ư
	//======================================================//
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//======================================================//
	//======================================================//
	//			������ �ı��Լ�
	//======================================================//
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//======================================================//
	//			�׸��� �Լ�
	//======================================================//
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//======================================================//
	//			��ư�� Ŭ�� �޽��� �޴´�
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//
	void OnTimer(UINT_PTR uTimerID);
};
