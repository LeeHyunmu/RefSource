
#ifndef __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IMainGdiDlg
{
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void ControlDestroy()=0;
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg 그리기 함수
	virtual void DrawPaint()=0;
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID)=0;
	//
	virtual BOOL MediaPlayTimer(UINT_PTR nIDEvent)=0;
	//
	virtual BOOL NfcEventCall(WPARAM wParam)=0;
	//
	//NDEFData Change Message
	virtual BOOL DeviceCmdCallMessage(WPARAM wParam)=0;
	//UI 조정 이벤트 받기 위한 함수
	virtual BOOL NfcAqDeviceMessage(WPARAM wParam)=0;
	//
	virtual BOOL CouponClickMessage(WPARAM wParam)=0;
	//
	//virtual BOOL CardFindConnectLoop(DWORD* conectflag)=0;
};

#endif //__MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__