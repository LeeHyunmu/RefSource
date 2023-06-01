// JapanTestDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"

// CJapanTestDlg 대화 상자
class CJapanTestDlg : public CDialog
{
	CAQdeviceCmd test;
	//AQ Device 객체
	CWaterCmd AQDevice;
// 생성입니다.
public:
	CJapanTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_JAPANTEST_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
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

	//water 콜백
	static DWORD WINAPI ReadEventFunc(LPVOID lpThreadParameter, CWaterCmdFormat::WaterCheck check);
	//클래스 함수 호출
	DWORD  ReadEventMessage(CWaterCmdFormat::WaterCheck check);

	void  DebugWrite(CString data);

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	CListBox m_DebugView;
};
