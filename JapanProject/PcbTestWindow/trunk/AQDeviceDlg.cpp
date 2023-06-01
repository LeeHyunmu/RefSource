
// AQDeviceDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "AQDevice.h"
#include "AQDeviceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString SamCmdListString[]=
{
	"5F8AC2D2DC870D71091C6A02595F999B"
};
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAQDeviceDlg 대화 상자




CAQDeviceDlg::CAQDeviceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAQDeviceDlg::IDD, pParent)
	, m_cmdstringvalue(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAQDeviceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DataPrint);
	DDX_Control(pDX, IDC_WATERCMD, m_watercmd);
	DDX_Control(pDX, IDC_CMDSTRING, m_cmdstring);
	DDX_Text(pDX, IDC_CMDSTRING, m_cmdstringvalue);
	DDV_MaxChars(pDX, m_cmdstringvalue, 2);
}

BEGIN_MESSAGE_MAP(CAQDeviceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_WATERCMD, &CAQDeviceDlg::OnCbnSelchangeWatercmd)
	ON_BN_CLICKED(IDC_CMDSEND, &CAQDeviceDlg::OnBnClickedCmdsend)
END_MESSAGE_MAP()


// CAQDeviceDlg 메시지 처리기

BOOL CAQDeviceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_watercmd.SetItemHeight(-1,20);
	m_watercmd.AddString(_T("4F"));
	m_watercmd.AddString(_T("46"));
	m_watercmd.AddString(_T("80"));
	m_watercmd.SetCurSel(0);

	CString comboitem;
	m_watercmd.GetLBText(m_watercmd.GetCurSel(), comboitem);
	m_cmdstring.SetWindowText(comboitem);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	bAQStart = AQDevice.initAQNFC(this, ReadEventFunc, DebugFunc);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAQDeviceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CAQDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CAQDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAQDeviceDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

DWORD WINAPI CAQDeviceDlg::ReadEventFunc(LPVOID lpThreadParameter, CAQCmdFormat::WaterCheck check)
{
	CAQDeviceDlg* mdlg = (CAQDeviceDlg*)lpThreadParameter;
	return mdlg->ReadEventMessage(check);
}
DWORD  CAQDeviceDlg::ReadEventMessage(CAQCmdFormat::WaterCheck check)
{
	switch(check)
	{
	case CAQCmdFormat::water_ready:
		{
			DebugWrite(_T("water ready"));
		}break;
	case CAQCmdFormat::water_readycomplete:
		{
			DebugWrite(_T("water ready complete"));
		}break;
	case CAQCmdFormat::water_run:
		{
			DebugWrite(_T("water run"));
		}break;
	case CAQCmdFormat::water_error:
		{
			DebugWrite(_T("water error"));
		}break;
	}
	return 0;
}
DWORD WINAPI CAQDeviceDlg::DebugFunc(LPVOID lpThreadParameter, byte* data, int datalen, bool bsend)
{
	CAQDeviceDlg* mdlg = (CAQDeviceDlg*)lpThreadParameter;
	return mdlg->DebugMessage(data, datalen, bsend);
}
DWORD  CAQDeviceDlg::DebugMessage(byte* data, int datalen, bool bsend)
{
	CString str = _T(">> ");
	if(bsend==false)
		str = _T("<< ");
	DebugWrite(str + byteToHexbyteValue(data, datalen));
	return 0;
}

static char hexChar[] = {
	0x30 , 0x31 , 0x32 , 0x33 ,
	0x34 , 0x35 , 0x36 , 0x37 ,
	0x38 , 0x39 , 0x41 , 0x42 ,
	0x43 , 0x44 , 0x45 , 0x46 
};
CString CAQDeviceDlg::byteToHexbyteValue(byte* data, int len) 
{
	CString hex="";
	for (int i = 0, h = 0; i < len; i++, h+=2) {
		hex += hexChar[(data[i]&0xF0)>>4];
		hex += hexChar[(data[i]&0x0F)];
	}
	return hex;
}
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
int  CAQDeviceDlg::ChangeHexCord(CString data, BYTE* hex, int maxlen)
{
	memset(hex, 0, maxlen);
	int dwtransLen=data.GetLength()/2;
	for(int i = 0 ; i < dwtransLen ; i++ )
	{
		hex[i] = (((BYTE)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			hex[i] += 0x90;

		hex[i] |= ((BYTE)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			hex[i] += 0x09;
	}
	return dwtransLen;
}
//--------------------------------------------------------------
//|
//|디버그 출력
//|
//---------------------------------------------------------------
void  CAQDeviceDlg::DebugWrite(CString data)
{
	m_DataPrint.AddString(data);
	int count = m_DataPrint.GetCount();
	m_DataPrint.SetCurSel(m_DataPrint.GetCount()-1);
}


void CAQDeviceDlg::OnCbnSelchangeWatercmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString comboitem;
	m_watercmd.GetLBText(m_watercmd.GetCurSel(), comboitem);
	m_cmdstring.SetWindowText(comboitem);
}

void CAQDeviceDlg::OnBnClickedCmdsend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	byte cmddata[MAX_COMMANDDATA];
	int cmddatalen = MAX_COMMANDDATA;
	//
	m_cmdstring.GetWindowText(str);
	cmddatalen = ChangeHexCord(str, cmddata, cmddatalen);
	DebugWrite(_T("----------명령어 시작--------------------------"));
	AQDevice.CommandSend(cmddata[0]);
	DebugWrite(_T("-----------명령어 끝-------------------------"));
}
