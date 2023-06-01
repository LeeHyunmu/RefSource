
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
	virtual void Release()=0;
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg 그리기 함수
	virtual void DrawPaint()=0;
	//콤보박스 IDC_VIDPID 메시지
	virtual void ComboboxVidPidMessage(UINT uNotifyCode, int nID)=0;
	//콤보박스 IDC_APDUSHORT 메시지
	virtual void ComboboxApduShortMessage(UINT uNotifyCode, int nID)=0;
	//리스트박스 IDC_PSCARDNMAE 메시지
	virtual void ListboxSCardNameMessage(UINT uNotifyCode, int nID)=0;
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual void ButtonClickedMessage(UINT uNotifyCode, int nID)=0;
	//Static Control CtlColor 변경
	virtual HBRUSH StaticCtlColorMessage(CDCHandle dc, CStatic wndStatic)=0;
};

#endif //__MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__