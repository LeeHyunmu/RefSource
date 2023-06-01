
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainGdiDlg : public IMainGdiDlg
{
public:
	//생성자
	CMainGdiDlg();
	//소멸자
	~CMainGdiDlg();
	//=================스래드 함수===========================//

private:
	HWND hWnd;
	//디버그 출력
	void DebugEditCall(CString str);
public:
	//======================================================//
	//			일반 함수
	//======================================================//

	CString   ChangeExt(CString sPathName, CString sNewExt, char find=_T('\\'));

	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//클래스에 있는 객체 메모리 해제
	virtual void ControlDestroy();
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd);
	//Dlg 그리기 함수
	virtual void DrawPaint();
	//그림을 다시 그리기 위해서 호출
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
