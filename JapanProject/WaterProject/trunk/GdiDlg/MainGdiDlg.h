
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMainGdiDlg
{
	static enum ButState{
		dlgpurchase=0x00,
		dlgpointcheck=0x01
	};
	//활성화된 dlg
	ButState dlgState;
	//dlg 생성
	vector<CWindow*> dlgWater;

	//메모리 DC
	CMemDC* m_memDC;
	//배경 이미지
	CBitmapHandle backimage;
	//ani image num
	int AniSelectNum;
	int AniNum;
	//Rgn
	CRgnHandle m_hRgn;

	CAQdeviceCmd test;
	//Atype Call back func
	static DWORD WINAPI AtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD AtypEventMessage(byte* eventdata, int eventdatalen);
	//Mtype Call back func
	static DWORD WINAPI MtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD MtypEventMessage(byte* eventdata, int eventdatalen);
	//disconnect Call back func
	static DWORD WINAPI DisConnectEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
	DWORD DisConnectEventMessage(byte* eventdata, int eventdatalen);
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
	//======================================================//
	//			스래드함수
	//======================================================//
	void ThreadMakeObject();
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	void MainThreadFunc();

	//=======================================================//
	//			컨트롤 변수
	//======================================================//

	//=======================================================//
	//			변수 선언
	//======================================================//
	HWND hWnd;
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
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

	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//클래스에 있는 객체 메모리 해제
	virtual void Release();
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd);
	//배경 그리기 함수
	virtual BOOL EraseBkgnd(CDCHandle dc);
	//Dlg 그리기 함수
	virtual void DrawPaint(CDCHandle dc);
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
