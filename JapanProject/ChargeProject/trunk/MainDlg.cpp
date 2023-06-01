// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GdiDlg/GdiDlg.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	//메인 초기화 함수
	maingdi.initDlg(m_hWnd);
	DebugDlg->Write(_T("S"));
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	//=============================================
	//  MainGid Class 객체 파괴함수
	//=============================================
	maingdi.Release();
	//|=============================================
	//|디버그 출력 창을 제일 나중에 종료한다
	//|==================================================
	CDebugDlg::ReleaseDebug();
	return 0;
}
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnEraseBkgnd(CDCHandle dc)
{
	return maingdi.EraseBkgnd(dc);
}
//--------------------------------------------------------------
//|
//|그리기 함수
//|
//---------------------------------------------------------------
void CMainDlg::OnPaint(CDCHandle dc)
{
	// 여기서 뭔가를 실제로 그려준다.
	maingdi.DrawPaint();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if(maingdi.ButtonClickedMessage(uNotifyCode,nID)==FALSE)
		SetMsgHandled(FALSE);
}