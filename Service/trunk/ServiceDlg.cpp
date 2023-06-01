// UpdateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Service.h"
#include "ServiceDlg.h"
#include <windows.h>
#include <io.h>
#include "staticmyfile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
LPCTSTR  szFilePath="C:\\WINDOWS\\system32\\scardsvr.exe";
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServiceDlg dialog

CServiceDlg::CServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServiceDlg::IDD, pParent),
	StartType(0)
{
	//{{AFX_DATA_INIT(CServiceDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServiceDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServiceDlg, CDialog)
	//{{AFX_MSG_MAP(CServiceDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServiceDlg message handlers

BOOL CServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowText("스마트 카드 서비스 등록중");
	ShowWindow(SW_HIDE);
	AfxGetApp()->HideApplication();

	/*if(_access("C:\\WINDDK", 2) != 0)
	{
		MessageBox("dddd");
	}*/
	if(StartRemoteRegistry(SCardSvr))
	{
		/*int  nt=0;
		CWnd *pWnd=NULL;
		PROCESS_INFORMATION ProcessInfo; 
		STARTUPINFO sInfo;
		ZeroMemory(&sInfo, sizeof(STARTUPINFO));
		sInfo.cb                  = sizeof(STARTUPINFO); 
		sInfo.lpReserved      = NULL; 
		sInfo.lpReserved2    = NULL; 
		sInfo.cbReserved2   = 0; 
		sInfo.lpDesktop       = NULL; 
		sInfo.lpTitle             = NULL; 
		sInfo.dwFlags          = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES; 
		sInfo.dwX                = 0; 
		sInfo.dwY                = 0; 
		sInfo.dwFillAttribute  = 0; 
		sInfo.wShowWindow = SW_SHOWNORMAL;
		if(CStaticMyFile::FindFileAndFolder(POKO))
		{
			while(TRUE)
			{

				BOOL b=::CreateProcess(POKO, NULL, NULL, NULL, FALSE, 0, NULL,NULL, &sInfo, &ProcessInfo);
				if(!b)
				{
					AfxMessageBox("Can't execute Poko.exe program."); 
				}
				else 
				{
					if(WaitForInputIdle(ProcessInfo.hProcess, 1000)==0)
					{
						pWnd=FindWindow(NULL,"MainPlug");
						if(ProcessInfo.hProcess)
							CloseHandle( ProcessInfo.hProcess );
						if(ProcessInfo.hThread)
							CloseHandle( ProcessInfo.hThread );
					}							
				}		
				if(pWnd)
				{
					break;
				}
				if(nt>3) break;
			}
		}*/
		CStaticMyFile::logfile("서비스등록 완료");
	}
	SendMessage(WM_CLOSE);	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServiceDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServiceDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//#include <windows.h>
#include <shlobj.h>
//#include ".\servicedlg.h"

BOOL CServiceDlg::SelfDelete()
{
	ShowWindow(SW_HIDE);
	AfxGetApp()->HideApplication();
	
	//AfxMessageBox("PokoUSB Update가 완료되었습니다.\n\nPokoUSB를 삽입해주세요.", MB_OK);

	SHELLEXECUTEINFO sei;

    TCHAR   szModule[MAX_PATH];

    // get file path names:
    ::ZeroMemory(szModule, sizeof(TCHAR) * MAX_PATH);
    if((GetModuleFileName(0, szModule, MAX_PATH) != 0))
    {
        CString strBatch, strTemp(szModule);
        int     nFind;

        nFind = strTemp.ReverseFind('\\');
        if(nFind < 0)
            return FALSE;

        CFile   file;

        strBatch.Format(_T("%s\\Delete.bat"), strTemp.Left(nFind));

        if(!file.Open(strBatch, CFile::modeCreate | CFile::modeWrite))
            return FALSE;

        // 배치파일내의 경로는 모두 절대경로로 할 것
        strTemp.Format(_T(":Repeat\r\ndel \"%s\"\r\n")
                       _T("if exist \"%s\" goto Repeat\r\n")
                       _T("del \"%s\"\r\n"), szModule, szModule, strBatch);

        file.Write(strTemp, strTemp.GetLength());
        file.Flush();
        file.Close();

        // set struct members
        sei.cbSize       = sizeof(sei);
        sei.hwnd         = NULL;
        sei.lpVerb       = _T("Open");
        sei.lpFile       = strBatch;
        sei.lpParameters = NULL;
        sei.lpDirectory  = 0;
        sei.nShow        = SW_HIDE;
        sei.fMask        = SEE_MASK_NOCLOSEPROCESS;
       

        // increase resource allocation to program
        SetPriorityClass (GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        SetThreadPriority(GetCurrentThread (), THREAD_PRIORITY_TIME_CRITICAL);      

        // invoke command shell
        if(ShellExecuteEx(&sei))
        {
            // suppress command shell process until program exits
            SetPriorityClass       (sei.hProcess, IDLE_PRIORITY_CLASS);
            SetProcessPriorityBoost(sei.hProcess, TRUE);
            
            // notify explorer shell of deletion
            SHChangeNotify(SHCNE_DELETE, SHCNF_PATH, szModule, 0);
            return TRUE;
        }
        else // if error, normalize allocation
        {
            SetPriorityClass (GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
            SetThreadPriority(GetCurrentThread (), THREAD_PRIORITY_NORMAL);
        }
    }   

    return FALSE;

}

void CServiceDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(!SelfDelete())
	{
		MessageBox("Fail");
	}
	
	CDialog::OnClose();
}
BOOL CServiceDlg::CreatDevice(char* sc_name, char* sc_path)

{
	SC_HANDLE hSCMan = NULL;
	SC_HANDLE hService = NULL;
	if((hSCMan = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS)) == 0)
	{
		TRACE("서비스를 열수 없습니다.");
		return FALSE;
	}
	if((hService = CreateServiceA(hSCMan, sc_name, sc_name, SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, sc_path, 0, 0, 0, 0, 0)) == 0)
	{
		if(GetLastError() == ERROR_SERVICE_EXISTS)
			TRACE("이미 설치되어있습니다.\n");
		else if(GetLastError() == ERROR_SERVICE_MARKED_FOR_DELETE)
			TRACE("서비스가 제거되도록 설정되어 있어 생성불가.\n");
		CloseServiceHandle(hSCMan);
		return FALSE;
	}
	StartService(hService, 0, 0);
	TRACE("설치완료\n");
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCMan);
	getchar();
	return TRUE;
}

BOOL CServiceDlg::UnloadDevice(char* sc_name)
{
	SECURITY_ATTRIBUTES ;
	DWORD dwError = ERROR_SUCCESS;
	SC_HANDLE hSCMan = NULL;
	SC_HANDLE hService = NULL;
	SERVICE_STATUS serviceStatus;
	if((hSCMan = OpenSCManager(0, 0, SC_MANAGER_ALL_ACCESS)) == 0)
	{
		TRACE("서비스를 열수 없습니다.");
		return FALSE;
	}
	if((hService = OpenServiceA(hSCMan, sc_name, SERVICE_ALL_ACCESS)) == 0)
	{
		TRACE("서비스가 설치되지 않았습니다.\n");
		CloseServiceHandle(hSCMan);
		return FALSE;
	}
	ControlService(hService,SERVICE_CONTROL_STOP,&serviceStatus); 
	if(DeleteService(hService) == 0)
	{
		if(GetLastError() == ERROR_SERVICE_MARKED_FOR_DELETE)
			TRACE("서비스가 이미 제거 설정되었습니다.\n");
	}
	else
		TRACE("서비스가 제거되었습니다. 프로그램을 종료해야합니다.\n");
	CloseServiceHandle(hSCMan);
	return TRUE;
}
BOOL CServiceDlg::StartServiceExcu(char *svcName)
{
	SERVICE_STATUS status;
	/*DWORD   dwServiceType;
	DWORD   dwCurrentState;
	DWORD   dwControlsAccepted;
	DWORD   dwWin32ExitCode;
	DWORD   dwServiceSpecificExitCode;
	DWORD   dwCheckPoint;
	DWORD   dwWaitHint;*/
	BOOL bRet = FALSE;
	int nErrCode = 0;
	SC_HANDLE hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if ( hScm ) 
	{
		SC_HANDLE hService = OpenService(hScm, svcName, SERVICE_START | SERVICE_QUERY_STATUS);
		if ( hService ) 
		{
			QueryServiceStatus(hService, &status);
			if (status.dwCurrentState != SERVICE_RUNNING)
			{
				// 서비스를 시작시키고 완전히 시작할 때까지 대기한다.
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				if (bRet = StartService(hService, 0, NULL) == TRUE) 
				{
					bRet = TRUE;
					QueryServiceStatus(hService, &status);
					while (status.dwCurrentState != SERVICE_RUNNING)
					{
						Sleep(status.dwWaitHint);
						QueryServiceStatus(hService, &status);
					}
				}
				else
					nErrCode = GetLastError();
				SetCursor(LoadCursor(NULL, IDC_ARROW)); 
			}
		}
		CloseServiceHandle(hService);
	}
	CloseServiceHandle(hScm);
	return bRet;
}

BOOL CServiceDlg::ChangeServiceConfig_my(LPCTSTR lpServicName)
{
	SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);//SC_MANAGER_LOCK);//
	SC_HANDLE svc = OpenService(scm, lpServicName, SERVICE_ALL_ACCESS);
	QUERY_SERVICE_CONFIG config;
	/*DWORD   dwServiceType;
	DWORD   dwStartType;
	DWORD   dwErrorControl;
	LPSTR   lpBinaryPathName;
	LPSTR   lpLoadOrderGroup;
	DWORD   dwTagId;
	LPSTR   lpDependencies;
	LPSTR   lpServiceStartName;
	LPSTR   lpDisplayName;*/

	DWORD s=sizeof(config);
	DWORD sa;
	while(1)
	{
		if(!QueryServiceConfig(svc, &config, s, &sa))
		{
			if(s!=sa) s=sa;
			else break;
		}
	}
	//BOOL b=QueryServiceConfig(svc, &config, s, &sa);
	/*printf(")
	printf("config_ServiceType:%d, config_StartType%d,
		config.dwErrorControl:%d\n, config.lpBinaryPathName:%s\n, config.lpLoadOrderGroup:%s\n,
		&config.dwTagId:%d\n,config.lpDependencies:%s\n, config.lpServiceStartName:%s\n,
		config.lpDisplayName:%s\n", config.dwServiceType, config.dwStartType,
		config.dwErrorControl, config.lpBinaryPathName, config.lpLoadOrderGroup,
		&config.dwTagId,config.lpDependencies, config.lpServiceStartName,
		config.lpDisplayName);*/
	/*printf("%d\n, %d\n, %d\n, %s\n, %s\n, %d\n, %s\n, %s\n,%s\n", config.dwServiceType, config.dwStartType,
		config.dwErrorControl, config.lpBinaryPathName, config.lpLoadOrderGroup,
		&config.dwTagId,config.lpDependencies, config.lpServiceStartName,
		config.lpDisplayName);*/
	if(!(config.dwStartType&SERVICE_AUTO_START))
	{
		SC_LOCK lock;
		while((lock=LockServiceDatabase(scm))==NULL)
		{
			if(GetLastError()!=ERROR_SERVICE_DATABASE_LOCKED)
			{
				CloseServiceHandle(scm);
			}
			Sleep(1000);
		}
		/*SC_HANDLE    hService,
		DWORD        dwServiceType,
		DWORD        dwStartType,
		DWORD        dwErrorControl,
		LPCSTR     lpBinaryPathName,
		LPCSTR     lpLoadOrderGroup,
		LPDWORD      lpdwTagId,
		LPCSTR     lpDependencies,
		LPCSTR     lpServiceStartName,
		LPCSTR     lpPassword,
		LPCSTR     lpDisplayName*/
		/* config.dwServiceType ======서비스타입=======
		#define SERVICE_KERNEL_DRIVER          0x00000001
		#define SERVICE_FILE_SYSTEM_DRIVER     0x00000002
		#define SERVICE_ADAPTER                0x00000004
		#define SERVICE_RECOGNIZER_DRIVER      0x00000008

		#define SERVICE_DRIVER                 (SERVICE_KERNEL_DRIVER | \
		SERVICE_FILE_SYSTEM_DRIVER | \
		SERVICE_RECOGNIZER_DRIVER)

		#define SERVICE_WIN32_OWN_PROCESS      0x00000010
		#define SERVICE_WIN32_SHARE_PROCESS    0x00000020
		#define SERVICE_WIN32                  (SERVICE_WIN32_OWN_PROCESS | \
		SERVICE_WIN32_SHARE_PROCESS)

		#define SERVICE_INTERACTIVE_PROCESS    0x00000100

		#define SERVICE_TYPE_ALL               (SERVICE_WIN32  | \
		SERVICE_ADAPTER | \
		SERVICE_DRIVER  | \
		SERVICE_INTERACTIVE_PROCESS)*/

		/*config.dwStartType ============스타트타입===============
		#define SERVICE_BOOT_START             0x00000000
		#define SERVICE_SYSTEM_START           0x00000001
		#define SERVICE_AUTO_START             0x00000002
		#define SERVICE_DEMAND_START           0x00000003
		#define SERVICE_DISABLED               0x00000004*/

		if(ChangeServiceConfig(svc,config.dwServiceType, config.dwStartType,
			config.dwErrorControl, config.lpBinaryPathName, config.lpLoadOrderGroup,
			&config.dwTagId,config.lpDependencies, config.lpServiceStartName,
			NULL,config.lpDisplayName))
			/*if(!ChangeServiceConfig(svc, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, 
			SERVICE_ERROR_NORMAL, "",NULL,
			NULL, NULL,NULL,
			NULL, ""))*/
		{
			CStaticMyFile::logfile("서비스컨피그성공");
		}
		UnlockServiceDatabase(lock);
	}
	CloseServiceHandle(svc);
	CloseServiceHandle(scm);
	return TRUE;

}
BOOL CServiceDlg::StartRemoteRegistry(LPCTSTR lpServicName)
{
	SC_HANDLE scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);//SC_MANAGER_LOCK);//
	SC_HANDLE schRegistryService;
	SERVICE_STATUS ssRegistryStatus;
	LPQUERY_SERVICE_CONFIG lpRegistryConfig;
	DWORD dwConfigSize;
	lpRegistryConfig = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, 1024);
	if (lpRegistryConfig == NULL)
	{
		return FALSE;
	}

	schRegistryService = OpenService( scm, lpServicName, SERVICE_START | SERVICE_CHANGE_CONFIG| SERVICE_QUERY_STATUS|SERVICE_QUERY_CONFIG);
	CStaticMyFile::logfile("서비스목록:%s",lpServicName);
	if (schRegistryService == NULL)
	{
		return FALSE;
	}

	if (!QueryServiceConfig(schRegistryService, lpRegistryConfig, 1024, &dwConfigSize))
	{
		return FALSE;
	}

	StartType = lpRegistryConfig->dwStartType;
	LocalFree(lpRegistryConfig);
	if (StartType == SERVICE_DISABLED||StartType == SERVICE_DEMAND_START)
	{
		CStaticMyFile::logfile("SERVICE_AUTO_START가 아니다");
		if (!ChangeServiceConfig(schRegistryService, 
			SERVICE_NO_CHANGE, 
			SERVICE_AUTO_START, 
			SERVICE_NO_CHANGE, 
			NULL, NULL, NULL, NULL, NULL, NULL,NULL))
		{
			return FALSE;
		}
	}

	if (!QueryServiceStatus(schRegistryService, &ssRegistryStatus))
	{
		return FALSE;
	}

	if (ssRegistryStatus.dwCurrentState != SERVICE_RUNNING)
	{
		CStaticMyFile::logfile("SERVICE_RUNNING이 아니다");
		if (!MyStartService(schRegistryService, "Smart Card"))
			return FALSE;
	}
	CloseServiceHandle(schRegistryService); 
	return TRUE;
}
BOOL CServiceDlg::MyStartService(SC_HANDLE schService, char* szServiceName)
{
	DWORD dwWaitTime;
	DWORD dwOldCheckPoint;
	DWORD dwStartTickCount;
	SERVICE_STATUS ssStatus;

	if (!(StartService(schService, 0, NULL)))
	{
		CStaticMyFile::logfile ("Starting %s service failed!", szServiceName);
		return FALSE;
	}

	if (!(QueryServiceStatus(schService, &ssStatus)))
	{
		CStaticMyFile::logfile ("Query %s service status failed!",szServiceName);
	}

	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while ( ssStatus.dwCurrentState == SERVICE_START_PENDING)
	{
		dwWaitTime = ssStatus.dwWaitHint / 10;
		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep(dwWaitTime);

		// Check the status again. 

		if (!QueryServiceStatus(schService, &ssStatus))
			break; 

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// The service is making progress.
			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				// No progress made within the wait hint
				break;
			}
		}
	}

	if ( ssStatus.dwCurrentState == SERVICE_RUNNING )
	{
		CStaticMyFile::logfile ("%s service is started successfully! %s service is running!", szServiceName, szServiceName);
	}
	else
	{
		CStaticMyFile::logfile ("%s service is not started!", szServiceName);
		return FALSE;
	}

	return TRUE;
}