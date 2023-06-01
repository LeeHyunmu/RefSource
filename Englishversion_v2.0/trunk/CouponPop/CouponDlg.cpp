//  : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CouponDlg.h"
CCouponDlg::CCouponDlg():
aniFlag(0),
aniTime(0),
bDestoryFlag(FALSE),
moveindex(0)
{

}
void CCouponDlg::init(HWND _hWnd)
{
	hWnd = _hWnd;
}

LRESULT CCouponDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);

	NfcThread.Create(ThreadProcess, (LPVOID)this);
	RespEvent.Create();

	CGdiPlusBitmapResource pBitmap1;
	pBitmap1.Load(IDB_CPPNG01,"PNG");
	CBitmap bit1 = pBitmap1.GetWinBitmap();
	BITMAP bitmap;
	::GetObject(bit1, sizeof(BITMAP), &bitmap); 
	int imgX = bitmap.bmWidth/3;
	int imgY = bitmap.bmHeight;

	couponbtn1.Create(m_hWnd, CRect(0, 0, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_1);
	couponbtn1.initSuperButton(HBITMAP(bit1));

	couponbtn2.Create(m_hWnd, CRect(imgX, 0, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_2);
	couponbtn2.initSuperButton(IDB_CPPNG02);

	couponbtn3.Create(m_hWnd, CRect(0, imgY, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_3);
	couponbtn3.initSuperButton(IDB_CPPNG03);

	couponbtn4.Create(m_hWnd, CRect(imgX, imgY, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_4);
	couponbtn4.initSuperButton(IDB_CPPNG04);

	couponbtn5.Create(m_hWnd, CRect(0, imgY*2, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_5);
	couponbtn5.initSuperButton(IDB_CPPNG05);

	couponbtn6.Create(m_hWnd, CRect(imgX, imgY*2, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_6);
	couponbtn6.initSuperButton(IDB_CPPNG06);


	couponbtn7.Create(m_hWnd, CRect(0, imgY*3, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_7);
	couponbtn7.initSuperButton(IDB_CPPNG07);

	couponbtn8.Create(m_hWnd, CRect(imgX, imgY*3, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_COUPONBUT_8);
	couponbtn8.initSuperButton(IDB_CPPNG08);

	move1.push_back(&couponbtn1);
	move1.push_back(&couponbtn3);
	move1.push_back(&couponbtn5);
	move1.push_back(&couponbtn7);

	move2.push_back(&couponbtn2);
	move2.push_back(&couponbtn4);
	move2.push_back(&couponbtn6);
	move2.push_back(&couponbtn8);
	//투명도 설정
	//SetWindowLong( GWL_EXSTYLE, GetWindowLong( GWL_EXSTYLE) | WS_EX_LAYERED);
	SetWindowPos(hWnd, CRect(0, (826-200), imgX*2, (826-200)+(imgY*2)), 0);
	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CCouponDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);

	bDestoryFlag=TRUE;
	couponbtn1.DestroyWindow();
	couponbtn2.DestroyWindow();
	couponbtn3.DestroyWindow();
	couponbtn4.DestroyWindow();
	couponbtn5.DestroyWindow();
	couponbtn6.DestroyWindow();
	couponbtn7.DestroyWindow();
	couponbtn8.DestroyWindow();

	return 0;
}
LRESULT CCouponDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DestroyWindow();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CCouponDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CCouponDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CCouponDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	GetParent().PostMessage(WM_COUPONCLICKMSG, nID);
}
//--------------------------------------------------------------
//|
//|버튼 애니메이션
//|
//---------------------------------------------------------------
void CCouponDlg::buttonAnimation()
{
	if(++aniTime>60)
	{
		static int  xMove = 0;
		static int  yMove = 0;
		yMove+=18;
		BOOL anistop;
		HDWP hdwp = BeginDeferWindowPos(8);
		for(int i = 0; i < 4; i++)
		{
			anistop =  (anistop+move1[(i+moveindex)%4]->MoveYAnimation(hdwp, yMove))&0x01;
			anistop =  (anistop+move2[(i+moveindex)%4]->MoveYAnimation(hdwp, yMove))&0x01;
		}
		EndDeferWindowPos(hdwp);
		if(anistop)
		{
			yMove=0; aniTime=0;
			HDWP hdwp1 = BeginDeferWindowPos(8);
			for(int i = 0; i < 4; i++)
			{
				move1[(i+moveindex)%4]->SetChangeYPos(hdwp1);
				move2[(i+moveindex)%4]->SetChangeYPos(hdwp1);
			}
			EndDeferWindowPos(hdwp1);
			++moveindex%=4;
		}

	}
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CCouponDlg::ThreadProcess(LPVOID lpArg)
{
	CCouponDlg *CouponDlg = (CCouponDlg *)lpArg;
	while (CouponDlg->NfcThread.IsRunning()  && CouponDlg->NfcThread.WaitForThread(COUPONDLGTHREAD_LOOPTIME)==FALSE)
	{
		CouponDlg->buttonAnimation();
	}
	return 0;
}