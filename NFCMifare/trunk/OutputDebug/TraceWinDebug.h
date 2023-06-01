
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
	static IOutput* debug;
	static HINSTANCE instance;
	static TCHAR  clipString[BUFFSIZE_4096];
	static BOOL bListClick;
	typedef deque<CString, allocator<CString> > CHARDEQUE;
	typedef queue<CString,CHARDEQUE> CHARQUEUE;
	CHARQUEUE qData;
public:
	//생성자
	CTraceWinDebug(TCHAR* name, DWORD width = 600, DWORD height = 400);
	//소멸자
	~CTraceWinDebug();
//=================스래드 함수===========================//
	static DWORD WINAPI OutputProcess(LPVOID lpArg);
//=================프로세서 콜백 함수===========================//
	static LRESULT CALLBACK WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam);
//=======================================================//
//			변수 선언
//======================================================//
//======================================================//
//			싱글톤 함수
//======================================================//
	static IOutput* GetDebug();
public:
	CThread  Outputthread;
	CEvent  OutputEvent;
	CEvent  CloseWinDebugEvent;
	BOOL bClose;
//======================================================//
//			일반함수
//======================================================//
	//
	void OutputThread();
//======================================================//
//			인터페이스 재 정의 함수
//======================================================//
public:
	//출력 호출 함수
	virtual void Write(TCHAR* format,...);
	//종료
	virtual void Clear();
	//디버그창 보이고 안보이고 설정
	virtual void SetVisible(BOOL visible);
	//디버그 창이 보이는가?
	virtual BOOL GetVisible();
	//스래드 출력함수
	virtual void OutputString();
	//메모리 해제 
	virtual void Release();
//=======================================================//
//			객체 선언
//======================================================//
private:
	DWORD _width;
	HWND _window;
	BOOL _visible;
	HICON hwinicon;
	//윈도우에는 리스트박스에 출력
	HWND _listbox;
//=======================================================//

};
#endif// __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
