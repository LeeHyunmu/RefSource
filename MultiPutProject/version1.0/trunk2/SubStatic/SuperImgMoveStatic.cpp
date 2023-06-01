
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
CSuperImgMoveStatic::CSuperImgMoveStatic(void):
CWindowImpl< CSuperImgMoveStatic, CStatic >( ),
fontpoint(0,0),
ownname(_T("")),
ownnamecount(0)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CSuperImgMoveStatic::~CSuperImgMoveStatic(void)
{
}
//--------------------------------------------------------------
//|
//|���� Ŭ���� �Լ�
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
//|Ŀ��
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
//|�ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::initSuperImgMoveStatic(CString _ownname, UINT ImageId, BOOL bOwnerDraw, BOOL bsize)
{
	//�ڽ��� �̸�
	ownname=_ownname;
	//����Ʈ ��Ʈ ����
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
		//�̹��� ���� �̸�
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
//|�ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::initSuperImgMoveStatic(CString _ownname, UINT ImageId, LPCWSTR type, BOOL bsize)
{
	//�ڽ��� �̸�
	ownname=_ownname;
	//����Ʈ ��Ʈ ����
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

	//�̹��� ���� �̸�
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
//|�ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::initSuperImgMoveStatic(CString _ownname, CBitmap bit, BOOL bOwnerDraw, BOOL bsize)
{
	//�ڽ��� �̸�
	ownname=_ownname;
	//����Ʈ ��Ʈ ����
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
	//�̹��� ���� �̸�
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
//|�̹��� �߰�
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::AddImageImgMoveStatic(CBitmap bit)
{
	//�̹��� ���� �̸�
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
//|�̹��� �߰� ������ �̹��� �ٲ��� �ʰ� �߰�
//|
//---------------------------------------------------------------
BOOL CSuperImgMoveStatic::AddImageNoChangeBitmapp(CBitmap bit)
{
	//�̹��� ���� �̸�
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
//|������ ���� �̹��� �߰�
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
//|���� ��ο� �Լ�
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
	//������ �ؽ�Ʈ ��������
	CString wintext;
	GetWindowText(wintext);
	if(wintext.GetLength())
	{
		DrawImgMoveStaticWhiteText(&dc,CSize(rc.Width(), rc.Height()),wintext);
	}
}

//--------------------------------------------------------------
//|
//|��Ʈ ����
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
//|��Ʈ ����
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::SetSuperImgMoveStaticFont(UINT lfHeight, CString lfFaceName, COLORREF* _fontcolor, CPoint* _fontpoint)
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
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ����
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
//|�ؽ�Ʈ ����
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
//|���� Ŭ���� ����
//|
//---------------------------------------------------------------
HWND CSuperImgMoveStatic::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperImgMoveStatic, CStatic>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|��ư Create �Լ� 
//|
//---------------------------------------------------------------
int CSuperImgMoveStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
BOOL CSuperImgMoveStatic::OnEraseBkgnd(CDCHandle dc)
{
	//��׶��� ����
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
//|Create ������ Desroy �ƴϸ� �̻� �߻�(�����)
//|
//---------------------------------------------------------------
LRESULT CSuperImgMoveStatic::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	//��ư �̸� ����� �� �̹��� ����
	ImgMoveStaticClear();
	return 0;
}
//--------------------------------------------------------------
//|
//|��ư�� ���̴� �̹��� ����
//|
//---------------------------------------------------------------
void CSuperImgMoveStatic::ImgMoveStaticClear()
{
	imagestorage.DeleteImageBitmapAll();
}
//--------------------------------------------------------------
//|
//|����̽� ȣȯ ��Ʈ�� ����
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
			//ATL Map ����
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
//|CE �̹��� �ε�(���ҽ����� �̹��� �ε�, png ����)
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
				pImage->Draw(hMemDC, &dstRect, NULL); //ȭ�鿡 �ٷ� �׷��ִ� �κ�. �ӵ� ������ ���� �޸𸮿� �׷��ְ� �װ���
				::SelectObject(hMemDC, hOldBmp);
				//ATL Map ����
				//�̹��� ���� �̸�
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
//|����̽� ȣȯ ��Ʈ�� ����
//|
//---------------------------------------------------------------
//�̹��� �̸� �߿��� ù��° �����ֱ�
BOOL CSuperImgMoveStatic::ShowFirstImageBitmap()
{
	imagestorage.GetFirstImageBitmap();
	InvalidateRect(NULL, FALSE);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|����̽� ȣȯ ��Ʈ�� ����
//|
//---------------------------------------------------------------
//�̹��� �߿��� ���� �˻��Ǵ� �̹��� �����ֱ�
BOOL CSuperImgMoveStatic::ShowNextImageBitmap()
{
	imagestorage.GetNextImageBitmap();
	InvalidateRect(NULL, FALSE);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|���� �������� �ִ� �̹����� ���Ϸ� �����(jpg)
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
		SysFreeString(bstrString); //�޸� ����.
	}
	return b;
}
//--------------------------------------------------------------
//|
//|���ú� �� ���� �˻��Ͽ� ������ �����ش�
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
//|���콺 ��ġ�� ���� �׷��� ������ �ִ°�
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