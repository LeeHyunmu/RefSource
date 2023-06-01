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
	//스마트카드 통신용 객체
	ISCardTm* scardtm;
	//======================================================//
	//			일반 함수
	//======================================================//
	//======================================================//
	//			변수
	//======================================================//

	//===========Class 객체 생성 함수=====================//
	BOOL CreateObject();
	//===========Class 객체 초기화 함수=====================//
	BOOL initObject();
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
	//			디바이스 컨넥션 작동 함수
	//======================================================//
	LRESULT OnDeviceChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
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
	//			Combobox VidPid 셀이 바뀌었을때
	//======================================================//
	LRESULT OnComboboxVidPidNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//======================================================//
	//			Combobox ApduShort 셀이 바뀌었을때
	//======================================================//
	LRESULT OnComboboxApduShortNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//======================================================//
	//			ListBox 셀이 클릭되었을 때
	//======================================================//
	LRESULT OnListboxSCardNameNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);

	//======================================================//
	//			버튼의 클릭 메시지 받는다
	//======================================================//
	void OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl);
	//======================================================//
	//			Static 컨트롤 
	//======================================================//
	HBRUSH OnCtlColorStatic(CDCHandle dc, CStatic wndStatic);
};
