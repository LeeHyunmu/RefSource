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

	BEGIN_MSG_MAP_EX(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DEVICECHANGE, OnDeviceChange)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDC_SCARDNAME,OnListboxSCardNameNotify)
		COMMAND_ID_HANDLER(IDC_VIDPID,OnComboboxVidPidNotify)
		COMMAND_ID_HANDLER(IDC_APDUSHORT,OnComboboxApduShortNotify)
		COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnButtonClickedEvent)
		MSG_WM_CTLCOLORSTATIC(OnCtlColorStatic)
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
	//����Ʈī�� ��ſ� ��ü
	ISCardTm* scardtm;
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
	//			����̽� ���ؼ� �۵� �Լ�
	//======================================================//
	LRESULT OnDeviceChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
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
	//			Combobox VidPid ���� �ٲ������
	//======================================================//
	LRESULT OnComboboxVidPidNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//======================================================//
	//			Combobox ApduShort ���� �ٲ������
	//======================================================//
	LRESULT OnComboboxApduShortNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//======================================================//
	//			ListBox ���� Ŭ���Ǿ��� ��
	//======================================================//
	LRESULT OnListboxSCardNameNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//======================================================//
	//			��ư�� Ŭ�� �޽��� �޴´�
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//======================================================//
	//			Static ��Ʈ�� 
	//======================================================//
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
};
