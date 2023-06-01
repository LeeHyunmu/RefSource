
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSuperButton::CSuperButton(void):
mgnum(0),
bNeedImage(TRUE),
caption("")
{
	butimage.clear();
	memset(&rgnbox,0,sizeof(rgnbox));
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSuperButton::~CSuperButton(void)
{
}
//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------


BOOL CSuperButton::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperButton, CButton>::SubclassWindow(hWnd);
	return bRet;
}
void CSuperButton::init(CString _caption, UINT ResourceId)
{
	fontinfo.lfHeight   = 12;
	fontinfo.lfWeight   = 0;
	fontinfo.lfEscapement  = 0;
	fontinfo.lfOrientation  = 0;
	fontinfo.lfWeight   = FW_BOLD;
	fontinfo.lfItalic   = FALSE;
	fontinfo.lfUnderline  = FALSE;
	fontinfo.lfStrikeOut  = FALSE;
	fontinfo.lfCharSet   = DEFAULT_CHARSET;
	fontinfo.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	fontinfo.lfQuality   = DEFAULT_QUALITY;
	fontinfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName), _T("굴림체"));
	fontcolor=0;
	//캡션 스트링
	caption=_caption;
	CBitmap bit;
	bit.LoadBitmap(IDB_BITBUT_1);
	DivideHbitmap(bit);
	SIZE bmSize;
	butimage[0].GetSize(bmSize);
	SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
	if(caption.GetLength())
		SetWindowText(caption);
}
void CSuperButton::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	CDC dc = lpdis->hDC;
	dc.SaveDC();
	if(bNeedImage)
	{
		CRect rect;
		GetClientRect(&rect);
		bNeedImage=FALSE;
	}
	DrawButton(&dc,lpdis->itemState);
}

void CSuperButton::DrawButton(CDC * pDC, UINT state)
{
	if(butimage.size()<BUT_MAXNUM)
		return;
	if (state & ODS_DISABLED)
	{
		mgnum=1;
	}
	else 
	{
		if (state & ODS_SELECTED)
		{
			mgnum=2;
		}
		else
		{
			mgnum=0;
		}	
	}
	// create memory DC
	pDC->SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC->m_hDC);
	CBitmap pOldBitmap;

	SIZE bmSize;
	butimage[mgnum].GetSize(bmSize);
	pOldBitmap = pMemDC -> SelectBitmap(butimage[mgnum]);

	pDC -> BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
	::SelectClipRgn(pDC -> m_hDC, NULL);

	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;
}

void CSuperButton::SetSuperButFont(LOGFONT* _fontinfo,COLORREF* _fontcolor)
{
	BOOL b=FALSE;
	if(_fontinfo)
	{
		fontinfo.lfHeight   = _fontinfo->lfHeight;
		fontinfo.lfWeight   = _fontinfo->lfWeight;
		fontinfo.lfEscapement  = _fontinfo->lfEscapement;
		fontinfo.lfOrientation  = _fontinfo->lfOrientation ;
		fontinfo.lfWeight   = _fontinfo->lfWeight;
		fontinfo.lfItalic   = _fontinfo->lfItalic;
		fontinfo.lfUnderline  = _fontinfo->lfUnderline;
		fontinfo.lfStrikeOut  = _fontinfo->lfStrikeOut;
		fontinfo.lfCharSet   = _fontinfo->lfCharSet;
		fontinfo.lfOutPrecision = _fontinfo->lfOutPrecision;
		fontinfo.lfClipPrecision = _fontinfo->lfClipPrecision;
		fontinfo.lfQuality   = _fontinfo->lfQuality;
		fontinfo.lfPitchAndFamily = _fontinfo->lfPitchAndFamily;
		_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName), _fontinfo->lfFaceName);
		b=TRUE;
	}
	if(_fontcolor)
	{
		fontcolor=*_fontcolor;
		b=TRUE;
	}
	if(b)
		bNeedImage=TRUE;
}
void CSuperButton::DrawWhiteText(CDC* pDC, CRect* pRect, CString& text )
{
	CFont font;
	font.CreateFontIndirect(&fontinfo);
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(fontcolor);
	HFONT hOldFont  = pDC->SelectFont (font);
	CRect textrc(pRect->left+30,pRect->top,pRect->right,pRect->bottom);
	pDC->DrawText (text, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);

	pDC->SelectFont (hOldFont);
	pDC->SetBkMode(nOldMode);
}

BOOL CSuperButton::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

HWND CSuperButton::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperButton, CButton>::UnsubclassWindow(bForce);
}

int CSuperButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	ModifyStyle(0, BS_OWNERDRAW | BS_PUSHBUTTON);	
	//프리트랜스 메시지
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	return 0;
}
BOOL CSuperButton::OnEraseBkgnd(CDCHandle dc)
{
	return TRUE;
}
LRESULT CSuperButton::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	//버튼 미리 만들어 둔 이미지 삭제
	butClear();
	return 0;
}
void CSuperButton::butClear()
{
	for (int but=0; but<(int)butimage.size(); but++)
	{
		DeleteObject(butimage[but]);
	}
	butimage.clear();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperButton::DivideHbitmap(CBitmap hbit)
{
	SIZE bmSize;
	if(hbit.GetSize(bmSize))
	{
		LONG width=bmSize.cx/BUT_MAXNUM;
		LONG Height=bmSize.cy;
		BITMAPINFOHEADER Header32Bit;
		::memset(&Header32Bit, 0, sizeof(Header32Bit));
		Header32Bit.biSize   = sizeof(BITMAPINFOHEADER);
		Header32Bit.biWidth   = width;
		Header32Bit.biHeight  = Height;
		Header32Bit.biPlanes  = 1;
		Header32Bit.biBitCount  = 32;
		Header32Bit.biCompression = BI_RGB;

		HDC hdc = GetDC();
		HDC hMemDC = ::CreateCompatibleDC(hdc);
		HDC hBmpDC = ::CreateCompatibleDC(hMemDC);
		CBitmap hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hbit);
		for(int bt = 0; bt<BUT_MAXNUM; bt++)
		{
			void *pBmpBits = NULL;
			CBitmapHandle h32BitBmp;
			if(h32BitBmp.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
				//if(h32BitBmp)
			{
				BITMAP bmpInfo32;
				::GetObject(h32BitBmp, sizeof(bmpInfo32), &bmpInfo32);
				if(bmpInfo32.bmWidthBytes % 4 > 0)
					bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
				CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
				::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hBmpDC, width*bt, 0, width,Height, SRCCOPY);
				::SelectObject(hMemDC, hOldBmp);
				butimage.push_back(h32BitBmp);
			}
		}
		::SelectObject(hBmpDC, hOldBmp2);
		::DeleteDC(hBmpDC);
		::DeleteDC(hMemDC);
		CDC cd = hdc;
		DrawButton(&cd, 4);
		::DeleteDC(hdc);
		if(butimage.size()==BUT_MAXNUM)
			return TRUE;
	}
	return FALSE;
}