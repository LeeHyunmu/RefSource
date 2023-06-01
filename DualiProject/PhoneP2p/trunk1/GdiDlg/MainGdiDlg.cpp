#include "stdAfx.h"
#include "MainGdiDlg.h"

#include <InitGuid.h>
#include <imaging.h>
#pragma comment(lib, "imaging.lib")
#pragma comment(lib, "commdlg.lib")
#pragma comment(lib, "ole32.lib")
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
backButShow(SW_HIDE)
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
	pImgFactory->Release();
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
	dualip2p=obj->GetDualiP2pMgObject();
}
//--------------------------------------------------------------
//|
//|dlg �ʱ�ȭ 
//|
//---------------------------------------------------------------
CRect ButPosRc_1;
CRect ButPosRc_2;
CRect ButPosRc_3;
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND ����
	//========================================
	hWnd = _hWnd;
	//========================================
	fontinfo.lfHeight   = 21;
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
	fontcolor=RGB(255,0,0);
//	CBitmap h = ::SHLoadImageFile("shin.jpg"); 

	pImgFactory = NULL;
	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER,	IID_IImagingFactory, (void**)&pImgFactory)))
	{
	}
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//��� ���� ��ư
	ModeChangeBut.Create(hWnd, CRect(iX-100, 60, 0, 0),_T("��庯��"), WS_CHILD | WS_VISIBLE , 0, IDC_MODECHANGE);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	ModeChangeBut.initSuperButton(IDB_BUT_2);

	// ���� ���� ��ư
	HospitalBut_1.Create(hWnd, CRect(399, 210, 0, 0),_T("H1"), WS_CHILD | WS_VISIBLE   , 0, IDC_HOSPITAL_1);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_1.initSuperButton(IDB_BUT_3);
	HospitalBut_1.ShowWindow(SW_HIDE);
	HospitalBut_1.GetWindowRect(&ButPosRc_1);

	//���� ���� ��ư
	HospitalBut_2.Create(hWnd, CRect(399, 280, 0, 0),_T("H2"), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_2);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_2.initSuperButton(IDB_BUT_4);
	HospitalBut_2.ShowWindow(SW_HIDE);
	HospitalBut_2.GetWindowRect(&ButPosRc_2);

	//���� ���� ��ư
	HospitalBut_3.Create(hWnd, CRect(399, 350, 0, 0),_T("H3"), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_3);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_3.initSuperButton(IDB_BUT_5);
	HospitalBut_3.ShowWindow(SW_HIDE);
	HospitalBut_3.GetWindowRect(&ButPosRc_3);
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
	InvalidateRect(hWnd, Rc, bErase);
	//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
	UpdateWindow(hWnd);
	SendMessage(hWnd, WM_TIMER,0,0);
}
//--------------------------------------------------------------
//|
//|�̹��� �����Ͽ� �׸���
//|
//---------------------------------------------------------------
TCHAR MobileSendData[][40]={_T("03poto"),_T("02http://www.ajussi2010.co.kr"), _T("0101022401020")};
void CMainGdiDlg::ImageChange(UINT _MgNum, BOOL bSendData)
{
	BOOL b=FALSE;
	//��尡 2�� ��
	if(MgMode==2)
	{
		//���� IDC_HOSPITAL_2 �� ���� �Ǿ���, ����Ÿ�� ������ �Ǵ� �� ���� �Ǿ��� ��
		if(hospitalnum==IDC_HOSPITAL_2&&bSendData==TRUE)
		{
			//����Ÿ�� ������ �̹��� �ѹ� ������ MgMode ��
			//TRUE�� �����ϸ� �̹����� �ٲٵ��� �Ѵ�
			if(dualip2p->SendDatafn(1, MobileSendData[MgMode]))
			{
				//�̹��� �ѹ�
				MgNum=_MgNum;
			}
		}
		//�̹��� �ѹ��� 0�̰� �̹����� ��ư�� ������ �� �� �״�ζ�� 
		//�̹����� �ٲ��� �ʵ��� ���ð��� �����Ѵ�
		else if(_MgNum==0&&MgNum==1)
		{
			MgNum = _MgNum;
		}
		//��� 2�� �� �̹��� �ѹ��� 0�̸� ��ư�� ��������
		if(MgNum==0)
		{
			//��ư3�� ����������, ���� ��ư Ŭ�� �ߴ� �� �ʱ�ȭ
			ButShow=SW_SHOW; hospitalnum=0;
		}
	}
	//��� 2�� �ƴ� ��
	else
	{
		//����Ÿ ������ �� ���� �Ǿ��� ��
		if(bSendData==TRUE)
		{
			//����Ÿ�� ������ �̹��� �ѹ� ������ MgMode ��
			//TRUE�� �����ϸ� �̹����� �ٲٵ��� �Ѵ�
			if(dualip2p->SendDatafn(1, MobileSendData[MgMode]))
			{
				MgNum=_MgNum;
			}
		}
		//����Ÿ�� ������ �޽��� �̿� ���� ��
		else if(_MgNum==0)
		{
			MgNum = _MgNum;
		}

	}
	//�ٽ� �׸���
	MainGdiinvalidateRect(NULL,FALSE);
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
			HospitalBut_1.InvalidateRect(NULL, FALSE);
			HospitalBut_2.InvalidateRect(NULL, FALSE);
			HospitalBut_3.InvalidateRect(NULL, FALSE);
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
			//��ư�� Ŭ�� �޽����� ����ߴ°��� �Ǵ�
			b=TRUE;
			//0 : ��带 �ٲ� ���� �̹��� �ѹ��� 0���� �����ϰ� ���� ��ư �ʱ�ȭ
			MgNum=0; hospitalnum=0;
		}break;
	case IDC_HOSPITAL_1:
		{
			hospitalnum=IDC_HOSPITAL_1;
			//0 : ��带 �ٲ� ���� �̹��� �ѹ��� 0���� ����
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