// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "GdiDlg/GdiDlg.h"
//--------------------------------------------------------------
//|
//|������
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
//|�ʱ�ȭ �Լ�
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
	//  Gdiplus ��� ����
	//=============================================
	CGdiPlusBitmap::GdiStart();
	//=============================================
	//ȭ�� ��ü ũ�⸦ ��´�
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,iX-100, iY-50,SWP_SHOWWINDOW);
	//=============================================
	// ������Ʈ ����
	//=============================================
	CreateObject();
	//=============================================
	// �����ư
	//=============================================
	CRect rc;
	GetClientRect(&rc);
	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(IDB_PNG2,"PNG");
	CBitmap bit = pBitmap.GetWinBitmap();
	CloseBut.Create(m_hWnd, CRect(rc.Width()-150, 20, 0, 0),_T("����"), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	CloseBut.initSuperButton(bit);
	//Ư�� ���� ����
	CloseBut.SetTransperent(RGB(100,100,100));

	gdi->ServerStartCall();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|��ü ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
	//======================================
	// Main Dlg Gdi Ŭ����
	//======================================
	gdi = new CMainGdiDlg;
	//======================================
	//=============================================
	// ������Ʈ ���� Ŭ���� ����
	//=============================================
	obj=new CClassObject;
	if(obj)
	{
		//��� ������Ʈ�� �����Ѵ�
		obj->CreateObject();
		//=============================================
		// UI �ʱ�ȭ(������ �ڵ��� �ѱ��)
		//=============================================
		gdi->initDlg(m_hWnd);
		//����Ŭ���� ��ü ������Ʈ Ŭ������ ����
		obj->AddMainGdiDlgObject(gdi);
		//=============================================
		// ������Ʈ �ʱ�ȭ �۾�
		//=============================================
		obj->initObject();
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü �Ҹ� �Լ�
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
//|��ü �Ҹ� �Լ�
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
	//  ��ư ��Ʈ�� ���� ����
	//=============================================
	if(gdi)
	{
		gdi->ControlDestroy();
		delete gdi;	gdi=NULL;
	}
	//=============================================
	//  ���� ��ư ����
	//=============================================
	CloseBut.DestroyWindow();
	//=============================================
	//  Gdiplus ��� ����
	//=============================================
	CGdiPlusBitmap::GdiShut();
	return 0;
}
//--------------------------------------------------------------
//|
//|���� ��ư
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//������Ʈ Ŭ���� ��� �޸� ����
	Release();
	//Dlg ���� �Լ�
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|DLG ���� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
//--------------------------------------------------------------
//|
//|�׸��� �Լ�
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// ���⼭ ������ ������ �׷��ش�.
	if(gdi)
	{
		gdi->MainGdiDlgDrawPaint();
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|��� �׷��ִ� �Լ�(����)
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
//|���ʵ�ο찡 �ƴ� static Ŭ���� ��� ����
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
//|������ ��ư Ŭ��
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
//|���콺 MOVE
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
//|ĸ�� ü����
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
//|Ű���� �޽���(DOWN)
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
//|Ű���� �޽���(UP)
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