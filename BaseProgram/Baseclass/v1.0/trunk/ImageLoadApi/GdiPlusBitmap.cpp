
#include "stdafx.h"
//Gdiplus 라이브러리 링크
#pragma comment(lib, "gdiplus.lib")
//==================================================================
//|
//|CGdiPlusBitmap 클래스
//|
//==================================================================
//
//--------------------------------------------------------------
//|
//|객체 초기화
//|
//---------------------------------------------------------------
ULONG_PTR       CGdiPlusBitmap::m_gdiplusToken=0;
//Gdiplus 사용 등록
void CGdiPlusBitmap::GdiStart()
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
}

//--------------------------------------------------------------
//|
//|Gdiplus 사용 해제
//|
//---------------------------------------------------------------
void CGdiPlusBitmap::GdiShut()
{
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}
//GIF 파일 애니메이션 예제
// How many frame dimensions does the Image object have?
// CLSID encoderClsid;
// int result = GetEncoderClsid( L"image/gif", &encoderClsid );
// if( result < 0 )
// {
// 
// }
// 
// Image *image1 = NULL;
// image1 = new Image( L"d:\\ani.gif", TRUE );
// if( !image1 )
// return -1;
// 
// if( image1->GetLastStatus() != Ok )
// return -1;
// 
// UINT count = 0;
// count = image1->GetFrameDimensionsCount();
// GUID *pDimensionIDs = new GUID[ count ];
// 
// // Get the list of frame dimensions from the Image object.
// image1->GetFrameDimensionsList( pDimensionIDs, count );
// 
// // Display the GUID of the first (and only) frame dimension.
// WCHAR strGuid[39];
// StringFromGUID2( pDimensionIDs[0], strGuid, 39 );
// 
// // Get the number of frames in the first dimension.
// UINT frameCount = image1->GetFrameCount(&pDimensionIDs[0]);
// 
// Graphics graph( hWnd );
// 
// static UINT uIndex = 0;
// 
// image1->SelectActiveFrame( &pDimensionIDs[0], uIndex );
// 
// float fRate = static_cast<float>(image1->GetHeight())/image1->GetWidth();
// 
// graph.FillRectangle( &SolidBrush( Color( 128, 128, 255 ) ), 0, 0, 600, static_cast<int>(600*fRate) );
// 
// graph.DrawImage( image1, 0, 0, 600, static_cast<int>(600*fRate) );
// 
// uIndex = (uIndex + 1) % 6;
// 
// delete []pDimensionIDs;
// 
// delete image1;
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CGdiPlusBitmap::CGdiPlusBitmap()
{
	m_pBitmap = NULL; 
}
//--------------------------------------------------------------
//|
//|파일 경로를 넘기는 생성자
//|
//---------------------------------------------------------------
CGdiPlusBitmap::CGdiPlusBitmap(LPCWSTR pFile)
{
	m_pBitmap = NULL; Load(pFile); 
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CGdiPlusBitmap::~CGdiPlusBitmap()
{
	Empty(); 
}
//--------------------------------------------------------------
//|
//|객체를 메모리 해제
//|
//---------------------------------------------------------------
void CGdiPlusBitmap::Empty()
{
	delete m_pBitmap; m_pBitmap = NULL; 
}
//--------------------------------------------------------------
//|
//|파일 로딩 함수
//|
//---------------------------------------------------------------
bool CGdiPlusBitmap::Load(LPCWSTR pFile)
{
	Empty();
	m_pBitmap = Gdiplus::Bitmap::FromFile(pFile);
	return m_pBitmap->GetLastStatus() == Gdiplus::Ok;
}
//--------------------------------------------------------------
//|
//|GDIPlus 객체를 넘긴다
//|
//---------------------------------------------------------------
CGdiPlusBitmap::operator Gdiplus::Bitmap*() const
{
	return m_pBitmap; 
}
//==================================================================
//|
//|CGdiPlusBitmapResource 클래스
//|
//==================================================================
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CGdiPlusBitmapResource::CGdiPlusBitmapResource()
{ 
	m_hBuffer = NULL; 
}
//--------------------------------------------------------------
//|
//|객체 인수, 
//|
//---------------------------------------------------------------
CGdiPlusBitmapResource::CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	m_hBuffer = NULL; Load(pName, pType, hInst); 
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CGdiPlusBitmapResource::CGdiPlusBitmapResource(UINT id, LPCTSTR pType, HMODULE hInst)
{
	m_hBuffer = NULL; Load(id, pType, hInst); 
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CGdiPlusBitmapResource::CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst)
{
	m_hBuffer = NULL; Load(id, type, hInst); 
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CGdiPlusBitmapResource::~CGdiPlusBitmapResource()
{
	Empty();
}
//--------------------------------------------------------------
//|
//|리소스 로딩
//|
//---------------------------------------------------------------
bool CGdiPlusBitmapResource::Load(UINT id, LPCTSTR pType, HMODULE hInst )
{
	return Load(MAKEINTRESOURCE(id), pType, hInst);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
bool CGdiPlusBitmapResource::Load(UINT id, UINT type, HMODULE hInst)
{
	return Load(MAKEINTRESOURCE(id), MAKEINTRESOURCE(type), hInst); 
}
//--------------------------------------------------------------
//|
//|객체를 비운다(
//|
//---------------------------------------------------------------
void CGdiPlusBitmapResource::Empty()
{
	CGdiPlusBitmap::Empty();
	if (m_hBuffer)
	{
		::GlobalUnlock(m_hBuffer);
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	} 
}
//--------------------------------------------------------------
//|
//|최종 로딩 함수(리소스)
//|
//---------------------------------------------------------------
bool CGdiPlusBitmapResource::Load(LPCTSTR pName, LPCTSTR pType, HMODULE hInst)
{
	Empty();

	HRSRC hResource = ::FindResource(hInst, pName, pType);
	if (!hResource)
		return false;

	DWORD imageSize = ::SizeofResource(hInst, hResource);
	if (!imageSize)
		return false;

	const void* pResourceData = ::LockResource(::LoadResource(hInst, hResource));
	if (!pResourceData)
		return false;

	m_hBuffer  = ::GlobalAlloc(GMEM_MOVEABLE, imageSize);
	if (m_hBuffer)
	{
		void* pBuffer = ::GlobalLock(m_hBuffer);
		if (pBuffer)
		{
			CopyMemory(pBuffer, pResourceData, imageSize);

			IStream* pStream = NULL;
			if (::CreateStreamOnHGlobal(m_hBuffer, FALSE, &pStream) == S_OK)
			{
				m_pBitmap = Gdiplus::Bitmap::FromStream(pStream);
				pStream->Release();
				if (m_pBitmap)
				{ 
					if (m_pBitmap->GetLastStatus() == Gdiplus::Ok)
						return true;

					delete m_pBitmap;
					m_pBitmap = NULL;
				}
			}
			::GlobalUnlock(m_hBuffer);
		}
		::GlobalFree(m_hBuffer);
		m_hBuffer = NULL;
	}
	return false;
}
//--------------------------------------------------------------
//|
//|비트맵을 넘긴다
//|
//---------------------------------------------------------------
CBitmap CGdiPlusBitmapResource::GetWinBitmap()
{
	HBITMAP bit=NULL;
	m_pBitmap->GetHBITMAP(RGB(0,0,0),&bit);
	return bit;
}

// BLENDFUNCTION bf;
// bf.BlendOp = AC_SRC_OVER;
// bf.BlendFlags =0;
// bf.SourceConstantAlpha = 150;   // 0: 투명, 255: 불투명
// bf.AlphaFormat=0;
// 
// 
// dc.AlphaBlend(bmpInfo.bmWidth*2, 20,
// 			  bmpInfo.bmWidth, bmpInfo.bmHeight,
// 			  &MemDC,
// 			  0,0,
// 			  bmpInfo.bmWidth, bmpInfo.bmHeight,
// 			  bf);


// void CBmpOutDemoView::OnPaint()
// {
// 	CPaintDC dc(this); // device context for painting
// 
// 	CDC MemDC;
// 	BITMAP bmpInfo;
// 	MemDC.CreateCompatibleDC(&dc);
// 
// 	CBitmap bmp;
// 	CBitmap* pOldBmp = NULL;
// 	bmp.LoadBitmapW(IDB_BITMAP1);
// 
// 	bmp.GetBitmap(&bmpInfo);
// 	pOldBmp = MemDC.SelectObject(&bmp);
// 
// 
// 
// 	dc.StretchBlt(20, 20,
// 		bmpInfo.bmWidth*2, bmpInfo.bmHeight*2,
// 		&MemDC,
// 		0, 0,
// 		bmpInfo.bmWidth, bmpInfo.bmHeight,
// 		SRCCOPY); // 래스터 연산 값
// 
// 
// 
// 	dc.TransparentBlt(bmpInfo.bmWidth*2, 20,
// 		bmpInfo.bmWidth*2, bmpInfo.bmHeight*2,
// 		&MemDC,
// 		0, 0,
// 		bmpInfo.bmWidth, bmpInfo.bmHeight,
// 		RGB(2,2,0)); //투명 처리될 색상의 RGB값
// 
// 
// 
// 	MemDC.SelectObject(pOldBmp);
// 
// }

