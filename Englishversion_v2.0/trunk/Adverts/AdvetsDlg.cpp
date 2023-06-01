//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdvertsDlg.h"

#define CONTENTSMALLTHREAD_LOOPTIME 200
CAdvertsDlg::CAdvertsDlg():
aniFlag(0),
aniTime(0),
bDestoryFlag(FALSE),
moveindex(0)
{

}
void CAdvertsDlg::init(HWND _hWnd)
{
	hWnd = _hWnd;
}
LRESULT CAdvertsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	//�ִϸ��̼� ������
	NfcThread.Create(ThreadProcess, (LPVOID)this);
	RespEvent.Create();

	CGdiPlusBitmapResource pBitmap1;
	pBitmap1.Load(IDB_ADVERTS01,"PNG");
	CBitmap bit1 = pBitmap1.GetWinBitmap();
	BITMAP bitmap1;
	::GetObject(bit1, sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth;
	int imgY = bitmap1.bmHeight;

	advertsimg1.Create(m_hWnd, CRect(0, 0, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	advertsimg1.initSuperStatic(HBITMAP(bit1), 2);

	CRect parentRect;
	::GetWindowRect(hWnd, &parentRect);
	SetWindowPos(hWnd, CRect(parentRect.left, parentRect.bottom-200, parentRect.left+imgX, parentRect.bottom), 0);
	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|��ü �Ҹ� �Լ�
//|
//---------------------------------------------------------------
LRESULT CAdvertsDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);

	bDestoryFlag=TRUE;
	advertsimg1.DestroyWindow();
	return 0;
}
LRESULT CAdvertsDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAdvertsDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CAdvertsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
}
//--------------------------------------------------------------
//|
//|��ư �ִϸ��̼�
//|
//---------------------------------------------------------------
void CAdvertsDlg::ImageAnimation()
{
	if(++aniTime>10)
	{
		BOOL anistop=0;
		anistop = advertsimg1.SetAnimation();
		if(anistop)
		{
			aniTime=0;
		}

	}
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CAdvertsDlg::ThreadProcess(LPVOID lpArg)
{
	CAdvertsDlg *MallDlg = (CAdvertsDlg *)lpArg;
	while (MallDlg->NfcThread.IsRunning()  && MallDlg->NfcThread.WaitForThread(CONTENTSMALLTHREAD_LOOPTIME)==FALSE)
	{
		MallDlg->ImageAnimation();
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAdvertsDlg::OnEraseBkgnd(CDCHandle dc)
{
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
	return TRUE;
}