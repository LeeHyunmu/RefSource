// UpdateDlg.h : header file
//

#if !defined(AFX_UPDATEDLG_H__7896ECBA_9245_479C_A53F_368648EE8342__INCLUDED_)
#define AFX_UPDATEDLG_H__7896ECBA_9245_479C_A53F_368648EE8342__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Sddl.h>
#include <WinSvc.h>
/////////////////////////////////////////////////////////////////////////////
// CServiceDlg dialog
#define  SCardSvr		"SCardSvr"
#define  SCardDrv		"SCardDrv"
class CServiceDlg : public CDialog
{
// Construction
public:
	BOOL SelfDelete();
	BOOL CreatDevice(char* sc_name, char* sc_path);
	BOOL UnloadDevice(char* sc_name);
	BOOL StartServiceExcu(char *svcName);
	BOOL ChangeServiceConfig_my(LPCTSTR lpServicName);
	//스마트카드 자동으로 수정, 시작 상태로 만든다
	BOOL StartRemoteRegistry(LPCTSTR lpServicName);
	BOOL MyStartService(SC_HANDLE schService, char* szServiceName);
	CServiceDlg(CWnd* pParent = NULL);	// standard constructor
// Dialog Data
	//{{AFX_DATA(CServiceDlg)
	enum { IDD = IDD_UPDATE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServiceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	DWORD StartType;

	// Generated message map functions
	//{{AFX_MSG(CServiceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATEDLG_H__7896ECBA_9245_479C_A53F_368648EE8342__INCLUDED_)
