
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
bNeedImage(TRUE),
fontpoint(30,0),
btransperent(FALSE),
transperentcolor(RGB(0,0,0))
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

	butoriginimage.LoadBitmap(ImageId);
	if(butoriginimage)
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butoriginimage.GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx/BUT_MAXNUM,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW | BS_PUSHBUTTON, SWP_FRAMECHANGED);
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

	butoriginimage =MakeResource(GetModuleHandle(NULL),ImageId,type);
	if(butoriginimage)
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butoriginimage.GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx/BUT_MAXNUM,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW | BS_PUSHBUTTON, SWP_FRAMECHANGED);
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
	if(bit)
	{
		butoriginimage = (HBITMAP)CopyImage((HANDLE)bit, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
		if(butoriginimage)
		{
			bNeedImage=TRUE;
			SIZE bmSize;
			butoriginimage.GetSize(bmSize);
			SetWindowPos(NULL, 0,0, bmSize.cx/BUT_MAXNUM,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
			if(bOwnerDraw)
			{
				ModifyStyle (0x0FL, BS_OWNERDRAW | BS_PUSHBUTTON, SWP_FRAMECHANGED);
			}
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
	CDCHandle dc = lpdis->hDC;
#ifndef WINCE
	dc.SaveDC();
#endif //WINCE
	if(bNeedImage)
	{
		CBitmap copybitImage = (HBITMAP)CopyImage((HANDLE)butoriginimage, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
		DivideHbitmap(dc, copybitImage);
		bNeedImage=FALSE;
	}
	DrawButton(&dc,lpdis->itemState);
}
//--------------------------------------------------------------
//|
//|그려주는 함수
//|
//---------------------------------------------------------------
void CSuperButton::DrawButton(CDCHandle* pDC, UINT state)
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
	CRect rc;
	butimage[mgnum].GetSize(bmSize);
	GetClientRect(&rc);

	pOldBitmap = pMemDC -> SelectBitmap(butimage[mgnum]);
	if(btransperent)
		pDC -> TransparentBlt(0,0,rc.Width(), rc.Height(),pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy, transperentcolor);
	else
		pDC -> BitBlt(0,0,rc.Width(), rc.Height(),pMemDC->m_hDC,0,0,SRCCOPY);
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
void CSuperButton::DrawWhiteText(CDCHandle* pDC, CSize winSize, CString text)
{
	if(text.GetLength())
	{
		CFont font;
		font.CreateFontIndirect(&fontinfo);
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontcolor);
		HFONT hOldFont  = pDC->SelectFont (font);
		CRect textrc(fontpoint.x,fontpoint.y,winSize.cx,winSize.cy);
		pDC->DrawText (text, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);

		pDC->SelectFont (hOldFont);
		pDC->SetBkMode(nOldMode);
	}
}
//--------------------------------------------------------------
//|
//|폰트 설정
//|
//---------------------------------------------------------------
void CSuperButton::SetSuperButFont(UINT lfHeight, CString lfFaceName, COLORREF* _fontcolor, CPoint* _fontpoint)
{
	//폰트 크기 변경
	if(lfHeight)
		fontinfo.lfHeight   = lfHeight;
	//폰트채 변경
	if(lfFaceName.GetLength())
		_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), lfFaceName);
	//폰트 색 변경
	if(_fontcolor)
		fontcolor=*_fontcolor;
	//폰트 시작 위치 변경
	if(_fontpoint)
		fontpoint.SetPoint(_fontpoint->x, _fontpoint->y);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperButton::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN||pMsg->message==WM_KEYUP)
	{
		return FALSE;
	}
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
	//백그라운드 투명 설정
	if(btransperent)
		dc.SetBkMode(TRANSPARENT);
	else
	{
		RECT cRect;
		GetClientRect( &cRect );
		dc.FillSolidRect( &cRect, GetSysColor( CTLCOLOR_STATIC ) );
	}
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
//|특정색 빼기
//|
//---------------------------------------------------------------
void CSuperButton::SetTransperent(COLORREF _transperentcolor, BOOL _btransperent)
{
	transperentcolor = _transperentcolor;
	btransperent = _btransperent;
}
//--------------------------------------------------------------
//|
//|버튼 이미지 저장(이미지만 4단계의 이미지로 분리한다)
//|
//---------------------------------------------------------------
BOOL CSuperButton::DivideHbitmap(HDC hdc, CBitmap hbit)
{
	SIZE bmSize;
	if(hbit&&hbit.GetSize(bmSize))
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

		HDC hMemDC = ::CreateCompatibleDC(hdc);
		HDC hBmpDC = ::CreateCompatibleDC(hMemDC);
		CBitmap hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hbit);
		//텍스트(버튼 텍스트)
		CString title;
		GetWindowText( title );
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
				//텍스트 쓰기
				if(title.GetLength())
					DrawWhiteText(&CDCHandle(hMemDC), CSize(bmpInfo32.bmWidth, bmpInfo32.bmHeight), title);
				::SelectObject(hMemDC, hOldBmp);
				butimage.push_back(h32BitBmp);
			}
		}
		::SelectObject(hBmpDC, hOldBmp2);
		::DeleteDC(hBmpDC);
		::DeleteDC(hMemDC);
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
CBitmap CSuperButton::MakeResource(HMODULE hMod, UINT resId, LPCWSTR type)
{
	CBitmap h32BitBmp=NULL;
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
			}
			::DeleteDC(hMemDC);
			::DeleteDC(hdc);
			pImage->Release();
			pImgFactory->Release();
		}
		//CoUninitialize();
	}
	return h32BitBmp;
}
#endif //WINCE
