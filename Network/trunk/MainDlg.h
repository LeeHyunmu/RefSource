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
		MESSAGE_HANDLER(WM_PAINT, OnPaint) // �޽������� �����ϵ�, ���� �Լ��� �������� �ʾƾ��Ѵ�.
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
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
	//===========Class ��ü �ʱ�ȭ �Լ�=====================//
	BOOL initObject();
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
	//			��ư�� Ŭ�� �޽��� �޴´�
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
};
