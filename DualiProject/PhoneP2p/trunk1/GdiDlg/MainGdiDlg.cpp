#include "stdAfx.h"
#include "MainGdiDlg.h"

#include <InitGuid.h>
#include <imaging.h>
#pragma comment(lib, "imaging.lib")
#pragma comment(lib, "commdlg.lib")
#pragma comment(lib, "ole32.lib")
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
#define  IDC_MODECHANGE  301
#define  IDC_HOSPITAL_1  401
#define  IDC_HOSPITAL_2  402
#define  IDC_HOSPITAL_3  403
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
backButShow(SW_HIDE)
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
	pImgFactory->Release();
}
//파괴
void  CMainGdiDlg::ControlDestroy()
{
	ModeChangeBut.DestroyWindow();
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
	dualip2p=obj->GetDualiP2pMgObject();
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------
CRect ButPosRc_1;
CRect ButPosRc_2;
CRect ButPosRc_3;
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================
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
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]),_T("굴림체"));
	fontcolor=RGB(255,0,0);
//	CBitmap h = ::SHLoadImageFile("shin.jpg"); 

	pImgFactory = NULL;
	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER,	IID_IImagingFactory, (void**)&pImgFactory)))
	{
	}
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//모드 변경 버튼
	ModeChangeBut.Create(hWnd, CRect(iX-100, 60, 0, 0),_T("모드변경"), WS_CHILD | WS_VISIBLE , 0, IDC_MODECHANGE);
	//이미지(비트맵) 인자로 넘긴다
	ModeChangeBut.initSuperButton(IDB_BUT_2);

	// 병원 선택 버튼
	HospitalBut_1.Create(hWnd, CRect(399, 210, 0, 0),_T("H1"), WS_CHILD | WS_VISIBLE   , 0, IDC_HOSPITAL_1);
	//이미지(비트맵) 인자로 넘긴다
	HospitalBut_1.initSuperButton(IDB_BUT_3);
	HospitalBut_1.ShowWindow(SW_HIDE);
	HospitalBut_1.GetWindowRect(&ButPosRc_1);

	//병원 선택 버튼
	HospitalBut_2.Create(hWnd, CRect(399, 280, 0, 0),_T("H2"), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_2);
	//이미지(비트맵) 인자로 넘긴다
	HospitalBut_2.initSuperButton(IDB_BUT_4);
	HospitalBut_2.ShowWindow(SW_HIDE);
	HospitalBut_2.GetWindowRect(&ButPosRc_2);

	//병원 선택 버튼
	HospitalBut_3.Create(hWnd, CRect(399, 350, 0, 0),_T("H3"), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_3);
	//이미지(비트맵) 인자로 넘긴다
	HospitalBut_3.initSuperButton(IDB_BUT_5);
	HospitalBut_3.ShowWindow(SW_HIDE);
	HospitalBut_3.GetWindowRect(&ButPosRc_3);
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
TCHAR MgFile[][3][50]={{_T("\\NandFlash\\PhoneP2p\\1_sample\\1.jpg"),_T("\\NandFlash\\PhoneP2p\\1_sample\\2.jpg"),_T("\\NandFlash\\PhoneP2p\\1_sample\\1.jpg")},
						{_T("\\NandFlash\\PhoneP2p\\2_sample\\1.jpg"),_T("\\NandFlash\\PhoneP2p\\2_sample\\2.jpg"), _T("\\NandFlash\\PhoneP2p\\2_sample\\1.jpg")},
						{_T("\\NandFlash\\PhoneP2p\\3_sample\\1.jpg"),_T("\\NandFlash\\PhoneP2p\\3_sample\\3.jpg"), _T("\\NandFlash\\PhoneP2p\\3_sample\\2.jpg")}};
void CMainGdiDlg::DrawPaint()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	CRect rc;
	GetClientRect(hWnd, &rc);
	CRect ImgRc(rc.left, rc.top, rc.right, rc.bottom);
	if(pImgFactory)
	{
		IImage* pImage = NULL;
		//파일로부터 이미지 로드
		if(SUCCEEDED(pImgFactory->CreateImageFromFile(MgFile[MgMode][MgNum], &pImage)))
		{
			pImage->Draw(hdc, &ImgRc, NULL);
			pImage->Release();
		}
	}
	EndPaint(hWnd, &ps);
}
//--------------------------------------------------------------
//|
//|Static 텍스트 색
//|
//---------------------------------------------------------------
void CMainGdiDlg::StaticText(CDCHandle dc)
{
// 	CFont font;
// 	font.CreateFontIndirect(&fontinfo);
// 	dc.SetTextColor(fontcolor);
// 	dc.SetBkColor(RGB(255,0,0));
// 	dc.SelectFont (font);
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
	SendMessage(hWnd, WM_TIMER,0,0);
}
//--------------------------------------------------------------
//|
//|이미지 변경하여 그리기
//|
//---------------------------------------------------------------
TCHAR MobileSendData[][40]={_T("03poto"),_T("02http://www.ajussi2010.co.kr"), _T("0101022401020")};
void CMainGdiDlg::ImageChange(UINT _MgNum, BOOL bSendData)
{
	BOOL b=FALSE;
	//모드가 2일 때
	if(MgMode==2)
	{
		//병원 IDC_HOSPITAL_2 값 설정 되었고, 데이타를 보내야 되는 값 설정 되었을 때
		if(hospitalnum==IDC_HOSPITAL_2&&bSendData==TRUE)
		{
			//데이타를 보낸다 이미지 넘버 지금은 MgMode 값
			//TRUE를 리턴하면 이미지를 바꾸도록 한다
			if(dualip2p->SendDatafn(1, MobileSendData[MgMode]))
			{
				//이미지 넘버
				MgNum=_MgNum;
			}
		}
		//이미지 넘버가 0이고 이미지가 버튼을 눌렀을 때 값 그대로라면 
		//이미지를 바꾸지 않도록 세팅값을 유지한다
		else if(_MgNum==0&&MgNum==1)
		{
			MgNum = _MgNum;
		}
		//모드 2일 때 이미지 넘버가 0이면 버튼이 보여진다
		if(MgNum==0)
		{
			//버튼3개 보여지도록, 병원 버튼 클릭 했던 값 초기화
			ButShow=SW_SHOW; hospitalnum=0;
		}
	}
	//모드 2가 아닐 때
	else
	{
		//데이타 보내는 값 설정 되었을 때
		if(bSendData==TRUE)
		{
			//데이타를 보낸다 이미지 넘버 지금은 MgMode 값
			//TRUE를 리턴하면 이미지를 바꾸도록 한다
			if(dualip2p->SendDatafn(1, MobileSendData[MgMode]))
			{
				MgNum=_MgNum;
			}
		}
		//데이타를 보내는 메시지 이외 값일 때
		else if(_MgNum==0)
		{
			MgNum = _MgNum;
		}

	}
	//다시 그린다
	MainGdiinvalidateRect(NULL,FALSE);
}
//타이머 메시지
void CMainGdiDlg::TimerMessage()
{
	if(ButShow==SW_SHOW)
	{
		if(backButShow!=SW_SHOW)
		{
			HospitalBut_1.ShowWindow(SW_SHOW);
			HospitalBut_2.ShowWindow(SW_SHOW);
			HospitalBut_3.ShowWindow(SW_SHOW);
			HospitalBut_1.InvalidateRect(NULL, FALSE);
			HospitalBut_2.InvalidateRect(NULL, FALSE);
			HospitalBut_3.InvalidateRect(NULL, FALSE);
			backButShow=SW_SHOW;
		}
	}
	else
	{
		if(backButShow!=SW_HIDE)
		{
			HospitalBut_1.ShowWindow(SW_HIDE);
			HospitalBut_2.ShowWindow(SW_HIDE);
			HospitalBut_3.ShowWindow(SW_HIDE);
			backButShow=SW_HIDE;
		}
	}
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
			MgMode=++MgMode%3;
			if(MgMode==2)
			{
				ButShow=SW_SHOW;
				MainGdiinvalidateRect(NULL,FALSE);
			}
			else
			{
				ButShow=SW_HIDE;
				MainGdiinvalidateRect(NULL,FALSE);
			}
			//버튼의 클릭 메시지를 사용했는가를 판단
			b=TRUE;
			//0 : 모드를 바꿀 때는 이미지 넘버를 0으로 세팅하고 병원 버튼 초기화
			MgNum=0; hospitalnum=0;
		}break;
	case IDC_HOSPITAL_1:
		{
			hospitalnum=IDC_HOSPITAL_1;
			//0 : 모드를 바꿀 때는 이미지 넘버를 0으로 세팅
			b=TRUE;
		}break;
	case IDC_HOSPITAL_2:
		{
			ButShow=SW_HIDE;
			//병원 그림에서 버튼이 눌렸을 때
			hospitalnum=IDC_HOSPITAL_2;	MgNum=2;
			MainGdiinvalidateRect(NULL,FALSE);
			//0 : 모드를 바꿀 때는 이미지 넘버를 0으로 세팅
			b=TRUE;
		}break;
	case IDC_HOSPITAL_3:
		{
			hospitalnum=IDC_HOSPITAL_3;
			//0 : 모드를 바꿀 때는 이미지 넘버를 0으로 세팅
			b=TRUE;
		}break;
	}
	return b;
}