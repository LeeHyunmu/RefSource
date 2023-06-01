//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CardDetectDlg.h"

#define THREAD_LOOPTIME 1000

CCardDetectDlg::CCardDetectDlg():
bDestoryFlag(FALSE),
AniSelectNum(0),
hWnd(NULL)
{

}
void CCardDetectDlg::init(HWND _hWnd)
{
	hWnd = _hWnd;
}

LRESULT CCardDetectDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	CBitmapHandle bit;
	_GARes::ImageLoad(GetModuleHandle(NULL),IDB_DLGBACK_SCREENRIGHTCARDREADER_01,_T("PNG"), bit);
	backimage.push_back(bit);
	bit=NULL;
	_GARes::ImageLoad(GetModuleHandle(NULL),IDB_DLGBACK_SCREENRIGHTCARDREADER_01,_T("PNG"), bit);
	backimage.push_back(bit);
	BITMAP bitmap1;
	::GetObject(backimage[0], sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth;
	int imgY = bitmap1.bmHeight;
	SetWindowPos( NULL, 0, 0, imgX, imgY, 0);

	//NEXT
	dlgnext.Create(m_hWnd, CRect(10, 150, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_DLGVIEW_NEXT);
	dlgnext.initRgnSuperButton(IDB_BTN_ZCOM_NEXT, IDB_BTNMASK_01);
	//B NEXT
	close.Create(m_hWnd, CRect(410, 150, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_CLOSE);
	close.initRgnSuperButton(IDB_BTN_POINTCHARGE, IDB_BTNMASK_02);

	//포인트 체크
	pointcheck.Create(m_hWnd, CRect(10, 350, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MAINDLGBUT_POINTCHECK);
	pointcheck.initRgnSuperButton(IDB_BTN_POINTFINDTHIS,IDB_BTNMASK_03);
	//구입
	purchase.Create(m_hWnd, CRect(410, 350, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MAINDLGBUT_PURCHASE);
	purchase.initRgnSuperButton(IDB_BTN_BUYTHIS,IDB_BTNMASK_03);

	//스래드 생성
	AniThread.Create(ThreadProcess, (LPVOID)this);
	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CCardDetectDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
	//

	if(dlgnext.IsWindow())
		dlgnext.DestroyWindow();
	if(close.IsWindow())
		close.DestroyWindow();

	for(int i = 0; i < (int)backimage.size(); i++)
		backimage[i].DeleteObject();

	return 0;
}
LRESULT CCardDetectDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CCardDetectDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CCardDetectDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CCardDetectDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	GetParent().PostMessage(WM_BUTTONLICKMSG, nID);
}
//--------------------------------------------------------------
//|
//|버튼 애니메이션
//|
//---------------------------------------------------------------
void CCardDetectDlg::AnimationFunc()
{
	if(backimage.size()>0)
	{
		++AniSelectNum%=backimage.size();
		InvalidateRect(NULL);
		//InvalidateRect를 강제로 바로 실행 하기 위해존제..
		UpdateWindow();
	}
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CCardDetectDlg::ThreadProcess(LPVOID lpArg)
{
	CCardDetectDlg *Dlg = (CCardDetectDlg *)lpArg;
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
BOOL CCardDetectDlg::OnEraseBkgnd(CDCHandle dc)
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
void CCardDetectDlg::OnActivate(UINT nState, BOOL bMinimized, CWindow wndOther)
{
	
}