#include "stdAfx.h"
#include "MainGdiDlg.h"
#include <imaging.h>
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
//������ Ÿ�Ӿƿ�
#define DETECTEVENTTIME 500

#define  IDC_MODECHANGE  301
#define  IDC_HOSPITAL_1  401
#define  IDC_HOSPITAL_2  402
#define  IDC_HOSPITAL_3  403

#define  CHANGE_TIME 3000
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
strMode3(""),
ImageNumber(0),
bCardConnection(FALSE),
changeTime(0),
imgbackup(0),
loopcount(0)
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

	int ImgSize = modeimage.size();
	for(int mgcount = 0; mgcount<ImgSize; mgcount++)
	{
		IImage* pImage = modeimage[mgcount];
		if(pImage)
		{
			pImage->Release(); pImage=NULL;
		}
	}
	maingdiEvent.SetEvent();
	Sleep(10);
	CloseEvent.WaitForEvent();
}
//�ı�
void  CMainGdiDlg::ControlDestroy()
{
	ModeChangeBut.DestroyWindow();
	CloseBut.DestroyWindow();
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
	//
	changeTime=GetTickCount();
	ThreadMakeObject();
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

	TCHAR szPath[MAX_PATH];
	vecFileStreamString file;
	CString strTemp;
	if(GetModuleFileName(NULL, szPath, MAX_PATH))
	{
		strTemp = (LPTSTR)szPath;
		int nFindcount = strTemp.ReverseFind('\\'); //���� ������ ��ġ�� ���ϸ���� String ��
		CString textname = strTemp.Right(strTemp.GetLength()-(nFindcount+1));
		int nFindname = textname.ReverseFind('.');
		textname = textname.Left(nFindname+1)+"txt";
		strTemp = strTemp.Left(nFindcount+1); //���ʺ��� ©�󳻱�(���� �̸� ����)
		_GAFILESTREAM::CreateLineReadFile(strTemp+textname, file);
	}
	loopcount = _wtol(file[0].GetBuffer());
	int filecount=1;
	int imgload = (loopcount*2)+filecount;
 	pImgFactory = NULL;
 	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER,	IID_IImagingFactory, (void**)&pImgFactory)))
 	{
		IImage* pImage = NULL;
		//���Ϸκ��� �̹��� �ε�
		for(; filecount<imgload; filecount++)
		{
			if(SUCCEEDED(pImgFactory->CreateImageFromFile(strTemp+file[filecount], &pImage)))
			{
				modeimage.push_back(pImage); pImage=NULL;
			}
		}
		pImgFactory->Release(); pImgFactory=NULL;
 	}
	//���Ϸκ��� Apdu ����
	int apduload = loopcount+filecount;
	if(apduload-1<(int)file.size())
	{
		for(; filecount<apduload; filecount++)
		{
			apdu.push_back(file[filecount]);
		}
	}
	//���� ����
	if(filecount<(int)file.size())
	{
		soundfile = strTemp+file[filecount];
	}
	//
	int iX  = GetSystemMetrics(SM_CXSCREEN);
	int iY  = GetSystemMetrics(SM_CYSCREEN);
	//��� ���� ��ư
	ModeChangeBut.Create(hWnd, CRect(iX-120, 60, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_MODECHANGE);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	ModeChangeBut.initSuperButton(IDB_PNG2,_T("PNG"));
	ModeChangeBut.ShowWindow(SW_HIDE);

	//=============================================
	// �����ư
	//=============================================
	CloseBut.Create(hWnd, CRect(iX-120, 20, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDOK);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	CloseBut.initSuperButton(IDB_PNG1,_T("PNG"));

	// ���� ���� ��ư
	HospitalBut_1.Create(hWnd, CRect(430, 210, 0, 0),_T(""), WS_CHILD | WS_VISIBLE   , 0, IDC_HOSPITAL_1);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_1.initSuperButton(IDB_PNG6, _T("PNG"));
	HospitalBut_1.ShowWindow(SW_HIDE);

	//���� ���� ��ư
	HospitalBut_2.Create(hWnd, CRect(430, 280, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_2);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_2.initSuperButton(IDB_PNG6, _T("PNG"));
	HospitalBut_2.ShowWindow(SW_HIDE);

	//���� ���� ��ư
	HospitalBut_3.Create(hWnd, CRect(430, 350, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_HOSPITAL_3);
	//�̹���(��Ʈ��) ���ڷ� �ѱ��
	HospitalBut_3.initSuperButton(IDB_PNG6, _T("PNG"));
	HospitalBut_3.ShowWindow(SW_HIDE);

	//
	rcMode3.SetRect(0,0,0,0);
}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hWnd, &ps);
	CRect rc;
	GetClientRect(hWnd, &rc);
	CRect ImgRc(rc.left, rc.top, rc.right, rc.bottom);
	IImage* pImage = modeimage[ImageNumber];
	if(pImage)
	{
		pImage->Draw(hdc, &ImgRc, NULL);
	}
	DrawWhiteText(&CDC(hdc),rcMode3,strMode3);
	EndPaint(hWnd, &ps);
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
		}break;
	case IDC_HOSPITAL_1:
		{

		}break;
	case IDC_HOSPITAL_2:
		{

		}break;
	case IDC_HOSPITAL_3:
		{

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
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainGdiDlg::ThreadMakeObject()
{
	////////////////////////////////////////////////////////////////
	// ������ ����
	maingdiEvent.Create();
	CloseEvent.Create();
	maingdithread.Create(ThreadProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CMainGdiDlg::ThreadProcess(LPVOID lpArg)
{
	CMainGdiDlg *maingdi = (CMainGdiDlg *)lpArg;
	maingdi->MainThreadFunc();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainThreadFunc()
{
	while(TRUE)
	{
		if(maingdiEvent.WaitForEvent(DETECTEVENTTIME))
		{
			break;
		}
		else
		{
			//if(!bCardConnection&&dualicard->CardFinddingLoop())
			if(!bCardConnection&&dualicard->FindCard())
			{
				//ī�� �ν��� �ѹ����ϵ���
				BOOL b=FALSE;
				CString scardRecv="";
				b = dualicard->ApduCommand("00A404000BD4100000004151436F0001",scardRecv);
				if(b&&ImageNumber<loopcount)
				{
					b = dualicard->ApduCommand(apdu[ImageNumber],scardRecv);
					imgbackup=ImageNumber; ImageNumber=ImageNumber+loopcount;
					InvalidateRect(hWnd, NULL, FALSE);
					//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
					UpdateWindow(hWnd);
					bCardConnection=TRUE;
					if(soundfile.GetLength())
					{
						GASound::SoundPlay(soundfile);
					}
				}
			}
			else if(bCardConnection)
			{
				BOOL b=FALSE;
				CString scardRecv="";
				b = dualicard->ApduCommand("00A404000BD4100000004151436F0001",scardRecv);
				if(b)
				{
					continue;
				}
				bCardConnection=FALSE; 
				ImageNumber=imgbackup;
				if(apdu.size()>1)
				{
					changeTime=(GetTickCount()+CHANGE_TIME);
				}
				InvalidateRect(hWnd, NULL, FALSE);
				//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
				UpdateWindow(hWnd);
			}
			else if(apdu.size()>1)
			{
				if(GetTickCount()>changeTime+CHANGE_TIME)
				{
					changeTime=GetTickCount(); ++ImageNumber%=(loopcount);
					InvalidateRect(hWnd, NULL, FALSE);
					//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
					UpdateWindow(hWnd);
				}				
			}
		}
	}
	CloseEvent.SetEvent();
}