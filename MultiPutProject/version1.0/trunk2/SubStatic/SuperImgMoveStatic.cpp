
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
CSuperImgMoveStatic::CSuperImgMoveStatic(void):
CWindowImpl< CSuperImgMoveStatic, CStatic >( ),
fontpoint(0,0),
ownname(_T("")),
ownnamecount(0)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSuperImgMoveStatic::~CSuperImgMoveStatic(void)
{
}
//--------------------------------------------------------------
//|
//|서브 클래싱 함수
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperImgMoveStatic, CStatic>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|커서
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
	static HCURSOR hcur = LoadCursor (NULL, MAKEINTRESOURCE(IDC_HANDCURSOR));
	if ( NULL != hcur)
	{
		SetCursor ( hcur );
		return TRUE;
	}
	else
	{
		SetMsgHandled(false);
		return FALSE;
	}
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::initSuperImgMoveStatic(CString _ownname, UINT ImageId, BOOL bOwnerDraw, BOOL bsize)
{
	//자신의 이름
	ownname=_ownname;
	//디폴트 폰트 설정
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
		//이미지 저장 이름
		TCHAR Imagename[MAX_PATH];
		sprintf_s(Imagename, _T("%s_%04d"),ownname, ++ownnamecount);
		if(ImgMoveStaticDivideHbitmap(Imagename, bit))
		{
			if(bsize)
			{
				SIZE bmSize;
				CBitmap bitimg = imagestorage.GetImageBitmap();
				bitimg.GetSize(bmSize);
				SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
			}
		}
	}
	if(bOwnerDraw)
	{
		ModifyStyle (0x0FL, SS_OWNERDRAW , SWP_FRAMECHANGED);
	}
}
#ifdef WINCE
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::initSuperImgMoveStatic(CString _ownname, UINT ImageId, LPCWSTR type, BOOL bsize)
{
	//자신의 이름
	ownname=_ownname;
	//디폴트 폰트 설정
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

	//이미지 저장 이름
	if(MakeResource(GetModuleHandle(NULL),ImageId,type))
	{
		if(bsize)
		{
			SIZE bmSize;
			CBitmap bitimg = imagestorage.GetImageBitmap();
			bitimg.GetSize(bmSize);
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
void CSuperImgMoveStatic::initSuperImgMoveStatic(CString _ownname, CBitmap bit, BOOL bOwnerDraw, BOOL bsize)
{
	//자신의 이름
	ownname=_ownname;
	//디폴트 폰트 설정
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
	//이미지 저장 이름
	TCHAR Imagename[MAX_PATH];
	sprintf_s(Imagename, _T("%s_%04d"),ownname, ++ownnamecount);
	if(ImgMoveStaticDivideHbitmap(Imagename, bit))
	{
		if(bsize)
		{
			SIZE bmSize;
			CBitmap bitimg = imagestorage.GetImageBitmap();
			bitimg.GetSize(bmSize);
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
BOOL CSuperImgMoveStatic::AddImageImgMoveStatic(CBitmap bit)
{
	//이미지 저장 이름
	TCHAR Imagename[MAX_PATH];
	sprintf_s(Imagename, _T("%s_%04d"),ownname, ++ownnamecount);
	if(bit&&ImgMoveStaticDivideHbitmap(Imagename, bit))
	{
		InvalidateRect(NULL, FALSE);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|이미지 추가 보여질 이미지 바꾸지 않고 추가
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::AddImageNoChangeBitmapp(CBitmap bit)
{
	//이미지 저장 이름
	TCHAR Imagename[MAX_PATH];
	sprintf_s(Imagename, _T("%s_%04d"),ownname, ++ownnamecount);
	if(bit&&ImgMoveStaticDivideHbitmap(Imagename, bit, FALSE))
	{
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|서버로 받은 이미지 추가
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::AddImageServerRecvImgMoveStatic(CString Imagename, CBitmap bit)
{
	if(bit&&ImgMoveStaticDivideHbitmap(Imagename, bit))
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
void CSuperImgMoveStatic::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
	CDCHandle dc = lpdis->hDC;
#ifndef WINCE
	dc.SaveDC();
#endif //WINCE
	CRect rc;
	GetClientRect(&rc);
	SIZE bmSize;
	CBitmap localimage = imagestorage.GetImageBitmap();
	if(localimage)
	{
		localimage.GetSize(bmSize);
		CBitmap pOldBitmap;
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(dc);
		pOldBitmap = pMemDC -> SelectBitmap( localimage );
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
		DrawImgMoveStaticWhiteText(&dc,CSize(rc.Width(), rc.Height()),wintext);
	}
}

//--------------------------------------------------------------
//|
//|폰트 설정
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::SetSuperImgMoveStaticFont(LOGFONT* _fontinfo,COLORREF* _fontcolor)
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
void CSuperImgMoveStatic::SetSuperImgMoveStaticFont(UINT lfHeight, CString lfFaceName, COLORREF* _fontcolor, CPoint* _fontpoint)
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
//|텍스트 쓴다
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::DrawImgMoveStaticWhiteText(CDCHandle* pDC, CSize winSize, CString text)
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
void CSuperImgMoveStatic::SetText(CString text)
{
	if(text.GetLength())
	{
		SetWindowText(text);
		InvalidateRect(NULL,TRUE);
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::PreTranslateMessage(MSG* pMsg)
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
HWND CSuperImgMoveStatic::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperImgMoveStatic, CStatic>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|버튼 Create 함수 
//|
//---------------------------------------------------------------
int CSuperImgMoveStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
BOOL CSuperImgMoveStatic::OnEraseBkgnd(CDCHandle dc)
{
	//백그라운드 설정
	CRect rc;
	GetClientRect(&rc);
	SIZE bmSize;
	CBitmap localimage = imagestorage.GetImageBitmap();;
	if(localimage)
	{
		localimage.GetSize(bmSize);
		CBitmap pOldBitmap;
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(dc);
		pOldBitmap = pMemDC -> SelectBitmap( localimage );
		int oldMode = dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(),pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy,SRCCOPY);
		dc.SetStretchBltMode(oldMode);
		pMemDC -> SelectBitmap(pOldBitmap);
		delete pMemDC;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|Create 했으면 Desroy 아니면 이상 발생(종료시)
//|
//---------------------------------------------------------------
LRESULT CSuperImgMoveStatic::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	//버튼 미리 만들어 둔 이미지 삭제
	ImgMoveStaticClear();
	return 0;
}
//--------------------------------------------------------------
//|
//|버튼에 쓰이는 이미지 삭제
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::ImgMoveStaticClear()
{
	imagestorage.DeleteImageBitmapAll();
}
//--------------------------------------------------------------
//|
//|디바이스 호환 비트맵 저장
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::ImgMoveStaticDivideHbitmap(CString Imagename, CBitmap hbit, BOOL bViewChange)
{
	BOOL b = FALSE;
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
			//ATL Map 저장
			imagestorage.AddImageBitmap(Imagename, h32BitBmp, bViewChange);
			 b = TRUE;
		}
		::SelectObject(hBmpDC, hOldBmp2);
		::DeleteDC(hBmpDC);
		::DeleteDC(hMemDC);
		::DeleteDC(hdc);
	}
	return b;
}
#ifdef WINCE
//--------------------------------------------------------------
//|
//|CE 이미지 로드(리소스에서 이미지 로드, png 파일)
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::MakeResource(HMODULE hMod, UINT resId, LPCWSTR type)
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
				//ATL Map 저장
				//이미지 저장 이름
				TCHAR Imagename[MAX_PATH];
				sprintf_s(Imagename, _T("%s_%04d"),ownname, ++ownnamecount);
				imagestorage.AddImageBitmap(filename, h32BitBmp);
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
//--------------------------------------------------------------
//|
//|디바이스 호환 비트맵 저장
//|
//---------------------------------------------------------------
//이미지 이름 중에서 첫번째 보여주기
BOOL CSuperImgMoveStatic::ShowFirstImageBitmap()
{
	imagestorage.GetFirstImageBitmap();
	InvalidateRect(NULL, FALSE);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|디바이스 호환 비트맵 저장
//|
//---------------------------------------------------------------
//이미지 중에서 다음 검색되는 이미지 보여주기
BOOL CSuperImgMoveStatic::ShowNextImageBitmap()
{
	imagestorage.GetNextImageBitmap();
	InvalidateRect(NULL, FALSE);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|현재 보여지고 있는 이미지를 파일로 만든다(jpg)
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::MakeSendFileJpg(CString& filename)
{
	BOOL b=FALSE;
	CBitmap localimage = imagestorage.GetImageBitmap();
	if(localimage)
	{
		CString name = imagestorage.GetDrawBitmapName();
		filename=name;
		name=name+_T(".jpg");
		BSTR bstrString = name.AllocSysString();
		LPWSTR pwstr = (LPWSTR)bstrString;
		b = CGdiPlusBitmap::HbitmapSaveasImageFile(pwstr, L"image/jpeg",localimage);
		SysFreeString(bstrString); //메모리 해제.
	}
	return b;
}
//--------------------------------------------------------------
//|
//|리시브 할 파일 검색하여 있으면 보여준다
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::RecvImageFileFind(CString filename)
{
	BOOL b=FALSE;
	CBitmap localimage = imagestorage.GetImageBitmap(filename);
	if(localimage)
	{
		InvalidateRect(NULL, FALSE);
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|마우스 위치가 내가 그려진 영역에 있는가
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::MouseCursorArea(CPoint _pt)
{
	BOOL b= FALSE;
	CRect winrc;
	GetWindowRect(winrc);
	if((winrc.left<_pt.x&&winrc.right>_pt.x)&&(winrc.top<_pt.y&&winrc.bottom>_pt.y))
		b=TRUE;
	return b;
}