
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
	//소멸자
	virtual ~IMainGdiDlg()=0;	//클래스에 있는 객체 메모리 해제
	virtual void ControlDestroy()=0;
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg 그리기 함수
	virtual void DrawPaint()=0;
};

#endif //__MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__