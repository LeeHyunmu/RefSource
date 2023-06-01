//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CompleteDlg.h"

CCompleteDlg::CCompleteDlg():
bClose(TRUE)
{

}
void CCompleteDlg::init(UINT id)
{
	//
	bClose=FALSE;
	imgid=id;
}
void CCompleteDlg::DlgClose()
{
	if(bClose==FALSE)
		PostMessage(WM_CLOSE);
	bClose=TRUE;
}
LRESULT CCompleteDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	CGdiPlusBitmapResource pBitmapx;
	pBitmapx.Load(imgid,"PNG");
	CBitmap bitx = pBitmapx.GetWinBitmap();
	BITMAP bitmapx;
	::GetObject(bitx, sizeof(BITMAP), &bitmapx); 
	int imghalfX = bitmapx.bmWidth/2;
	int imghalfY = bitmapx.bmHeight/2;
	// 
 	m_image.Create(m_hWnd, CRect(0, 0, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COMPLET);
 	m_image.initSuperStatic(HBITMAP(bitx));

	_GARgn::ImageLoadRgn(_Module.GetResourceInstance(),IDB_DLGMASK_1 ,"PNG",m_hRgn);
	SetWindowRgn(m_hRgn, false);

	int XCenter=(1080/2)-imghalfX;
	int YCenter=(826+(530/2))-imghalfY;
	SetWindowPos(NULL, CRect(XCenter, YCenter, XCenter+bitmapx.bmWidth, YCenter+bitmapx.bmHeight), 0);

	//종료 타이머
	m_Timer.SetTimerOwner(this); 
	if(!m_Timer.SetInterval(3000)) 
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
void CCompleteDlg::OnMMTimer(UINT uID)
{
	if(bClose==FALSE)
	{
		PostMessage(WM_CLOSE);
		bClose=TRUE;
	}
}
LRESULT CCompleteDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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
BOOL CCompleteDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CCompleteDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
// 	RECT rc;
// 	GetClientRect(&rc);
// 	if(rc.top+100>point.y)
// 		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(point.x, point.y));
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CCompleteDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	// unregister message filtering and idle updates
	m_image.DestroyWindow();
	return 0;
}
BOOL CCompleteDlg::GetClose()
{
	return bClose;
}