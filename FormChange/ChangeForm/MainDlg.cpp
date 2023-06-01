// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"


#define  MAXTIME_CAM  240000
#define  MAXTIME_ONLINE  600000
#define  MINTIME_CAM  3000
#define  MINTIME_ONLINE  6000

#define  MINTIME_CAMTIME  "3"
#define  MINTIME_ONLINETIME  "6"
#define  MINTIME_CAMNAME  "RASplus"
#define  MINTIME_ONLINENAME  "AQ Online System"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}
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
	OnlineDelayTime=MINTIME_ONLINE;
	CamDelayTime=MINTIME_CAM;
	UIAddChildWindowContainer(m_hWnd);

	//============================================================================
	OnlineTime.Attach(GetDlgItem(IDC_ONLINE));
	OnlineTime.SetWindowText(MINTIME_ONLINETIME);
	CamTime.Attach(GetDlgItem(IDC_CAMERA));
	CamTime.SetWindowText(MINTIME_CAMTIME);

	OnlineName.Attach(GetDlgItem(IDC_ONLINENAME));
	OnlineName.SetWindowText(MINTIME_ONLINENAME);
	CamName.Attach(GetDlgItem(IDC_CAMERANAME));
	CamName.SetWindowText(MINTIME_CAMNAME);
	hTopWnd=NULL;
	//hwin = FindWindow(NULL,"RASplus - Remote Administration System Plus [애니쿼터스]");
	//테스트용
	//hwin = FindWindow(NULL,"VS2003Project");
	//hEx = FindWindow("IEFrame",NULL);
	GetWindowHandle(MINTIME_ONLINENAME,MINTIME_CAMNAME);
	CRect rc;
	GetWindowRect(&rc);
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	SetWindowPos(NULL,iX-rc.Width(),iY-(rc.Height()+40),rc.Width(), rc.Height(),SWP_SHOWWINDOW);
	return TRUE;
}
int loop=0;
#define MAX_COUNT 3
LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			SetTopWindow(nIDEvent);
			KillTimer(1);
		}
		break;
	case 2:
		{
			SetTopWindow(nIDEvent);
			KillTimer(2);
		}
		break;
	case 3:
		{
			SetTopWindow(nIDEvent);
			KillTimer(3);
		}
		break;
	}
}
void CMainDlg::SetTopWindow(int Num)
{
	switch(Num)
	{
	case 1:
		{
/*			if(!hTopWnd.IsWindow())
				hTopWnd = GetForegroundWindow();
			CString wintex;
			hTopWnd.GetWindowText(wintex);
			if(hTopWnd.IsWindow())
			{
				::SetForegroundWindow(hTopWnd);
				::ShowWindow(hTopWnd, SW_SHOWNORMAL);
				::SetActiveWindow(hTopWnd);
			}*/
			SetTimer(2,500,NULL);
		}
		break;
	case 2:
		{
			if(hwin.IsWindow())
			{
				::SetForegroundWindow(hwin);
				::ShowWindow(hwin, SW_SHOWMAXIMIZED);
				::SetActiveWindow(hwin);
				SetTimer(3,CamDelayTime,NULL);
			}
		}
		break;
	case 3:
		{
			if(hEx.IsWindow())
			{
				::SetForegroundWindow(hEx);
				::ShowWindow(hEx, SW_SHOWMAXIMIZED);
				::SetActiveWindow(hEx);
				SetTimer(2,OnlineDelayTime,NULL);
			}
		}
		break;
	}
}


LRESULT CMainDlg::OnBnClickedTimeset(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString OnlineText;
	GetDlgItem(IDC_ONLINE).GetWindowText(OnlineText);
	CString CamTest;
	GetDlgItem(IDC_CAMERA).GetWindowText(CamTest);
	OnlineDelayTime=atoi(OnlineText)*1000;
	if(OnlineDelayTime<MINTIME_ONLINE)
		OnlineDelayTime=MINTIME_ONLINE;
	if(OnlineDelayTime>MAXTIME_ONLINE)
		OnlineDelayTime=MAXTIME_ONLINE;
	CamDelayTime=atoi(CamTest)*1000;
	if(CamDelayTime<MINTIME_CAM)
		CamDelayTime=MINTIME_CAM;
	if(CamDelayTime>MAXTIME_CAM)
		CamDelayTime=MAXTIME_CAM;
	CString strOnlineName;
	CString strCamName;
	OnlineName.GetWindowText(strOnlineName);
	CamName.GetWindowText(strCamName);
	GetWindowHandle(strOnlineName,strCamName);
	return 0;
}

LRESULT CMainDlg::OnBnClickedStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SetTimer(1,1000,NULL);
	return 0;
}

BOOL CMainDlg::GetWindowHandle(CString AqWinName, CString CamWinName)
{
	HWND tmpHwnd = FindWindow(NULL, NULL);
	while(tmpHwnd!=NULL)
	{
		if(::GetParent(tmpHwnd)==NULL) 
		{
			CWindow win = tmpHwnd;
			CString caption;
			win.GetWindowText(caption);
			if(AqWinName.CompareNoCase(caption.Left(AqWinName.GetLength()))==0)
				hwin=tmpHwnd;
			if(CamWinName.CompareNoCase(caption.Left(CamWinName.GetLength()))==0)
				hEx=tmpHwnd;
		}
		tmpHwnd = ::GetWindow(tmpHwnd, GW_HWNDNEXT);
		if(hwin.IsWindow()&&hEx.IsWindow())
			return TRUE;
	}
	return FALSE;
}