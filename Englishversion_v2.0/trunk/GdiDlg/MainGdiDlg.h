
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWTLIExplorer;
class CCouponPopDlg;
class CCouponDlg;
class CContentsMallDlg;
class CContentsMallPopDlg;
class CAdvertsDlg;
class CCompleteDlg;
class CMainGdiDlg : public IMainGdiDlg
{
	CSuperButton videochange;
public:
	//생성자
	CMainGdiDlg();
	//소멸자
	~CMainGdiDlg();
	//=================스래드 함수===========================//

	//=======================================================//
	//			변수 선언
	//======================================================//
	HWND hWnd;
	ULONG_PTR m_gdiplusToken;
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//======================================================//
	//			버튼 객체
	//======================================================//
	CComPtr<WMPLib::IWMPPlayer4> pWMP;
	CComPtr<WMPLib::IWMPControls> pCtrl;
	CComPtr<WMPLib::IWMPSettings> pSetting;
	//
	CSuperStatic mpw;

	//CWTLIExplorer* pBrowser;
	//쿠폰 dlg
	CCouponDlg* CouponDlg;
	//contents dlg
	CContentsMallDlg* contensmall;

	//AQ 베너 dlg
	CAdvertsDlg* AQadvertsDlg;

	//경로 저장
	CString FilePath;
	int filenum;

	//
	CCouponPopDlg* coupondlg;
	//
	CContentsMallPopDlg* mallPop;
	//완료 창
	CCompleteDlg* comdlg;
	//aq 디바이스
	CDeviceCmd AqDevice;
	//|=============================================
	//|카드 메시지 상태 변수
	//|==================================================
	int NFCCommandNum;
	BOOL bCouponClick;
	BOOL bContentsMallClick;
	//======================================================//
	//			일반 함수
	//======================================================//
	//그림을 다시 그리기 위해서 호출
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);

	CString   ChangeExt(CString sPathName, CString sNewExt, char find=_T('\\'));

	//=======================================================//
	//======================================================//
	//			변수
	//======================================================//
	CListBox List;
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;

	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void ControlDestroy();
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd);
	//Dlg 그리기 함수
	virtual void DrawPaint();
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);

	//
	virtual BOOL MediaPlayTimer(UINT_PTR nIDEvent);
	//NFC Call
	virtual BOOL NfcEventCall(WPARAM wParam);
	//
	//NDEFData Change Message
	virtual BOOL DeviceCmdCallMessage(WPARAM wParam);
	//UI 조정 이벤트 받기 위한 함수
	virtual BOOL NfcAqDeviceMessage(WPARAM wParam);
	//UI CouponClick Msg
	virtual BOOL CouponClickMessage(WPARAM wParam);
	//
	//virtual BOOL CardFindConnectLoop(DWORD* conectflag);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
