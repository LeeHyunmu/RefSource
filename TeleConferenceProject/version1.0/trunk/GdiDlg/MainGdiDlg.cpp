#include "stdAfx.h"
#include "MainGdiDlg.h"

#define MAX_BUF 1024

//Hot Key define
#define MAINGDIHK_CAPTURESET		1
#define MAINGDIHK_CAPTURESCREEN		2
#define MAINGDIHK_CAPTUREWINDOW		3

//--------------------------------------------------------------
//|
//|생성자
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
TcpServerBufFlag(SENDFLAGE_IDLESERVER),
TcpClientBufFlag(SENDFLAGE_IDLESERVER),
UdpServerBufFlag(SENDFLAGE_IDLESERVER),
UdpClientBufFlag(SENDFLAGE_IDLESERVER),
bBroadcast(FALSE)
{}

//--------------------------------------------------------------
//|
//|소멸자
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
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::ControlDestroy()
{
	//버튼 메모리 삭제
	TcpServerBut.DestroyWindow();
	UdpServerBut.DestroyWindow();
	TcpClientBut.DestroyWindow();
	UdpClientBut.DestroyWindow();
	UdpBroadcastBut.DestroyWindow();
	viewchange.DestroyWindow();
	firstImageselect.DestroyWindow();
	nextImageselect.DestroyWindow();
	viewImagesend.DestroyWindow();
	//CStatic 클래스(텍스트 출력) 삭제
	TcpServerText.DestroyWindow();
	UdpServerText.DestroyWindow();
	TcpClientText.DestroyWindow();
	UdpClientText.DestroyWindow();
	UdpBroadcastText.DestroyWindow();
	//ip show
	ipshowText.DestroyWindow();
	//이미지 출력
	//자신의이미지
	myImage.DestroyWindow();
	//받은 이미지
	revcImage.DestroyWindow();
	//백그라운드 이미지 삭제
	MainGidDlgImageClear();

	//파일 로딩 버퍼 메모리 해제
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
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// 오브젝트 관리 생성 관리 클래스
	//========================================
	obj=_obj;
	//========================================
	// 디버그 출력 클래스
	//========================================
	debug=obj->GetOutputObject();
	//========================================
	// Select Server 클래스
	//========================================
	selectserver = obj->GetSelectServerObject();
}
//--------------------------------------------------------------
//|
//|서버 Tcp Recv 콜백 함수
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
//|서버 Tcp Recv 처리 함수
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::TcpServerRevcData(SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
		if(TcpServerBufFlag == SENDFLAGE_FILEDATASENDENDOK)
		{
			TcpServerBufFlag=SENDFLAGE_IDLESERVER;
		}
	}
	else
	{
  		if(buf[0]==SENDFLAGE_FILEDATASEND)
 		{
			UINT _revclen = recvbytes-1;
			if(_revclen==maxrecv-1||(recvfiletotallen==recvfilelen+_revclen))
			{
				memcpy(&recvfilepointer[recvfilelen],&buf[1],_revclen);
				recvbytes = maxrecv = sendbytes = totalsend = 0;
				recvfilelen+=_revclen;
				if(recvfiletotallen==recvfilelen)
				{
					CBitmap bit = CGdiPlusBitmap::LoadImageFromDataJpg(recvfilepointer, recvfiletotallen);
					if(bit)
						revcImage.AddImageServerRecvImgMoveStatic(recvfilename, bit);
					//전송이 완료 되었으면 보낸 데이터 정보를 모두 삭제한다
					delete[] recvfilepointer; recvfilepointer=NULL;
					recvfiletotallen=recvfilelen=0;
					recvfilename=_T("");

					myImage.ShowWindow(SW_HIDE);
					revcImage.ShowWindow(SW_HIDE);
					AnimateWindow(revcImage, 500, AW_SLIDE|AW_HOR_POSITIVE); bMyShow=FALSE;
					if(debug)
					{
						debug->Write(_T("완료revctotal:%d"), recvfiletotallen);
					}

				}
			}
		}
		else if(buf[0]==SENDFLAGE_MOUSEDATA)
		{
			//상대에게서 받은 이미지를 보고 있을 때
			if(bMyShow==FALSE)
			{
				UINT filepos=0;
				CPoint Mousept;
				memcpy(&Mousept,&buf[1],sizeof(CPoint));
				filepos=1+sizeof(CPoint);
				CSize _systemXY;
				memcpy(&_systemXY, &buf[filepos], sizeof(CSize));
				SetCursorPos((int)(Mousept.x*(systemXY.cx/(float)_systemXY.cx)),(int)( Mousept.y*(systemXY.cy/(float)_systemXY.cy)));
			}
			recvbytes = maxrecv = sendbytes = totalsend = 0;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|서버 Udp Recv 콜백 함수
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
//|서버 Udp Recv 처리 함수
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::UdpServerRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
		if(UdpServerBufFlag == SENDFLAGE_FILERECVOK)
		{
			buf[0]=SENDFLAGE_FILERECVOK;
			memcpy(&buf[1], recvfilename.GetBuffer(recvfilename.GetLength()), recvfilename.GetLength());
			totalsend=1+recvfilename.GetLength();
			UdpServerBufFlag=SENDFLAGE_IDLESERVER;
		}
		else if(UdpServerBufFlag == SENDFLAGE_FILERECVON)
		{
			buf[0]=SENDFLAGE_FILERECVON;
			memcpy(&buf[1], recvfilename.GetBuffer(recvfilename.GetLength()), recvfilename.GetLength());
			totalsend=1+recvfilename.GetLength();
			UdpServerBufFlag=SENDFLAGE_IDLESERVER;
		}
		else if(UdpServerBufFlag == SENDFLAGE_FILERECVIPSEND)
		{
			CString ip;
			ip=selectserver->GetLocalServerIp();
			buf[0]=SENDFLAGE_FILERECVIPSEND;
			memcpy(&buf[1], ip.GetBuffer(ip.GetLength()), ip.GetLength());
			totalsend=1+ip.GetLength();
			UdpServerBufFlag=SENDFLAGE_IDLESERVER;
		}
	}
	else
	{
		if(buf[0]==SENDFLAGE_FILERECVINFO)
		{
			memcpy((void*)&recvfiletotallen,&buf[1],sizeof(UINT));
			recvfilename= &buf[5];
			char _recvname[MAX_PATH]={0,};
			memcpy(&_recvname[0],&buf[5],recvbytes-5);
			recvfilename=_recvname;
			//리시브 할 파일이 있는가 검사
			if(revcImage.RecvImageFileFind(recvfilename))
			{
				UdpServerBufFlag = SENDFLAGE_FILERECVOK;
				myImage.ShowWindow(SW_HIDE);
				revcImage.ShowWindow(SW_HIDE);
				AnimateWindow(revcImage, 500, AW_SLIDE|AW_HOR_POSITIVE); bMyShow=FALSE;
			}
			else
			{
				//파일이 없을 경우 받을 메모리를 할당하고 파일 전송을 요청한다
				recvfilepointer=new char[recvfiletotallen+1];
				memset(recvfilepointer, 0, recvfiletotallen+1);
				UdpServerBufFlag = SENDFLAGE_FILERECVON;
			}
			recvbytes = maxrecv = sendbytes = totalsend = 0;
			if(selectserver->UdpServerSend()==FALSE)
			{
				UdpServerBufFlag = SENDFLAGE_IDLESERVER;
			}
		}
		else if(buf[0]==SENDFLAGE_FILERECVIPSEND)
		{
			char ip[16]={0,};
			UINT _revclen = recvbytes-1;
			memcpy(&ip[0],&buf[1],_revclen);
			CString myip;
			myip=selectserver->GetLocalServerIp();
			if(myip.Compare(ip)!=0)
			{
				UdpServerBufFlag = SENDFLAGE_FILERECVIPSEND;
				recvbytes = maxrecv = sendbytes = totalsend = 0;
				bBroadcast = ClientConnect(ip);
				if(selectserver->UdpServerSend()==FALSE)
				{
					UdpServerBufFlag = SENDFLAGE_IDLESERVER;
				}
				m_ip_show.SetWindowText(ip);
			}
			else
			{
				recvbytes = maxrecv = sendbytes = totalsend = 0;
			}
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|클라이언트 Tcp Recv 콜백 함수
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
//|클라이언트 Tcp Recv 처리 함수
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::TcpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
		if(TcpClientBufFlag==SENDFLAGE_FILEDATASEND)
		{
			Sleep(10);
			buf[0]=SENDFLAGE_FILEDATASEND;
			UINT _sendlen = sendfiletotallen - sendfilelen;
			UINT _totalsend = totalsend-1;
			if(_sendlen>=_totalsend)
			{
				memcpy(&buf[1], &sendfilepointer[sendfilelen], _totalsend);
				sendfilelen+=_totalsend;
				return TRUE;
			}
			else
			{
				memcpy(&buf[1], &sendfilepointer[sendfilelen], _sendlen);
				sendfilelen+=_sendlen;
				totalsend=_sendlen+1;
				//전송이 완료 되었으면 보낸 데이터 정보를 모두 삭제한다
				delete[] sendfilepointer; sendfilepointer=NULL;
				sendfiletotallen = sendfilelen =0;
				sendfilename=_T("");
				TcpClientBufFlag=SENDFLAGE_IDLESERVER;
			}
		}
		else if(TcpClientBufFlag==SENDFLAGE_MOUSEDATA)
		{
			//마우스 데이타 전송
			buf[0]=SENDFLAGE_MOUSEDATA;
			memcpy(&buf[1], &Mousept, sizeof(CPoint));
			totalsend=1+sizeof(CPoint);
			//해상도 전송
			memcpy(&buf[totalsend], &systemXY, sizeof(CSize));
			totalsend=totalsend+sizeof(CSize);
			TcpClientBufFlag=SENDFLAGE_IDLESERVER;
		}
	}
	else
	{
		if(debug)
		{
			CString strrecv;
			debug->Write(_T("TcpClientRcv데이터:%s"), strrecv);
		}
		recvbytes = maxrecv = sendbytes = totalsend = 0;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|클라이언트 Udp Recv 콜백 함수
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
//|클라이언트 Udp Recv 처리 함수
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::UdpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend)
{
	if(totalsend)
	{
		if(UdpClientBufFlag==SENDFLAGE_GENDATA)
		{
			UdpClientBufFlag=SENDFLAGE_IDLESERVER;
		}
		else if(UdpClientBufFlag==SENDFLAGE_FILERECVINFO)
		{
			buf[0]=SENDFLAGE_FILERECVINFO;
			memcpy(&buf[1],(void*)&sendfiletotallen,sizeof(UINT));
			memcpy(&buf[5], sendfilename.GetBuffer(sendfilename.GetLength()), sendfilename.GetLength());
			totalsend=1+sizeof(UINT)+sendfilename.GetLength();
			UdpClientBufFlag=SENDFLAGE_IDLESERVER;
		}
	}
	else
	{
		if(buf[0] == SENDFLAGE_FILERECVOK)
		{
			//전송할 파일이 이미 있다는 메시지를 받았으면 전송 정보를 모두 삭제한다
			delete[] sendfilepointer; sendfilepointer=NULL;
			sendfiletotallen = sendfilelen =0;
			sendfilename=_T("");

			CString str = buf[1];
			if(debug)
				debug->Write(_T("%s파일이 있다"), str.GetBuffer(0));

		}
		else if(buf[0] == SENDFLAGE_FILERECVON)
		{
			TcpClientBufFlag=SENDFLAGE_FILEDATASEND;
			if(selectserver->TcpClientSend()==FALSE)
			{
				TcpClientBufFlag=SENDFLAGE_IDLESERVER;
			}
			CString str = buf[1];
			if(debug)
				debug->Write(_T("%s파일이 없다"), str.GetBuffer(0));
		}
		recvbytes = maxrecv = sendbytes = totalsend = 0;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================
	// Rgn으로 원을 그린다
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
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName),_T("굴림체"));
	fontcolor=RGB(255,0,0);
	
	//백그라운드 이미지
// 	CGdiPlusBitmapResource pbackgrandBitmap11;
// 	//이미지 로드
// 	pbackgrandBitmap11.Load(IDB_BITMAP2);
// 	pbackgrandBitmap11.Empty();

	//백그라운드 이미지
	CGdiPlusBitmapResource LoadBitmap;
	//이미지 로드
	LoadBitmap.Load(IDB_JPG2,"JPG");
	//비트맵 파일로변경
	MainGdiDlgbitmap(LoadBitmap.GetWinBitmap());
	LoadBitmap.Empty();

	CRect rc;
	GetClientRect(hWnd, &rc);
	//Tcp Server 버튼
	 LoadBitmap.Load(IDB_PNG1,"PNG");
	TcpServerBut.Create(hWnd, CRect(rc.Width()-150, 50, 0, 0),_T("TCP서버"), WS_CHILD | WS_VISIBLE , 0, IDC_TCPSERVER);
	TcpServerBut.initSuperButton(LoadBitmap.GetWinBitmap());
	TcpServerBut.SetTransperent(RGB(100,100,100));
	//Udp Server 버튼
	UdpServerBut.Create(hWnd, CRect(rc.Width()-150, 80, 0, 0),_T("UDP서버"), WS_CHILD | WS_VISIBLE , 0, IDC_UDPSERVER);
	UdpServerBut.initSuperButton(LoadBitmap.GetWinBitmap());
	UdpServerBut.SetTransperent(RGB(100,100,100));
	//Tcp Client 버튼
	TcpClientBut.Create(hWnd, CRect(rc.Width()-150, 110, 0, 0),_T("TCP클라이언트"), WS_CHILD | WS_VISIBLE , 0, IDC_TCPCLIENT);
	TcpClientBut.initSuperButton(LoadBitmap.GetWinBitmap());
	TcpClientBut.SetTransperent(RGB(100,100,100));
	//Udp Client 버튼
	UdpClientBut.Create(hWnd, CRect(rc.Width()-150, 140, 0, 0),_T("UDP클라이언트"), WS_CHILD | WS_VISIBLE , 0, IDC_UDPCLIENT);
	UdpClientBut.initSuperButton(LoadBitmap.GetWinBitmap());
	UdpClientBut.SetTransperent(RGB(100,100,100));
	//Udp Broadcast 버튼
	UdpBroadcastBut.Create(hWnd, CRect(rc.Width()-150, 170, 0, 0),_T("UDP브로드케스트"), WS_CHILD | WS_VISIBLE , 0, IDC_UDPBROADCAST);
	UdpBroadcastBut.initSuperButton(LoadBitmap.GetWinBitmap());
	UdpBroadcastBut.SetTransperent(RGB(100,100,100));

	//이미지 뷰어 전환 버튼
	viewchange.Create(hWnd, CRect(rc.left+20, rc.top+20, 0, 0),_T("전환"), WS_CHILD | WS_VISIBLE , 0, IDC_VIEWCHANGE);
	viewchange.initSuperButton(LoadBitmap.GetWinBitmap());
	viewchange.SetTransperent(RGB(100,100,100));

	//뷰어 이미지 이동(처음으로)
	firstImageselect.Create(hWnd, CRect(rc.left+150, rc.top+20, 0, 0),_T("처음으로"), WS_CHILD | WS_VISIBLE , 0, IDC_VIEWFIRSTIMAGE);
	firstImageselect.initSuperButton(LoadBitmap.GetWinBitmap());
	firstImageselect.SetTransperent(RGB(100,100,100));
	//뷰어 이미지 이동(다음으로)
	nextImageselect.Create(hWnd, CRect(rc.left+280, rc.top+20, 0, 0),_T("다음으로"), WS_CHILD | WS_VISIBLE , 0, IDC_VIEWNEXTIMAGE);
	nextImageselect.initSuperButton(LoadBitmap.GetWinBitmap());
	nextImageselect.SetTransperent(RGB(100,100,100));
	//뷰어 이미지 전송
	viewImagesend.Create(hWnd, CRect(rc.left+410, rc.top+20, 0, 0),_T("이미지전송"), WS_CHILD | WS_VISIBLE , 0, IDC_VIEWIMAGESEND);
	viewImagesend.initSuperButton(LoadBitmap.GetWinBitmap());
	viewImagesend.SetTransperent(RGB(100,100,100));
	viewImagesend.EnableWindow(FALSE);
	LoadBitmap.Empty();

	//자신의 이미지
	LoadBitmap.Load(IDB_TEXTBACK1);
	//폰트 색 지정
	COLORREF color = RGB(255,0,0);
	//Tcp 서버 상태 텍스트
	TcpServerText.Create(hWnd, CRect(rc.Width()-280, 50, 0, 0),_T("준 비"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	TcpServerText.initSuperTextStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//폰트 정보 스타일 수정(크기, 글자체, 색)
	TcpServerText.SetSuperTextStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));

	//Udp 서버 상태 텍스트
	UdpServerText.Create(hWnd, CRect(rc.Width()-280, 80, 0, 0),_T("준 비"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	UdpServerText.initSuperTextStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//폰트 정보 스타일 수정(크기, 글자체, 색)
	UdpServerText.SetSuperTextStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));

	//Tcp 클라이언트 상태 텍스트
	TcpClientText.Create(hWnd, CRect(rc.Width()-280, 110, 0, 0),_T("준 비"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	TcpClientText.initSuperTextStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//폰트 정보 스타일 수정(크기, 글자체, 색)
	TcpClientText.SetSuperTextStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));

	//Udp 서버 상태 텍스트
	UdpClientText.Create(hWnd, CRect(rc.Width()-280, 140, 0, 0),_T("준 비"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	UdpClientText.initSuperTextStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//폰트 정보 스타일 수정(크기, 글자체, 색)
	UdpClientText.SetSuperTextStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));

	//Udp 브로드케스트 상태 텍스트
	UdpBroadcastText.Create(hWnd, CRect(rc.Width()-280, 170, 0, 0),_T("준 비"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	UdpBroadcastText.initSuperStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//폰트 정보 스타일 수정(크기, 글자체, 색)
	UdpBroadcastText.SetSuperStaticFont(20,_T("Corbel"),&color, &CPoint(30,0));

	//IP 텍스트
	ipshowText.Create(hWnd, CRect(rc.Width()-463, 20, 0, 0),_T("접속 IP"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	ipshowText.initSuperStatic(LoadBitmap.GetWinBitmap(), TRUE);
	//폰트 정보 스타일 수정(크기, 글자체, 색)
	ipshowText.SetSuperStaticFont(30,_T("Corbel"),&color, &CPoint(30,0));
	LoadBitmap.Empty();

// 	//이미지 출력
	//자신의 이미지
	//이미지 로드
	LoadBitmap.Load(IDB_JPG1,"JPG");
	myImage.Create(hWnd, CRect(rc.left+20,rc.top+50,rc.left+(rc.Width()-280), rc.top+(rc.Height()-20)),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	myImage.initSuperImgMoveStatic(_T("LoadImage"), LoadBitmap.GetWinBitmap(), TRUE, FALSE);
	LoadBitmap.Empty();
	LoadBitmap.Load(IDB_JPG4,"JPG");
	myImage.AddImageNoChangeBitmapp(LoadBitmap.GetWinBitmap());
	LoadBitmap.Empty();
	//받은 이미지
	//revcImage;
	LoadBitmap.Load(IDB_BITMAP3);
	revcImage.Create(hWnd, CRect(rc.left+20,rc.top+50,rc.left+(rc.Width()-280), rc.top+(rc.Height()-20)),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	revcImage.initSuperImgMoveStatic(_T("LoadImage"), LoadBitmap.GetWinBitmap(), TRUE, FALSE);
	revcImage.ShowWindow(SW_HIDE);
	LoadBitmap.Empty();

	//폰트 정보 변경(색, 스타일, 크기, 좌표)
	//imgt.SetSuperStaticFont(50,_T("Corbel"),&color, &CPoint(20,0));
	//CGdiPlusBitmap::OnSaveasJpg(L"test.jpg", L"jpg", hWnd);
	RegisterHotKey(hWnd, MAINGDIHK_CAPTURESET, MOD_CONTROL, VK_F4);
	RegisterHotKey(hWnd, MAINGDIHK_CAPTURESCREEN, MOD_CONTROL, VK_F3);
	RegisterHotKey(hWnd, MAINGDIHK_CAPTUREWINDOW   , MOD_CONTROL, VK_F2);

	//IP 컨트롤
	m_ip_show = GetDlgItem(hWnd, IDC_IPCLIENTSHOW);
	m_ip_show.SetWindowPos(NULL,rc.Width()-330, 23,0, 0,SWP_NOSIZE|SWP_SHOWWINDOW);
	m_ip_show.SetAddress(MAKEIPADDRESS(127, 0, 0, 1));

	//화면 전체 크기를 얻는다
	systemXY.cx  = GetSystemMetrics(SM_CXSCREEN);
	systemXY.cy  = GetSystemMetrics(SM_CYSCREEN);
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiDlgDrawPaint()
{
	//  Create the memory DC if it doesn't already exist.
	if( ! m_memDC )
	{
		m_memDC = new CMemDC(hWnd);
	}
	//클라이언트 영역
	CRect cRect;
	GetClientRect(hWnd, &cRect );
	//m_memDC->FillSolidRect( &cRect, GetSysColor( CTLCOLOR_DLG ) );
	//  Generate the button using the memory DC.
	DrawTextStatic(m_memDC, cRect);
	m_memDC->Paint( );
}
//--------------------------------------------------------------
//|
//|Image 그리기(백그라운드 처리)
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
//|이미지 저장
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
//|버튼에 쓰이는 이미지 삭제
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGidDlgImageClear()
{
	DeleteObject(maingdibackgrandimage);
}
//--------------------------------------------------------------
//|
//| 버튼 클릭 이벤트 메시지
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
			// 서버 연결
			if(selectserver->CreateTcpServer(9501,SelectTcpServerRecvFunc,(LPVOID)this))
			{
				TcpServerText.SetText(_T("서버 작동"));
				CString ipaddress;
				m_ip_show.GetWindowText(ipaddress);
				CString myip;
				myip=selectserver->GetLocalServerIp();
				if(loopbackip.Compare(ipaddress)==0)
				{
					m_ip_show.SetWindowText(myip);
				}
				TcpServerBut.EnableWindow(FALSE);
			}
		}break;
	case IDC_UDPSERVER:
		{
			// 서버 연결
			if(selectserver->CreateUdpServer(9502,SelectUdpServerRecvFunc,(LPVOID)this))
			{
				UdpServerText.SetText(_T("서버 작동"));
				CString ipaddress;
				m_ip_show.GetWindowText(ipaddress);
				CString myip;
				myip=selectserver->GetLocalServerIp();
				if(loopbackip.Compare(ipaddress)==0)
				{
					m_ip_show.SetWindowText(myip);
				}
				UdpServerBut.EnableWindow(FALSE);
			}
		}break;
	case IDC_TCPCLIENT:
	case IDC_UDPCLIENT:
		{
			CString ipaddress;
			m_ip_show.GetWindowText(ipaddress);
			CString myip;
			myip=selectserver->GetLocalServerIp();
			if(loopbackip.Compare(ipaddress)!=0&&myip.Compare(ipaddress)!=0)
			{
				// TCP 클라이언트 연결
				if(selectserver->CreateTcpClient( 9501, ipaddress, SelectTcpClientRecvFunc,(LPVOID)this))
				{
					TcpClientText.SetText(_T("접 속"));
					TcpClientBut.EnableWindow(FALSE);
					// UDP 클라이언트 연결
					if(selectserver->CreateUdpClient( 9502, ipaddress, SelectUdpClientRecvFunc,(LPVOID)this))
					{
						UdpClientText.SetText(_T("접 속"));
						UdpClientBut.EnableWindow(FALSE);
						bBroadcast=TRUE;
						//이미지 전송 버튼 활성화
						viewImagesend.EnableWindow(TRUE);
					}
				}
			}
		}break;
	case IDC_UDPBROADCAST:
		{
			//클라이언트 접속 상태임
			if(bBroadcast) break;

			char buf[MAX_PATH];
			UINT msglen;
			CString sendMsg;
			sendMsg=selectserver->GetLocalServerIp();
			buf[0]=SENDFLAGE_FILERECVIPSEND;
			memcpy(&buf[1], sendMsg.GetBuffer(sendMsg.GetLength()), sendMsg.GetLength());
			msglen=1+sendMsg.GetLength();
			// 브로드케스팅
			if(selectserver->Sendbroadcast(9502, buf,msglen, MAX_PATH))
			{
				char ip[16]={0,};
				UINT _revclen = msglen-1;
				memcpy(&ip[0],&buf[1],_revclen);
				m_ip_show.SetWindowText(ip);
				bBroadcast = ClientConnect(ip);
				if(debug)
				{
					debug->Write(_T("BroadcastRecv데이터:%s"), ip);
				}
			}
		}break;
	case IDC_VIEWCHANGE:
		{
			if(bMyShow)
			{
				myImage.ShowWindow(SW_HIDE);
				AnimateWindow(revcImage, 500, AW_SLIDE|AW_HOR_POSITIVE); bMyShow=FALSE;
			}
			else
			{
				revcImage.ShowWindow(SW_HIDE);
				AnimateWindow(myImage, 500, AW_SLIDE|AW_HOR_POSITIVE); bMyShow=TRUE;
			}
		}break;
	case IDC_VIEWFIRSTIMAGE:
		{
			if(bMyShow)
			{
				myImage.ShowFirstImageBitmap();
			}
			else
			{
				revcImage.ShowFirstImageBitmap();
			}
		}break;
	case IDC_VIEWNEXTIMAGE:
		{
			if(bMyShow)
			{
				myImage.ShowNextImageBitmap();
			}
			else
			{
				revcImage.ShowNextImageBitmap();
			}
		}break;
	case IDC_VIEWIMAGESEND:
		{
			if(bMyShow)
			{
				if(!myImage.MakeSendFileJpg(sendfilename))
				{
					break;
				}
				CString fileloadstr = sendfilename+_T(".jpg");
				if(!SendImageFileLoading(fileloadstr))
				{
					break;
				}
				//Udp 클라이언트 버퍼
				UdpClientBufFlag=SENDFLAGE_FILERECVINFO;
				if(selectserver->UdpClientSend()==FALSE)
				{
					UdpClientBufFlag=SENDFLAGE_IDLESERVER;
				}
			}
		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//| 마우스 이벤트 받기
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
			CBitmapHandle bitm;
			if(MakeBitmapFullScreenCapture(hWnd, bitm))
			{
				myImage.AddImageImgMoveStatic(bitm.m_hBitmap);
				bitm.DeleteObject();
				myImage.ShowWindow(SW_HIDE);
				revcImage.ShowWindow(SW_HIDE);
				AnimateWindow(myImage, 500, AW_SLIDE|AW_HOR_POSITIVE); bMyShow=TRUE;
			}
			b=TRUE;
		}break;
	case MAINGDIHK_CAPTUREWINDOW:
		{
			CBitmapHandle bitm;
			if(MakeBitmapFocusWindowCapture(hWnd, bitm))
			{
				myImage.AddImageImgMoveStatic(bitm.m_hBitmap);
				bitm.DeleteObject();
				myImage.ShowWindow(SW_HIDE);
				revcImage.ShowWindow(SW_HIDE);
				AnimateWindow(myImage, 500, AW_SLIDE|AW_HOR_POSITIVE); bMyShow=TRUE;
			}
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
	//현재 마우스 포인트 위치
	memset(&Mousept, 0, sizeof(CPoint));
	GetCursorPos(&Mousept);
	//그림 영역에 마우스 있는가
	if(NULL != waithcur && TcpClientBufFlag==SENDFLAGE_FILEDATASEND)
	{
		SetCursor ( waithcur );		b = TRUE;
	}
	else if ( NULL != hcur&& myImage.MouseCursorArea(Mousept))
	{
		//마우스 좌표를 보낸다
		//나의 이미지를 보여지고 있을 때
// 		if(bMyShow)
// 		{
// 			TcpClientBufFlag=SENDFLAGE_MOUSEDATA;
// 			if(selectserver->TcpClientSend()==FALSE)
// 			{
// 				TcpClientBufFlag=SENDFLAGE_IDLESERVER;
// 			}
// 		}
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
//|강제로 다시 그리기
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiinvalidateRect(CRect* Rc, BOOL bErase)
{
	InvalidateRect(hWnd, Rc, bErase);
	//InvalidateRect를 강제로 바로 실행 하기 위해존제..
	UpdateWindow(hWnd);
}

BOOL CMainGdiDlg::CtlColorStatic(CDCHandle dc, CStatic wndStatic, HBRUSH& hbrush)
{
	hbrush=0;
	if(wndStatic.GetDlgCtrlID()==TcpServerText.GetDlgCtrlID())
	{
		::SetTextColor(dc, RGB(255, 255, 255));
		::SetBkMode(dc, TRANSPARENT);
		hbrush =(HBRUSH) GetStockObject(NULL_BRUSH);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|char를 hex로 변환
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
//|카드로 부터 받은 데이타를 변환 한다(0x00~0xFF까지 이므로 스트링형으로 변환해서 판단한다)
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
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
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
	// 기존의 윈도우 스타일을 얻는다.
	UINT style = GetWindowLong( hwnd, GWL_STYLE );
	// 스타일을 변경한다.
	style = style | Add;
	style = style & ~Remove;
	// 변경된 스타일을 Window Object 에 넣는다.
	SetWindowLong( hwnd, GWL_STYLE, style);
	// 스타일을 변경한 경우 다시 그리게 한다.
	if ( bRedraw )
		SetWindowPos( hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);
}
//--------------------------------------------------------------
//|
//|최상위 윈도우 Capture Bitmap 만들기
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

	//화면 전체 DC를 얻는다
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
//|전체화면 Capture Bitmap 만들기
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

	//화면 전체 DC를 얻는다
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
//|전송 이미지 파일 로딩
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::SendImageFileLoading(CString file)
{
	if(sendfilepointer)
	{
		if(debug)
		{
			debug->Write(_T("파일전송중.."));
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
//|서버를 시작한다
//|
//---------------------------------------------------------------
//서버를 시작한다
BOOL CMainGdiDlg::ServerStartCall()
{
	BOOL b= FALSE;
	// TCP 서버 연결
	if(selectserver->CreateTcpServer(9501,SelectTcpServerRecvFunc,(LPVOID)this))
	{
		TcpServerText.SetText(_T("서버 작동"));
		CString ipaddress;
		m_ip_show.GetWindowText(ipaddress);
		CString myip;
		myip=selectserver->GetLocalServerIp();
		if(loopbackip.Compare(ipaddress)==0)
		{
			m_ip_show.SetWindowText(myip);
		}
		TcpServerBut.EnableWindow(FALSE);
		// UDP 서버 연결
		if(selectserver->CreateUdpServer(9502,SelectUdpServerRecvFunc,(LPVOID)this))
		{
			UdpServerText.SetText(_T("서버 작동"));
			CString ipaddress;
			m_ip_show.GetWindowText(ipaddress);
			CString myip;
			myip=selectserver->GetLocalServerIp();
			if(loopbackip.Compare(ipaddress)==0)
			{
				m_ip_show.SetWindowText(myip);
			}
			UdpServerBut.EnableWindow(FALSE);
			//서버 정상 작동
			b=TRUE;
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|클라이언트를 접속한다(TCP, UDP)
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::ClientConnect(CString _ip)
{
	BOOL b=FALSE;
	// Tcp 클라이언트 연결
	if(selectserver->CreateTcpClient( 9501, _ip, SelectTcpClientRecvFunc,(LPVOID)this))
	{
		TcpClientText.SetText(_T("접 속"));
		TcpClientBut.EnableWindow(FALSE);
		// Udp 클라이언트 연결
		if(selectserver->CreateUdpClient( 9502, _ip, SelectUdpClientRecvFunc,(LPVOID)this))
		{
			UdpClientText.SetText(_T("접 속"));
			UdpClientBut.EnableWindow(FALSE);
			b=TRUE;
			//이미지 전송 버튼 활성화
			viewImagesend.EnableWindow(TRUE);
		}
	}
	return b;
}