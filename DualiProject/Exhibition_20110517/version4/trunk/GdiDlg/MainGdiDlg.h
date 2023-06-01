
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


interface IImagingFactory;
interface IImage;

class CMainGdiDlg : public IMainGdiDlg
{
	typedef vector<IImage*> MODEIMAGE;
	typedef vector<CString> vecApudString;
public:
	//생성자
	CMainGdiDlg();
	//소멸자
	~CMainGdiDlg();
	//=================스래드 함수===========================//
	//======================================================//
	//			스래드변수
	//======================================================//
	CThread  maingdithread;
	CEvent   maingdiEvent;
	CEvent   CloseEvent;
	MODEIMAGE modeimage;
	//======================================================//
	//			스래드함수
	//======================================================//
	void ThreadMakeObject();
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	void MainThreadFunc();

	//=======================================================//
	//			컨트롤 변수
	//======================================================//
	//CSuperButton but1;
	//=======================================================//
	//			변수 선언
	//======================================================//
	HWND hWnd;
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//IDualiP2pMg*  dualip2p;
	IDualiCardMg* dualicard;
	//======================================================//
	//			인터페이스 포인터 
	//======================================================//

	//======================================================//
	//			일반 함수
	//======================================================//
	void DrawWhiteText(CDC* pDC, CRect textrc, CString text);
	//=======================================================//
	//======================================================//
	//			변수
	//======================================================//
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	//
	UINT MgNum;
	//
	UINT MgMode;
	//
	IImagingFactory* pImgFactory;
	//모드 변경 버튼
	CSuperButton ModeChangeBut;
	//종료버튼
	CSuperButton CloseBut;
	//버튼
	CSuperButton HospitalBut_1;
	//버튼
	CSuperButton HospitalBut_2;
	//버튼
	CSuperButton HospitalBut_3;
	//
	UINT hospitalnum;
	//
	UINT ButShow;
	UINT backButShow;
	//모드 3에서 전화번호를 텍스트로 출력하는 값
	CString strMode3;
	CRect   rcMode3;
	//이미지 모드 전환
	int ImageNumber;
	int imgbackup;
	BOOL bCardConnection;
	DWORD changeTime;
	//아이템 수
	int loopcount;
	//APDU String
	vecApudString apdu;
	//사운드 경로
	CString soundfile;
	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void Release();
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd);
	//Dlg 그리기 함수
	virtual void DrawPaint();
	//파괴
	virtual void  ControlDestroy();
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//모드3에서 텍스트 출력함수
	virtual void SetMode3Text(CString text, CRect* rc);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
