
#ifndef __IGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IGdiDlg
{
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void Release()=0;
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg 그리기 함수
	virtual void DrawPaint()=0;
	//색 채우기
	virtual void SetDrawItem(int index, COLORREF _color=RGB(0xff,0x00,0x00))=0;
	//텍스트 추가(리스트박스)
	virtual void SetListString(CString _string)=0;
	//VIEW 텍스트 크기 변경
	virtual void StaticText(CDCHandle dc)=0;
	//VIEW 텍스트 변경
	virtual void SetStaticString(CString _string)=0;
	//
	virtual void SetTerminalStaticString(CString _string)=0;
};

#endif //__IGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__