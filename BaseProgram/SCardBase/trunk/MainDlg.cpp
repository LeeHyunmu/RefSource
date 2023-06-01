// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainDlg::CMainDlg():
obj(NULL),
gdi(NULL),
scardtm(NULL)
{}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CMainDlg::OnIdle()
{
	return FALSE;
}
//--------------------------------------------------------------
//|
//|장치가 들어오면 작동되는 함수
//|
//---------------------------------------------------------------
//BOOL CMainDlg::OnDeviceChange( UINT nEventType, DWORD dwData )
LRESULT CMainDlg::OnDeviceChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	PDEV_BROADCAST_HDR pDBH = NULL; 
	pDBH = (PDEV_BROADCAST_HDR)(DWORD_PTR)lParam ; //lParam입니다.
	switch(wParam) 
	{ 
	case DBT_DEVICEARRIVAL:
		{
			if(pDBH->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) //요 파라메타에 따라 시리얼, 페러럴,네트웍장치등 
			{
				if(scardtm)
					scardtm->DeviceConnect(lParam, TRUE);
			}
			if(pDBH->dbch_devicetype == DBT_DEVTYP_VOLUME) 
			{
				//PDEV_BROADCAST_VOLUME pp = (PDEV_BROADCAST_VOLUME)(DWORD_PTR)lParam;
			}
		}break; 
	case DBT_DEVICEREMOVECOMPLETE:
		{
			if(pDBH->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) //요 파라메타에 따라 시리얼, 페러럴,네트웍장치등 
			{
				if(scardtm)
					scardtm->DeviceConnect(lParam, FALSE);
			}
			if(pDBH->dbch_devicetype == DBT_DEVTYP_VOLUME) // 메모리 장치
			{
				//PDEV_BROADCAST_VOLUME pp = (PDEV_BROADCAST_VOLUME)(DWORD_PTR)lParam;
				//char Vol = VolumMask(pp->dbcv_unitmask);
				//_SFILE::logfile("사이즈:%d,디바이스타입:%p, Volum:%c, flages:%d",pp->dbcv_size,pp->dbcv_devicetype,Vol, pp->dbcv_flags);
			}
		}
		break; 
	}	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|초기화 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	//=============================================
	// 스마트카드 컨넥션 메시지 등록
	//=============================================
	_GASCARD::RegisterForDeviceNotifications(m_hWnd);
	//=============================================
	// 오브젝트 생성
	//=============================================
	CreateObject();
	//=============================================
	// scardtm
	//=============================================
	scardtm = obj->GetSCardTmObject();
	//=============================================
	// MainDlg GDI 클래스 연결 작업
	//=============================================
	//=============================================
	// gdi 객체를 받는다
	//=============================================
	gdi = obj->GetMainGdiObject();
	//=============================================
	// UI 초기화(윈도우 핸들을 넘긴다)
	//=============================================
	if(gdi)
		gdi->initDlg(m_hWnd);
	//=============================================
	// 오브젝트 초기화
	//=============================================
	initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 생성 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
	//=============================================
	// 오브젝트 관리 클래스 생성
	//=============================================
	obj=new CClassObject;
	if(obj)
		obj->CreateObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|오브젝트 객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CMainDlg::initObject() 
{
	//=============================================
	// 오브젝트 관리 클래스 생성
	if(obj)
		obj->initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|오브젝트 객체 소멸 함수
//|
//---------------------------------------------------------------
void CMainDlg::Release()
{
	if(obj)
	{
		obj->KillObject();
		delete ((CClassObject*)obj);	obj=NULL;
	}
}
//--------------------------------------------------------------
//|
//|객체 소멸 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}
//--------------------------------------------------------------
//|
//|종료 버튼
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	Release();
	CloseDialog(wID);
	return 0;
}
//--------------------------------------------------------------
//|
//|DLG 종료 함수
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
//--------------------------------------------------------------
//|
//|그리기 함수
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// 여기서 뭔가를 실제로 그려준다.
	if(gdi)
		gdi->DrawPaint();
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnListboxSCardNameNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch (wNotifyCode)
	{
	case LBN_DBLCLK:		
		break;
	case LBN_SELCHANGE:
		{
			if(gdi)
				gdi->ListboxSCardNameMessage(wNotifyCode,wID);
		}break;
	case LBN_SETFOCUS:
		
		break;
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnComboboxVidPidNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch (wNotifyCode)
	{
	case CBN_SELCHANGE:
		{
			if(gdi)
			   gdi->ComboboxVidPidMessage(wNotifyCode,wID);
		}break;
	case CBN_ERRSPACE:	break;
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnComboboxApduShortNotify(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	switch (wNotifyCode)
	{
	case CBN_SELCHANGE:
		{
			if(gdi)
				gdi->ComboboxApduShortMessage(wNotifyCode,wID);
		}break;
	case CBN_ERRSPACE:	break;
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CMainDlg::OnButtonClickedEvent(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	if(gdi)
		return gdi->ButtonClickedMessage(uNotifyCode,nID);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HBRUSH CMainDlg::OnCtlColorStatic(CDCHandle dc, CStatic wndStatic)
{
	if(gdi)
		return gdi->StaticCtlColorMessage(dc,wndStatic);
	return 0;
}