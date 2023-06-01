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
	TestBut.DestroyWindow();
	TestStatic.DestroyWindow();
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

	CGdiPlusBitmapResource pBitmap;
 	pBitmap.Load(IDB_PNG1,"PNG");
 	CBitmap bit = pBitmap.GetWinBitmap();
	//테스트 버튼
	TestBut.Create(hWnd, CRect(10, 10, 0, 0),_T("Test"), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERBUTTONTEST);
	TestBut.initSuperButton(bit);

	CGdiPlusBitmapResource pBitmap1;
	pBitmap1.Load(IDB_JPG1,"JPG");
	CBitmap bit1 = pBitmap1.GetWinBitmap();
	TestStatic.Create(hWnd, CRect(10, 50, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_SUPERSTATIC);
	TestStatic.initSuperStatic(bit1);
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