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
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//�����ư
	CloseBut.Create(m_hWnd, CRect(iX-100, 20, 0, 0),_T("�׽�Ʈ��ư"), WS_CHILD | WS_VISIBLE , 0, IDOK);
	CloseBut.initSuperButton(IDB_BUT_1);
	//=============================================
	// ������Ʈ ����
	//=============================================
	CreateObject();
	//======================================
	// Main Dlg Gdi Ŭ����
	//======================================
	gdi = new CMainGdiDlg;
	//======================================
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
	//=============================================
	// ������ ��ü ȭ��
	//=============================================
	SetWindowPos(NULL,0,0,iX, iY,SWP_SHOWWINDOW);
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
		obj->CreateObject();
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
	return 0;
}