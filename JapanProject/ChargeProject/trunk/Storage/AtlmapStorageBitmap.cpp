
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
CAtlmapStorageBitmap::CAtlmapStorageBitmap(void):
drawname(_T(""))
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CAtlmapStorageBitmap::~CAtlmapStorageBitmap(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::AddImageBitmap(CString name, CBitmapHandle bit, BOOL bViewChange)
{
	BOOL b=FALSE;
	if(storage.Lookup(name)==NULL)
	{
		storage[name]=bit;
		if(bViewChange)
			drawname = name;
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::DeleteImageBitmap(CString name)
{
	if(storage.IsEmpty()) return FALSE;
	BOOL b=FALSE;
	BitmapPair bitpair = NULL;
	if(bitpair = storage.Lookup(name))
	{
		//CString strkey = bitpair->m_key;
		CBitmap bithandle = bitpair->m_value;
		//메모리 해제
		bithandle.DeleteObject();
		storage.RemoveKey(name);
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::DeleteImageBitmapAll()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	while (pos)
	{
		BitmapPair bitpair = NULL;
		bitpair = storage.GetNext(pos);
		CBitmap bithandle = bitpair->m_value;
		//비트맵 핸들 메모리 제거
		bithandle.DeleteObject();
	}
	//리스트에서 모두 삭제
	storage.RemoveAll();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CBitmap CAtlmapStorageBitmap::GetImageBitmap(CString name)
{
	if(storage.IsEmpty()&&name.IsEmpty()) return NULL;
	CBitmapHandle bitvalue;
	drawname=name;
	if(storage.Lookup(drawname, bitvalue))
	{
		return BitmapCopy(bitvalue);
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CBitmap CAtlmapStorageBitmap::GetImageBitmap()
{
	if(storage.IsEmpty()&&drawname.IsEmpty()) return NULL;
	CBitmapHandle bitvalue;
	if(storage.Lookup(drawname, bitvalue))
	{
		return BitmapCopy(bitvalue);
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::GetNextImageBitmap()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	while (pos)
	{
		BitmapPair bitpair = NULL;
		bitpair = storage.GetNext(pos);
		CString strname = bitpair->m_key;
		if(pos&&strname.Compare(drawname)==0)
		{
			bitpair = NULL;
			bitpair = storage.GetAt(pos);
			drawname = bitpair->m_key;
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::GetFirstImageBitmap()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	if(pos)
	{
		BitmapPair bitpair = NULL;
		bitpair = storage.GetAt(pos);
		drawname = bitpair->m_key;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|비트맵 저장 이름 가져 오기
//|
//---------------------------------------------------------------
CString CAtlmapStorageBitmap::GetDrawBitmapName()
{
	return drawname;
}
//--------------------------------------------------------------
//|
//|비트맵 이미지 카피
//|
//---------------------------------------------------------------
CBitmap CAtlmapStorageBitmap::BitmapCopy(CBitmapHandle hbit, HWND hWnd)
{
	CBitmapHandle copybit=NULL;
	if(hbit)
	{
		SIZE bmSize;
		if(hbit.GetSize(bmSize))
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

			HDC hdc = GetDC(hWnd);
			HDC hMemDC = ::CreateCompatibleDC(hdc);
			HDC hBmpDC = ::CreateCompatibleDC(hMemDC);
			CBitmap hOldBmp2 = (HBITMAP)::SelectObject(hBmpDC, hbit);
			void *pBmpBits = NULL;
			if(copybit.CreateDIBSection(hMemDC,(BITMAPINFO*)&Header32Bit,DIB_RGB_COLORS,&pBmpBits,NULL,0))
			{
				BITMAP bmpInfo32;
				::GetObject(copybit, sizeof(bmpInfo32), &bmpInfo32);
				if(bmpInfo32.bmWidthBytes % 4 > 0)
					bmpInfo32.bmWidthBytes += (4 - (bmpInfo32.bmWidthBytes % 4));
				CBitmap hOldBmp = (HBITMAP)::SelectObject(hMemDC, copybit);
				::StretchBlt(hMemDC, 0, 0, bmpInfo32.bmWidth, bmpInfo32.bmHeight,hBmpDC, 0, 0, width,Height, SRCCOPY);
				::SelectObject(hMemDC, hOldBmp);
			}
			::SelectObject(hBmpDC, hOldBmp2);
			::DeleteDC(hBmpDC);
			::DeleteDC(hMemDC);
			::DeleteDC(hdc);
			return HBITMAP(copybit);
		}
	}
	return NULL;
}