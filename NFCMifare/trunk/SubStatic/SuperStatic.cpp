
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------
//
#ifdef WINCE
//#include <InitGuid.h>
#include <imaging.h>
#endif //WINCE
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSuperStatic::CSuperStatic(void):
mgnum(0),
bNeedImage(TRUE)
{
	staticimage.clear();
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSuperStatic::~CSuperStatic(void)
{
}
//--------------------------------------------------------------
//|
//|서브 클래싱 함수
//|
//---------------------------------------------------------------
BOOL CSuperStatic::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperStatic, CStatic>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initSuperStatic(UINT ImageId, BOOL bOwnerDraw)
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

	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(ImageId,"PNG");
	CBitmap bit = pBitmap.GetWinBitmap();
	if(DivideHbitmap(HBITMAP(bit)))
	{
		SIZE bmSize;
		staticimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
		}
		//자신의 위치 저장
		CRect rc;
		GetWindowRect(&rc);
		xpos = rc.left;
		ypos = rc.top;
	}
}
#ifdef WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initSuperStatic(UINT ImageId, LPCWSTR type, BOOL bOwnerDraw)
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
		SIZE bmSize;
		staticimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
		}
		//자신의 위치 저장
		CRect rc;
		GetWindowRect(&rc);
		xpos = rc.left;
		ypos = rc.top;
	}
}
#endif //WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initSuperStatic(CBitmapHandle bit, BOOL bOwnerDraw)
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
		SIZE bmSize;
		staticimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
		}
	}
	//자신의 위치 저장
	CRect rc;
	GetWindowRect(&rc);
	xpos = rc.left;
	ypos = rc.top;
	//CBitmapHandle h32BitBmp = (HBITMAP)CopyImage((HANDLE)bit, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	//staticimage.push_back(h32BitBmp);
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initRgnSuperStatic(UINT ImageId, UINT MaskId, BOOL bOwnerDraw)
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

	//Rgn 이미지 저장
	_GARgn::ImageLoadRgn(_Module.GetResourceInstance(),MaskId ,"PNG",m_hRgn);

	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(ImageId,"PNG");
	CBitmap bit = pBitmap.GetWinBitmap();
	if(bit && RgnDivideHbitmap(HBITMAP(bit)))
	{
		SIZE bmSize;
		staticimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
		}
		//자신의 위치 저장
		CRect rc;
		GetWindowRect(&rc);
		xpos = rc.left;
		ypos = rc.top;
	}
}
//--------------------------------------------------------------
//|
//|이미지 추가 함수
//|
//---------------------------------------------------------------
void CSuperStatic::AddImagedata(CBitmapHandle bit)
{
	if(DivideHbitmap(bit))
	{
	}
	//CBitmapHandle h32BitBmp = (HBITMAP)CopyImage((HANDLE)bit, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	//staticimage.push_back(h32BitBmp);
}
//--------------------------------------------------------------
//|
//|이미지 추가 함수
//|
//---------------------------------------------------------------
void CSuperStatic::AddImagedata(UINT ImageId)
{
	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(ImageId,"PNG");
	CBitmap bit = pBitmap.GetWinBitmap();
	if(DivideHbitmap(HBITMAP(bit)))
	{
	}
	//CBitmapHandle h32BitBmp = (HBITMAP)CopyImage((HANDLE)bit, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	//staticimage.push_back(h32BitBmp);
}
//--------------------------------------------------------------
//|
//|rgn + 이미지 추가 함수
//|
//---------------------------------------------------------------
void CSuperStatic::AddImagedataRgn(UINT ImageId, UINT MaskId)
{
	//Rgn
	CRgnHandle hRgn;
	_GARgn::ImageLoadRgn(_Module.GetResourceInstance(),MaskId ,"PNG",hRgn);
	SetWindowRgn(hRgn, true);

	CGdiPlusBitmapResource pBitmap;
	pBitmap.Load(ImageId,"PNG");
	CBitmap bit = pBitmap.GetWinBitmap();
	if(RgnDivideHbitmap(HBITMAP(bit)))
	{
	}
	//CBitmapHandle h32BitBmp = (HBITMAP)CopyImage((HANDLE)bit, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	//staticimage.push_back(h32BitBmp);
}
//--------------------------------------------------------------
//|
//|오너 드로우 함수
//|
//---------------------------------------------------------------
void CSuperStatic::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	CDC dc = lpdis->hDC;
#ifndef WINCE
	dc.SaveDC();
#endif //WINCE
	if(bNeedImage)
	{
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(lpdis->hDC);
		for(int staticset = 0; staticset<(int)staticimage.size(); staticset++)
		{
			CBitmapHandle pOldBitmap = pMemDC -> SelectBitmap(staticimage[staticset]);
			SIZE bmSize;
			staticimage[staticset].GetSize(bmSize);
			DrawWhiteText(pMemDC,bmSize);
			pMemDC -> SelectBitmap(pOldBitmap);
		}
		delete pMemDC;
		bNeedImage=FALSE;
	}
	DrawStatic(&dc,lpdis->itemState);
}
//--------------------------------------------------------------
//|
//|그려주는 함수
//|
//---------------------------------------------------------------
void CSuperStatic::DrawStatic(CDC * pDC, UINT state)
{
	if(staticimage.size()==0)
		return;
	// create memory DC
	pDC->SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC->m_hDC);
	CBitmapHandle pOldBitmap;

	HRGN hRgn;
	if(m_hRgn)
	{
		// get region
		SetWindowRgn(m_hRgn, true);
		hRgn = CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);
		::SelectClipRgn(pDC ->m_hDC, hRgn);
	}
	SIZE bmSize;
	staticimage[mgnum].GetSize(bmSize);
	pOldBitmap = pMemDC -> SelectBitmap(staticimage[mgnum]);
	pDC -> BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
	if(m_hRgn)
	{
		::SelectClipRgn(pDC -> m_hDC, NULL);
		// clean up
		DeleteObject(hRgn);
	}
	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;
}
//--------------------------------------------------------------
//|
//|폰트 설정
//|
//---------------------------------------------------------------
void CSuperStatic::SetSuperStaticFont(LOGFONT* _fontinfo,COLORREF* _fontcolor)
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
void CSuperStatic::DrawWhiteText(CDC* pDC, SIZE bmSize)
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
BOOL CSuperStatic::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|서브 클래싱 해제
//|
//---------------------------------------------------------------
HWND CSuperStatic::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperStatic, CStatic>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|버튼 Create 함수 
//|
//---------------------------------------------------------------
int CSuperStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
BOOL CSuperStatic::OnEraseBkgnd(CDCHandle dc)
{
// 	if(staticimage.size()!=0)
// 	{
// 		// create memory DC
// 		dc.SetBkColor(RGB(255,0,0));
// 		CDC * pMemDC = new CDC;
// 		pMemDC -> CreateCompatibleDC(dc.m_hDC);
// 		CBitmapHandle pOldBitmap;
// 
// 		SIZE bmSize;
// 		staticimage[mgnum].GetSize(bmSize);
// 		pOldBitmap = pMemDC -> SelectBitmap(staticimage[mgnum]);
// 		dc.BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
// 		pMemDC -> SelectBitmap(pOldBitmap);
// 		delete pMemDC;
// 	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|Create 했으면 Desroy 아니면 이상 발생(종료시)
//|
//---------------------------------------------------------------
LRESULT CSuperStatic::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	//버튼 미리 만들어 둔 이미지 삭제
	StaticClear();
	return 0;
}
//--------------------------------------------------------------
//|
//|버튼에 쓰이는 이미지 삭제
//|
//---------------------------------------------------------------
void CSuperStatic::StaticClear()
{
	for (int staticImg=0; staticImg<(int)staticimage.size(); staticImg++)
	{
		DeleteObject(staticimage[staticImg]);
	}
	staticimage.clear();
	if(m_hRgn)
	{
		m_hRgn.DeleteObject();
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperStatic::DivideHbitmap(CBitmapHandle hbit)
{
	SIZE bmSize;
	if(hbit.GetSize(bmSize))
	{
		width=bmSize.cx;
		Height=bmSize.cy;
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
		CBitmapHandle hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hbit);
		void *pBmpBits = NULL;
		CBitmapHandle h32BitBmp;
		if(h32BitBmp.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
		{
			BITMAP bmpInfo32;
			::GetObject(h32BitBmp, sizeof(bmpInfo32), &bmpInfo32);
			if(bmpInfo32.bmWidthBytes % 4 > 0)
				bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
			CBitmapHandle hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
			::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hBmpDC, 0, 0, width,Height, SRCCOPY);
			::SelectObject(hMemDC, hOldBmp);
			staticimage.push_back(h32BitBmp);
		}
		::SelectObject(hBmpDC, hOldBmp2);
		::DeleteDC(hBmpDC);
		::DeleteDC(hMemDC);
		::DeleteDC(hdc);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperStatic::RgnDivideHbitmap(CBitmapHandle hbit)
{
	SIZE bmSize;
	if(hbit.GetSize(bmSize))
	{
		width=bmSize.cx;
		Height=bmSize.cy;
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
		CBitmapHandle hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hbit);
		void *pBmpBits = NULL;
		CBitmapHandle h32BitBmp;
		if(h32BitBmp.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
		{
			BITMAP bmpInfo32;
			::GetObject(h32BitBmp, sizeof(bmpInfo32), &bmpInfo32);
			if(bmpInfo32.bmWidthBytes % 4 > 0)
				bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
			CBitmapHandle hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);

			//최종 이미지 만들기
			::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hBmpDC, 0, 0, width,Height, SRCCOPY);
			::SelectObject(hMemDC, hOldBmp);
			staticimage.push_back(h32BitBmp);
		}
		::SelectObject(hBmpDC, hOldBmp2);
		::DeleteDC(hBmpDC);
		::DeleteDC(hMemDC);
		::DeleteDC(hdc);
		return TRUE;
	}
	return FALSE;
}
//
void CSuperStatic::SetChangeXPos()
{
	xpos-=width;
	if(xpos<0)
	{
		xpos=width;
	}
	MoveWindow(xpos,ypos, width, Height,TRUE);
}
//
BOOL CSuperStatic::MoveXAnimation(int& xmove)
{
	BOOL b=FALSE;
	if(xmove>width)
	{
		xmove=width;
		b = TRUE;
	}
	MoveWindow(xpos-xmove,ypos, width, Height,TRUE);
	return b;
}
//
void CSuperStatic::SetChangeYPos(HDWP _hdwp)
{
	ypos-=Height;
	if(ypos<0)
	{
		ypos=Height;
	}
	DeferWindowPos(_hdwp, HWND_TOP,xpos,ypos, width, Height,SWP_NOSIZE);
	//MoveWindow(xpos,ypos, width, Height,TRUE);
}
//
BOOL CSuperStatic::MoveYAnimation(HDWP _hdwp, int& ymove)
{
	BOOL b=FALSE;
	if(ymove>Height)
	{
		ymove=Height;
		b = TRUE;
	}
	//MoveWindow(xpos, ypos-ymove, width, Height,TRUE);
	DeferWindowPos(_hdwp, HWND_TOP,xpos, ypos-ymove, width, Height,SWP_NOSIZE);
	return b;
}
#ifdef WINCE
//--------------------------------------------------------------
//|
//|CE 이미지 로드(리소스에서 이미지 로드, png 파일)
//|
//---------------------------------------------------------------
BOOL CSuperStatic::MakeResource(HMODULE hMod, UINT resId, LPCWSTR type)
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
				CBitmapHandle hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
				RECT dstRect;
				SetRect(&dstRect,0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight);
				pImage->Draw(hMemDC, &dstRect, NULL); //화면에 바로 그려주는 부분. 속도 개선을 위해 메모리에 그려주고 그것을
				::SelectObject(hMemDC, hOldBmp);
				staticimage.push_back(h32BitBmp);
				b = TRUE;
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


