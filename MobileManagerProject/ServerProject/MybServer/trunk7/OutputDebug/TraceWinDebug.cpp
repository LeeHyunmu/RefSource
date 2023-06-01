
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
//|������
//|
//---------------------------------------------------------------
CTraceWinDebug::CTraceWinDebug(TCHAR* name, DWORD width, DWORD height)
{
	//������ ����
	// �ν��Ͻ��� ��� ����� ���ؼ� �����Ѵ�.
	HINSTANCE instance = GetModuleHandle( NULL );
	WNDCLASS wc;
	// ������ Ŭ���� �ۼ�.
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
	// ������ Ŭ���� ���.
	RegisterClass(&wc);
	// �����츦 �����Ѵ�.
	_window = CreateWindow(name, name, WS_POPUP | WS_CAPTION, 
		GetSystemMetrics( SM_CXSCREEN ) - width, 
		0, //GetSystemMetrics( SM_CYSCREEN ) - height, 
		width, height, NULL, NULL, instance, NULL );
	if(_window == NULL)
	{
		MessageBox(NULL, _T("cannot create Window for Trace"), _T("Alert"), MB_OK);
		return;
	}
	// �����츦 ȭ�鿡 ���̰��Ѵ�.
	ShowWindow( _window, SW_SHOW );
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CTraceWinDebug::~CTraceWinDebug()
{
	if(_window) DestroyWindow(_window);
}

//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ���
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
	// �����쿡 ���.
	HDC hdc;
	hdc = GetDC(_window);
	TextOut(hdc,x,y,fullText,_tcslen(fullText));
	ReleaseDC(_window,hdc);
	delete [] buffer;
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//| ���� ����
//|
//---------------------------------------------------------------
void CTraceWinDebug::Clear()
{
	if(_visible == FALSE || _window == NULL) return;
	InvalidateRect(_window, NULL, TRUE);
	//InvalidateRect�� ������ �ٷ� ���� �ϱ� ��������..
	UpdateWindow(_window);
}
//--------------------------------------------------------------
//|
//|SHOW, HIDE ����
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
//|SHOW HIDE ����
//|
//---------------------------------------------------------------
BOOL CTraceWinDebug::GetVisible()
{
	return _visible;
}