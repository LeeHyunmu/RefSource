
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainGdiDlg : public IGdiDlg
{
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
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
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
	HRGN hrgn[3];
	COLORREF color[3];
	CListBox List;
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	//
	CStatic stc;
	//
	CStatic terminalstc;
	//
	CBitmap Aqlogo;
	CPoint  AqImgsize;
	CBitmap AqMainlogo;
	CPoint  AqMainImgsize;

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
	//색 채우기
	virtual void SetDrawItem(int index, COLORREF _color=RGB(0xff,0x00,0x00));
	//텍스트 추가(리스트박스)
	virtual void SetListString(CString _string);
	//
	virtual void StaticText(CDCHandle dc);
	//
	virtual void SetStaticString(CString _string);
	//
	virtual void SetTerminalStaticString(CString _string);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
