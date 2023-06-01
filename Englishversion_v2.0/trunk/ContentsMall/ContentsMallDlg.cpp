//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContentsMall.h"

#define CONTENTSMALLTHREAD_LOOPTIME 200
CContentsMallDlg::CContentsMallDlg():
aniFlag(0),
aniTime(0),
bDestoryFlag(FALSE),
moveindex(0), 
imgnum(0)
{

}
void CContentsMallDlg::init(HWND _hWnd)
{
	hWnd = _hWnd;
}
int CContentsMallDlg::imgId[16]={IDB_CONTENTSBACKGROUND01, IDB_CONTENTSBACKGROUND02, IDB_CONTENTSBACKGROUND03, IDB_CONTENTSBACKGROUND04,
								IDB_CONTENTSBACKGROUND05, IDB_CONTENTSBACKGROUND06, IDB_CONTENTSBACKGROUND07, IDB_CONTENTSBACKGROUND08,
								IDB_CONTENTSBACKGROUND09, IDB_CONTENTSBACKGROUND10, IDB_CONTENTSBACKGROUND11, IDB_CONTENTSBACKGROUND12,
								IDB_CONTENTSBACKGROUND13, IDB_CONTENTSBACKGROUND14, IDB_CONTENTSBACKGROUND15, IDB_CONTENTSBACKGROUND16};
LRESULT CContentsMallDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	//애니메이션 스래드
	NfcThread.Create(ThreadProcess, (LPVOID)this);
	RespEvent.Create();

	CGdiPlusBitmapResource pBitmap1;
	pBitmap1.Load(IDB_CPPNG01,"PNG");
	CBitmap bit1 = pBitmap1.GetWinBitmap();
	BITMAP bitmap1;
	::GetObject(bit1, sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth/3;
	int imgY = bitmap1.bmHeight;

	contentsbtn1.Create(m_hWnd, CRect(32, 28, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_CONTENTSBUT_1);
	contentsbtn1.initRgnSuperButton(IDB_CONTENTSBTN1, IDB_BTNMASK_2);

	contentsbtn2.Create(m_hWnd, CRect(422, 28, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_CONTENTSBUT_2);
	contentsbtn2.initRgnSuperButton(IDB_CONTENTSBTN2, IDB_BTNMASK_2);

	contentsbtn3.Create(m_hWnd, CRect(231, 335, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_CONTENTSBUT_3);
	contentsbtn3.initSuperButton(IDB_CONTENTSBTN3);

	contentsbtn4.Create(m_hWnd, CRect(583, 335, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_CONTENTSBUT_4);
	contentsbtn4.initSuperButton(IDB_CONTENTSBTN4);

	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(imgId[0],"PNG");
	CBitmap backimage = pBitmap.GetWinBitmap();
	BITMAP bitmap;
	::GetObject(backimage, sizeof(BITMAP), &bitmap); 
	int imgBackX = bitmap.bmWidth;
	int imgBackY = bitmap.bmHeight;

	CRect parentRect;
	::GetWindowRect(hWnd, &parentRect);
	SetWindowPos(hWnd, CRect(parentRect.left, (826+530-200), parentRect.left+imgBackX, (826+530-200)+imgBackY), 0);
	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CContentsMallDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);

	bDestoryFlag=TRUE;
	contentsbtn1.DestroyWindow();
	contentsbtn2.DestroyWindow();
	contentsbtn3.DestroyWindow();
	contentsbtn4.DestroyWindow();
	return 0;
}
LRESULT CContentsMallDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CContentsMallDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CContentsMallDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CContentsMallDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	GetParent().PostMessage(WM_COUPONCLICKMSG, nID);
}
//--------------------------------------------------------------
//|
//|버튼 애니메이션
//|
//---------------------------------------------------------------
void CContentsMallDlg::buttonAnimation()
{
	++imgnum%=16;
	InvalidateRect(NULL);
	//InvalidateRect를 강제로 바로 실행 하기 위해존제..
	UpdateWindow();
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CContentsMallDlg::ThreadProcess(LPVOID lpArg)
{
	CContentsMallDlg *MallDlg = (CContentsMallDlg *)lpArg;
	while (MallDlg->NfcThread.IsRunning()  && MallDlg->NfcThread.WaitForThread(CONTENTSMALLTHREAD_LOOPTIME)==FALSE)
	{
		MallDlg->buttonAnimation();
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CContentsMallDlg::OnEraseBkgnd(CDCHandle dc)
{
	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(imgId[imgnum],"PNG");
	CBitmap backimage = pBitmap.GetWinBitmap();

	// create memory DC
	dc.SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(dc.m_hDC);
	CBitmapHandle pOldBitmap;

	SIZE bmSize;
	backimage.GetSize(bmSize);
	pOldBitmap = pMemDC -> SelectBitmap(backimage);
	dc.BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;
	return FALSE;
}