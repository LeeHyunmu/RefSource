
#ifndef __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//
//======================================================//
//			
//=======================================================//
class CTraceWinDebug : public IOutput
{
public:
	//생성자
	CTraceWinDebug(TCHAR* name, DWORD width = 400, DWORD height = 600);
	//소멸자
	~CTraceWinDebug();
//======================================================//
//			인터페이스 재 정의 함수
//======================================================//
public:
	static LRESULT CALLBACK WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam);
	void Write(int x, int y, TCHAR* format, ...);
	void Clear();
	void SetVisible(BOOL visible);
	BOOL GetVisible();
//=======================================================//
//			객체 선언
//======================================================//
private:
	HWND _window;
	BOOL _visible;
//=======================================================//

};
#endif// __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
