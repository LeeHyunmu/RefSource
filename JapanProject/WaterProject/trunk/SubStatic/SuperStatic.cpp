
#include "stdAfx.h"
#include "SuperStatic.h"
#ifdef WINCE
#include <imaging.h>
#endif //WINCE
//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------
//
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSuperStatic::CSuperStatic(void):
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
	BOOL bRet = CWindowImpl<CSuperStatic, CButton>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initSuperStatic(UINT ImageId, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw)
{
	CBitmap bit;
	bit.LoadOEMBitmap(ImageId);
	if(DivideHbitmap(bit, imgcount, _bXCount))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW , SWP_FRAMECHANGED); 
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
void CSuperStatic::initSuperStatic(UINT ImageId, LPCWSTR type, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw)
{
	if(MakeResource(GetModuleHandle(NULL),ImageId,type, imgcount, _bXCount))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW , SWP_FRAMECHANGED);
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
void CSuperStatic::initSuperStatic(CBitmap bit, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw)
{
	if(DivideHbitmap(bit, imgcount, _bXCount))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW | BS_NOTIFY , SWP_FRAMECHANGED);
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
//|초기화 함수
//|
//---------------------------------------------------------------
void CSuperStatic::initRgnSuperStatic(UINT ImageId, UINT MaskId, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw)
{
	//Rgn 이미지 저장
	_GARgn::ImageLoadRgn(_Module.GetResourceInstance(),MaskId ,_T("PNG"),m_hRgn, m_hWnd);
	SetWindowRgn(m_hRgn, TRUE);
	if(MakeResource(_Module.GetResourceInstance(),ImageId,_T("PNG"), imgcount, _bXCount))
	{
		bNeedImage=TRUE;
		SIZE bmSize;
		butimage[0].GetSize(bmSize);
		SetWindowPos(NULL, 0,0, bmSize.cx,bmSize.cy,SWP_NOMOVE|SWP_NOOWNERZORDER);
		if(bOwnerDraw)
		{
			ModifyStyle (0x0FL, BS_OWNERDRAW  , SWP_FRAMECHANGED);
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
	DrawStatic(&dc,lpdis->itemState);
}
//--------------------------------------------------------------
//|
//|그려주는 함수
//|
//---------------------------------------------------------------
void CSuperStatic::DrawStatic(CDC * pDC, UINT state)
{
	if(butimage.size()<=0)
		return;
	// create memory DC
	pDC->SetBkColor(RGB(255,0,0));
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC->m_hDC);
	CBitmap pOldBitmap;

	CBrush brush;
	brush.CreateSolidBrush (RGB(0xff,0x00,0x00));

	CRgn hRgn=NULL;
	if(m_hRgn)
	{
		hRgn = CreateRectRgn(0, 0, 0, 0);
		GetWindowRgn(hRgn);
		::SelectClipRgn(pMemDC->m_hDC, m_hRgn);
	}

	SIZE bmSize;
	butimage[mgnum].GetSize(bmSize);
	pOldBitmap = pMemDC -> SelectBitmap(butimage[mgnum]);
	pDC -> BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
	if(hRgn)
	{
		pDC->SelectClipRgn(NULL);
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
	if(title.GetLength()&&fontinfo.lfFaceName)
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
	return CWindowImpl<CSuperStatic, CButton>::UnsubclassWindow(bForce);
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
	for (int but=0; but<(int)butimage.size(); but++)
	{
		DeleteObject(butimage[but]);
	}
	butimage.clear();
	if(m_hRgn)
	{
		m_hRgn.DeleteObject();
	}
}
//--------------------------------------------------------------
//|
//|버튼 이미지 저장(이미지만 4단계의 이미지로 분리한다)
//|
//---------------------------------------------------------------
BOOL CSuperStatic::DivideHbitmap(CBitmap hbit, UINT imgcount, BOOL _bXCount)
{
	SIZE bmSize;
	if(hbit.GetSize(bmSize))
	{
		if(_bXCount)
		{
			width=bmSize.cx/imgcount;
			Height=bmSize.cy;
		}
		else
		{
			width=bmSize.cx;
			Height=bmSize.cy/imgcount;
		}
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

		for(int divide = 0; divide<BUT_MAXNUM; divide++)
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
				//최종 이미지
				if(_bXCount)
				{
					::StretchBlt(hMemDC, 0, 0, width*divide, Height,hBmpDC, bmpInfo32.bmWidth - (width*divide), 0, width*divide,Height, SRCCOPY);
					::StretchBlt(hMemDC, width* divide, 0, bmpInfo32.bmWidth - (width*divide), Height,hBmpDC, 0, 0, bmpInfo32.bmWidth - (width*divide),Height, SRCCOPY);	
				}
				else
				{
					::StretchBlt(hMemDC, 0, 0, width, Height*divide,hBmpDC, 0, bmpInfo32.bmHeight - (Height*divide), width, Height*divide, SRCCOPY);
					::StretchBlt(hMemDC, 0, Height* divide, width, bmpInfo32.bmHeight - (Height*divide),hBmpDC, 0, 0, width,bmpInfo32.bmHeight - (Height*divide), SRCCOPY);					
				}	
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
#ifdef WINCE
//--------------------------------------------------------------
//|
//|CE 이미지 로드(리소스에서 이미지 로드, png 파일)
//|
//---------------------------------------------------------------
BOOL CSuperStatic::MakeResource(HMODULE hMod, UINT resId, LPCWSTR type, UINT imgcount, BOOL _bXCount)
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
				b = DivideHbitmap(h32BitBmp, imgcount, _bXCount);
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


