
#include "stdAfx.h"
#include "TraceWinDebug.h"
#include <time.h>

//--------------------------------------------------------------
//|
//|Window CALLBACK Func
//|
//---------------------------------------------------------------
LRESULT CALLBACK CTraceWinDebug::WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lparam);
}
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CTraceWinDebug::CTraceWinDebug(TCHAR* name, DWORD width, DWORD height)
{
	//윈도우 생성
	// 인스턴스를 얻는 방법에 대해서 설명한다.
	HINSTANCE instance = GetModuleHandle( NULL );
	WNDCLASS wc;
	// 윈도우 클래스 작성.
	wc.style   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc  = (WNDPROC)WndProc;
	wc.cbClsExtra  = 0;         // No Extra Window Data
	wc.cbWndExtra  = 0;         // No Extra Window Data
	wc.hInstance  = instance;
	wc.hIcon   = LoadIcon(NULL, IDI_WINLOGO);   // Load The Default Icon
	wc.hCursor   = LoadCursor(NULL, IDC_ARROW);   // Load The Arrow Pointer
	wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH) ;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = name;
	// 윈도우 클래스 등록.
	RegisterClass(&wc);
	// 윈도우를 생성한다.
	_window = CreateWindow(name, name, WS_POPUP | WS_CAPTION, 
		GetSystemMetrics( SM_CXSCREEN ) - width, 
		0, //GetSystemMetrics( SM_CYSCREEN ) - height, 
		width, height, NULL, NULL, instance, NULL );
	if(_window == NULL)
	{
		MessageBox(NULL, _T("cannot create Window for Trace"), _T("Alert"), MB_OK);
		return;
	}
	// 윈도우를 화면에 보이게한다.
	ShowWindow( _window, SW_SHOW );
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CTraceWinDebug::~CTraceWinDebug()
{
	if(_window) DestroyWindow(_window);
}

//--------------------------------------------------------------
//|
//|텍스트 출력
//|
//---------------------------------------------------------------
void CTraceWinDebug::Write(int x, int y, TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	va_list args;
	TCHAR* buffer;
	va_start(args, format);
	int len = _vsctprintf(format, args) + 1; 
	buffer = new TCHAR[len];
	_vstprintf_s(buffer, len, format, args);
	va_end(args);
	TCHAR date[16];
	TCHAR time[16];
	TCHAR fullText[1024];
	_tstrdate_s(date, _countof(date));
	_tstrtime_s(time,_countof(time));
	_stprintf_s(fullText, _countof(fullText), _T("%s %s> %s"), date, time, buffer);
	// 윈도우에 출력.
	HDC hdc;
	hdc = GetDC(_window);
	TextOut(hdc,x,y,fullText,_tcslen(fullText));
	ReleaseDC(_window,hdc);
	delete [] buffer;
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//| 내용 삭제
//|
//---------------------------------------------------------------
void CTraceWinDebug::Clear()
{
	if(_visible == FALSE || _window == NULL) return;
	InvalidateRect(_window, NULL, TRUE);
	//InvalidateRect를 강제로 바로 실행 하기 위해존제..
	UpdateWindow(_window);
}
//--------------------------------------------------------------
//|
//|SHOW, HIDE 설정
//|
//---------------------------------------------------------------
void CTraceWinDebug::SetVisible(BOOL visible)
{
	_visible = visible;
	if(_visible == TRUE)
	{
		if(_window) ShowWindow( _window, SW_SHOW );
	}
	else
	{
		if(_window) ShowWindow( _window, SW_HIDE );
	}
}
//--------------------------------------------------------------
//|
//|SHOW HIDE 상태
//|
//---------------------------------------------------------------
BOOL CTraceWinDebug::GetVisible()
{
	return _visible;
}