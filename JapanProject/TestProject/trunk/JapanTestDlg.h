// JapanTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"

// CJapanTestDlg ��ȭ ����
class CJapanTestDlg : public CDialog
{
	CAQdeviceCmd test;
	//AQ Device ��ü
	CWaterCmd AQDevice;
// �����Դϴ�.
public:
	CJapanTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_JAPANTEST_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	static CString byteToHexbyteValue(byte* data, int len) ;
	static int  ChangeHexCord(CString data, BYTE* hex, int maxlen);
	HICON m_hIcon;
	//Atype Call back func
	static DWORD WINAPI AtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD AtypEventMessage(byte* eventdata, int eventdatalen);
	//Mtype Call back func
	static DWORD WINAPI MtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD MtypEventMessage(byte* eventdata, int eventdatalen);
	//disconnect Call back func
	static DWORD WINAPI DisConnectEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD DisConnectEventMessage(byte* eventdata, int eventdatalen);

	//water �ݹ�
	static DWORD WINAPI ReadEventFunc(LPVOID lpThreadParameter, CWaterCmdFormat::WaterCheck check);
	//Ŭ���� �Լ� ȣ��
	DWORD  ReadEventMessage(CWaterCmdFormat::WaterCheck check);

	void  DebugWrite(CString data);

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CListBox m_DebugView;
};
