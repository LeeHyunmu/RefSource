
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------
LRESULT CSuperEdit::OnSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
	static HCURSOR hcur = LoadCursor (_Module.GetResourceInstance(), MAKEINTRESOURCE(IDC_CURSOR1));
	if ( NULL != hcur )
	{
		SetCursor ( hcur );
		return TRUE;
	}
	else
	{
		SetMsgHandled(false);
		return FALSE;
	}
}

BOOL CSuperEdit::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperEdit, CEdit>::SubclassWindow(hWnd);
	return bRet;
}
void CSuperEdit::init()
{
	m_bTracking=FALSE;m_bHover=FALSE;
}

void CSuperEdit::OnMouseHover(WPARAM wParam, CPoint ptPos)
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}

void CSuperEdit::OnMouseLeave()
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}
void CSuperEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
}
BOOL CSuperEdit::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

HWND CSuperEdit::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperEdit, CEdit>::UnsubclassWindow(bForce);
}
int CSuperEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	return 0;
}
BOOL CSuperEdit::OnEraseBkgnd(CDCHandle dc)
{
	return TRUE;
}
LRESULT CSuperEdit::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	return 0;
}
void CSuperEdit::PreSubclassWindow(HWND hWnd)
{
	
}