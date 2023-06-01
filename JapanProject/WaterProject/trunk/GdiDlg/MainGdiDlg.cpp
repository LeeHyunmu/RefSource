#include "stdAfx.h"
#include <imaging.h>
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
//스래드 타임아웃
#define DETECTEVENTTIME 500
#define MAINGDIDLGTHREAD_LOOPTIME 1000

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
hWnd(NULL),
m_memDC(NULL),
AniSelectNum(0),
AniNum(1)
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
	//
	backimage.DeleteObject();
	//
	for (int dlg = 0; dlg < (int)dlgWater.size(); dlg++)
	{
		delete dlgWater[dlg];
	}
		
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------

void CMainGdiDlg::initDlg(HWND _hWnd)
{
	int Systemwidth  = ::GetSystemMetrics( SM_CXSCREEN );
	int Systemheight = ::GetSystemMetrics( SM_CYSCREEN );
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================

	//_GARgn::ImageLoadRgn(_Module.GetResourceInstance(),IDB_DLGMASK_1 ,_T("PNG"),m_hRgn);
	//SetWindowRgn(hWnd, m_hRgn, false);

	//배경 이미지
	_GARes::ImageLoad(GetModuleHandle(NULL),IDB_DLGBACK_BUTTONTOUCH_01,_T("PNG"), backimage);
	BITMAP bitmap1;
	::GetObject(backimage, sizeof(BITMAP), &bitmap1); 
	int imgX = bitmap1.bmWidth;
	int imgY = bitmap1.bmHeight;
	SetWindowPos(hWnd, NULL, 0, 0, imgX, imgX, 0);
	
	BOOL b = test.initAQdeviceCmd(hWnd, _T("DualDlleMbedded.dll"));
	test.NFCAtypeConnect(this, AtypEventMessageCall, MtypEventMessageCall, DisConnectEventMessageCall);

	ThreadMakeObject();
	//
	CManuSelectDlg* selectdlg=NULL;
	selectdlg=new CManuSelectDlg;
	selectdlg->Create(hWnd);
	dlgWater.push_back(selectdlg);
	//
	CCardDetectDlg* detectdlg=NULL;
	detectdlg=new CCardDetectDlg;
	detectdlg->Create(hWnd);
	dlgWater.push_back(detectdlg);

	//
	dlgWater[dlgpurchase]->ShowWindow(SW_SHOW);
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
		m_memDC = new CMemDC( hWnd );
	}

	// create memory DC
	if(m_memDC && backimage)
	{
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(m_memDC->m_hDC);
		CBitmapHandle pOldBitmap;
		pMemDC -> SetBkColor(RGB(255,0,0));
		SIZE bmSize;
		backimage.GetSize(bmSize);
		pOldBitmap = pMemDC -> SelectBitmap(backimage);
		m_memDC->BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
		pMemDC -> SelectBitmap(pOldBitmap);
		delete pMemDC;
	}

	//모두 그리고 복사한다
	m_memDC->Paint( );
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint(CDCHandle dc)
{
	if(!m_memDC)
	{
		m_memDC = new CMemDC( hWnd );
	}

	// create memory DC
	if(m_memDC && backimage)
	{
		CDC * pMemDC = new CDC;
		pMemDC -> CreateCompatibleDC(m_memDC->m_hDC);
		CBitmapHandle pOldBitmap;
		pMemDC -> SetBkColor(RGB(255,0,0));
		SIZE bmSize;
		backimage.GetSize(bmSize);
		pOldBitmap = pMemDC -> SelectBitmap(backimage);
		m_memDC->BitBlt(0,0,bmSize.cx, bmSize.cy,pMemDC->m_hDC,0,0,SRCCOPY);
		pMemDC -> SelectBitmap(pOldBitmap);
		delete pMemDC;
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
	switch(nID)
	{
	case IDC_MAINDLGBUT_POINTCHECK:
		{
			dlgWater[dlgpurchase]->ShowWindow(SW_SHOW);
			dlgWater[dlgpointcheck]->ShowWindow(SW_HIDE);
		}break;
	case IDC_MAINDLGBUT_PURCHASE:
		{
			dlgWater[dlgpointcheck]->ShowWindow(SW_SHOW);
			dlgWater[dlgpurchase]->ShowWindow(SW_HIDE);
		}break;
	case IDC_CLOSE:
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}break;
	}
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
	maingdithread.Create(ThreadProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CMainGdiDlg::ThreadProcess(LPVOID lpArg)
{
	CMainGdiDlg *MallDlg = (CMainGdiDlg *)lpArg;
	while (MallDlg->maingdithread.IsRunning()  && MallDlg->maingdithread.WaitForThread(MAINGDIDLGTHREAD_LOOPTIME)==FALSE)
	{
		MallDlg->MainThreadFunc();
	}
	MallDlg->maingdithread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CMainGdiDlg::MainThreadFunc()
{
// 	if(backimage[AniSelectNum].size()>0)
// 	{
// 		++AniNum%=backimage[AniSelectNum].size();
// 		InvalidateRect(hWnd, NULL, FALSE);
// 		//InvalidateRect를 강제로 바로 실행 하기 위해존제..
// 		UpdateWindow(hWnd);
// 	}
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