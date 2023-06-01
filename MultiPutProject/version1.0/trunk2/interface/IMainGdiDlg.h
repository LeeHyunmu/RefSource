
#ifndef __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IMainGdiDlg
{
	//순수가상 소멸자
	virtual ~IMainGdiDlg()=0;
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void ControlDestroy()=0;
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg 그리기 함수
	virtual void MainGdiDlgDrawPaint()=0;
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID)=0;

	virtual BOOL CtlColorStatic(CDCHandle dc, CStatic wndStatic, HBRUSH& hbrush)=0;
	//마우스 이벤트 받기
	virtual BOOL MouseEventMessage(UINT Msgtype, CPoint cuserpos)=0;
	//Hot Key Message
	virtual BOOL HotKeyMessage(UINT KeyId)=0;
	//cursor Message
	virtual BOOL MouseCursorMessage(CWindow wnd, UINT nHitTest, UINT message)=0;
	//Key Message
	virtual BOOL VirtualKeyMessage(UINT Msgtype, UINT nChar, UINT nRepCnt)=0;
	//==================char type (hex, byte, string) 변환 함수===========================//
	//|char를 hex로 변환
	virtual CString Changcharbyhex(CString pass)=0;
	//|카드로 부터 받은 데이타를 변환 한다(0x00~0xFF까지 이므로 스트링형으로 변환해서 판단한다)
	virtual BOOL ChangBytebyhex(char* pass, DWORD len,  CString& repass)=0;
	//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
	virtual int  ChangeHexCord(CString data, char hex[])=0;
	//서버를 시작한다
	virtual BOOL ServerStartCall()=0;
};

#endif //__MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__