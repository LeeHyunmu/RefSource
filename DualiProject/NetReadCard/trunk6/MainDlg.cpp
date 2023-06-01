// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainDlg::CMainDlg():
obj(NULL),
gdi(NULL)
{}
//--------------------------------------------------------------
//|
//|메시지 함수
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
//|다이얼로그 초기화
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
	// 오브젝트 생성 클래스
	//=============================================
	CreateObject();
	//=============================================
	// gdi 생성
	//=============================================
	gdi = obj->GetMainGdiObject();
	//=============================================
	// UI 초기화
	//=============================================
	gdi->initDlg(m_hWnd);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
	//=============================================
	// 오브젝트 관리 클래스 생성
	//=============================================
	obj=new CClassObject;
	if(obj)
		obj->CreateObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
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
//| 종료 함수
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedDualicardconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->CardConnent("USB", TRUE);
	GetDlgItem(IDC_DUALICARDCONNECT).EnableWindow(FALSE);
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}
LRESULT CMainDlg::OnBnClickedDebugpane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
//| 그리기 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	gdi->DrawPaint();
	return 0;
}
LRESULT CMainDlg::OnBnClickedBut0(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->SetControlMode();
	gdi->SetTerminalStaticString(_T("0"));
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}
LRESULT CMainDlg::OnBnClickedBut1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->SetControlMode(1);
	gdi->SetTerminalStaticString(_T("1"));
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	IDualiCardMg* duali = obj->GetDualiCardMgObject();
	duali->SetControlMode(2);
	gdi->SetTerminalStaticString(_T("2"));
	_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_GASOUND::SoundPlay(_T("Flash Disk\\error.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut4(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	_GASOUND::SoundPlay(_T("Flash Disk\\error.wav"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedBut5(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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