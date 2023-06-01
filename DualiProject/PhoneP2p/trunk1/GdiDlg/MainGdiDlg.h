
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IImagingFactory;
class CMainGdiDlg : public IMainGdiDlg
{
public:
	//생성자
	CMainGdiDlg();
	//소멸자
	~CMainGdiDlg();
	//=================스래드 함수===========================//
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
	IDualiP2pMg*  dualip2p;
	//======================================================//
	//			인터페이스 포인터 
	//======================================================//

	//======================================================//
	//			일반 함수
	//======================================================//

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
	//모드 변경 버튼
	CSuperButton HospitalBut_1;
	//모드 변경 버튼
	CSuperButton HospitalBut_2;
	//모드 변경 버튼
	CSuperButton HospitalBut_3;
	//
	UINT hospitalnum;
	//
	UINT ButShow;
	UINT backButShow;
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
	//
	virtual void StaticText(CDCHandle dc);
	//파괴
	virtual void  ControlDestroy();
	//그림을 다시 그리기 위해서 호출
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
	//이미지 변경 및 데이타를 보냄(휴대폰으로)
	virtual void ImageChange(UINT _MgNum, BOOL bSendData=TRUE);
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//타이머 메시지
	virtual void TimerMessage();
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
