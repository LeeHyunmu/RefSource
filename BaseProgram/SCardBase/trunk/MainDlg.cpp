// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
//--------------------------------------------------------------
//|
//|������
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
//|��ġ�� ������ �۵��Ǵ� �Լ�
//|
//---------------------------------------------------------------
//BOOL CMainDlg::OnDeviceChange( UINT nEventType, DWORD dwData )
LRESULT CMainDlg::OnDeviceChange(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	PDEV_BROADCAST_HDR pDBH = NULL; 
	pDBH = (PDEV_BROADCAST_HDR)(DWORD_PTR)lParam ; //lParam�Դϴ�.
	switch(wParam) 
	{ 
	case DBT_DEVICEARRIVAL:
		{
			if(pDBH->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) //�� �Ķ��Ÿ�� ���� �ø���, �䷯��,��Ʈ����ġ�� 
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
			if(pDBH->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE) //�� �Ķ��Ÿ�� ���� �ø���, �䷯��,��Ʈ����ġ�� 
			{
				if(scardtm)
					scardtm->DeviceConnect(lParam, FALSE);
			}
			if(pDBH->dbch_devicetype == DBT_DEVTYP_VOLUME) // �޸� ��ġ
			{
				//PDEV_BROADCAST_VOLUME pp = (PDEV_BROADCAST_VOLUME)(DWORD_PTR)lParam;
				//char Vol = VolumMask(pp->dbcv_unitmask);
				//_SFILE::logfile("������:%d,����̽�Ÿ��:%p, Volum:%c, flages:%d",pp->dbcv_size,pp->dbcv_devicetype,Vol, pp->dbcv_flags);
			}
		}
		break; 
	}	
	return TRUE;
}
//--------------------------------------------------------------
//|
//|�ʱ�ȭ �Լ�
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
	// ����Ʈī�� ���ؼ� �޽��� ���
	//=============================================
	_GASCARD::RegisterForDeviceNotifications(m_hWnd);
	//=============================================
	// ������Ʈ ����
	//=============================================
	CreateObject();
	//=============================================
	// scardtm
	//=============================================
	scardtm = obj->GetSCardTmObject();
	//=============================================
	// MainDlg GDI Ŭ���� ���� �۾�
	//=============================================
	//=============================================
	// gdi ��ü�� �޴´�
	//=============================================
	gdi = obj->GetMainGdiObject();
	//=============================================
	// UI �ʱ�ȭ(������ �ڵ��� �ѱ��)
	//=============================================
	if(gdi)
		gdi->initDlg(m_hWnd);
	//=============================================
	// ������Ʈ �ʱ�ȭ
	//=============================================
	initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|��ü ���� �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::CreateObject() 
{
	//=============================================
	// ������Ʈ ���� Ŭ���� ����
	//=============================================
	obj=new CClassObject;
	if(obj)
		obj->CreateObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CMainDlg::initObject() 
{
	//=============================================
	// ������Ʈ ���� Ŭ���� ����
	if(obj)
		obj->initObject();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������Ʈ ��ü �Ҹ� �Լ�
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
//|��ü �Ҹ� �Լ�
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
//|���� ��ư
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
//|DLG ���� �Լ�
//|
//---------------------------------------------------------------
void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
//--------------------------------------------------------------
//|
//|�׸��� �Լ�
//|
//---------------------------------------------------------------
LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// ���⼭ ������ ������ �׷��ش�.
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