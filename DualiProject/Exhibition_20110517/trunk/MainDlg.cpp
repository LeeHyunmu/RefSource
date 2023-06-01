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
	//=============================================
	// ������ ��ü ȭ��
	//=============================================
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,0,0,iX, iY,SWP_SHOWWINDOW);
	//=============================================
	// �����ư
	//=============================================
	//CloseBut.Create(m_hWnd, CRect(iX-120, 20, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	////�̹���(��Ʈ��) ���ڷ� �ѱ��
	//CloseBut.initSuperButton(IDB_PNG1,_T("PNG"));

	//======================================
	// Main Dlg Gdi Ŭ����
	//======================================
	gdi = new CMainGdiDlg;
	//======================================
	//=============================================
	// ������Ʈ ����
	//=============================================
	CreateObject();
	//======================================
	// Main Dlg Gdi Ŭ���� �ʱ�ȭ
	//======================================
	if(gdi)
	{
		gdi->initInterfaceObject(obj);
	}
	//=============================================
	// UI �ʱ�ȭ(������ �ڵ��� �ѱ��)
	//=============================================
	if(gdi)
		gdi->initDlg(m_hWnd);
	//=============================================
	// ������Ʈ �ʱ�ȭ
	//=============================================
	initObject();
#ifndef _DEBUG
	CWindow pTaskBar = ::FindWindow(_T("HHTaskBar"), _T(""));
	if(pTaskBar && ::IsWindow(pTaskBar))
	{
		pTaskBar.ShowWindow(SW_HIDE);
	}
#endif //_DEBUG
	return TRUE;
}
//--------------------------------------------------------------
//|
//|�����Լ�
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	Release();
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|��ü ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
	//=============================================
	// ������Ʈ ���� Ŭ���� ����
	//=============================================
	obj=new CClassObject;
	if(obj)
		obj->CreateObject(gdi);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::initObject() 
{
	//=============================================
	// ������Ʈ ���� Ŭ���� ����
	if(obj)
		obj->initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü �Ҹ� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::Release()
{
	if(obj)
	{
		obj->KillObject();
		delete ((CClassObject*)obj);	obj=NULL;
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
	if(gdi)
	{
		gdi->ControlDestroy();
		gdi->Release();
		delete ((CMainGdiDlg*)gdi);	gdi=NULL;
	}
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	CloseBut.DestroyWindow();
#ifndef _DEBUG
	// Hide the task bar
	CWindow pTaskBar = ::FindWindow(_T("HHTaskBar"), _T(""));
	if(pTaskBar && ::IsWindow(pTaskBar))
	{
		pTaskBar.ShowWindow(SW_SHOW);
	}
#endif //_DEBUG
	return 0;
}
//--------------------------------------------------------------
//|
//| �׸��� �Լ�
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
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
	BOOL b=FALSE;
	if(gdi)
		b = gdi->ButtonClickedMessage(uNotifyCode,nID);
	if(!b) SetMsgHandled(FALSE);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnTimer(UINT_PTR uTimerID)
{
}