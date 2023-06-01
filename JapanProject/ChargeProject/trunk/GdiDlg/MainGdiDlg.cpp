#include "stdAfx.h"
#include "MainGdiDlg.h"
#include <imaging.h>
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
//스래드 타임아웃
#define DETECTEVENTTIME 500

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
hWnd(NULL),
m_memDC(NULL),
pImgFactory(NULL)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
	if( m_memDC )
	{
		delete m_memDC;
	}
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::Release()
{
	test.NFCDisConnect();
// 	int ImgSize = modeimage.size();
// 	for(int mgcount = 0; mgcount<ImgSize; mgcount++)
// 	{
// 		IImage* pImage = modeimage[mgcount];
// 		if(pImage)
// 		{
// 			pImage->Release(); pImage=NULL;
// 		}
// 	}
// 	maingdiEvent.SetEvent();
// 	Sleep(10);
// 	CloseEvent.WaitForEvent();

	if(videochange.IsWindow())
		videochange.DestroyWindow();
	//
	if(backimage)
		backimage.DeleteObject();
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------

void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================

	_GARgn::ImageLoadRgn(_Module.GetResourceInstance(),IDB_DLGMASK_1 ,_T("PNG"),m_hRgn);
	SetWindowRgn(hWnd, m_hRgn, false);

	_GARes::ImageLoad(GetModuleHandle(NULL),IDB_DLGIMG_BACK01,_T("PNG"), backimage);
	BITMAP bitmap1;
	::GetObject(backimage, sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth;
	int imgY = bitmap1.bmHeight;
	SetWindowPos(hWnd, NULL, 50, 50, imgX, imgX, 0);

	videochange.Create(hWnd, CRect(10, 50, 0, 0),_T(""), WS_CHILD | WS_VISIBLE , 0, IDC_NEXT);
	videochange.initRgnSuperButton(IDB_BTNIMG_NEXT, IDB_BTNMASK_NEXT);

	DebugDlg->Write(_T("유니코드 테스트"));
	DebugDlg->Write(_T("English 한글"));

	BOOL b = test.initAQdeviceCmd(hWnd, _T("DualDlleMbedded.dll"));
	test.NFCAtypeConnect(this, AtypEventMessageCall, MtypEventMessageCall, DisConnectEventMessageCall);

//	CBitmap h = ::SHLoadImageFile("shin.jpg"); 

// 	TCHAR szPath[MAX_PATH];
// 	vecFileStreamString file;
// 	CString strTemp;
// 	if(GetModuleFileName(NULL, szPath, MAX_PATH))
// 	{
// 		strTemp = (LPTSTR)szPath;
// 		int nFindcount = strTemp.ReverseFind('\\'); //뒤의 마지막 위치한 파일명까지 String 수
// 		CString textname = strTemp.Right(strTemp.GetLength()-(nFindcount+1));
// 		int nFindname = textname.ReverseFind('.');
// 		textname = textname.Left(nFindname+1)+"txt";
// 		strTemp = strTemp.Left(nFindcount+1); //왼쪽부터 짤라내기(파일 이름 제외)
// 		_GAFILESTREAM::CreateLineReadFile(strTemp+textname, file);
// 	}
// 	loopcount = _wtol(file[0].GetBuffer());
// 	int filecount=1;
// 	int imgload = (loopcount*2)+filecount;
//  	pImgFactory = NULL;
//  	if(SUCCEEDED(CoCreateInstance(CLSID_ImagingFactory, NULL, CLSCTX_INPROC_SERVER,	IID_IImagingFactory, (void**)&pImgFactory)))
//  	{
// 		IImage* pImage = NULL;
// 		//파일로부터 이미지 로드
// 		for(; filecount<imgload; filecount++)
// 		{
// 			if(SUCCEEDED(pImgFactory->CreateImageFromFile(strTemp+file[filecount], &pImage)))
// 			{
// 				modeimage.push_back(pImage); pImage=NULL;
// 			}
// 		}
// 		pImgFactory->Release(); pImgFactory=NULL;
//  	}

}
//--------------------------------------------------------------
//|
//|Dlg 배경 그리기 함수
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::EraseBkgnd(CDCHandle dc)
{
	if(!m_memDC)
	{
		m_memDC = new CMemDC(hWnd);
	}
	// create memory DC
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(dc.m_hDC);
	CBitmapHandle pOldBitmap;
	pMemDC -> SetBkColor(RGB(255,0,0));
	SIZE bmSize;
	backimage.GetSize(bmSize);
	pOldBitmap = pMemDC -> SelectBitmap(backimage);
	dc.BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
	pMemDC -> SelectBitmap(pOldBitmap);
	delete pMemDC;

	//모두 그리고 복사한다
	m_memDC->Paint( );
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	if(!m_memDC)
	{
		m_memDC = new CMemDC(hWnd);
	}


	//모두 그리고 복사한다
	m_memDC->Paint( );
}

//--------------------------------------------------------------
//|
//| 버튼 클릭 이벤트 메시지
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	BOOL b=FALSE;
// 	switch(nID)
// 	{
// 	case IDC_MODECHANGE:
// 		{
// 		}break;
// 	case IDC_HOSPITAL_1:
// 		{
// 
// 		}break;
// 	case IDC_HOSPITAL_2:
// 		{
// 
// 		}break;
// 	case IDC_HOSPITAL_3:
// 		{
// 
// 		}break;
// 	}
	return b;
}
//--------------------------------------------------------------
//|
//|텍스트 쓴다
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawWhiteText(CDC* pDC, CRect textrc, CString text)
{
// 	if(text.GetLength())
// 	{
// 		CFont font;
// 		font.CreateFontIndirect(&fontinfo);
// 		int nOldMode = pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetTextColor(fontcolor);
// 		HFONT hOldFont  = pDC->SelectFont (font);
// 		pDC->DrawText (text, -1, textrc, DT_SINGLELINE | DT_LEFT |	DT_VCENTER);
// 
// 		pDC->SelectFont (hOldFont);
// 		pDC->SetBkMode(nOldMode);
// 	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainGdiDlg::ThreadMakeObject()
{
	////////////////////////////////////////////////////////////////
	// 스레드 생성
	maingdiEvent.Create();
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
}
DWORD WINAPI CMainGdiDlg::AtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpThreadParameter;
	return mdlg->AtypEventMessage(eventdata, eventdatalen);
}
DWORD CMainGdiDlg::AtypEventMessage(byte* eventdata, int eventdatalen)
{
	DebugDlg->Write(_T("a type"));
	return 0;
}
DWORD WINAPI CMainGdiDlg::MtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpThreadParameter;
	return mdlg->MtypEventMessage(eventdata, eventdatalen);
}
DWORD CMainGdiDlg::MtypEventMessage(byte* eventdata, int eventdatalen)
{
	int senddatalen=0;
	BYTE senddata[BUFFSIZE_1024];
	DebugDlg->Write(_T("Mifare type"));
	senddata[0]=0x31;
	senddata[1]=0x32;
	senddata[2]=0x33;
	senddata[3]=0x34;
	senddata[4]=0x00;
	senddata[5]=0x00;
	NFCErrorCode err = test.DEA_Authkey(senddata, 4);
	if(err == DE_OK)
	{
		DebugDlg->Write(_T("Mifare type key 인증"));
	}
	return 0;
}
DWORD WINAPI CMainGdiDlg::DisConnectEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen)
{
	CMainGdiDlg* mdlg = (CMainGdiDlg*)lpThreadParameter;
	return mdlg->DisConnectEventMessage(eventdata, eventdatalen);
}
DWORD CMainGdiDlg::DisConnectEventMessage(byte* eventdata, int eventdatalen)
{
	DebugDlg->Write(_T("Card DisConnect"));
	return 0;
}