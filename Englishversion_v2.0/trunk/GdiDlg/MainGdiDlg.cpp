#include "stdAfx.h"
#include "../Explorer/\ExplorerDlg.h"
#include "../CompleteDlg/CompleteDlg.h"
#include "../CouponPop/CouponPopDlg.h"
#include "../CouponPop/CouponDlg.h"
#include "../ContentsMall/ContentsMall.h"
#include "../ContentsMall/ContentsMallPopDlg.h"
#include "../Adverts/AdvertsDlg.h"
#include "MainGdiDlg.h"

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
#define NOTICURL	_T("http://smartaq.com/")

TCHAR filename[3][10] = {"ad1.avi", "ad2.avi","ad3.avi"};

CString file;
#define GET_HINSTANCE(hWnd)    ((HINSTANCE)::GetWindowLong((hWnd), GWL_HINSTANCE)) 

//NDEF 통신 데이터 넘버(미디어폴, 쿠폰)
#define COUPON_COMMANDNUM1	0
#define COUPON_COMMANDNUM2	1
#define COUPON_COMMANDNUM3	2
#define COUPON_COMMANDNUM4	3
#define COUPON_COMMANDNUM5	4
#define COUPON_COMMANDNUM6	5
#define COUPON_COMMANDNUM7	6
#define COUPON_COMMANDNUM8	7

#define COUPON_CONTENTSMALL1	8
#define COUPON_CONTENTSMALL2	9
#define COUPON_CONTENTSMALL3	10
#define COUPON_CONTENTSMALL4	11

#define ADVERT_COMMANDNUM1	12
#define ADVERT_COMMANDNUM2	13
#define ADVERT_COMMANDNUM3	14
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
hWnd(NULL),
//pBrowser(NULL),
filenum(0),
NFCCommandNum(ADVERT_COMMANDNUM1),
bCouponClick(FALSE),
bContentsMallClick(FALSE),
coupondlg(NULL),
mallPop(NULL),
comdlg(NULL)
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
void CMainGdiDlg::ControlDestroy()
{
	mpw.DestroyWindow();
	videochange.DestroyWindow();
	//CouponDlg->DestroyWindow();
	delete CouponDlg;
	//
	delete contensmall;
	//
	delete AQadvertsDlg;
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
	debug=MYDEBUG;

	//동영상 플레이
	TCHAR CurrentName[MAX_PATH];
	GetModuleFileName(0,CurrentName,sizeof(CurrentName));
	FilePath = CurrentName;
	//
	HRESULT hr = ::CoCreateInstance(WMPLib::CLSID_WindowsMediaPlayer, NULL, CLSCTX_ALL, WMPLib::IID_IWMPPlayer4, (LPVOID*)&pWMP);
	if (SUCCEEDED(hr)) {
		pWMP->put_uiMode(L"none");
		pWMP->get_controls(&pCtrl);
		pWMP->put_enableContextMenu(FALSE);
		pWMP->put_stretchToFit(TRUE);
		pWMP->put_fullScreen(FALSE);
		AtlAxAttachControl(pWMP, mpw.m_hWnd, NULL);
		//		BSTR url(L"mms://liveongame.gscdn.com/L1");
		file = ChangeExt(FilePath, filename[0]);

		pWMP->put_URL(file.AllocSysString());
		pWMP->get_settings(&pSetting);
		pSetting->put_volume(80);
	}
	//

	//쿠폰 팝업 창(GIF)
	coupondlg = new CCouponPopDlg();
	//
	mallPop = new CContentsMallPopDlg();
	//
	comdlg =  new CCompleteDlg();
	//쿠폰 리스트 창
	CouponDlg = new CCouponDlg();
	CouponDlg->init(hWnd);
	CouponDlg->Create(hWnd);
	CouponDlg->ShowWindow(SW_SHOW);

	//Contents Mall
	contensmall = new CContentsMallDlg();
	contensmall->init(hWnd);
	contensmall->Create(hWnd);
	contensmall->ShowWindow(SW_SHOW);

	//AQ 베너 광고 창
	AQadvertsDlg = new CAdvertsDlg();
	AQadvertsDlg->init(hWnd);
	AQadvertsDlg->Create(hWnd);
	AQadvertsDlg->ShowWindow(SW_SHOW);

	if(AqDevice.initDevice(hWnd, obj))
	{
		AqDevice.SetFirstNFCCardModeSetting(NFCCommandNum);
		//AqDevice.SetFirstNFCReaderModeSetting(NFCCommandNum);
	}
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

	vector<CString> valueString;
	CRegistHelp::GetFindRegKeyEnumValue(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), valueString);

	CRect parentRect;
	GetWindowRect(_hWnd, &parentRect);

	//미디어 플레이어
	mpw.Create(hWnd, CRect(100, 0, parentRect.right-100, 826-200),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);

	videochange.Create(_hWnd, CRect(100, 10, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_TEMP);
	videochange.initRgnSuperButton(IDB_NEXTBTN1, IDB_BTNMASK_4);
// 	pBrowser = new CWTLIExplorer();//(this->m_hWnd);
// 	if(pBrowser!=NULL)
// 	{
// 		if (::IsWindow(pBrowser->Create(_hWnd, CRect(parentRect.left,826+530+10, parentRect.right,parentRect.bottom), _T("about:blank"), WS_CHILD|WS_VISIBLE|WS_BORDER|WS_HSCROLL|WS_VSCROLL)))
// 		{
// 			CComVariant vNull;
// 			CComVariant vUrl(NOTICURL);
// 			(*pBrowser)->Navigate2(&vUrl, &vNull, &vNull, &vNull, &vNull);
// 		}
// 	}
}
CString   CMainGdiDlg::ChangeExt(CString sPathName, CString sNewExt, char find)
{
	int            nPos;
	CString         sResult;
	nPos= sPathName.ReverseFind(find);
	if(nPos==-1)                        // .을 찾을 수 없다..
		sResult = sPathName +find+sNewExt;
	else
		sResult = sPathName.Left(nPos)+find+sNewExt;
	return sResult;
}

//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
}
//--------------------------------------------------------------
//|
//| 버튼 클릭 이벤트 메시지
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	switch(nID)
	{
	case IDC_TEMP:
		{
			filenum++;
			filenum%=3;
			file = ChangeExt(FilePath, filename[filenum]);
			pWMP->put_URL(file.AllocSysString());
		}break;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::MediaPlayTimer(UINT_PTR nIDEvent)
{
	BOOL b=FALSE;
	switch(nIDEvent)
	{
	case MAINCLOSE_DELAYTIME:
		{
			b=true;
			WMPLib::WMPPlayState state;
			pWMP->get_playState(&state);
			if(state==WMPLib::wmppsStopped)
			{
				filenum++;
				filenum%=3;
				file = ChangeExt(FilePath, filename[filenum]);
				pWMP->put_URL(file.AllocSysString());
			}
			else if(state==WMPLib::wmppsPlaying)
			{
				if(bCouponClick==FALSE&&bContentsMallClick==FALSE)
				{
					NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
					AqDevice.SetNFCCommandNum(NFCCommandNum);
				}
			}
 		}
		break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

BOOL CMainGdiDlg::NfcEventCall(WPARAM wParam)
{
	return AqDevice.NfcEventCall(wParam);;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

BOOL CMainGdiDlg::DeviceCmdCallMessage(WPARAM wParam)
{
	return AqDevice.DeviceCmdCallMessage(wParam);;
}
//--------------------------------------------------------------
//|
//|CouponClick Msg
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::CouponClickMessage(WPARAM wParam)
{
	if(bCouponClick==TRUE || bContentsMallClick==TRUE)
	{
		return TRUE;
	}
	BOOL b=FALSE;
	switch(wParam)
	{
	case IDC_COUPONBUT_1:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG01);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;
		}break;
	case IDC_COUPONBUT_2:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM2;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG02);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;

		}break;
	case IDC_COUPONBUT_3:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM3;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG03);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;

		}break;
	case IDC_COUPONBUT_4:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM4;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG04);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;

		}break;
	case IDC_COUPONBUT_5:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM5;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG05);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;

		}break;
	case IDC_COUPONBUT_6:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM6;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG06);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;					
		}break;
	case IDC_COUPONBUT_7:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM7;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG07);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;

		}break;
	case IDC_COUPONBUT_8:
		{
			bCouponClick=TRUE;
			NFCCommandNum = COUPON_COMMANDNUM8;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			coupondlg->init(IDB_COUPONBIG08);
			coupondlg->DoModal(hWnd);
			bCouponClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;

		}break;
	case IDC_CONTENTSBUT_1:
		{
			bContentsMallClick=TRUE;
			NFCCommandNum = COUPON_CONTENTSMALL1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			mallPop->init(IDB_CONTENTSBIG01);
			mallPop->DoModal(hWnd);
			bContentsMallClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;
		}break;
	case IDC_CONTENTSBUT_2:
		{
			bContentsMallClick=TRUE;
			NFCCommandNum = COUPON_CONTENTSMALL2;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			mallPop->init(IDB_CONTENTSBIG01);
			mallPop->DoModal(hWnd);
			bContentsMallClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;
		}break;
	case IDC_CONTENTSBUT_3:
		{
			bContentsMallClick=TRUE;
			NFCCommandNum = COUPON_CONTENTSMALL3;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			mallPop->init(IDB_CONTENTSBIG01);
			mallPop->DoModal(hWnd);
			bContentsMallClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;
		}break;
	case IDC_CONTENTSBUT_4:
		{
			bContentsMallClick=TRUE;
			NFCCommandNum = COUPON_CONTENTSMALL4;
			AqDevice.SetNFCCommandNum(NFCCommandNum);

			mallPop->init(IDB_CONTENTSBIG01);
			mallPop->DoModal(hWnd);
			bContentsMallClick=FALSE;
			NFCCommandNum = filenum+ADVERT_COMMANDNUM1;
			AqDevice.SetNFCCommandNum(NFCCommandNum);
			b=TRUE;
		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//| UI 조정 이벤트 받기 위한 함수
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::NfcAqDeviceMessage(WPARAM wParam)
{
	BOOL b=FALSE;
	switch (wParam)
	{
	case AQDCMSG_COMPLET:
		{
			if(bCouponClick=TRUE)
			{
				coupondlg->DlgClose(); bCouponClick=FALSE;
			}
			if(bContentsMallClick=TRUE)
			{
				mallPop->DlgClose(); bContentsMallClick=FALSE;
			}
			//사운드
			GASound::SoundPlay(_T("TaDa.WAV"));
			PostMessage(hWnd, WM_NFCAQDEVICEMESSAGE, AQDCMSG_COMPLETDLG, NULL);
		}break;
	case AQDCMSG_COMPLETDLG:
		{
			if(comdlg->GetClose())
			{
				comdlg->init(IDB_COMPLET);
				comdlg->DoModal(hWnd);
			}
		}break;
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