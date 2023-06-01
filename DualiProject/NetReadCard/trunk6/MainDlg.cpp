// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
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
//|�޽��� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}
//--------------------------------------------------------------
//|
//|���̾�α� �ʱ�ȭ
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
	// ������Ʈ ���� Ŭ����
	//=============================================
	CreateObject();
	//=============================================
	// gdi ����
	//=============================================
	gdi = obj->GetMainGdiObject();
	//=============================================
	// UI �ʱ�ȭ
	//=============================================
	gdi->initDlg(m_hWnd);
	return TRUE;
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
//|��ü �Ҹ� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::Release()
{
	if(obj)
	{
		obj->KillObject();
		delete obj;obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	Release();
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//| ���� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedDualicardconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->CardConnent("USB", TRUE);
	GetDlgItem(IDC_DUALICARDCONNECT).EnableWindow(FALSE);
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}
LRESULT CMainDlg::OnBnClickedDebugpane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IOutput* debug = obj->GetOutputObject();
	if(debug)
	{
		if(debug->GetVisible())
		{
			debug->SetVisible(FALSE);
		}
		else
		{
			debug->SetVisible(TRUE);
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| �׸��� �Լ�
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	gdi->DrawPaint();
	return 0;
}
LRESULT CMainDlg::OnBnClickedBut0(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->SetControlMode();
	gdi->SetTerminalStaticString(_T("0"));
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}
LRESULT CMainDlg::OnBnClickedBut1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->SetControlMode(1);
	gdi->SetTerminalStaticString(_T("1"));
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->SetControlMode(2);
	gdi->SetTerminalStaticString(_T("2"));
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_GASOUND::SoundPlay(_T("Flash Disk\\error.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_GASOUND::SoundPlay(_T("Flash Disk\\error.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	_GASOUND::SoundPlay(_T("Flash Disk\\error.wav"));
	return 0;
}
HBRUSH  CMainDlg::OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
{
	if((wndStatic==GetDlgItem(IDC_VIEWTEXT))||(wndStatic==GetDlgItem(IDC_TERMINALSET)))
	{
		gdi->StaticText(dc);
	}
	return 0;
}