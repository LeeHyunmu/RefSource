//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ContentsMallPopDlg.h"

void CContentsMallPopDlg::init(UINT id)
{
	//
	bClose=FALSE;
	imgid=id;
}
void CContentsMallPopDlg::DlgClose()
{
	if(bClose==FALSE)
		PostMessage(WM_CLOSE);
	bClose=TRUE;
}
LRESULT CContentsMallPopDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{

	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	// 		CSize size = m_image->GetSize();
	// 
	// 		int Xcenter = 0;
	// 		int Ycenter = 826;
	//  		couponpopimg.Create(this->m_hWnd, CRect(0, 0, size.cx, size.cy),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	// 		m_image->InitAnimation(couponpopimg.m_hWnd, CPoint(0,0));
	// 		SetWindowPos(NULL, CRect(Xcenter, Ycenter, Xcenter+size.cx, Ycenter+size.cy), 0);

	int Xcenter = 0;
	int Ycenter = 826-200;

	CGdiPlusBitmapResource pBitmap1;
	pBitmap1.Load(imgid,"PNG");
	CBitmap bit1 = pBitmap1.GetWinBitmap();
	BITMAP bitmap1;
	::GetObject(bit1, sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth;
	int imgY = bitmap1.bmHeight;

	SetWindowPos(NULL, CRect(Xcenter, Ycenter, Xcenter+imgX, 1920), 0);

	img1.Create(m_hWnd, CRect(0, 0, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	img1.initSuperStatic(HBITMAP(bit1));

	img2.Create(m_hWnd, CRect(0, imgY, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);

	if (img2.m_hWnd)
	{
		m_wndBanner.SubclassWindow(img2.m_hWnd);
		if (m_wndBanner.Load(MAKEINTRESOURCE(IDR_GIF101),_T("GIF")))
			m_wndBanner.Draw();
	};

	//종료 타이머
	m_Timer.SetTimerOwner(this); 
	if(!m_Timer.SetInterval(8000)) 
	{ 
		return TRUE; 
	} 
	m_Timer.SetCount(1); 
	m_Timer.StartTimer(); 
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CContentsMallPopDlg::OnMMTimer(UINT uID)
{
	if(bClose==FALSE)
		PostMessage(WM_CLOSE);
	bClose=TRUE;
}
LRESULT CContentsMallPopDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Timer.StopTimer();
	EndDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CContentsMallPopDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CContentsMallPopDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
// 	RECT rc;
// 	GetClientRect(&rc);
// 	if(rc.top+100>point.y)
// 		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(point.x, point.y));
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CContentsMallPopDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	switch(nID)
	{
	case IDC_COUPONBUT_X:
		{
			PostMessage(WM_CLOSE);
		}
	}
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CContentsMallPopDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	// unregister message filtering and idle updates
	m_wndBanner.UnLoad();
	m_wndBanner.UnsubclassWindow();
	img1.DestroyWindow();
	img2.DestroyWindow();
	return 0;
}