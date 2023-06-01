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
	Aqlogo.DeleteObject();
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
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------
#define  LISTBOX_ID1 1101
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================
	// Rgn으로 원을 그린다
	//========================================
	hrgn[0] = _GARgn::CreateRoundRectRgn (15,30,45,60,30,30);
	hrgn[1] = _GARgn::CreateRoundRectRgn (100,30,130,60,30,30);
	hrgn[2] = _GARgn::CreateRoundRectRgn (185,30,215,60,30,30);
	color[0]=RGB(0xff,0x00,0x00);
	color[1]=RGB(0xff,0x00,0x00);
	color[2]=RGB(0xff,0x00,0x00);

//	CRect rt;
//	rt.SetRect(0,80,250,200);
//	List.Create(hWnd,rt,_T(""),WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,	0,LISTBOX_ID1);
	List.Attach(GetDlgItem(hWnd, IDC_LIST1));
	List.AddString(_T("단말기 초기화 버튼을 눌러주세요"));
	int count = List.GetCount();
	List.SetCurSel(count-1);
	//
	fontinfo.lfHeight   = 16;
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
	_tcscpy(fontinfo.lfFaceName, _T("굴림체"));
	fontcolor=RGB(255,0,0);
	stc.Attach(GetDlgItem(hWnd, IDC_VIEWTEXT));
	SetStaticString(_T("대기상태"));

	BITMAP  AqBitmap;
	Aqlogo.LoadBitmap(IDB_AQLOGO);
	Aqlogo.GetBitmap(AqBitmap);
	AqImgsize.x = (int)AqBitmap.bmWidth;  // 그림의 폭 
	AqImgsize.y = (int)AqBitmap.bmHeight;  // 그림의 넓이
	AqMainlogo.LoadBitmap(IDB_AQMAINLOGO);
	AqMainlogo.GetBitmap(AqBitmap);
	AqMainImgsize.x = (int)AqBitmap.bmWidth;  // 그림의 폭 
	AqMainImgsize.y = (int)AqBitmap.bmHeight;  // 그림의 넓이

}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
	CBrush brush[3];
	brush[0].CreateSolidBrush (color[0]);
	brush[1].CreateSolidBrush (color[1]);
	brush[2].CreateSolidBrush (color[2]);
	dc.FillRgn (hrgn[0] , brush[0]);
	dc.FillRgn (hrgn[1] , brush[1]);
	dc.FillRgn (hrgn[2] , brush[2]);

	CBitmap Bit,OldBit;
	CDC MemDC;
	MemDC.CreateCompatibleDC();
	OldBit=MemDC.SelectBitmap(Aqlogo);
	dc.BitBlt(280,253,AqImgsize.x,AqImgsize.y,MemDC,0,0,SRCCOPY);
	MemDC.SelectBitmap(AqMainlogo);
	dc.BitBlt(460,0,AqMainImgsize.x,AqMainImgsize.y,MemDC,0,0,SRCCOPY);
	MemDC.SelectBitmap(OldBit);
}
//--------------------------------------------------------------
//|
//|색 바꾸기
//|
//---------------------------------------------------------------
void CMainGdiDlg::SetDrawItem(int index, COLORREF _color)
{
	color[index]=_color;
	InvalidateRect(hWnd, NULL, TRUE);
	//InvalidateRect를 강제로 바로 실행 하기 위해존제..
	UpdateWindow(hWnd);
}
//--------------------------------------------------------------
//|
//|스트링 추가
//|
//---------------------------------------------------------------
void CMainGdiDlg::SetListString(CString _string)
{
	List.AddString(_string);
	int count = List.GetCount();
	List.SetCurSel(count-1);
}
void CMainGdiDlg::SetStaticString(CString _string)
{
	stc.SetWindowText(_string);
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
// 	int nOldMode = pDC->SetBkMode(TRANSPARENT);
// 	pDC->SetTextColor(fontcolor);
// 	HFONT hOldFont  = pDC->SelectFont (font);
// 	CRect textrc(pRect->left+30,pRect->top,pRect->right,pRect->bottom);
// 	pDC->DrawText (text, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);
// 
// 	pDC->SelectFont (hOldFont);
// 	pDC->SetBkMode(nOldMode);
	CFont font;
	font.CreateFontIndirect(&fontinfo);
	dc.SetTextColor(fontcolor);
	dc.SetBkColor(RGB(255,0,0));
	dc.SelectFont (font);
}