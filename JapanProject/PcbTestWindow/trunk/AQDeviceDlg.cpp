
// AQDeviceDlg.cpp : ���� ����
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
// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CAQDeviceDlg ��ȭ ����




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


// CAQDeviceDlg �޽��� ó����

BOOL CAQDeviceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	m_watercmd.SetItemHeight(-1,20);
	m_watercmd.AddString(_T("4F"));
	m_watercmd.AddString(_T("46"));
	m_watercmd.AddString(_T("80"));
	m_watercmd.SetCurSel(0);

	CString comboitem;
	m_watercmd.GetLBText(m_watercmd.GetCurSel(), comboitem);
	m_cmdstring.SetWindowText(comboitem);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	bAQStart = AQDevice.initAQNFC(this, ReadEventFunc, DebugFunc);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CAQDeviceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CAQDeviceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAQDeviceDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
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
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
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
//|����� ���
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString comboitem;
	m_watercmd.GetLBText(m_watercmd.GetCurSel(), comboitem);
	m_cmdstring.SetWindowText(comboitem);
}

void CAQDeviceDlg::OnBnClickedCmdsend()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString str;
	byte cmddata[MAX_COMMANDDATA];
	int cmddatalen = MAX_COMMANDDATA;
	//
	m_cmdstring.GetWindowText(str);
	cmddatalen = ChangeHexCord(str, cmddata, cmddatalen);
	DebugWrite(_T("----------��ɾ� ����--------------------------"));
	AQDevice.CommandSend(cmddata[0]);
	DebugWrite(_T("-----------��ɾ� ��-------------------------"));
}
