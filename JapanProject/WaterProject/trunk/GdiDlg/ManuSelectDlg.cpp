//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ManuSelectDlg.h"

#define THREAD_LOOPTIME 1000

CManuSelectDlg::CManuSelectDlg():
bDestoryFlag(FALSE),
AniSelectNum(0),
hWnd(NULL)
{

}
void CManuSelectDlg::init(HWND _hWnd)
{
	hWnd = _hWnd;
}

LRESULT CManuSelectDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	CBitmapHandle bit;
	_GARes::ImageLoad(GetModuleHandle(NULL),IDB_DLGBACK_BUTTONTOUCH_01,_T("PNG"), bit);
	backimage.push_back(bit);
	bit=NULL;
	_GARes::ImageLoad(GetModuleHandle(NULL),IDB_DLGBACK_BUTTONTOUCH_02,_T("PNG"), bit);
	backimage.push_back(bit);
	BITMAP bitmap1;
	::GetObject(backimage[0], sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth;
	int imgY = bitmap1.bmHeight;
	SetWindowPos(NULL, 0, 0, imgX, imgX, 0);

	//charge
	charge.Create(m_hWnd, CRect(39, 135, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_CLOSE);
	charge.initRgnSuperButton(IDB_BTN_POINTCHARGE, IDB_BTNMASK_02);

	//����Ʈ üũ
	pointcheck.Create(m_hWnd, CRect(40, 368, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MAINDLGBUT_POINTCHECK);
	pointcheck.initRgnSuperButton(IDB_BTN_POINTFINDTHIS,IDB_BTNMASK_03);
	//����
	purchase.Create(m_hWnd, CRect(419, 368, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MAINDLGBUT_PURCHASE);
	purchase.initRgnSuperButton(IDB_BTN_BUYTHIS,IDB_BTNMASK_03);

	waterstate.Create(m_hWnd, CRect(453, 135, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDS_STATIC);
	waterstate.initSuperStatic(IDB_STA_WATERSUPPLYMAKING, _T("PNG"), 2, TRUE);

	//������ ����
	AniThread.Create(ThreadProcess, (LPVOID)this);
	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|��ü �Ҹ� �Լ�
//|
//---------------------------------------------------------------
LRESULT CManuSelectDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	if(AniThread.IsRunning())
	{
		AniThread.Release();
	}
	bDestoryFlag=TRUE;
	if(pointcheck.IsWindow())
		pointcheck.DestroyWindow();
	if(purchase.IsWindow())
		purchase.DestroyWindow();

	if(charge.IsWindow())
		charge.DestroyWindow();

	//
	for(int i = 0; i < (int)backimage.size(); i++)
		backimage[i].DeleteObject();
	return 0;
}
LRESULT CManuSelectDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CManuSelectDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CManuSelectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CManuSelectDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	GetParent().PostMessage(WM_BUTTONLICKMSG, nID);
}
//--------------------------------------------------------------
//|
//|��ư �ִϸ��̼�
//|
//---------------------------------------------------------------
void CManuSelectDlg::AnimationFunc()
{
	if(backimage.size()>0)
	{
		++AniSelectNum%=backimage.size();
		InvalidateRect(NULL);
		//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
		UpdateWindow();
	}
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CManuSelectDlg::ThreadProcess(LPVOID lpArg)
{
	CManuSelectDlg *Dlg = (CManuSelectDlg *)lpArg;
	while (Dlg->AniThread.IsRunning()  && Dlg->AniThread.WaitForThread(THREAD_LOOPTIME)==FALSE)
	{
		Dlg->AnimationFunc();
	}
	Dlg->AniThread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CManuSelectDlg::OnEraseBkgnd(CDCHandle dc)
{
	// create memory DC
	if( backimage.size())
	{
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(dc.m_hDC);
		CBitmapHandle pOldBitmap;
		pMemDC -> SetBkColor(RGB(255,0,0));
		SIZE bmSize;
		backimage[AniSelectNum].GetSize(bmSize);
		pOldBitmap = pMemDC -> SelectBitmap(backimage[AniSelectNum]);
		dc.BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
		pMemDC -> SelectBitmap(pOldBitmap);
		delete pMemDC;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CManuSelectDlg::OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
{

}