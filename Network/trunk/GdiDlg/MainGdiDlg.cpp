#include "stdAfx.h"
#include "MainGdiDlg.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
hWnd(NULL)
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
	DeleteObject (hrgn[0]);
	DeleteObject (hrgn[1]);
	DeleteObject (hrgn[2]);
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
	// 서버 클래스
	//========================================
	socketserver=obj->GetWinSocketObject();
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
	hrgn[0] = _GARgn::CreateRoundRectRgn (20,30,50,60,30,30);
	hrgn[1] = _GARgn::CreateRoundRectRgn (110,30,140,60,30,30);
	hrgn[2] = _GARgn::CreateRoundRectRgn (195,30,225,60,30,30);
	color[0]=RGB(0xff,0x00,0x00);
	color[1]=RGB(0xff,0x00,0x00);
	color[2]=RGB(0xff,0x00,0x00);

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
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
	dc.SetBkColor(RGB(255,0,0));
	CBrush brush[3];
	brush[0].CreateSolidBrush (color[0]);
	brush[1].CreateSolidBrush (color[1]);
	brush[2].CreateSolidBrush (color[2]);
	dc.FillRgn (hrgn[0] , brush[0]);
	dc.FillRgn (hrgn[1] , brush[1]);
	dc.FillRgn (hrgn[2] , brush[2]);
}
//--------------------------------------------------------------
//|
//| 버튼 클릭 이벤트 메시지
//|
//---------------------------------------------------------------
void CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	switch(nID)
	{
	case IDC_BUT_SERVERSTART:
		{
			socketserver->MakeServer(9000,"");	
		}break;
	case 1:
		{

		}break;
	case 2:
		{

		}break;
	}
}