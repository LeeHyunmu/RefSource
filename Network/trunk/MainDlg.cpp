// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainDlg::CMainDlg():
obj(NULL),
gdi(NULL)
{}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnIdle()
{
	return FALSE;
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
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
	//=============================================
	// 오브젝트 생성
	//=============================================
	CreateObject();
	//=============================================
	// MainDlg GDI 클래스 연결 작업
	//=============================================
	//=============================================
	// gdi 객체를 받는다
	//=============================================
	gdi = obj->GetMainGdiObject();
	//=============================================
	// UI 초기화(윈도우 핸들을 넘긴다)
	//=============================================
	gdi->initDlg(m_hWnd);
	//=============================================
	// 오브젝트 초기화
	//=============================================
	initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
	//=============================================
	// 오브젝트 관리 클래스 생성
	//=============================================
	obj=new CClassObject;
	if(obj)
		obj->CreateObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|오브젝트 객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::initObject() 
{
	//=============================================
	// 오브젝트 관리 클래스 생성
	if(obj)
		obj->initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|오브젝트 객체 소멸 함수
//|
//---------------------------------------------------------------
void CMainDlg::Release()
{
	if(obj)
	{
		obj->KillObject();
		delete obj;obj=NULL;
	}
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

	return 0;
}
//--------------------------------------------------------------
//|
//|종료 버튼
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|DLG 종료 함수
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
//--------------------------------------------------------------
//|
//|그리기 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 여기서 뭔가를 실제로 그려준다.
	if(gdi)
		gdi->DrawPaint();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if(gdi)
		return gdi->ButtonClickedMessage(uNotifyCode,nID);
}