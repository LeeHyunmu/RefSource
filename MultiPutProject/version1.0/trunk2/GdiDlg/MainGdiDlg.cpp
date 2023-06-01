#include "stdAfx.h"
#include "MainGdiDlg.h"

#define MAX_BUF 1024

//Hot Key define
#define MAINGDIHK_CAPTURESET		1
#define MAINGDIHK_CAPTURESCREEN		2
#define MAINGDIHK_CAPTUREWINDOW		3

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
hWnd(NULL),
m_memDC(NULL),
bMyShow(TRUE),
sendfilepointer(NULL),
sendfilelen(0),
sendfiletotallen(0),
recvfilepointer(NULL),
recvfilelen(0),
recvfiletotallen(0),
tcprecvdatastr(_T("")),
TcpServerBufFlag(SERVERFLAGE_IDLESERVER),
TcpClientBufFlag(SERVERFLAGE_IDLESERVER),
UdpServerBufFlag(SERVERFLAGE_IDLESERVER),
UdpClientBufFlag(SERVERFLAGE_IDLESERVER)
{}

//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
	if( m_memDC )
	{
		delete m_memDC;
	}
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::ControlDestroy()
{
	//��ư �޸� ����
	TcpServerBut.DestroyWindow();
	UdpServerBut.DestroyWindow();
	//CStatic Ŭ����(�ؽ�Ʈ ���) ����
	TcpServerText.DestroyWindow();
	UdpServerText.DestroyWindow();

	sensoreventopengl.DestroyWindow();

	keyeventopengl.DestroyWindow();
	//��׶��� �̹��� ����
	MainGidDlgImageClear();

	//���� �ε� ���� �޸� ����
	if(sendfilepointer)
	{
		delete[] sendfilepointer; sendfilepointer=NULL;
		sendfilelen=sendfiletotallen=0;
	}
	if(recvfilepointer)
	{
		delete[] recvfilepointer; recvfilepointer=NULL;
		recvfilelen=recvfiletotallen=0;
	}
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
	//========================================
	debug=obj->GetOutputObject();
	//========================================
	// Select Server Ŭ����
	//========================================
	selectserver = obj->GetSelectServerObject();
}
//--------------------------------------------------------------
//|
//|���� Tcp Recv �ݹ� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::SelectTcpServerRecvFunc(LPVOID lpArg, SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpArg;
	if(mdlg->TcpServerRevcData(sock, buf, recvbytes, maxrecv, sendbytes, totalsend))
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------
//|
//|���� Tcp Recv ó�� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::TcpServerRevcData(SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
		TcpServerBufFlag=SERVERFLAGE_IDLESERVER;
	}
	else
	{
		if(SERVERFLAGE_KEYEVENTHEAD==((BYTE)buf[0]))
		{
			if(maxrecv==1)
			{
				maxrecv=3;
			}
			else if(recvbytes==3)
			{
				BYTE data[2];
				int datepoint=1;
				memcpy(data, &buf[datepoint], sizeof(data));
				datepoint+=sizeof(data);
				if(data[0]&0x80)
				{
					keyeventopengl.SetVirtualKey(0);
				}
				else
				{
					keyeventopengl.SetVirtualKey((UINT)data[1]&0xFF);
				}
				UINT uint = (UINT) MAKEWORD(data[1], data[0]);
				SendMessageGame(uint);
				//���� �����͸� �ޱ� ���� �ʱ�ȭ
				recvbytes = maxrecv = sendbytes = totalsend = 0;
				TcpServerBufFlag = SERVERFLAGE_IDLESERVER;
			}
			else
			{
				//���� �����͸� �ޱ� ���� �ʱ�ȭ
				recvbytes = maxrecv = sendbytes = totalsend = 0;
				TcpServerBufFlag = SERVERFLAGE_IDLESERVER;
			}
		}
		else if(SERVERFLAGE_SENSOREVENTHEAD==((BYTE)buf[0]))
		{
			if(maxrecv==1)
			{
				maxrecv=14;
			}
			else if(recvbytes==14)
			{
				BYTE SensorType;
				int datepoint=1;
				memcpy(&SensorType, &buf[datepoint], sizeof(BYTE));
				datepoint+=sizeof(BYTE);
				if(SensorType==SENSORFLAGE_ORIENTATION)
				{
					UINT fdata[3];
					float* Pitch, *Roll, *Yaw;
					Pitch = Roll = Yaw = NULL;
					memcpy(fdata, &buf[datepoint], sizeof(fdata));
					Pitch=(float*)&fdata[1];
					Roll=(float*)&fdata[2];
					Yaw=(float*)&fdata[0];
					//�������� ��ǥ�� �Է�
					sensoreventopengl.SetPitchRollYaw(*Pitch*-1.0f, *Roll*-1.0f, *Yaw*-1.0f);
				}
				//���� �����͸� �ޱ� ���� �ʱ�ȭ
				recvbytes = maxrecv = sendbytes = totalsend = 0;
				TcpServerBufFlag = SERVERFLAGE_IDLESERVER;
			}
			else
			{
				//���� �����͸� �ޱ� ���� �ʱ�ȭ
				recvbytes = maxrecv = sendbytes = totalsend = 0;
				TcpServerBufFlag = SERVERFLAGE_IDLESERVER;
			}
		}
		else
		{
			recvbytes = maxrecv = sendbytes = totalsend = 0;
			TcpServerBufFlag = SERVERFLAGE_IDLESERVER;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|���� Udp Recv �ݹ� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::SelectUdpServerRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpArg;
	if(mdlg->UdpServerRevcData(buf, recvbytes, maxrecv, sendbytes, totalsend))
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------
//|
//|���� Udp Recv ó�� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::UdpServerRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
		if(UdpServerBufFlag == SERVERFLAGE_IpResponse)
		{
			CString ip=_T("IP=");
			ip+=selectserver->GetLocalServerIp();
			//buf[0]=SERVERFLAGE_IpResponse;
			memcpy(&buf[0], ip.GetBuffer(ip.GetLength()), ip.GetLength());
			totalsend=ip.GetLength();
			UdpServerBufFlag=SERVERFLAGE_IDLESERVER;
			if(debug)
			{
				debug->Write(_T("UDP ������ ����"));
			}
		}
	}
	else
	{
		if(buf[0]==SERVERFLAGE_IpRequest)
		{
			//IP ������ ���� ����
			UdpServerBufFlag = SERVERFLAGE_IpResponse;
			recvbytes = maxrecv = sendbytes = totalsend = 0;
			if(selectserver->UdpServerSend()==FALSE)
			{
				UdpServerBufFlag = SERVERFLAGE_IDLESERVER;
			}
		}
		else
		{
			recvbytes = maxrecv = sendbytes = totalsend = 0;
			UdpServerBufFlag = SERVERFLAGE_IDLESERVER;
		}
		if(debug)
		{
			debug->Write(_T("UDP ����"));
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Ŭ���̾�Ʈ Tcp Recv �ݹ� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::SelectTcpClientRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpArg;
	if(mdlg->TcpClientRevcData(buf, recvbytes, maxrecv, sendbytes, totalsend))
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Ŭ���̾�Ʈ Tcp Recv ó�� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::TcpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
	}
	else
	{
		recvbytes = maxrecv = sendbytes = totalsend = 0;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Ŭ���̾�Ʈ Udp Recv �ݹ� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::SelectUdpClientRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpArg;
	if(mdlg->UdpClientRevcData(buf, recvbytes, maxrecv, sendbytes, totalsend))
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Ŭ���̾�Ʈ Udp Recv ó�� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::UdpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
	}
	else
	{
		recvbytes = maxrecv = sendbytes = totalsend = 0;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|dlg �ʱ�ȭ 
//|
//---------------------------------------------------------------
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND ����
	//========================================
	hWnd = _hWnd;
	//========================================
	// Rgn���� ���� �׸���
	//========================================
	//
	fontinfo.lfHeight   = 21;
	fontinfo.lfWeight   = 0;
	fontinfo.lfEscapement  = 0;
	fontinfo.lfOrientation  = 0;
	fontinfo.lfWeight   = FW_BOLD;
	fontinfo.lfItalic   = FALSE;
	fontinfo.lfUnderline  = FALSE;
	fontinfo.lfStrikeOut  = FALSE;
	fontinfo.lfCharSet   = DEFAULT_CHARSET;
	fontinfo.lfOutPrecision = OUT_DEFAULT_PRECIS;
	fontinfo.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	fontinfo.lfQuality   = DEFAULT_QUALITY;
	fontinfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName),_T("����ü"));
	fontcolor=RGB(255,0,0);
	
	//��׶��� �̹���
	CGdiPlusBitmapResource LoadBitmap;
	//�̹��� �ε�
	LoadBitmap.Load(IDB_JPG2,"JPG");
	//��Ʈ�� ���Ϸκ���
	MainGdiDlgbitmap(LoadBitmap.GetWinBitmap());
	LoadBitmap.Empty();

	CRect rc;
	GetClientRect(hWnd, &rc);
	//Tcp Server ��ư
	 LoadBitmap.Load(IDB_PNG1,"PNG");
	TcpServerBut.Create(hWnd, CRect(rc.Width()-150, 50, 0, 0),_T("TCP����"), WS_CHILD | WS_VISIBLE , 0, IDC_TCPSERVER);
	TcpServerBut.initSuperButton(LoadBitmap.GetWinBitmap());
	TcpServerBut.SetTransperent(RGB(100,100,100));
	//Udp Server ��ư
	UdpServerBut.Create(hWnd, CRect(rc.Width()-150, 80, 0, 0),_T("UDP����"), WS_CHILD | WS_VISIBLE , 0, IDC_UDPSERVER);
	UdpServerBut.initSuperButton(LoadBitmap.GetWinBitmap());
	UdpServerBut.SetTransperent(RGB(100,100,100));
	LoadBitmap.Empty();

	//�ڽ��� �̹���
	LoadBitmap.Load(IDB_TEXTBACK1);
	//��Ʈ �� ����
	COLORREF color = RGB(255,0,0);
	//Tcp ���� ���� �ؽ�Ʈ
	TcpServerText.Create(hWnd, CRect(rc.Width()-280, 50, 0, 0),_T("�� ��"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	TcpServerText.initSuperTextStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//��Ʈ ���� ��Ÿ�� ����(ũ��, ����ü, ��)
	TcpServerText.SetSuperTextStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));

	//Udp ���� ���� �ؽ�Ʈ
	UdpServerText.Create(hWnd, CRect(rc.Width()-280, 80, 0, 0),_T("�� ��"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	UdpServerText.initSuperTextStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//��Ʈ ���� ��Ÿ�� ����(ũ��, ����ü, ��)
	UdpServerText.SetSuperTextStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));
	LoadBitmap.Empty();

	//����
	sensoreventopengl.initOpenGLStatic(new CSensorEventObject());
	sensoreventopengl.Create(hWnd, CRect(20, 80, 600, 800),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	//Ű����
	keyeventopengl.initOpenGLStatic(new CKeyEventObject());
	keyeventopengl.Create(hWnd, CRect(620, 80, 1200, 800),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);

	RegisterHotKey(hWnd, MAINGDIHK_CAPTURESET, MOD_CONTROL, VK_F4);
	RegisterHotKey(hWnd, MAINGDIHK_CAPTURESCREEN, MOD_CONTROL, VK_F3);
	RegisterHotKey(hWnd, MAINGDIHK_CAPTUREWINDOW   , MOD_CONTROL, VK_F2);

	//ȭ�� ��ü ũ�⸦ ��´�
	systemXY.cx  = GetSystemMetrics(SM_CXSCREEN);
	systemXY.cy  = GetSystemMetrics(SM_CYSCREEN);

	//���� ���� �ε�
	sampleGame=NULL;
	sampleGame=GetWindowHandle("Seth");
	BYTE buf[2];
	buf[0]=0x80;
	buf[1]=0x4F;
	UINT uint = (UINT) MAKEWORD(buf[1], buf[0]);
	if(GLKEYUP_RIGHTARROW==uint)
		int iiii= 0;
}
//--------------------------------------------------------------
//|
//|������ �̸����� �ڵ� ��������
//|
//---------------------------------------------------------------
CWindow CMainGdiDlg::GetWindowHandle(CString WinCapName)
{
	HWND tmpHwnd = FindWindow(NULL, NULL);
	CWindow hwin=NULL;
	while(tmpHwnd!=NULL)
	{
		if(::GetParent(tmpHwnd)==NULL) 
		{
			CWindow win = tmpHwnd;
			CString caption;
			win.GetWindowText(caption);
			if(WinCapName.CompareNoCase(caption.Left(WinCapName.GetLength()))==0)
				hwin=tmpHwnd;
		}
		tmpHwnd = ::GetWindow(tmpHwnd, GW_HWNDNEXT);
		if(hwin.IsWindow())
			return hwin;
	}
	return NULL;
}
void CMainGdiDlg::SendMessageGame(UINT nChar)
{
	if(sampleGame==NULL)
		return;
	switch (nChar)
	{
	case GLKEYUP_RIGHTARROW: ::SendMessage(sampleGame, WM_KEYUP, (WPARAM)VK_RIGHT, (LPARAM)0);
		break;
	case GLKEYDOWN_RIGHTARROW: ::SendMessage(sampleGame, WM_KEYDOWN, (WPARAM)VK_RIGHT, (LPARAM)0);
		break;
	case GLKEYDOWN_LEFTARROW: ::SendMessage(sampleGame, WM_KEYDOWN, (WPARAM)VK_LEFT, (LPARAM)0);
		break;
	case GLKEYUP_LEFTARROW: ::SendMessage(sampleGame, WM_KEYUP, (WPARAM)VK_LEFT, (LPARAM)0);
		break;
	case GLKEYDOWN_DOWNARROW:
	case GLKEYDOWN_UPARROW:	::SendMessage(sampleGame, WM_KEYDOWN, (WPARAM)VK_SPACE, (LPARAM)0);
		break;
	case GLKEYUP_DOWNARROW:
	case GLKEYUP_UPARROW: ::SendMessage(sampleGame, WM_KEYUP, (WPARAM)VK_SPACE, (LPARAM)0);
		break;
	}
}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiDlgDrawPaint()
{
	//  Create the memory DC if it doesn't already exist.
	if( ! m_memDC )
	{
		m_memDC = new CMemDC(hWnd);
	}
	//Ŭ���̾�Ʈ ����
	CRect cRect;
	GetClientRect(hWnd, &cRect );
	//m_memDC->FillSolidRect( &cRect, GetSysColor( CTLCOLOR_DLG ) );
	//  Generate the button using the memory DC.
	DrawTextStatic(m_memDC, cRect);
	m_memDC->Paint( );
}
//--------------------------------------------------------------
//|
//|Image �׸���(��׶��� ó��)
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawTextStatic(CDC * pDC,const CRect rc)
{
	if(maingdibackgrandimage==NULL)
		return;
	// create memory DC
	pDC->SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC->m_hDC);
	CBitmap pOldBitmap;

	SIZE bmSize;
	maingdibackgrandimage.GetSize(bmSize);
	int oldMode = pDC->SetStretchBltMode(HALFTONE);
	pOldBitmap = pMemDC -> SelectBitmap(maingdibackgrandimage);
	pDC -> StretchBlt(0,0,rc.Width(), rc.Height(),pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy,SRCCOPY);
	pDC->SetStretchBltMode(oldMode);
	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;
	//CGdiPlusBitmap::OnHbitmapSaveasJpg(L"backgraund.jpg", L"image/jpeg",(HBITMAP)CopyImage((HANDLE)maingdibackgrandimage, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG));
}
//--------------------------------------------------------------
//|
//|�̹��� ����
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::MainGdiDlgbitmap(CBitmap hbit)
{
	if(hbit)
	{
		SIZE bmSize;
		if(hbit.GetSize(bmSize))
		{
			LONG width=bmSize.cx;
			LONG Height=bmSize.cy;
			BITMAPINFOHEADER Header32Bit;
			::memset(&Header32Bit, 0, sizeof(Header32Bit));
			Header32Bit.biSize   = sizeof(BITMAPINFOHEADER);
			Header32Bit.biWidth   = width;
			Header32Bit.biHeight  = Height;
			Header32Bit.biPlanes  = 1;
			Header32Bit.biBitCount  = 32;
			Header32Bit.biCompression = BI_RGB;

			HDC hdc = GetDC(hWnd);
			HDC hMemDC = ::CreateCompatibleDC(hdc);
			HDC hBmpDC = ::CreateCompatibleDC(hMemDC);
			CBitmap hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hbit);
			void *pBmpBits = NULL;
			if(maingdibackgrandimage.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
			{
				BITMAP bmpInfo32;
				::GetObject(maingdibackgrandimage, sizeof(bmpInfo32), &bmpInfo32);
				if(bmpInfo32.bmWidthBytes % 4 > 0)
					bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
				CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, maingdibackgrandimage);
				::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hBmpDC, 0, 0, width,Height, SRCCOPY);
				::SelectObject(hMemDC, hOldBmp);
			}
			::SelectObject(hBmpDC, hOldBmp2);
			::DeleteDC(hBmpDC);
			::DeleteDC(hMemDC);
			::DeleteDC(hdc);
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|��ư�� ���̴� �̹��� ����
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGidDlgImageClear()
{
	DeleteObject(maingdibackgrandimage);
}
//--------------------------------------------------------------
//|
//| ��ư Ŭ�� �̺�Ʈ �޽���
//|
//---------------------------------------------------------------
CString loopbackip = _T("127.0.0.1");
BOOL CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	BOOL b=FALSE;
	switch(nID)
	{
	case IDC_TCPSERVER:
		{
			// ���� ����
			if(selectserver->CreateTcpServer(9001,SelectTcpServerRecvFunc,(LPVOID)this))
			{
				TcpServerText.SetText(_T("���� �۵�"));
				CString myip;
				myip=selectserver->GetLocalServerIp();
				TcpServerBut.EnableWindow(FALSE);
				b=TRUE;
			}
		}break;
	case IDC_UDPSERVER:
		{
			// ���� ����
			if(selectserver->CreateUdpServer(9000,SelectUdpServerRecvFunc,(LPVOID)this))
			{
				UdpServerText.SetText(_T("���� �۵�"));
				CString myip;
				myip=selectserver->GetLocalServerIp();
				UdpServerBut.EnableWindow(FALSE);
				b=TRUE;
			}
		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//| ���콺 �̺�Ʈ �ޱ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::MouseEventMessage(UINT Msgtype, CPoint cuserpos)
{
	BOOL b=FALSE;
	switch(Msgtype)
	{
	case WM_RBUTTONDOWN:
		{
		}break;
	case WM_MOUSEMOVE:
		{
		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//| Hot Key Message
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::HotKeyMessage(UINT KeyId)
{
	BOOL b=FALSE;
	switch(KeyId)
	{
	case MAINGDIHK_CAPTURESET:
		{
			b=TRUE;
		}break;
	case MAINGDIHK_CAPTURESCREEN:
		{
			b=TRUE;
		}break;
	case MAINGDIHK_CAPTUREWINDOW:
		{
			b=TRUE;
		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//| cursor Message
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::MouseCursorMessage(CWindow wnd, UINT nHitTest, UINT message)
{
	BOOL b=FALSE;
	static HCURSOR hcur = LoadCursor (GetModuleHandle(NULL),MAKEINTRESOURCE(IDC_HANDCURSOR));
	static HCURSOR waithcur = LoadCursor (NULL,MAKEINTRESOURCE(IDC_WAIT));
	//���� ���콺 ����Ʈ ��ġ
	memset(&Mousept, 0, sizeof(CPoint));
	GetCursorPos(&Mousept);
	//�׸� ������ ���콺 �ִ°�
	if(NULL != hcur)
	{
		SetCursor ( hcur );		b = TRUE;
	}
	else
	{
		b = FALSE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//| Key Message
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::VirtualKeyMessage(UINT Msgtype, UINT nChar, UINT nRepCnt)
{
	if(sampleGame)
		::SendMessage(sampleGame, Msgtype, (WPARAM)nChar, (LPARAM)0);
	BOOL b=FALSE;
	if(Msgtype==WM_KEYDOWN)
	{
		switch(nChar)
		{
		case VK_LEFT: keyeventopengl.SetVirtualKey(GLKEYDOWN_LEFTARROW); b=TRUE;
			break;
		case VK_UP: keyeventopengl.SetVirtualKey(GLKEYDOWN_UPARROW); b=TRUE;
			break;
		case VK_RIGHT: keyeventopengl.SetVirtualKey(GLKEYDOWN_RIGHTARROW); b=TRUE;
			break;
		case VK_DOWN: keyeventopengl.SetVirtualKey(GLKEYDOWN_DOWNARROW); b=TRUE;
			break;
		}
	}
	else if(Msgtype == WM_KEYUP)
	{
		switch(nChar)
		{
		case VK_LEFT:
		case VK_UP:
		case VK_RIGHT:
		case VK_DOWN: keyeventopengl.SetVirtualKey(0); b=TRUE;
			break;
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|������ �ٽ� �׸���
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiinvalidateRect(CRect* Rc, BOOL bErase)
{
	InvalidateRect(hWnd, Rc, bErase);
	//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
	UpdateWindow(hWnd);
}

BOOL CMainGdiDlg::CtlColorStatic(CDCHandle dc, CStatic wndStatic, HBRUSH& hbrush)
{
	hbrush=0;

	return FALSE;
}
//--------------------------------------------------------------
//|
//|char�� hex�� ��ȯ
//|
//---------------------------------------------------------------
CString CMainGdiDlg::Changcharbyhex(CString pass)
{
	CString retvalue="";
	CString temp;
	char *a = pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format(_T("%02X"), a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|ī��� ���� ���� ����Ÿ�� ��ȯ �Ѵ�(0x00~0xFF���� �̹Ƿ� ��Ʈ�������� ��ȯ�ؼ� �Ǵ��Ѵ�)
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::ChangBytebyhex(char* pass, DWORD len,  CString& repass)
{
	for( int i = 0; i < (int)len; i++ )
	{
		char temp[3]={0,};
		//temp.Format("%02X", pass[i]);
		sprintf_s(temp,3,_T("%02X"),pass[i]&0xFF);
		repass += temp;
	}
	if(len==(repass.GetLength()/2))
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------
//|
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
//|
//---------------------------------------------------------------
int  CMainGdiDlg::ChangeHexCord(CString data, char hex[])
{
	memset(hex, 0, sizeof(hex));
	int dwtransLen=data.GetLength()/2;
	for(int i = 0 ; i < dwtransLen ; i++ )
	{
		hex[i] = (((char)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			hex[i] += 0x90;

		hex[i] |= ((char)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			hex[i] += 0x09;
	}
	return dwtransLen;
}

void CMainGdiDlg::ModifyStyle( HWND hwnd, UINT Add, UINT Remove, BOOL bRedraw )
{
	// ������ ������ ��Ÿ���� ��´�.
	UINT style = GetWindowLong( hwnd, GWL_STYLE );
	// ��Ÿ���� �����Ѵ�.
	style = style | Add;
	style = style & ~Remove;
	// ����� ��Ÿ���� Window Object �� �ִ´�.
	SetWindowLong( hwnd, GWL_STYLE, style);
	// ��Ÿ���� ������ ��� �ٽ� �׸��� �Ѵ�.
	if ( bRedraw )
		SetWindowPos( hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
}
//--------------------------------------------------------------
//|
//|�ֻ��� ������ Capture Bitmap �����
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::MakeBitmapFocusWindowCapture(HWND hwnd, CBitmapHandle& captureImg)
{
	if(!hwnd) return FALSE;
	BOOL b=FALSE;
	CDC hScrDC;
	CDC memdc;
	CRect rc;

	CWindow ForegroundWindow = GetForegroundWindow();
	ForegroundWindow.GetWindowRect( &rc);
	ForegroundWindow.ScreenToClient(&rc);
	ForegroundWindow.ClientToScreen(&rc);
	rc.left = max(0,(int)rc.left);
	rc.top = max(0,(int)rc.top);
	rc.right = min((int)rc.right, GetSystemMetrics(SM_CXSCREEN));
	rc.bottom = min((int)rc.bottom, GetSystemMetrics(SM_CYSCREEN));

	//ȭ�� ��ü DC�� ��´�
	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	memdc.CreateCompatibleDC(hScrDC);
	CDC hMemDC;
	BITMAPINFOHEADER Header32Bit;
	::memset(&Header32Bit, 0, sizeof(Header32Bit));
	Header32Bit.biSize   = sizeof(BITMAPINFOHEADER);
	Header32Bit.biWidth   = rc.Width();
	Header32Bit.biHeight  = rc.Height();
	Header32Bit.biPlanes  = 1;
	Header32Bit.biBitCount  = 32;
	Header32Bit.biCompression = BI_RGB;

	hMemDC.CreateCompatibleDC(hScrDC);
	void *pBmpBits = NULL;
	if(captureImg.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
	{
		BITMAP bmpInfo32;
		::GetObject(captureImg, sizeof(bmpInfo32), &bmpInfo32);
		if(bmpInfo32.bmWidthBytes % 4 > 0)
			bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
		CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, captureImg);
		::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hScrDC, rc.left, rc.top, rc.Width(),rc.Height(), SRCCOPY);
		::SelectObject(hMemDC, hOldBmp);
		b=TRUE;
	}
	hMemDC.DeleteDC();
	hScrDC.DeleteDC();
	return b;
}
//--------------------------------------------------------------
//|
//|��üȭ�� Capture Bitmap �����
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::MakeBitmapFullScreenCapture(HWND hwnd, CBitmapHandle& captureImg)
{
	if(!hwnd) return FALSE;
	BOOL b=FALSE;
	CDC hScrDC;
	CDC memdc;

	int ScreenWidth =  GetSystemMetrics(SM_CXSCREEN);
	int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	//ȭ�� ��ü DC�� ��´�
	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	memdc.CreateCompatibleDC(hScrDC);
	CDC hMemDC;
	BITMAPINFOHEADER Header32Bit;
	::memset(&Header32Bit, 0, sizeof(Header32Bit));
	Header32Bit.biSize   = sizeof(BITMAPINFOHEADER);
	Header32Bit.biWidth   = ScreenWidth;
	Header32Bit.biHeight  = ScreenHeight;
	Header32Bit.biPlanes  = 1;
	Header32Bit.biBitCount  = 32;
	Header32Bit.biCompression = BI_RGB;

	hMemDC.CreateCompatibleDC(hScrDC);
	void *pBmpBits = NULL;
	if(captureImg.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
	{
		BITMAP bmpInfo32;
		::GetObject(captureImg, sizeof(bmpInfo32), &bmpInfo32);
		if(bmpInfo32.bmWidthBytes % 4 > 0)
			bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
		CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, captureImg);
		::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hScrDC, 0, 0, ScreenWidth,ScreenHeight, SRCCOPY);
		::SelectObject(hMemDC, hOldBmp);
		b=TRUE;
	}
	hMemDC.DeleteDC();
	hScrDC.DeleteDC();
	return b;
}
//--------------------------------------------------------------
//|
//|���� �̹��� ���� �ε�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::SendImageFileLoading(CString file)
{
	if(sendfilepointer)
	{
		if(debug)
		{
			debug->Write(_T("����������.."));
		}
		return FALSE;
	}
	LPBYTE bytebuf;
	DWORD bytelen;
	if(!GAFileStream::CreateByteReadFile(file, bytebuf, bytelen))
	{
		return FALSE;
	}
	sendfilepointer=(char*)bytebuf;
	sendfiletotallen=(UINT)bytelen;
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������ �����Ѵ�
//|
//---------------------------------------------------------------
//������ �����Ѵ�
BOOL CMainGdiDlg::ServerStartCall()
{
	BOOL b= FALSE;
	// TCP ���� ����
	if(selectserver->CreateTcpServer(9001,SelectTcpServerRecvFunc,(LPVOID)this))
	{
		TcpServerText.SetText(_T("���� �۵�"));
		CString myip;
		myip=selectserver->GetLocalServerIp();
		TcpServerBut.EnableWindow(FALSE);
		// UDP ���� ����
		if(selectserver->CreateUdpServer(9000,SelectUdpServerRecvFunc,(LPVOID)this))
		{
			UdpServerText.SetText(_T("���� �۵�"));
			CString myip;
			myip=selectserver->GetLocalServerIp();
			UdpServerBut.EnableWindow(FALSE);
			//���� ���� �۵�
			b=TRUE;
		}
	}
// 	char fdata[]={0,0,96, 64};
// 	float ftemp = 3.141f;
// 	float *ftemp2 = (float*)fdata;
// 	UINT* utemp = (UINT*)fdata;
// 	int* itemp = (int*)fdata;
// 	if(debug)
// 	{
// 		debug->Write(_T("%d, 0x%08x, %3.3f, %3.3f"),*itemp, *utemp, ftemp, *(float*)utemp);
// 	}
	return b;
}