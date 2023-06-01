#include "stdAfx.h"
#include "MainGdiDlg.h"
#include <imaging.h>
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
#define  IDC_MODECHANGE  301
#define  IDC_HOSPITAL_1  401
#define  IDC_HOSPITAL_2  402
#define  IDC_HOSPITAL_3  403
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
hWnd(NULL),
MgNum(0),
MgMode(0),
pImgFactory(NULL),
hospitalnum(0),
ButShow(SW_HIDE),
backButShow(SW_HIDE),
strMode3("")
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::Release()
{
	//pImgFactory->Release();
}
//�ı�
void  CMainGdiDlg::ControlDestroy()
{
	ModeChangeBut.DestroyWindow();
	HospitalBut_1.DestroyWindow();
	HospitalBut_2.DestroyWindow();
	HospitalBut_3.DestroyWindow();
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
	//========================================
	debug=obj->GetOutputObject();
	//========================================
	// ������ P2P Ŭ����
	//========================================
	//dualip2p=obj->GetDualiP2pMgObject();
	//========================================
	// ������ 
	//========================================
	dualicard=obj->GetDualiCardMgObject();
}
//--------------------------------------------------------------
//|
//|dlg �ʱ�ȭ 
//|
//---------------------------------------------------------------
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND ����
	//========================================
	hWnd = _hWnd;
	//========================================
	fontinfo.lfHeight   = 50;
	fontinfo.lfWeight   = 0;
	fontinfo.lfEscapement  = 0;
	fontinfo.lfOrientation  = 0;
	fontinfo.lfWeight   = FW_BOLD;
	fontinfo.lfItalic   = FALSE;
	fontinfo.lfUnderline  = FALSE;
	fontinfo.lfStrikeOut  = FALSE;
	fontinfo.lfCharSet   = DEFAULT_CHARSET;
	fontinfo.lfOutPrecision = OUT_DEFAULT_PRECIS;
	fontinfo.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	fontinfo.lfQuality   = DEFAULT_QUALITY;
	fontinfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName)/sizeof(fontinfo.lfFaceName[0]),_T("����ü"));
	fontcolor=RGB(0,0,0);
//	CBitmap h = ::SHLoadImageFile("shin.jpg"); 

 	pImgFactory = NULL;
 	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER,	IID_IImagingFactory, (void**)&pImgFactory)))
 	{
 	}
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//��� ���� ��ư
	ModeChangeBut.Create(hWnd, CRect(iX-120, 60, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MODECHANGE);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	ModeChangeBut.initSuperButton(IDB_PNG2,_T("PNG"));

	// ���� ���� ��ư
	HospitalBut_1.Create(hWnd, CRect(430, 210, 0, 0),_T(""), WS_CHILD | WS_VISIBLE   , 0, IDC_HOSPITAL_1);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_1.initSuperButton(IDB_PNG7, _T("PNG"));
	HospitalBut_1.ShowWindow(SW_HIDE);

	//���� ���� ��ư
	HospitalBut_2.Create(hWnd, CRect(430, 280, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_2);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_2.initSuperButton(IDB_PNG8, _T("PNG"));
	HospitalBut_2.ShowWindow(SW_HIDE);

	//���� ���� ��ư
	HospitalBut_3.Create(hWnd, CRect(430, 350, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_3);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_3.initSuperButton(IDB_PNG9, _T("PNG"));
	HospitalBut_3.ShowWindow(SW_HIDE);

	// ��� ü���� �Լ�(�ÿ� ���� ���)
	dualicard->ChangeModeShow(MgMode);

	//
	rcMode3.SetRect(0,0,0,0);
}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
//|
//---------------------------------------------------------------
TCHAR MgFile[][3][50]={{_T("\\NandFlash\\PhoneP2p\\1_sample\\1.jpg"),_T("\\NandFlash\\PhoneP2p\\1_sample\\2.jpg"),_T("\\NandFlash\\PhoneP2p\\1_sample\\1.jpg")},
						{_T("\\NandFlash\\PhoneP2p\\2_sample\\1.jpg"),_T("\\NandFlash\\PhoneP2p\\2_sample\\2.jpg"), _T("\\NandFlash\\PhoneP2p\\2_sample\\1.jpg")},
						{_T("\\NandFlash\\PhoneP2p\\3_sample\\1.jpg"),_T("\\NandFlash\\PhoneP2p\\3_sample\\3.jpg"), _T("\\NandFlash\\PhoneP2p\\3_sample\\2.jpg")}};
void CMainGdiDlg::DrawPaint()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	CRect rc;
	GetClientRect(hWnd, &rc);
	CRect ImgRc(rc.left, rc.top, rc.right, rc.bottom);
	if(pImgFactory)
	{
		IImage* pImage = NULL;
		//���Ϸκ��� �̹��� �ε�
		if(SUCCEEDED(pImgFactory->CreateImageFromFile(MgFile[MgMode][MgNum], &pImage)))
		{
			pImage->Draw(hdc, &ImgRc, NULL);
			pImage->Release();
		}
	}
	DrawWhiteText(&CDC(hdc),rcMode3,strMode3);
	EndPaint(hWnd, &ps);
}
//--------------------------------------------------------------
//|
//|Static �ؽ�Ʈ ��
//|
//---------------------------------------------------------------
void CMainGdiDlg::StaticText(CDCHandle dc)
{
// 	CFont font;
// 	font.CreateFontIndirect(&fontinfo);
// 	dc.SetTextColor(fontcolor);
// 	dc.SetBkColor(RGB(255,0,0));
// 	dc.SelectFont (font);
}
//--------------------------------------------------------------
//|
//|������ �ٽ� �׸���
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainGdiinvalidateRect(CRect* Rc, BOOL bErase)
{
	//��ư�� �ٽñ׸���
	TimerMessage();
	//������ ��ȿȭ
	InvalidateRect(hWnd, Rc, bErase);
	//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
	UpdateWindow(hWnd);
	//SendMessage(hWnd, WM_TIMER,0,0);
}
//--------------------------------------------------------------
//|
//|��� ���¿��� �׸��� �� �������� ���� ó�� Ȯ�� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::GetModeException()
{
	if(MgMode==2&&MgNum==0)
	{
		return FALSE;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|�̹��� �����Ͽ� �׸���
//|
//---------------------------------------------------------------
//TCHAR MobileSendData[][40]={_T("shin.jpg"),_T("02http://www.ajussi2010.co.kr"), _T("0101094323475")};
BOOL CMainGdiDlg::ImageChange(UINT _MgNum)
{
	MgNum = _MgNum;
	//��� 2�� �� �̹��� �ѹ��� 0�̸� ��ư�� ��������
	if(MgMode==2&&MgNum==0)
	{
		//��ư3�� ����������, ���� ��ư Ŭ�� �ߴ� �� �ʱ�ȭ
		ButShow=SW_SHOW; hospitalnum=0;
	}
	//�ٽ� �׸���
	MainGdiinvalidateRect(NULL,FALSE);
	return TRUE;
}
//Ÿ�̸� �޽���
void CMainGdiDlg::TimerMessage()
{
	if(ButShow==SW_SHOW)
	{
		if(backButShow!=SW_SHOW)
		{
			HospitalBut_1.ShowWindow(SW_SHOW);
			HospitalBut_2.ShowWindow(SW_SHOW);
			HospitalBut_3.ShowWindow(SW_SHOW);
// 			HospitalBut_1.InvalidateRect(NULL, FALSE);
// 			HospitalBut_2.InvalidateRect(NULL, FALSE);
// 			HospitalBut_3.InvalidateRect(NULL, FALSE);
			backButShow=SW_SHOW;
		}
	}
	else
	{
		if(backButShow!=SW_HIDE)
		{
			HospitalBut_1.ShowWindow(SW_HIDE);
			HospitalBut_2.ShowWindow(SW_HIDE);
			HospitalBut_3.ShowWindow(SW_HIDE);
			backButShow=SW_HIDE;
		}
	}
}
//--------------------------------------------------------------
//|
//| ��ư Ŭ�� �̺�Ʈ �޽���
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	BOOL b=FALSE;
	switch(nID)
	{
	case IDC_MODECHANGE:
		{
			//0 : ��带 �ٲ� ���� �̹��� �ѹ��� 0���� �����ϰ� ���� ��ư �ʱ�ȭ
			MgNum=0; hospitalnum=0;
			MgMode=++MgMode%3;
			if(MgMode==2)
			{
				ButShow=SW_SHOW;
				MainGdiinvalidateRect(NULL,FALSE);
			}
			else
			{
				ButShow=SW_HIDE;
				MainGdiinvalidateRect(NULL,FALSE);
			}
			// ��� ü���� �Լ�(�ÿ� ���� ���)
			dualicard->ChangeModeShow(MgMode);
			//��ư�� Ŭ�� �޽����� ����ߴ°��� �Ǵ�
			b=TRUE;
		}break;
	case IDC_HOSPITAL_1:
		{
			hospitalnum=IDC_HOSPITAL_1;
			b=TRUE;
		}break;
	case IDC_HOSPITAL_2:
		{
			ButShow=SW_HIDE;
			//���� �׸����� ��ư�� ������ ��
			hospitalnum=IDC_HOSPITAL_2;	MgNum=2;
			MainGdiinvalidateRect(NULL,FALSE);
			//0 : ��带 �ٲ� ���� �̹��� �ѹ��� 0���� ����
			b=TRUE;
		}break;
	case IDC_HOSPITAL_3:
		{
			hospitalnum=IDC_HOSPITAL_3;
			//0 : ��带 �ٲ� ���� �̹��� �ѹ��� 0���� ����
			b=TRUE;
		}break;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ����
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawWhiteText(CDC* pDC, CRect textrc, CString text)
{
	if(text.GetLength())
	{
		CFont font;
		font.CreateFontIndirect(&fontinfo);
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontcolor);
		HFONT hOldFont  = pDC->SelectFont (font);
		pDC->DrawText (text, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);

		pDC->SelectFont (hOldFont);
		pDC->SetBkMode(nOldMode);
	}
}
//--------------------------------------------------------------
//|
//|Mode 3���� ��ȭ��ȣ �ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CMainGdiDlg::SetMode3Text(CString text, CRect* rc)
{
	strMode3=text;
	if(rc) rcMode3 = *rc;
	else rcMode3.SetRect(0,0,0,0);
}