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
	//  Gdiplus 사용 시작
	//=============================================
	CGdiPlusBitmap::GdiStart();
	//=============================================
	// 오브젝트 생성
	//=============================================
	CreateObject();
	//=============================================
	//화면 전체 크기를 얻는다
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,iX-800, iY-600,SWP_SHOWWINDOW);
	//=============================================
	// 종료버튼
	//=============================================
// 	CGdiPlusBitmapResource pBitmap;
// 	pBitmap.Load(IDB_PNG2,"PNG");
// 	CBitmap bit = pBitmap.GetWinBitmap();
	CloseBut.Create(m_hWnd, CRect(iX-950, 20, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//이미지(비트맵) 인자로 넘긴다
	CloseBut.initRgnSuperButton(IDB_BTNIMAGE_X, IDB_BTNMASK_X);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
#ifdef _DEBUG
	debug = new CTraceWinDebug(_T("Baseclass_Debug"));
#else
	debug = new CTraceFileDebug(_T("Baseclass_Debug"));
#endif
	//======================================
	// Main Dlg Gdi 클래스
	//======================================
	gdi = new CMainGdiDlg;
	//======================================
	//=============================================
	// 오브젝트 관리 클래스 생성
	//=============================================
	obj=new CClassObject;
	if(obj)
	{
		//멤버 오브젝트를 생성한다
		obj->CreateObject();
		//=============================================
		// UI 초기화(윈도우 핸들을 넘긴다)
		//=============================================
		gdi->initDlg(m_hWnd);
		//메인클래스 객체 오브젝트 클래스에 저장
		obj->AddMainGdiDlgObject(gdi);
		//=============================================
		// 오브젝트 초기화 작업
		//=============================================
		obj->initObject();
	}
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
	//=============================================
	//  버튼 컨트롤 생성 제거
	//=============================================
	if(gdi)
	{
		gdi->ControlDestroy();
		delete gdi;	gdi=NULL;
	}
	//=============================================
	//  종료 버튼 제거
	//=============================================
	CloseBut.DestroyWindow();
	//=============================================
	//  Gdiplus 사용 종료
	//=============================================
	CGdiPlusBitmap::GdiShut();
	//=============================================
	//  디버그 클래스 종료
	//=============================================
	if(debug)
	{
		debug->Release();
		delete debug;	debug=NULL;
	}
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
	//오브젝트 클래스 멤버 메모리 해제
	Release();
	//Dlg 종료 함수
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
	if(gdi)
	{
		if(gdi->ButtonClickedMessage(uNotifyCode,nID)==FALSE)
			SetMsgHandled(FALSE);
	}
}