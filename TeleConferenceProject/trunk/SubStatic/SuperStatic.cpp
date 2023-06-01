
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
CWindowImpl< CSuperStatic, CStatic >( ),
mgnum(0),
fontpoint(0,0),
btransparent(TRUE),
drawalpha(RGB(100,100,100))
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
void CSuperStatic::initSuperStatic(UINT ImageId, BOOL bOwnerDraw, BOOL bsize)
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
	//폰트 색
	fontcolor=0;
	if(ImageId)
	{
		CBitmap bit;
		bit.LoadBitmap(ImageId);
		if(DivideHbitmap(bit))
		{
			if(bsize)
			{
				SIZE bmSize;
				staticimage[0].GetSize(bmSize);
				SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
			}
			if(bOwnerDraw)
			{
				ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
			}
		}
	}
}
#ifdef WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initSuperStatic(UINT ImageId, LPCWSTR type, BOOL bsize)
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
		if(bsize)
		{
			SIZE bmSize;
			staticimage[0].GetSize(bmSize);
			SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		}
	}
}
#endif //WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initSuperStatic(CBitmap bit, BOOL bOwnerDraw, BOOL bsize)
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
	//폰트색
	fontcolor=0;
	CRect rc;
	GetClientRect(&rc);
	if(bit&&DivideHbitmap(bit))
	{
		if(bsize)
		{
			SIZE bmSize;
			staticimage[0].GetSize(bmSize);
			SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		}
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
		}
	}
}
//--------------------------------------------------------------
//|
//|이미지 추가
//|
//---------------------------------------------------------------
BOOL CSuperStatic::AddImage(CBitmap bit, COLORREF* _drawalpha)
{
	if(_drawalpha)
		drawalpha = *_drawalpha;
	if(bit&&DivideHbitmap(bit))
	{
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|오너 드로우 함수
//|
//---------------------------------------------------------------
void CSuperStatic::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	CDCHandle dc = lpdis->hDC;
#ifndef WINCE
	dc.SaveDC();
#endif //WINCE
	CRect rc;
	GetClientRect(&rc);
	SIZE bmSize;
	CBitmap localimage = GetSuperStaticBitmap(0);
	if(localimage)
	{
		localimage.GetSize(bmSize);
		CBitmap membitmap;
		CBitmap pOldBitmap;
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(dc);
		membitmap.CreateCompatibleBitmap( dc, bmSize.cx, bmSize.cy);
		pOldBitmap = pMemDC -> SelectBitmap( membitmap );
		//DrawTransperImage(dc);
		DrawStatic(pMemDC,lpdis->itemState);
		int oldMode = dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(),pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy,SRCCOPY);
		dc.SetStretchBltMode(oldMode);
		pMemDC -> SelectBitmap(pOldBitmap);
		delete pMemDC;
	}
	//윈도우 텍스트 가져오기
	CString wintext;
	GetWindowText(wintext);
	if(wintext.GetLength())
	{
		DrawWhiteText(&dc,CSize(rc.Width(), rc.Height()),wintext);
	}
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
	//pDC->SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC->m_hDC);

	CBitmap pOldBitmap;
	//int oldMode = pDC->SetStretchBltMode(COLORONCOLOR);
	for(int drawloop = 0; drawloop < (int)staticimage.size(); drawloop++)
	{
		SIZE bmSize;
		CBitmap localimage = GetSuperStaticBitmap(drawloop);
		localimage.GetSize(bmSize);
		if(drawloop==0)
		{
			pOldBitmap = pMemDC -> SelectBitmap(localimage);
			pDC -> StretchBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy,SRCCOPY);
		}
		else
		{
			pMemDC -> SelectBitmap(localimage);
			pDC -> TransparentBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy,drawalpha);
		}
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
	}
	if(_fontcolor)
	{
		fontcolor=*_fontcolor;
	}
}
//--------------------------------------------------------------
//|
//|폰트 설정
//|
//---------------------------------------------------------------
void CSuperStatic::SetSuperStaticFont(UINT lfHeight, CString lfFaceName, COLORREF* _fontcolor, CPoint* _fontpoint, BOOL* _btransparent)
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
	//배경 불투명으로
	if(_btransparent)
		btransparent=*_btransparent;
}
//--------------------------------------------------------------
//|
//|텍스트 쓴다
//|
//---------------------------------------------------------------
void CSuperStatic::DrawWhiteText(CDCHandle* pDC, CSize winSize, CString text)
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
//|텍스트 설정
//|
//---------------------------------------------------------------
void CSuperStatic::SetText(CString text)
{
	if(text.GetLength())
	{
		SetWindowText(text);
		ShowWindow(SW_HIDE);
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
	//백그라운드 투명 설정
	if(btransparent)
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
}
//--------------------------------------------------------------
//|
//|디바이스 호환 비트맵 저장
//|
//---------------------------------------------------------------
BOOL CSuperStatic::DivideHbitmap(CBitmap hbit)
{
	SIZE bmSize;
	if(hbit&&hbit.GetSize(bmSize))
	{
		LONG width=bmSize.cx;
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
		void *pBmpBits = NULL;
		CBitmapHandle h32BitBmp;
		if(h32BitBmp.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
		{
			BITMAP bmpInfo32;
			::GetObject(h32BitBmp, sizeof(bmpInfo32), &bmpInfo32);
			if(bmpInfo32.bmWidthBytes % 4 > 0)
				bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
			CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
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
				CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
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
void CSuperStatic::DrawTransperImage(CDC dc)
{
	CDC dc_background;
	CDC dc_image;
	CDC dc_mask;
	CBitmap bmp_background, p_bmp_background;
	CBitmap bmp_image, p_bmp_image;
	CBitmap bmp_mask, p_bmp_mask;
	BITMAP bmsize_background;
	BITMAP bmsize_image;
	COLORREF clr_mask = RGB(0,0,255);	// BACKGROUND COLOR : BLUE
	COLORREF old_clr_bkgnd;
	COLORREF old_clr_text;
	CPoint position(0,0);

	// 호환DC 생성
	dc_background.CreateCompatibleDC(dc);
	dc_image.CreateCompatibleDC(dc);
	dc_mask.CreateCompatibleDC(dc);

	// RESOURCE 에서 BITMAP LOAD
	// MASK BITMAP 생성
	bmp_background.LoadBitmap(IDB_BITMAP2);
	bmp_image.LoadBitmap(IDB_BITMAP2);
	bmp_background.GetBitmap(&bmsize_background);
	bmp_image.GetBitmap(&bmsize_image);
	bmp_mask.CreateBitmap(bmsize_image.bmWidth, bmsize_image.bmHeight, 1, 1, NULL);

	// DC에서 BITMAP OBJECT 선택
	p_bmp_background = dc_background.SelectBitmap(bmp_background);
	p_bmp_image = dc_image.SelectBitmap(bmp_image);
	p_bmp_mask = dc_mask.SelectBitmap(bmp_mask);

	// RESOURCE에서 읽은 BITMAP을 화면에 표시(background, image, mask)
	//dc.BitBlt(position.x, position.y, bmsize_background.bmWidth, bmsize_background.bmHeight, dc_background, 0, 0, SRCCOPY);
	position.y += bmsize_background.bmHeight + 5;
	//dc.BitBlt(position.x, position.y, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_image, 0, 0, SRCCOPY);
	position.y += bmsize_image.bmHeight + 5;
	//dc.BitBlt(position.x, position.y, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_mask, 0, 0, SRCCOPY);
	position.y += bmsize_image.bmHeight + 20;


	//-----------------------------------------------------------------------
	// 투명 비트맵 만들기 시작
	//-----------------------------------------------------------------------

	// 1. 마스크 비트맵을 생성
	old_clr_bkgnd = dc_image.SetBkColor(clr_mask);
	dc_mask.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_image, 0, 0, SRCCOPY);
	dc_image.SetBkColor(old_clr_bkgnd);
	///*1. draw mask*/ dc.BitBlt(position.x, position.y, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_mask, 0, 0, SRCCOPY);

	position.y += bmsize_image.bmHeight + 5;

	// 2. 배경에 마스크 비트맵 출력 - AND 연산
	dc_background.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_mask, 0, 0, SRCAND);
	///*2. draw background*/ dc.BitBlt(position.x, position.y, bmsize_background.bmWidth, bmsize_background.bmHeight, dc_background, 0, 0, SRCCOPY);

	position.y += bmsize_background.bmHeight + 5;

	// 3. 이미지에 마스크 비트맵 출력 - AND 연산
	old_clr_bkgnd = dc_image.SetBkColor(RGB(0, 0, 0));
	old_clr_text = dc_image.SetTextColor(RGB(255, 255, 255));
	dc_image.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_mask, 0, 0, SRCAND);
	dc_image.SetBkColor(old_clr_bkgnd);
	dc_image.SetTextColor(old_clr_text);
	///*3. draw image*/ dc.BitBlt(position.x, position.y, bmsize_background.bmWidth, bmsize_background.bmHeight, dc_image, 0, 0, SRCAND);

	position.y += bmsize_background.bmHeight + 5;

	// 4. 배경에 이미지를 출력 - OR 연산
	dc_background.BitBlt(0, 0, bmsize_image.bmWidth, bmsize_image.bmHeight, dc_image, 0, 0, SRCPAINT);
	///*4. draw background*/ dc.BitBlt(position.x, position.y, bmsize_background.bmWidth, bmsize_background.bmHeight, dc_background, 0, 0, SRCCOPY);

	position.y += bmsize_background.bmHeight + 20;

	// 최종 완성본
	dc.BitBlt(position.x, position.y, bmsize_background.bmWidth, bmsize_background.bmHeight, dc_background, 0, 0, SRCCOPY);

	// 메모리 해제 
	dc_background.SelectBitmap(p_bmp_background);
	dc_image.SelectBitmap(p_bmp_image);
	dc_mask.SelectBitmap(p_bmp_mask);

	dc_background.DeleteDC();
	dc_image.DeleteDC();
	dc_mask.DeleteDC();

	bmp_background.DeleteObject();
	bmp_image.DeleteObject();
	bmp_mask.DeleteObject();
}
//--------------------------------------------------------------
//|
//|비트맵을 넘긴다
//|
//---------------------------------------------------------------
CBitmap CSuperStatic::GetSuperStaticBitmap(int imgnum)
{
	if(imgnum>=0&& imgnum<(int)staticimage.size())
	{
		return (HBITMAP)CopyImage((HANDLE)staticimage[imgnum], IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	}
	return NULL;
}