// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "GdiDlg/GdiDlg.h"
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
	//=============================================
	// 윈도우 전체 화면
	//=============================================
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,iX, iY,SWP_SHOWWINDOW);
	//=============================================
	// 종료버튼
	//=============================================
	//CloseBut.Create(m_hWnd, CRect(iX-120, 20, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	////이미지(비트맵) 인자로 넘긴다
	//CloseBut.initSuperButton(IDB_PNG1,_T("PNG"));

	//======================================
	// Main Dlg Gdi 클래스
	//======================================
	gdi = new CMainGdiDlg;
	//======================================
	//=============================================
	// 오브젝트 생성
	//=============================================
	CreateObject();
	//======================================
	// Main Dlg Gdi 클래스 초기화
	//======================================
	if(gdi)
	{
		gdi->initInterfaceObject(obj);
	}
	//=============================================
	// UI 초기화(윈도우 핸들을 넘긴다)
	//=============================================
	if(gdi)
		gdi->initDlg(m_hWnd);
	//=============================================
	// 오브젝트 초기화
	//=============================================
	initObject();
#ifndef _DEBUG
	CWindow pTaskBar = ::FindWindow(_T("HHTaskBar"), _T(""));
	if(pTaskBar && ::IsWindow(pTaskBar))
	{
		pTaskBar.ShowWindow(SW_HIDE);
	}
#endif //_DEBUG
	return TRUE;
}
//--------------------------------------------------------------
//|
//|종료함수
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	Release();
	CloseDialog(wID);
	return 0;
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
		obj->CreateObject(gdi);
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
		delete ((CClassObject*)obj);	obj=NULL;
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
	if(gdi)
	{
		gdi->ControlDestroy();
		gdi->Release();
		delete ((CMainGdiDlg*)gdi);	gdi=NULL;
	}
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	CloseBut.DestroyWindow();
#ifndef _DEBUG
	// Hide the task bar
	CWindow pTaskBar = ::FindWindow(_T("HHTaskBar"), _T(""));
	if(pTaskBar && ::IsWindow(pTaskBar))
	{
		pTaskBar.ShowWindow(SW_SHOW);
	}
#endif //_DEBUG
	return 0;
}
//--------------------------------------------------------------
//|
//| 그리기 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(gdi)
	{
		gdi->DrawPaint();
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	BOOL b=FALSE;
	if(gdi)
		b = gdi->ButtonClickedMessage(uNotifyCode,nID);
	if(!b) SetMsgHandled(FALSE);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnTimer(UINT_PTR uTimerID)
{
}