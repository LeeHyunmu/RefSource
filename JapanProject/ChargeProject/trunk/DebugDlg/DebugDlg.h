// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#define DebugDlg CDebugDlg::GetDebug()
class CDebugDlg : public CDialogImpl<CDebugDlg>, public CMessageFilter
{
	typedef deque<CString, allocator<CString> > CHARDEQUE;
	typedef queue<CString,CHARDEQUE> CHARQUEUE;
	CHARQUEUE qData;
	//
	CEvent OutputEvent;
	//
	CThread Outputthread;
	HWND hWnd;
	CListBox DebugMessage;
	//
	static CDebugDlg* debug;
public:
	enum { IDD = IDD_DEBUGDLG};

	BEGIN_MSG_MAP(CDebugDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()
	//======================================================//
	//			
	//======================================================//
	//생성자
	CDebugDlg();
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
	//
	//thread 함수
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	void Write(TCHAR* format,...);
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	void OutputString();
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	static CDebugDlg* GetDebug();
	//--------------------------------------------------------------
	//|
	//|
	//|
	//---------------------------------------------------------------
	static void ReleaseDebug();
};
