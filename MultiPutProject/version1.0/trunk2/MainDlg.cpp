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
	if(pMsg->message==WM_KEYDOWN||pMsg->message==WM_KEYUP)
	{
		if(gdi->VirtualKeyMessage(pMsg->message, (UINT)pMsg->wParam,(UINT) pMsg->lParam & 0xFFFF)==FALSE)
		{
			return TRUE;
		}
	}
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
	//화면 전체 크기를 얻는다
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,iX-100, iY-50,SWP_SHOWWINDOW);
	//=============================================
	// 오브젝트 생성
	//=============================================
	CreateObject();
	//=============================================
	// 종료버튼
	//=============================================
	CRect rc;
	GetClientRect(&rc);
	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(IDB_PNG2,"PNG");
	CBitmap bit = pBitmap.GetWinBitmap();
	CloseBut.Create(m_hWnd, CRect(rc.Width()-150, 20, 0, 0),_T("종료"), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//이미지(비트맵) 인자로 넘긴다
	CloseBut.initSuperButton(bit);
	//특정 색을 뺀다
	CloseBut.SetTransperent(RGB(100,100,100));

	gdi->ServerStartCall();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
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
		gdi->MainGdiDlgDrawPaint();
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|배경 그려주는 함수(투명)
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnEraseBkgnd(CDCHandle dc)
{
	return TRUE;
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
//--------------------------------------------------------------
//|
//|오너드로우가 아닌 static 클래스 배경 투명
//|
//---------------------------------------------------------------
HBRUSH CMainDlg::OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
{
	HBRUSH hbrush;
	if(gdi)
	{
		if(gdi->CtlColorStatic(dc, wndStatic, hbrush)==FALSE)
			SetMsgHandled(FALSE);
	}
	return hbrush;
}
//--------------------------------------------------------------
//|
//|오른쪽 버튼 클릭
//|
//---------------------------------------------------------------
void CMainDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(gdi)
	{
		if(gdi->MouseEventMessage(WM_RBUTTONDOWN, point)==FALSE)
			SetMsgHandled(FALSE);
	}
}
//--------------------------------------------------------------
//|
//|마우스 MOVE
//|
//---------------------------------------------------------------
void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(gdi)
	{
		if(gdi->MouseEventMessage(WM_MOUSEMOVE, point)==FALSE)
			SetMsgHandled(FALSE);
	}
}
//--------------------------------------------------------------
//|
//|캡쳐 체인지
//|
//---------------------------------------------------------------
void CMainDlg::OnCaptureChanged(CWindow wnd)
{
}
//--------------------------------------------------------------
//|
//|Hot Key Message
//|
//---------------------------------------------------------------
void CMainDlg::OnHotKey(int nHotKeyID, UINT uModifiers, UINT uVirtKey)
{
	if(gdi)
	{
		if(gdi->HotKeyMessage(nHotKeyID)==FALSE)
			SetMsgHandled(FALSE);
	}
}
//--------------------------------------------------------------
//|
//|Hot Key Message
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
	if(gdi)
	{
		if(gdi->MouseCursorMessage(wnd, nHitTest, message)==FALSE)
		{
			SetMsgHandled(FALSE); return FALSE;
		}
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|키보드 메시지(DOWN)
//|
//---------------------------------------------------------------
void CMainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(gdi)
	{
		if(gdi->VirtualKeyMessage(WM_KEYDOWN, nChar, nRepCnt)==FALSE)
		{
			SetMsgHandled(FALSE);
		}
	}
}
//--------------------------------------------------------------
//|
//|키보드 메시지(UP)
//|
//---------------------------------------------------------------
void CMainDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(gdi)
	{
		if(gdi->VirtualKeyMessage(WM_KEYUP, nChar, nRepCnt)==FALSE)
		{
			SetMsgHandled(FALSE);
		}
	}
}