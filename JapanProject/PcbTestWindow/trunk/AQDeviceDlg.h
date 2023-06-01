
// AQDeviceDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CAQDeviceDlg ��ȭ ����
class CAQDeviceDlg : public CDialog
{
// �����Դϴ�.
public:
	CAQDeviceDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_AQDEVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

//AQ Device ��ü
	CDeviceCmd AQDevice;
	//
	CAQCmdFormat::WaterErrorCode bAQStart;
	//Polling start type


	//=======================================================//
	//			�ݹ� �Լ� ����
	//======================================================//
	//water check �ݹ�
	static DWORD WINAPI ReadEventFunc(LPVOID lpThreadParameter, CAQCmdFormat::WaterCheck check);
	//Ŭ���� �Լ� ȣ��
	DWORD  ReadEventMessage(CAQCmdFormat::WaterCheck check);

	//����� �ݹ�
	static DWORD WINAPI DebugFunc(LPVOID lpThreadParameter, byte* data, int datalen, bool bsend);
	//Ŭ���� �Լ� ȣ��
	DWORD  DebugMessage(byte* data, int datalen, bool bsend);
// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
public:
	static CString byteToHexbyteValue(byte* data, int len) ;
	static int  ChangeHexCord(CString data, BYTE* hex, int maxlen);
	CListBox m_DataPrint;
	//����� ���
	void  DebugWrite(CString data);
	CComboBox m_watercmd;
	CEdit m_cmdstring;
	afx_msg void OnCbnSelchangeWatercmd();
	// ��ɾ� data ��
	CString m_cmdstringvalue;
	afx_msg void OnBnClickedCmdsend();
};
