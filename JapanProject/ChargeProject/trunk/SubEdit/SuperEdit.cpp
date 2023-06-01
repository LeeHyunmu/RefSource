
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSuperEdit::CSuperEdit(void):
uCursorID(0)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSuperEdit::~CSuperEdit(void)
{
}
//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------
LRESULT CSuperEdit::OnSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg)
{
	static HCURSOR hcur = LoadCursor (_Module.GetResourceInstance(), MAKEINTRESOURCE(IDC_FINGER));
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
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperEdit::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperEdit, CEdit>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperEdit::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HWND CSuperEdit::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperEdit, CEdit>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
int CSuperEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CSuperEdit::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CSuperEdit::PreSubclassWindow(HWND hWnd)
{
	
}