#include "stdAfx.h"
#include "MainGdiDlg.h"
#include <imaging.h>
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
//스래드 타임아웃
#define DETECTEVENTTIME 500

#define  IDC_MODECHANGE  301
#define  IDC_HOSPITAL_1  401
#define  IDC_HOSPITAL_2  402
#define  IDC_HOSPITAL_3  403


#define MGFILECOUNT_0 4

TCHAR MgFile0[][64]={_T("\\NandFlash\\PhoneTag\\image\\1.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\2.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\3.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\4.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\5.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\6.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\7.PNG"),
_T("\\NandFlash\\PhoneTag\\image\\8.PNG")
};

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
hWnd(NULL),
MgNum(0),
MgMode(0),
pImgFactory(NULL),
hospitalnum(0),
ButShow(SW_HIDE),
backButShow(SW_HIDE),
strMode3(""),
ImageNumber(0),
bCardConnection(FALSE),
changeTime(0),
imgbackup(0)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::Release()
{

	int ImgSize = modeimage.size();
	for(int mgcount = 0; mgcount<ImgSize; mgcount++)
	{
		IImage* pImage = modeimage[mgcount];
		if(pImage)
		{
			pImage->Release(); pImage=NULL;
		}
	}
	maingdiEvent.SetEvent();
	Sleep(10);
	CloseEvent.WaitForEvent();
}
//파괴
void  CMainGdiDlg::ControlDestroy()
{
	ModeChangeBut.DestroyWindow();
	CloseBut.DestroyWindow();
	HospitalBut_1.DestroyWindow();
	HospitalBut_2.DestroyWindow();
	HospitalBut_3.DestroyWindow();
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
	// 듀얼아이 P2P 클래스
	//========================================
	//dualip2p=obj->GetDualiP2pMgObject();
	//========================================
	// 듀얼아이 
	//========================================
	dualicard=obj->GetDualiCardMgObject();
	//
	changeTime=GetTickCount();
	ThreadMakeObject();
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
	fontinfo.lfHeight   = 50;
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
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]),_T("굴림체"));
	fontcolor=RGB(0,0,0);
//	CBitmap h = ::SHLoadImageFile("shin.jpg"); 

 	pImgFactory = NULL;
 	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER,	IID_IImagingFactory, (void**)&pImgFactory)))
 	{
		IImage* pImage = NULL;
		//파일로부터 이미지 로드
		for(int mgcount = 0; mgcount<MGFILECOUNT_0+MGFILECOUNT_0; mgcount++)
		{
			if(SUCCEEDED(pImgFactory->CreateImageFromFile(MgFile0[mgcount], &pImage)))
			{
				modeimage.push_back(pImage); pImage=NULL;
			}
		}
		pImgFactory->Release(); pImgFactory=NULL;
 	}
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//모드 변경 버튼
	ModeChangeBut.Create(hWnd, CRect(iX-120, 60, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MODECHANGE);
	//이미지(비트맵) 인자로 넘긴다
	ModeChangeBut.initSuperButton(IDB_PNG2,_T("PNG"));
	ModeChangeBut.ShowWindow(SW_HIDE);

	//=============================================
	// 종료버튼
	//=============================================
	CloseBut.Create(hWnd, CRect(iX-120, 20, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//이미지(비트맵) 인자로 넘긴다
	CloseBut.initSuperButton(IDB_PNG1,_T("PNG"));

	// 병원 선택 버튼
	HospitalBut_1.Create(hWnd, CRect(430, 210, 0, 0),_T(""), WS_CHILD | WS_VISIBLE   , 0, IDC_HOSPITAL_1);
	//이미지(비트맵) 인자로 넘긴다
	HospitalBut_1.initSuperButton(IDB_PNG6, _T("PNG"));
	HospitalBut_1.ShowWindow(SW_HIDE);

	//병원 선택 버튼
	HospitalBut_2.Create(hWnd, CRect(430, 280, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_2);
	//이미지(비트맵) 인자로 넘긴다
	HospitalBut_2.initSuperButton(IDB_PNG6, _T("PNG"));
	HospitalBut_2.ShowWindow(SW_HIDE);

	//병원 선택 버튼
	HospitalBut_3.Create(hWnd, CRect(430, 350, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_3);
	//이미지(비트맵) 인자로 넘긴다
	HospitalBut_3.initSuperButton(IDB_PNG6, _T("PNG"));
	HospitalBut_3.ShowWindow(SW_HIDE);

	//
	rcMode3.SetRect(0,0,0,0);
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	CRect rc;
	GetClientRect(hWnd, &rc);
	CRect ImgRc(rc.left, rc.top, rc.right, rc.bottom);
	IImage* pImage = modeimage[ImageNumber];
	if(pImage)
	{
		pImage->Draw(hdc, &ImgRc, NULL);
	}
	DrawWhiteText(&CDC(hdc),rcMode3,strMode3);
	EndPaint(hWnd, &ps);
}

//--------------------------------------------------------------
//|
//| 버튼 클릭 이벤트 메시지
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	BOOL b=FALSE;
	switch(nID)
	{
	case IDC_MODECHANGE:
		{
		}break;
	case IDC_HOSPITAL_1:
		{

		}break;
	case IDC_HOSPITAL_2:
		{

		}break;
	case IDC_HOSPITAL_3:
		{

		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|텍스트 쓴다
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawWhiteText(CDC* pDC, CRect textrc, CString text)
{
	if(text.GetLength())
	{
		CFont font;
		font.CreateFontIndirect(&fontinfo);
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontcolor);
		HFONT hOldFont  = pDC->SelectFont (font);
		pDC->DrawText (text, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);

		pDC->SelectFont (hOldFont);
		pDC->SetBkMode(nOldMode);
	}
}
//--------------------------------------------------------------
//|
//|Mode 3에서 전화번호 텍스트 출력
//|
//---------------------------------------------------------------
void CMainGdiDlg::SetMode3Text(CString text, CRect* rc)
{
	strMode3=text;
	if(rc) rcMode3 = *rc;
	else rcMode3.SetRect(0,0,0,0);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainGdiDlg::ThreadMakeObject()
{
	////////////////////////////////////////////////////////////////
	// 스레드 생성
	maingdiEvent.Create();
	CloseEvent.Create();
	maingdithread.Create(ThreadProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CMainGdiDlg::ThreadProcess(LPVOID lpArg)
{
	CMainGdiDlg *maingdi = (CMainGdiDlg *)lpArg;
	maingdi->MainThreadFunc();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
static CString MainApud = _T("00D60000");
#define CHANGE_TIME 3000
static CString ApduCom[]={
_T("00D600001AD101165500687474703A2F2F7777772E676F6F676C652E636F6D"),  //	http://www.google.com 
_T("00D6000019D101155500687474703A2F2F7777772E69616E79712E636F6D"),   //	http://www.ianyq.com
_T("00D6000039D101355500687474703A2F2F7777772E6E66632D666F72756D2E6F72672F6D656D6265725F636F6D70616E6965732F6173736F63696174652F"), //	http://www.nfc-forum.org/member_companies/associate/
_T("00D6000020D1010C55053031303232343031303230") //	01022401020
};
static CString ApduLen[]={
	_T("1A"),
	_T("19"),
	_T("39"),
	_T("20")
};
void CMainGdiDlg::MainThreadFunc()
{
	while(TRUE)
	{
		if(maingdiEvent.WaitForEvent(DETECTEVENTTIME))
		{
			break;
		}
		else
		{
			//if(!bCardConnection&&dualicard->CardFinddingLoop())
			if(!bCardConnection&&dualicard->FindCard())
			{
				//카드 인식을 한번만하도록
				BOOL b=FALSE;
				CString scardRecv="";
				b = dualicard->ApduCommand("00A404000BD4100000004151436F0001",scardRecv);
				if(b&&ImageNumber<4)
				{
					b = dualicard->ApduCommand(ApduCom[ImageNumber],scardRecv);
					imgbackup=ImageNumber; ImageNumber=ImageNumber+4;
					InvalidateRect(hWnd, NULL, FALSE);
					//InvalidateRect를 강제로 바로 실행 하기 위해존제..
					UpdateWindow(hWnd);
					bCardConnection=TRUE;
					GASound::SoundPlay(_T("NandFlash\\PhoneTag\\alarm.wav"));
				}
			}
			else if(bCardConnection)
			{
				BOOL b=FALSE;
				CString scardRecv="";
				b = dualicard->ApduCommand("00A404000BD4100000004151436F0001",scardRecv);
				if(b)
				{
					continue;
				}
				bCardConnection=FALSE; changeTime=(GetTickCount()+3000);
				ImageNumber=imgbackup;
				InvalidateRect(hWnd, NULL, FALSE);
				//InvalidateRect를 강제로 바로 실행 하기 위해존제..
				UpdateWindow(hWnd);
			}
			else
			{
				if(GetTickCount()>changeTime+CHANGE_TIME)
				{
					changeTime=GetTickCount(); ++ImageNumber%=(MGFILECOUNT_0);
					InvalidateRect(hWnd, NULL, FALSE);
					//InvalidateRect를 강제로 바로 실행 하기 위해존제..
					UpdateWindow(hWnd);
				}				
			}
		}
	}
	CloseEvent.SetEvent();
}