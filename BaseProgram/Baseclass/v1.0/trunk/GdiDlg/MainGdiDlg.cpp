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
void CMainGdiDlg::ControlDestroy()
{
	TestStatic.DestroyWindow();
	TestEdit.DestroyWindow();
	TestCombo.DestroyWindow();
	TestListBox.DestroyWindow();
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

	CGdiPlusBitmapResource pBitmap1;
	pBitmap1.Load(IDB_JPG1,"JPG");
	CBitmap bit1 = pBitmap1.GetWinBitmap();
	TestStatic.Create(hWnd, CRect(10, 50, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	//TestStatic.initSuperStatic(HBITMAP(bit1));
	TestStatic.initRgnSuperStatic(IDB_BTNIMAGE_X, IDB_BTNMASK_X);

	TestEdit.Create(hWnd, CRect(0, 50, 100, 70), _T("55555"), WS_VISIBLE|WS_CHILD|WS_BORDER, WS_EX_CLIENTEDGE, 111);
	DWORD dwStyle = WS_VISIBLE|WS_CHILD | WS_CLIPCHILDREN | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED;
	HWND Comhwnd = TestCombo.Create(hWnd, CRect(30, 40, 90, 60), _T("1111"), dwStyle,NULL, 113 );
	TestCombo.SetItemHeight(-1,20);
	TestCombo.AddString("111111111111111111111");
	TestCombo.AddString("222");
	TestCombo.AddString("333");
	TestCombo.SetCurSel(0);

	DWORD dwLStyle = WS_VISIBLE|WS_CHILD | WS_CLIPCHILDREN | LBS_OWNERDRAWFIXED;
	HWND Listhwnd = TestListBox.Create(hWnd, CRect(70, 40, 140, 200), _T("1111"), dwLStyle,NULL, 114 );
	TestListBox.SetItemHeight(-1,20);
	TestListBox.AddString("111111111111111111111");
	TestListBox.AddString("222");
	TestListBox.AddString("333");
	TestListBox.SetCurSel(0);
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
	BOOL b=FALSE;
	switch(nID)
	{
	case IDC_SUPERBUTTONTEST:
		{
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