
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------
//
#ifdef WINCE
#include <InitGuid.h>
#include <imaging.h>
#endif //WINCE
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSuperButton::CSuperButton(void):
mgnum(0),
bNeedImage(TRUE)
#ifndef WINCE
,m_bTracking(FALSE)
,m_bHover(FALSE)
#endif //WINCE
{
	butimage.clear();
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
//|서브 클래싱 함수
//|
//---------------------------------------------------------------
BOOL CSuperButton::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperButton, CButton>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperButton::initSuperButton(UINT ImageId, BOOL bOwnerDraw)
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
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _T("굴림체"));
	fontcolor=0;

	CBitmap bit;
	bit.LoadOEMBitmap(ImageId);
	if(DivideHbitmap(bit))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW  | BS_PUSHBUTTON, SWP_FRAMECHANGED);
		}
	}
}
#ifdef WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperButton::initSuperButton(UINT ImageId, LPCWSTR type, BOOL bOwnerDraw)
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
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _T("굴림체"));
	fontcolor=0;

	if(MakeResource(GetModuleHandle(NULL),ImageId,type))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW |  BS_PUSHBUTTON, SWP_FRAMECHANGED);
		}
	}
}
#endif //WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperButton::initSuperButton(CBitmap bit, BOOL bOwnerDraw)
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
	//CE에서 안되는 부분이다
#ifndef WINCE
	fontinfo.lfOutPrecision = OUT_CHARACTER_PRECIS;
#endif //WINCE
	fontinfo.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	fontinfo.lfQuality   = DEFAULT_QUALITY;
	fontinfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _T("굴림체"));
	fontcolor=0;

	if(DivideHbitmap(bit))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW | BS_PUSHBUTTON, SWP_FRAMECHANGED);
		}
	}
}
//--------------------------------------------------------------
//|
//|오너 드로우 함수
//|
//---------------------------------------------------------------
void CSuperButton::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	CDC dc = lpdis->hDC;
#ifndef WINCE
	dc.SaveDC();
#endif //WINCE
	if(bNeedImage)
	{
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(lpdis->hDC);
		for(int butset = 0; butset<(int)butimage.size(); butset++)
		{
			CBitmap pOldBitmap = pMemDC -> SelectBitmap(butimage[butset]);
			SIZE bmSize;
			butimage[butset].GetSize(bmSize);
			DrawWhiteText(pMemDC,bmSize);
			pMemDC -> SelectBitmap(pOldBitmap);
		}
		delete pMemDC;
		bNeedImage=FALSE;
	}
	DrawButton(&dc,lpdis->itemState);
}
//--------------------------------------------------------------
//|
//|그려주는 함수
//|
//---------------------------------------------------------------
void CSuperButton::DrawButton(CDC * pDC, UINT state)
{
	if(butimage.size()<BUT_MAXNUM)
		return;
	if (state & ODS_DISABLED)
	{
		mgnum=3;
	}
	else 
	{
		if (state & ODS_SELECTED)
		{
			mgnum=2;
		}
		else
		{
#ifndef WINCE
			if (m_bHover)
				mgnum=1;
			else 
#endif //WINCE
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
	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;
}
//--------------------------------------------------------------
//|
//|폰트 설정
//|
//---------------------------------------------------------------
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
#ifndef WINCE
		fontinfo.lfOutPrecision = _fontinfo->lfOutPrecision;
#endif //WINCE
		fontinfo.lfClipPrecision = _fontinfo->lfClipPrecision;
		fontinfo.lfQuality   = _fontinfo->lfQuality;
		fontinfo.lfPitchAndFamily = _fontinfo->lfPitchAndFamily;
		_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _fontinfo->lfFaceName);
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
//--------------------------------------------------------------
//|
//|텍스트 쓴다
//|
//---------------------------------------------------------------
void CSuperButton::DrawWhiteText(CDC* pDC, SIZE bmSize)
{
	CString title;
	GetWindowText( title );
	if(title.GetLength())
	{
		CFont font;
		font.CreateFontIndirect(&fontinfo);
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontcolor);
		HFONT hOldFont  = pDC->SelectFont (font);
		CRect textrc(30,0,bmSize.cx,bmSize.cy);
		pDC->DrawText (title, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);

		pDC->SelectFont (hOldFont);
		pDC->SetBkMode(nOldMode);
		title.ReleaseBuffer();
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperButton::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|서브 클래싱 해제
//|
//---------------------------------------------------------------
HWND CSuperButton::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperButton, CButton>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|버튼 Create 함수 
//|
//---------------------------------------------------------------
int CSuperButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//프리트랜스 메시지
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	return 0;
}
//--------------------------------------------------------------
//|
//|배경 그려주는 함수(투명)
//|
//---------------------------------------------------------------
BOOL CSuperButton::OnEraseBkgnd(CDCHandle dc)
{
	// create memory DC
	dc.SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(dc.m_hDC);
	CBitmap pOldBitmap;

	SIZE bmSize;
	butimage[mgnum].GetSize(bmSize);
	pOldBitmap = pMemDC -> SelectBitmap(butimage[mgnum]);
	dc.BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;

	return TRUE;
}
//--------------------------------------------------------------
//|
//|Create 했으면 Desroy 아니면 이상 발생(종료시)
//|
//---------------------------------------------------------------
LRESULT CSuperButton::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	//버튼 미리 만들어 둔 이미지 삭제
	butClear();
	return 0;
}
//--------------------------------------------------------------
//|
//|버튼에 쓰이는 이미지 삭제
//|
//---------------------------------------------------------------
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
//|버튼 이미지 저장(이미지만 4단계의 이미지로 분리한다)
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
		::DeleteDC(hdc);
		if(butimage.size()==BUT_MAXNUM)
			return TRUE;
	}
	return FALSE;
}
#ifndef WINCE
//--------------------------------------------------------------
//|
//|후버 버튼 
//|
//---------------------------------------------------------------
//버튼의 후버를 위한 함수
void CSuperButton::OnMouseHover(WPARAM wParam, CPoint ptPos)
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}

void CSuperButton::OnMouseLeave()
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}
void CSuperButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
}
#endif //WINCE
#ifdef WINCE
//--------------------------------------------------------------
//|
//|CE 이미지 로드(리소스에서 이미지 로드, png 파일)
//|
//---------------------------------------------------------------
BOOL CSuperButton::MakeResource(HMODULE hMod, UINT resId, LPCWSTR type)
{
	BOOL b = FALSE;
	HRSRC hResInfo = FindResource(hMod, MAKEINTRESOURCE(resId), type);
	if(hResInfo != NULL)
	{        
		DWORD dwImageSize = SizeofResource(hMod, hResInfo);
		HGLOBAL hResData = LoadResource(hMod, hResInfo);
		LPVOID data = LockResource(hResData);

		IImagingFactory *pImgFactory = NULL;
		IImage *pImage = NULL;
		ImageInfo imageInfo;
		//CoInitializeEx(NULL, COINIT_MULTITHREADED);

		if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, 
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IImagingFactory, 
			(void **)&pImgFactory)))
		{
			pImgFactory->CreateImageFromBuffer(data, 
				(UINT)dwImageSize, 
				BufferDisposalFlagNone, 
				&pImage);
			pImage->GetImageInfo(&imageInfo);

			LONG width=imageInfo.Width;///BUT_MAXNUM;
			LONG Height=imageInfo.Height;
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

			void *pBmpBits = NULL;
			CBitmap h32BitBmp;
			if(h32BitBmp.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
			{
				BITMAP bmpInfo32;
				::GetObject(h32BitBmp, sizeof(bmpInfo32), &bmpInfo32);
				if(bmpInfo32.bmWidthBytes % 4 > 0)
					bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
				CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
				RECT dstRect;
				SetRect(&dstRect,0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight);
				pImage->Draw(hMemDC, &dstRect, NULL); //화면에 바로 그려주는 부분. 속도 개선을 위해 메모리에 그려주고 그것을
				::SelectObject(hMemDC, hOldBmp);
				b = DivideHbitmap(h32BitBmp);
			}
			::DeleteDC(hMemDC);
			::DeleteDC(hdc);
			pImage->Release();
			pImgFactory->Release();
		}
		//CoUninitialize();
	}
	return b;
}
#endif //WINCE


