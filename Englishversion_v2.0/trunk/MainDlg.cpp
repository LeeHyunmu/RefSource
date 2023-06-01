// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "GdiDlg/GdiDlg.h"
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMainDlg::CMainDlg():
obj(NULL),
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
//|�ʱ�ȭ �Լ�
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
	//  Gdiplus ��� ����
	//=============================================
	CGdiPlusBitmap::GdiStart();
	//=============================================
	//ȭ�� ��ü ũ�⸦ ��´�
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//SetWindowPos(NULL,0,0,iX , iY,SWP_SHOWWINDOW);
	SetWindowPos(NULL,0,0,1080, 1920,SWP_SHOWWINDOW);
	//=============================================
	// ������Ʈ ����
	//=============================================
	CreateObject();
	//=============================================
	// �����ư
	//=============================================
// 	CGdiPlusBitmapResource pBitmap;
// 	pBitmap.Load(IDB_CPPNGX,"PNG");
// 	CBitmap bit = pBitmap.GetWinBitmap();
	CloseBut.Create(m_hWnd, CRect(1080-80, 10, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	//CloseBut.initSuperButton(IDB_CPPNGX);
	CloseBut.initRgnSuperButton(IDB_CPPNGX, IDB_BTNMASK_1);

	//������ �ٲٱ� Ÿ�̸�
	m_Timer.SetTimerOwner(this); 
	if(!m_Timer.SetInterval(500)) 
	{ 
		return TRUE; 
	} 
	m_Timer.SetCount(-1); 
	m_Timer.StartTimer(); 
	return TRUE;
}
//--------------------------------------------------------------
//|
//|��ü ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
#ifdef _DEBUG
	debug = new CTraceWinDebug(_T("Baseclass_Debug"));
#else
	debug = new CTraceFileDebug(_T("Baseclass_Debug"));
#endif
	//======================================
	// Main Dlg Gdi Ŭ����
	//======================================
	gdi = new CMainGdiDlg;
	//======================================
	//=============================================
	// ������Ʈ ���� Ŭ���� ����
	//=============================================
	obj=new CClassObject;
	if(obj)
	{
		//��� ������Ʈ�� �����Ѵ�
		obj->CreateObject();
		//=============================================
		// UI �ʱ�ȭ(������ �ڵ��� �ѱ��)
		//=============================================
		SetWindowPos(NULL,0,0,1080, 1920,SWP_SHOWWINDOW);
		gdi->initDlg(m_hWnd);
		//����Ŭ���� ��ü ������Ʈ Ŭ������ ����
		obj->AddMainGdiDlgObject(gdi);
		//=============================================
		// ������Ʈ �ʱ�ȭ �۾�
		//=============================================
		obj->initObject();
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü �Ҹ� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::Release()
{
	m_Timer.StopTimer();
	if(obj)
	{
		obj->KillObject();
		delete obj;obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|��ü �Ҹ� �Լ�
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
	//  ��ư ��Ʈ�� ���� ����
	//=============================================
	if(gdi)
	{
		gdi->ControlDestroy();
		delete ((CMainGdiDlg*)gdi);	gdi=NULL;
	}
	//|=============================================
	//|����� ��� â�� ���� ���߿� �����Ѵ�
	//|==================================================
	if(debug)
	{
		debug->Release();
#ifdef _DEBUG
		delete ((CTraceWinDebug*)debug);	debug=NULL;
#else
		delete ((CTraceFileDebug*)debug);	debug=NULL;
#endif
	}
	//=============================================
	//  ���� ��ư ����
	//=============================================
	CloseBut.DestroyWindow();
	//=============================================
	//  Gdiplus ��� ����
	//=============================================
	CGdiPlusBitmap::GdiShut();
	return 0;
}
//--------------------------------------------------------------
//|
//|���� ��ư
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	//������Ʈ Ŭ���� ��� �޸� ����
	Release();
	//Dlg ���� �Լ�
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|DLG ���� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
	//KillTimer(MAINCLOSE_DELAYTIME);
}
//--------------------------------------------------------------
//|
//|�׸��� �Լ�
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// ���⼭ ������ ������ �׷��ش�.
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
		if(gdi->ButtonClickedMessage(uNotifyCode,nID)==FALSE)
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
// 	RECT rc;
// 	GetClientRect(&rc);
// 	if(rc.top+50>point.y)
// 		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKEWPARAM(point.x, point.y));
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnMMTimer(UINT uID)
{
 	PostMessage(WM_USERTIMER);
// 	if(gdi)
// 	{
// 		DWORD flag;
// 		gdi->CardFindConnectLoop(&flag);
// 	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnUserTimer(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(gdi)
	{
		gdi->MediaPlayTimer(MAINCLOSE_DELAYTIME);
	}
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnNfcEventCall(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(gdi)
	{
		gdi->NfcEventCall(wParam);
	}
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnDeviceCmdCallMessage(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(gdi)
	{
		gdi->DeviceCmdCallMessage(wParam);
	}
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnNfcAqDeviceMessage(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(gdi)
	{
		gdi->NfcAqDeviceMessage(wParam);
	}
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnCouponClickMessage(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if(gdi)
	{
		gdi->CouponClickMessage(wParam);
	}
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnEraseBkgnd(CDCHandle dc)
{
	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(IDB_MAINBACKGRAND1,"PNG");
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