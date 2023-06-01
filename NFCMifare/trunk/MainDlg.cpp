// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "GdiDlg/GdiDlg.h"
#include "OutputDebug/OutputDebug.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainDlg::CMainDlg():
gdi(NULL)
{}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
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
//|초기화 함수
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
	//  Gdiplus 사용 시작
	//=============================================
	CGdiPlusBitmap::GdiStart();
	//=============================================
	//화면 전체 크기를 얻는다
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//SetWindowPos(NULL,0,0,iX , iY,SWP_SHOWWINDOW);
	SetWindowPos(NULL,0,0,1080, 1920,SWP_SHOWWINDOW);
	//=============================================
	// 오브젝트 생성
	//=============================================
	CreateObject();
	//=============================================
	// 종료버튼
	//=============================================
// 	CGdiPlusBitmapResource pBitmap;
// 	pBitmap.Load(IDB_CPPNGX,"PNG");
// 	CBitmap bit = pBitmap.GetWinBitmap();
	CloseBut.Create(m_hWnd, CRect(1080-75, 10, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//이미지(비트맵) 인자로 넘긴다
	//CloseBut.initSuperButton(IDB_CPPNGX);
	CloseBut.initRgnSuperButton(IDB_MAINBTN_X, IDB_MAINBTNMASK_X);

	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
# if DEBUGVIEWER
#ifdef _DEBUG
	debug = new CTraceWinDebug(_T("Baseclass_Debug"));
#else
	debug = new CTraceFileDebug(_T("Baseclass_Debug"));
#endif
#endif //DEBUGVIEWER
	//======================================
	// Main Dlg Gdi 클래스
	//======================================
	gdi = new CMainGdiDlg;
	if(gdi)
	{
		//======================================
		//=============================================
		// UI 초기화(윈도우 핸들을 넘긴다)
		//=============================================
		SetWindowPos(NULL,0,0,1080, 1920,SWP_SHOWWINDOW);
		gdi->initDlg(m_hWnd);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|오브젝트 객체 소멸 함수
//|
//---------------------------------------------------------------
void CMainDlg::Release()
{
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
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
	//  버튼 컨트롤 생성 제거
	//=============================================
	if(gdi)
	{
		gdi->ControlDestroy();
		delete gdi;	gdi=NULL;
	}
	//|=============================================
	//|디버그 출력 창을 제일 나중에 종료한다
	//|==================================================
# if DEBUGVIEWER
	if(debug)
	{
		debug->Release();
		delete debug;	debug=NULL;
	}
#endif //DEBUGVIEWER
	//=============================================
	//  종료 버튼 제거
	//=============================================
	CloseBut.DestroyWindow();
	//=============================================
	//  Gdiplus 사용 종료
	//=============================================
	CGdiPlusBitmap::GdiShut();
	return 0;
}
//--------------------------------------------------------------
//|
//|종료 버튼
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//오브젝트 클래스 멤버 메모리 해제
	Release();
	//Dlg 종료 함수
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|DLG 종료 함수
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
//--------------------------------------------------------------
//|
//|그리기 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 여기서 뭔가를 실제로 그려준다.
	if(gdi)
	{
		gdi->DrawPaint();
	}
	return 0;
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
		SetMsgHandled(FALSE);
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnEraseBkgnd(CDCHandle dc)
{
	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(IDB_MAINBACK_1,"PNG");
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
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnDebugMessageCall(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	switch(uMsg)
	{
	case WM_COMMANDDEBUGSTRING:
		{
			char vc_b[BUFFSIZE_2048]={0,}; 
			byte* data = (byte*)lParam;
			UINT len;
			memcpy(&len, &data[0], 4);
			memcpy(&vc_b[0], &data[4], len);
			delete data;
			debug->Write(vc_b);

		}break;
	default:
		{
		}break;
	}
	return S_OK;
}