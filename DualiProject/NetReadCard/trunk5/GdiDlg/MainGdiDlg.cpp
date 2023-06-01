#include "stdAfx.h"
#include "MainGdiDlg.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
hWnd(NULL)
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
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
}
//--------------------------------------------------------------
//|
//|dlg �ʱ�ȭ 
//|
//---------------------------------------------------------------
#define  LISTBOX_ID1 1101
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND ����
	//========================================
	hWnd = _hWnd;
	//========================================
	// Rgn���� ���� �׸���
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
	List.AddString(_T("�ܸ��� �ʱ�ȭ ��ư�� �����ּ���"));
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
	_tcscpy(fontinfo.lfFaceName, _T("����ü"));
	fontcolor=RGB(255,0,0);
	stc.Attach(GetDlgItem(hWnd, IDC_VIEWTEXT));
	SetStaticString(_T("������"));

	BITMAP  AqBitmap;
	Aqlogo.LoadBitmap(IDB_AQLOGO);
	Aqlogo.GetBitmap(AqBitmap);
	AqImgsize.x = (int)AqBitmap.bmWidth;  // �׸��� �� 
	AqImgsize.y = (int)AqBitmap.bmHeight;  // �׸��� ����
	AqMainlogo.LoadBitmap(IDB_AQMAINLOGO);
	AqMainlogo.GetBitmap(AqBitmap);
	AqMainImgsize.x = (int)AqBitmap.bmWidth;  // �׸��� �� 
	AqMainImgsize.y = (int)AqBitmap.bmHeight;  // �׸��� ����

}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
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
//|�� �ٲٱ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::SetDrawItem(int index, COLORREF _color)
{
	color[index]=_color;
	InvalidateRect(hWnd, NULL, TRUE);
	//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
	UpdateWindow(hWnd);
}
//--------------------------------------------------------------
//|
//|��Ʈ�� �߰�
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
//|Static �ؽ�Ʈ ��
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