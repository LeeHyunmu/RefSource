
// AQDeviceDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CAQDeviceDlg 대화 상자
class CAQDeviceDlg : public CDialog
{
// 생성입니다.
public:
	CAQDeviceDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AQDEVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

//AQ Device 객체
	CDeviceCmd AQDevice;
	//
	CAQCmdFormat::WaterErrorCode bAQStart;
	//Polling start type


	//=======================================================//
	//			콜백 함수 정의
	//======================================================//
	//water check 콜백
	static DWORD WINAPI ReadEventFunc(LPVOID lpThreadParameter, CAQCmdFormat::WaterCheck check);
	//클래스 함수 호출
	DWORD  ReadEventMessage(CAQCmdFormat::WaterCheck check);

	//디버그 콜백
	static DWORD WINAPI DebugFunc(LPVOID lpThreadParameter, byte* data, int datalen, bool bsend);
	//클래스 함수 호출
	DWORD  DebugMessage(byte* data, int datalen, bool bsend);
// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
	//디버그 출력
	void  DebugWrite(CString data);
	CComboBox m_watercmd;
	CEdit m_cmdstring;
	afx_msg void OnCbnSelchangeWatercmd();
	// 명령어 data 값
	CString m_cmdstringvalue;
	afx_msg void OnBnClickedCmdsend();
};
