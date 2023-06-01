
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
GARgn::GARgn(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
GARgn::~GARgn(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HRGN GARgn::CreateRoundRectRgn (int left, int top, int right, int bottom, int ellipse_width, int ellipse_height )
{
	HRGN              hrgn = 0;
	int               asq, bsq, d, xd, yd;
	RECT              rect;
	std::vector<RECT> rects;
	if (ellipse_width == 0 || ellipse_height == 0)
		return CreateRectRgn( left, top, right, bottom );
	/* Make the dimensions sensible */
	if (left > right ) { int tmp = left; left = right;  right  = tmp; }
	if (top  > bottom) { int tmp = top;  top  = bottom; bottom = tmp; }
	ellipse_width  = abs(ellipse_width);
	ellipse_height = abs(ellipse_height);
	/* Check parameters */
	if (ellipse_width  > right-left) ellipse_width  = right-left;
	if (ellipse_height > bottom-top) ellipse_height = bottom-top;
	/* Ellipse algorithm, based on an article by K. Porter */
	/* in DDJ Graphics Programming Column, 8/89 */
	asq = ellipse_width  * ellipse_width  / 4; /* a^2 */
	bsq = ellipse_height * ellipse_height / 4; /* b^2 */
	if (asq == 0) asq = 1;
	if (bsq == 0) bsq = 1;
	d  = bsq - asq * ellipse_height / 2 + asq / 4; /* b^2 - a^2b + a^2/4 */
	xd = 0;
	yd = asq * ellipse_height;                     /* 2a^2b */
	rect.left   = left + ellipse_width  / 2;
	rect.right  = right - ellipse_width / 2;
	rects.reserve (ellipse_height);
	/* Loop to draw first half of quadrant */
	while (xd < yd)
	{
		/* if nearest pixel is toward the center */
		if (d > 0)  
		{
			/* move toward center */
			rect.top    = top++;
			rect.bottom = rect.top + 1;
			rects.push_back (rect);
			rect.top    = --bottom;
			rect.bottom = rect.top + 1;
			rects.push_back (rect);
			yd -= 2*asq;
			d  -= yd;
		}
		/* next horiz point */
		rect.left --;        
		rect.right++;
		xd += 2*bsq;
		d  += bsq + xd;
	}
	/* Loop to draw second half of quadrant */
	d += (3 * (asq-bsq) / 2 - (xd+yd)) / 2;
	while (yd >= 0)
	{
		/* next vertical point */
		rect.top    = top++;
		rect.bottom = rect.top + 1;
		rects.push_back (rect);
		rect.top    = --bottom;
		rect.bottom = rect.top + 1;
		rects.push_back (rect);
		/* if nearest pixel is outside ellipse */
		if (d < 0)   
		{
			/* move away from center */
			rect.left --;     
			rect.right++;
			xd += 2*bsq;
			d  += xd;
		}
		yd -= 2*asq;
		d  += asq - yd;
	}
	/* Add the inside rectangle */
	if (top <= bottom)
	{
		rect.top    = top;
		rect.bottom = bottom;
		//rects.push_back (rect);
		rects.insert (rects.begin(), rect);
	}

	static int count=0;
	std::vector<RECT>::iterator it;
	for (it=rects.begin(); it!=rects.end(); it++)
	{
		ATLTRACE (_T("[%d]%3d,%3d:%3d,%3d \r\n"), ++count,
			(*it).top,
			(*it).bottom,
			(*it).left,
			(*it).right
			);
	}
	if (rects.empty())
		return NULL;
	RGNDATAHEADER* pRgnData;
	UINT           RgnDataSize;
	UINT           RgnRectCount;
	LPRECT         pRect;
	RgnRectCount = rects.size();
	RgnDataSize  = sizeof(RGNDATAHEADER) + sizeof(RECT)*RgnRectCount;
	pRgnData     = (RGNDATAHEADER*)new BYTE[ RgnDataSize ];
	if (pRgnData==NULL)
		return NULL;
	pRgnData->dwSize         = sizeof(RGNDATAHEADER);
	pRgnData->iType          = RDH_RECTANGLES;
	pRgnData->nCount         = RgnRectCount;
	pRgnData->rcBound.left   = 0;
	pRgnData->rcBound.top    = 0;
	pRgnData->rcBound.right  = 0;
	pRgnData->rcBound.bottom = 0;
	pRect = (LPRECT) ( (LPBYTE) pRgnData + sizeof(RGNDATAHEADER) );
	memcpy (pRect, &*rects.begin(), RgnRectCount*sizeof(RECT));
	hrgn = ExtCreateRegion(NULL, RgnDataSize, (LPRGNDATA)pRgnData);
	delete pRgnData;
	return hrgn;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL GARgn::ImageLoadRgn(HMODULE hModule,UINT ResourceName, LPCTSTR ResourceType, CRgnHandle& rgn)
//=============================================================================
{
	BOOL bResult = FALSE;
	HGLOBAL		hGlobal = NULL;
	HRSRC		hSource = NULL;
	LPVOID		lpVoid  = NULL;
	int			nSize   = 0;
	hSource = FindResource(hModule, MAKEINTRESOURCE(ResourceName), ResourceType);
	if(hSource == NULL)
	{
		return bResult;
	}
	hGlobal = LoadResource(hModule, hSource);
	if(hGlobal == NULL)
	{
		return bResult;
	}
	lpVoid = LockResource(hGlobal);
	if(lpVoid == NULL)
	{
		return bResult;
	}
	nSize = (UINT)SizeofResource(hModule, hSource);
	IStream* pStream = NULL;
	if(CreateStreamOnHGlobal(NULL, TRUE,(LPSTREAM*)&pStream) == S_OK)
	{
		CImage Texture;
		ULONG ulWrite = 0;
		pStream->Write(lpVoid,nSize,&ulWrite);
		Texture.Load(pStream);
		CBitmapHandle hbit;
		hbit=(HBITMAP)Texture.Detach();
		if(SetShapeBitmap(hbit,RGB(254,254,254),RGB(255,255,255),rgn))
			bResult=TRUE;
		hbit.DeleteObject();
	}
	if(pStream)
	{
		pStream->Release();
	}
	UnlockResource(hGlobal); // 16Bit Windows Needs This
	FreeResource(hGlobal); // 16Bit Windows Needs This (32Bit - Automatic Release)
	return bResult;
}
//--------------------------------------------------------------
//|
//|윈도우 비트맵으로 모양 만들기(Rgn 저장)
//|
//---------------------------------------------------------------
BOOL GARgn::SetShapeBitmap(CBitmapHandle hBitmap, COLORREF rgbstart,COLORREF rgbend, CRgnHandle& rgn)
{
	CRgn rgnBitmap;
	memset(&rgnBitmap,0,sizeof(CRgn));
	rgnBitmap.CreateRectRgn(0, 0, 0, 0);
	SIZE bmSize;
	hBitmap.GetSize(bmSize);
	BITMAPINFOHEADER Header32Bit;
	::memset(&Header32Bit, 0, sizeof(Header32Bit));
	Header32Bit.biSize   = sizeof(BITMAPINFOHEADER);
	Header32Bit.biWidth   = bmSize.cx;
	Header32Bit.biHeight  = bmSize.cy;
	Header32Bit.biPlanes  = 1;
	Header32Bit.biBitCount  = 32;
	Header32Bit.biCompression = BI_RGB;

	HDC hScreenDC = ::GetDC(NULL);
	HDC hMemDC = ::CreateCompatibleDC(hScreenDC);
	void *pBmpBits = NULL;
	CBitmapHandle h32BitBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&Header32Bit,	DIB_RGB_COLORS, &pBmpBits, NULL, 0);
	if(h32BitBmp)
	{
		BITMAP bmpInfo32;
		::GetObject(h32BitBmp, sizeof(bmpInfo32), &bmpInfo32);
		if(bmpInfo32.bmWidthBytes % 4 > 0)
			bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
		CBitmapHandle hOldBmp = (HBITMAP)::SelectObject(hMemDC, h32BitBmp);
		HDC hBmpDC = ::CreateCompatibleDC(hMemDC);
		CBitmapHandle hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hBitmap);
		::BitBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hBmpDC, 0, 0, SRCCOPY);
		::SelectObject(hBmpDC, hOldBmp2);
		::DeleteDC(hBmpDC);
		DWORD dwMaxRect = 100;
		HANDLE hRgnData = ::GlobalAlloc(GHND, sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect));
		RGNDATA *pRgnData = (RGNDATA*)::GlobalLock(hRgnData);
		pRgnData->rdh.dwSize = sizeof(RGNDATAHEADER);
		pRgnData->rdh.iType  = RDH_RECTANGLES;
		pRgnData->rdh.nCount = 0;
		pRgnData->rdh.nRgnSize = 0;
		::SetRect(&pRgnData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

		BYTE lr = GetRValue(rgbstart);
		BYTE lg = GetGValue(rgbstart);
		BYTE lb = GetBValue(rgbstart);
		BYTE hr = min(0xff, lr+GetRValue(rgbend));
		BYTE hg = min(0xff, lg+GetGValue(rgbend));
		BYTE hb = min(0xff, lb+GetBValue(rgbend));

		BYTE *pPixel = (BYTE*)bmpInfo32.bmBits + ((bmpInfo32.bmHeight - 1) * bmpInfo32.bmWidthBytes);
		int x, y, nStartX;
		long *pRGB;
		RECT *pRect;
		for(y = 0; y < bmpInfo32.bmHeight; y++) 
		{
			for(x = 0; x < bmpInfo32.bmWidth; x++)
			{
				nStartX = x;
				pRGB = (long*)pPixel + x;
				while(x<bmpInfo32.bmWidth)
				{
					BYTE b = GetRValue(*pRGB);
					if(b>=lr&&b<=hr)
					{
						b=GetGValue(*pRGB);
						if(b>lg&&b<=hg)
						{
							b=GetBValue(*pRGB);
							if(b>=lb&&b<=hb)
								break;
						}
					}
					pRGB++;
					x++;
				}
				if(x > nStartX) 
				{
					if(pRgnData->rdh.nCount >= dwMaxRect)
					{
						::GlobalUnlock(hRgnData);
						dwMaxRect += 100;
						hRgnData = ::GlobalReAlloc(hRgnData, sizeof(RGNDATAHEADER)+ (sizeof(RECT) * dwMaxRect), GMEM_MOVEABLE);
						pRgnData = (RGNDATA*)::GlobalLock(hRgnData);     
					}
					pRect = (RECT*)&pRgnData->Buffer;
					::SetRect(pRect + pRgnData->rdh.nCount, nStartX, y, x, y + 1);  
					if(nStartX < pRgnData->rdh.rcBound.left) 
						pRgnData->rdh.rcBound.left = nStartX;
					if(y < pRgnData->rdh.rcBound.top) 
						pRgnData->rdh.rcBound.top = y;
					if(x > pRgnData->rdh.rcBound.right) 
						pRgnData->rdh.rcBound.right = x;
					if(y + 1 > pRgnData->rdh.rcBound.bottom) 
						pRgnData->rdh.rcBound.bottom = y + 1;
					pRgnData->rdh.nCount++;
					if(pRgnData->rdh.nCount == 2000)
					{
						CRgn NewRgn;
						if(NewRgn.CreateFromData(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect), pRgnData))
						{
							rgnBitmap.CombineRgn(NewRgn.m_hRgn, RGN_OR);
							rgn=NULL;
							rgn=rgnBitmap.Detach();
						}
						pRgnData->rdh.nCount=0;
						SetRect(&pRgnData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					}
				}
			}
			pPixel -= bmpInfo32.bmWidthBytes;
		}
		CRgn NewRgn;
		if(NewRgn.CreateFromData(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * dwMaxRect), pRgnData))
		{
			rgnBitmap.CombineRgn(NewRgn.m_hRgn, RGN_OR);
			rgn=NULL;
			rgn=rgnBitmap.Detach();
		}
		::GlobalUnlock(hRgnData);
		::GlobalFree(hRgnData);

		::SelectObject(hMemDC, hOldBmp);
		::DeleteObject(h32BitBmp);
	}
	::DeleteDC(hMemDC);
	::ReleaseDC(NULL, hScreenDC);
	if(rgn)
	{
		return TRUE;
	}
	return FALSE;
}