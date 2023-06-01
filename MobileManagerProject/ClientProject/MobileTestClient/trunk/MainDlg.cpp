// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"

CMainDlg::CMainDlg():
obj(NULL)
{}
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}
#include <tchar.h>

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	CreateObject();
	client= obj->GetClient();
	::SetDlgItemText(m_hWnd,IDC_IPADDRESS, "127.0.0.1");
	::SetDlgItemText(m_hWnd,IDC_EDPORT, "9000");
	typecode.Attach(GetDlgItem(IDC_COTYPECODE));

	typecode.AddString("카드Enable");
	typecode.AddString("사용자변경");
	typecode.AddString("소득공제");
	typecode.AddString("사용내역");
	typecode.SendMessage(CB_SETCURSEL, 0, 0); 
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject()
{
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
		obj->Release();
		delete obj;obj=NULL;
	}
}
LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Release();
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedBuconnect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString ip;
	u_short port;
	::GetDlgItemText(m_hWnd,IDC_IPADDRESS, (LPSTR)(LPCTSTR)ip,16);
	port=(u_short)::GetDlgItemInt(m_hWnd,IDC_EDPORT, NULL, FALSE );
	if(client->ServerConnect(port, ip)==FALSE)
		PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainDlg::OnBnClickedBusend(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int datacode[] = {RECVCODE_CARDENABLE,RECVCODE_USERCHANGE,RECVCODE_DEDUCTION,RECVCODE_USECHECK};
	int cd = RECVCODE_VMCLIENTRECV|datacode[typecode.GetCurSel()];
	TCHAR CardNm[1024];
	memset(CardNm, 0x00,sizeof(TCHAR)*1024);
	GetDlgItemText(IDC_EDSENDCARDNUM,CardNm,1024);
	TCHAR buf[1024];
	memset(buf, 0x00,sizeof(TCHAR)*1024);
	GetDlgItemText(IDC_EDSENDDATA,buf,1024);
	client->SendData(cd, CardNm, buf);
	return 0;
}

LRESULT CMainDlg::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	Release();
	CloseDialog(0);
	return 0;
}
