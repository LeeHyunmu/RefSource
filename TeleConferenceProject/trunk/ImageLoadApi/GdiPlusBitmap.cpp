
#include "stdafx.h"
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
//--------------------------------------------------------------
//|
//|파일 로딩
//|
//---------------------------------------------------------------
HBITMAP	CGdiPlusBitmap::LoadImageFromFile(LPCTSTR szFileName)
{
	HANDLE	hFile	= INVALID_HANDLE_VALUE;
	LPVOID	pvData	= NULL;
	HGLOBAL hGlobal	= NULL;
	LPPICTURE pPicture;
	try
	{
		HRESULT hr;
		DWORD	dwFileSize;
		// 파일 열기
		hFile = CreateFile(szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		// 파일이 없을 경우
		if(hFile == INVALID_HANDLE_VALUE)
		{
			ATLTRACE("GetBitmapFromFile() File Open Error");
			throw -1;
		}
		// 파일 크기 얻기
		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize == -1)
		{
			ATLTRACE("GetBitmapFromFile() File Read Error");
			throw -1;		
		}
		// 파일 크기만큼 동적할당
		hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
		if(hGlobal == NULL)
		{
			ATLTRACE("GetBitmapFromFile() GlobalAlloc Error");
			throw -1;
		}
		pvData = GlobalLock(hGlobal);
		if(pvData == NULL)
		{
			ATLTRACE("GetBitmapFromFile() GlobalLock Error");
			throw -1;
		}
		DWORD dwBytesRead = 0;
		BOOL bRead = ReadFile(hFile, pvData, dwFileSize, &dwBytesRead, NULL);
		if(!bRead)
		{
			ATLTRACE("GetBitmapFromFile() ReadFile Error");
			throw -1;
		}
		GlobalUnlock(hGlobal);
		CloseHandle(hFile);
		LPSTREAM pstm = NULL;
		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
		if(S_OK != hr || pstm == NULL)
			throw -1;
		// Stream 에서 IPicture 를 생성
		hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&pPicture);
		if(S_OK != hr || pPicture == NULL)
		{
			ATLTRACE("Graphic File Load Error");
			return false;
		}
		pstm->Release();
		GlobalFree(hGlobal);
		HBITMAP PicHandle;
		pPicture->get_Handle((unsigned int *)&PicHandle);
		HBITMAP NewHandle = (HBITMAP)CopyImage(PicHandle, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
		pPicture->Release();
		return NewHandle;
	}
	catch (int e)
	{
		UNREFERENCED_PARAMETER (e);
		if (hFile != INVALID_HANDLE_VALUE)
			CloseHandle(hFile);
		if (hGlobal)
		{
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
		}
		if (pPicture)
			pPicture->Release();
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|jpg 데이타를 바로 비트맵으로 로딩한다
//|
//---------------------------------------------------------------
HBITMAP	CGdiPlusBitmap::LoadImageFromDataJpg(LPSTR szFile, DWORD	dwFileSize)
{
	LPVOID	pvData	= NULL;
	HGLOBAL hGlobal	= NULL;
	LPPICTURE pPicture;
	try
	{
		HRESULT hr;
		// 파일 크기만큼 동적할당
		hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwFileSize);
		if(hGlobal == NULL)
		{
			ATLTRACE("GetBitmapFromFile() GlobalAlloc Error");
			throw -1;
		}
		pvData = GlobalLock(hGlobal);
		if(pvData == NULL)
		{
			ATLTRACE("GetBitmapFromFile() GlobalLock Error");
			throw -1;
		}
		memcpy(pvData, szFile, dwFileSize);
		GlobalUnlock(hGlobal);
		LPSTREAM pstm = NULL;
		hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pstm);
		if(S_OK != hr || pstm == NULL)
			throw -1;
		// Stream 에서 IPicture 를 생성
		hr = ::OleLoadPicture(pstm, dwFileSize, FALSE, IID_IPicture, (LPVOID *)&pPicture);
		if(S_OK != hr || pPicture == NULL)
		{
			ATLTRACE("Graphic File Load Error");
			return false;
		}
		pstm->Release();
		GlobalFree(hGlobal);
		HBITMAP PicHandle;
		pPicture->get_Handle((unsigned int *)&PicHandle);
		HBITMAP NewHandle = (HBITMAP)CopyImage(PicHandle, IMAGE_BITMAP, 0, 0, LR_COPYRETURNORG);
		pPicture->Release();
		return NewHandle;
	}
	catch (int e)
	{
		UNREFERENCED_PARAMETER (e);
		if (hGlobal)
		{
			GlobalUnlock(hGlobal);
			GlobalFree(hGlobal);
		}
		if (pPicture)
			pPicture->Release();
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|리소스 로딩
//|
//---------------------------------------------------------------
HBITMAP CGdiPlusBitmap::ReadImgResource(HMODULE hModule,UINT lpName,LPCTSTR lpType,HDC hDC)
{
	//ASSERT(AfxOleInit()); //<-- 어디선가 초기화되면 되겠지요.
	//HMODULE module = AfxGetInstanceHandle(); //인자로 넘어오는것은 이거나 밑에거나 사용하면됨
	//HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC src = FindResource(hModule,MAKEINTRESOURCE(lpName),lpType);  // lpname은 ID,lpType은 사용자가 정의한타잎
	HGLOBAL hGlobal = LoadResource(hModule,src);
	void * resData = LockResource(hGlobal);    // 찾은 리소스의 데이터포인트찾기
	int size = SizeofResource(hModule,src);   // 찾은 리소스의 크기찾기
	IStream * pStream = NULL;
	ULONG dwReadWrite=0;
	CreateStreamOnHGlobal(NULL, TRUE, &pStream);    // IStream을 만들어서 적기
	pStream->Write(resData,size,&dwReadWrite);
	CImage img;
	HRESULT res = img.Load(pStream);    // Load중에 스트림을 받는것이있습니다.
	HDC memDC;
	memDC = CreateCompatibleDC(hDC);
	HBITMAP hBitmap = CreateCompatibleBitmap(hDC,img.GetWidth(),img.GetHeight());
	SelectObject(memDC,hBitmap);
	img.BitBlt(memDC,0,0,img.GetWidth(),img.GetHeight(),0,0,SRCCOPY);
	DeleteDC(memDC);
	img.Destroy();
	return hBitmap;
}
//--------------------------------------------------------------
//|
//| 윈도우 이미지로 저장
//|
//---------------------------------------------------------------
// void CGdiPlusBitmap::CapWindowSaveasJpg(LPCWSTR szFileName, HWND capWnd)
// {
// 	int width;
// 	int height;
// 	RECT rc;
// 	HDC windc;
// 	CDC memdc;
// 
// 	GetWindowRect(capWnd, &rc);
// 	width = rc.right - rc.left;
// 	height = rc.bottom - rc.top;
// 	windc = GetDC(capWnd);
// 
// 	memdc.CreateCompatibleDC(windc);
// 	CBitmap bitmap;
// 	bitmap.CreateCompatibleBitmap(windc, width, height);
// 	memdc.SelectBitmap(bitmap);
// 	memdc.BitBlt(0, 0, width, height,
// 		windc, 0, 0, SRCCOPY);
// 
// 
// 	ImageFormatPtr img = ImageFormat::create();
// 	//img->loadAndSaveAs((HBITMAP)bitmap, 0, L"jpg", L"test.jpg");
// 	//img->loadAndSaveAs((HBITMAP)bitmap, 0, imgtype, szFileName);
// 	img->loadAndSaveAs(bitmap, 0, L"jpg", szFileName);
// }
//--------------------------------------------------------------
//|
//| 이미지 CLSID 가져오기
//|
//---------------------------------------------------------------
BOOL CGdiPlusBitmap::GetEncCLSID(LPCWSTR mime, CLSID *pClsid)
{
	UINT num,size,i;
	Gdiplus::ImageCodecInfo *arCod;
	BOOL bFound=FALSE;

	Gdiplus::GetImageEncodersSize(&num,&size);
	arCod=(Gdiplus::ImageCodecInfo *)malloc(size);
	Gdiplus::GetImageEncoders(num,size,arCod);

	for (i=0;i<num;i++) {
		if(wcscmp(arCod[i].MimeType,mime)==0) {
			*pClsid=arCod[i].Clsid;
			bFound=TRUE;
			break;
		}    
	}
	free(arCod);
	return bFound;
}
//--------------------------------------------------------------
//|
//| 비트맵 JPEG로 저장
//|
//---------------------------------------------------------------
//비트맵 JPEG로 저장
BOOL CGdiPlusBitmap::HbitmapSaveasImageFile(LPCWSTR szFileName, LPCWSTR imgtype, CBitmap hBitmap)
{
	Gdiplus::Bitmap *bit = Gdiplus::Bitmap::FromHBITMAP(hBitmap, 0); 
	CLSID Clsid;
	GetEncCLSID(imgtype,&Clsid);    // 1번 파라미터를 바꿔주면 어떤포멧으로도 변환 가능하다.
	Gdiplus::Status s = bit->Save(szFileName, &Clsid);
	if (s != Gdiplus::Ok) {
		if (s == Gdiplus::Win32Error) {
			DWORD ec = GetLastError();
			_RPT1(_CRT_ASSERT, "ImageFormat::saveAs, Win32Error: %d", ec);
			return FALSE;
		}
	}
	return TRUE;
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
HBITMAP CGdiPlusBitmapResource::GetWinBitmap()
{
	HBITMAP bit=NULL;
	m_pBitmap->GetHBITMAP(RGB(0,0,0),&bit);
	return (HBITMAP)CopyImage((HANDLE)bit, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
}


