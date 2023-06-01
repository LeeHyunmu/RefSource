
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|��ư ��� �Լ�
//|
//---------------------------------------------------------------
//
#ifdef WINCE
//#include <InitGuid.h>
#include <imaging.h>
#endif //WINCE
//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CSuperStatic::~CSuperStatic(void)
{
}
//--------------------------------------------------------------
//|
//|���� Ŭ���� �Լ�
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
//|�ʱ�ȭ �Լ�
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
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _T("����ü"));
	//��Ʈ ��
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
//|�ʱ�ȭ �Լ�
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
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _T("����ü"));
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
//|�ʱ�ȭ �Լ�
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
	//CE���� �ȵǴ� �κ��̴�
#ifndef WINCE
	fontinfo.lfOutPrecision = OUT_CHARACTER_PRECIS;
#endif //WINCE
	fontinfo.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	fontinfo.lfQuality   = DEFAULT_QUALITY;
	fontinfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), _T("����ü"));
	//��Ʈ��
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
//|�̹��� �߰�
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
//|���� ��ο� �Լ�
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
		DrawStatic(pMemDC,lpdis->itemState);
		int oldMode = dc.SetStretchBltMode(HALFTONE);
		dc.StretchBlt(0,0,rc.Width(), rc.Height(),pMemDC->m_hDC,0,0,bmSize.cx, bmSize.cy,SRCCOPY);
		dc.SetStretchBltMode(oldMode);
		pMemDC -> SelectBitmap(pOldBitmap);
		delete pMemDC;
	}
	//������ �ؽ�Ʈ ��������
	CString wintext;
	GetWindowText(wintext);
	if(wintext.GetLength())
	{
		DrawWhiteText(&dc,CSize(rc.Width(), rc.Height()),wintext);
	}
}
//--------------------------------------------------------------
//|
//|�׷��ִ� �Լ�
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
//|��Ʈ ����
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
//|��Ʈ ����
//|
//---------------------------------------------------------------
void CSuperStatic::SetSuperStaticFont(UINT lfHeight, CString lfFaceName, COLORREF* _fontcolor, CPoint* _fontpoint, BOOL* _btransparent)
{
	//��Ʈ ũ�� ����
	if(lfHeight)
		fontinfo.lfHeight   = lfHeight;
	//��Ʈä ����
	if(lfFaceName.GetLength())
		_tcscpy_s(fontinfo.lfFaceName, sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]), lfFaceName);
	//��Ʈ �� ����
	if(_fontcolor)
		fontcolor=*_fontcolor;
	//��Ʈ ���� ��ġ ����
	if(_fontpoint)
		fontpoint.SetPoint(_fontpoint->x, _fontpoint->y);
	//��� ����������
	if(_btransparent)
		btransparent=*_btransparent;
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ����
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
//|�ؽ�Ʈ ����
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
//|���� Ŭ���� ����
//|
//---------------------------------------------------------------
HWND CSuperStatic::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperStatic, CStatic>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
int CSuperStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//����Ʈ���� �޽���
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	return 0;
}
//--------------------------------------------------------------
//|
//|��� �׷��ִ� �Լ�(����)
//|
//---------------------------------------------------------------
BOOL CSuperStatic::OnEraseBkgnd(CDCHandle dc)
{
	//��׶��� ���� ����
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
//|Create ������ Desroy �ƴϸ� �̻� �߻�(�����)
//|
//---------------------------------------------------------------
LRESULT CSuperStatic::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	//��ư �̸� ����� �� �̹��� ����
	StaticClear();
	return 0;
}
//--------------------------------------------------------------
//|
//|��ư�� ���̴� �̹��� ����
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
//|����̽� ȣȯ ��Ʈ�� ����
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
//|CE �̹��� �ε�(���ҽ����� �̹��� �ε�, png ����)
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
				pImage->Draw(hMemDC, &dstRect, NULL); //ȭ�鿡 �ٷ� �׷��ִ� �κ�. �ӵ� ������ ���� �޸𸮿� �׷��ְ� �װ���
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
//--------------------------------------------------------------
//|
//|��Ʈ���� �ѱ��
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