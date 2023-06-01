
#include "stdAfx.h"
#include "TraceWinDebug.h"
#include <strsafe.h>

#define  DEBUGWINDOW_MAXLEN 76
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
	bClose=FALSE;
	_width = width;
	hwinicon = AtlLoadIconImage(IDR_MAINFRAME);
	//DestroyIcon();
	//SHGetFileInfo()
	//==========================================
	//윈도우 생성
	// 인스턴스를 얻는 방법에 대해서 설명한다.
	//============================================
	HINSTANCE instance = GetModuleHandle( NULL );
	WNDCLASS wc;
	// 윈도우 클래스 작성.
	wc.style   = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc  = (WNDPROC)WndProc;
	wc.cbClsExtra  = 0;         // No Extra Window Data
	wc.cbWndExtra  = 0;         // No Extra Window Data
	wc.hInstance  = instance;
	wc.hIcon   = hwinicon;
	wc.hCursor   = LoadCursor(NULL, IDC_ARROW);   // Load The Arrow Pointer
	wc.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH) ;
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = name;
	// 윈도우 클래스 등록.
	RegisterClass(&wc);
	//============================================
	// 윈도우를 생성한다.
	//=============================================
	_window = CreateWindow(name, name, WS_POPUP | WS_CAPTION, 
		GetSystemMetrics( SM_CXSCREEN ) - width, 
		0, //GetSystemMetrics( SM_CYSCREEN ) - height, 
		width, height, NULL, NULL, instance, NULL );
	if(_window == NULL)
	{
		MessageBox(NULL, _T("cannot create Window for Trace"), _T("Alert"), MB_OK);
		return;
	}
	//============================================
	// 생성한 윈도우의 클라이언트 영역의 크기를 구한다.
	//=============================================
	RECT rc;
	GetClientRect( _window, &rc );
	//============================================
	// 리스트박스를 생성한다
	//=============================================
	_listbox = CreateWindow( _T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL, 0, 0, rc.right, rc.bottom, _window, NULL, instance, NULL );
	if(_listbox == NULL)
	{
		MessageBox(NULL, _T("cannot create LISTBOX for TraceWindow"), _T("Alert"), MB_OK);
		return;
	}
	// 윈도우를 화면에 보이게한다.
	SetVisible(TRUE);
	//ShowWindow( _window, SW_SHOW );
	// 리스트박스를 화면에 보이게한다.
	ShowWindow( _listbox, SW_SHOW );
	// 스래드 생성
	OutputThread();
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CTraceWinDebug::~CTraceWinDebug()
{
	if(OutputEvent.IsOpen())
	{
		bClose=TRUE;
		OutputEvent.SetEvent();
		Sleep(1);
	}
	if(hwinicon)  DestroyIcon(hwinicon);
	hwinicon=NULL;
	if(_listbox) DestroyWindow(_listbox);
	_listbox=NULL;
	if(_window) DestroyWindow(_window);
	_window=NULL;
	Outputthread.WaitForThread();
	while(qData.empty()==false)
		qData.pop();
}
//--------------------------------------------------------------
//|
//|스래드 생성
//|
//---------------------------------------------------------------
void CTraceWinDebug::OutputThread()
{
	////////////////////////////////////////////////////////////////
	//종료 스레드 생성
	OutputEvent.Create();
	Outputthread.Create(OutputProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|카드 감지 및 처리 함수
//|
//---------------------------------------------------------------
void CTraceWinDebug::OutputString()
{
	while(OutputEvent.WaitForEvent())
	{
		if(bClose)
		{
			break;
		}
		if(_window&&_listbox)
		{
			while (!qData.empty())
			{
				TCHAR* pdt = qData.front().GetBuffer(0);
				// 리스트 박스에 문자열을 추가한다.
				SendMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) pdt);
				// 리스트 박스의 컨텐츠 개수를 구한다.
				DWORD n = SendMessage( _listbox, LB_GETCOUNT, 0, 0L ) - 1;
				// 리스트 박스의 컨텐츠 개수만큼 커서를 아래로 이동시킨다.
				SendMessage( _listbox, LB_SETCURSEL, (WPARAM)n, 0L );
				qData.pop();
			}
		}
		else
			break;
	}
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CTraceWinDebug::OutputProcess(LPVOID lpArg)
{
	CTraceWinDebug *debug = (CTraceWinDebug *)lpArg;
	if(debug)
		debug->OutputString();
	return 0;
}
//--------------------------------------------------------------
//|
//|텍스트 출력
//|
//---------------------------------------------------------------
void CTraceWinDebug::Write(TCHAR* format,...)
{
	if(GetVisible())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// 가변인자로부터 문자열 얻기
		//=============================================
		va_list args;
		TCHAR buffer[512]={0,};
		va_start(args, format);
		_vstprintf_s(buffer, sizeof(buffer), format, args);
		va_end(args);
		WCHAR fullText[1024];
		//============================================
		// 날짜 시간 얻기
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf_s(fullText, sizeof(fullText), _T("%s> %s"), strtime, buffer);
		//============================================
		// 출력
		//=============================================
		DWORD nLen = _tcslen(fullText);
		DWORD textpoint = 0;
		if(_listbox)
		{
			while(nLen != textpoint)
			{
				WCHAR WriteData[DEBUGWINDOW_MAXLEN+1];
				memset(WriteData, 0, sizeof(WriteData));
				int offset = nLen-textpoint;
				if(offset > DEBUGWINDOW_MAXLEN)
				{
					memcpy(WriteData,fullText+textpoint,DEBUGWINDOW_MAXLEN*sizeof(WCHAR)); textpoint+=DEBUGWINDOW_MAXLEN;
				}
				else
				{
					memcpy(WriteData,fullText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
				}
				//저장
				qData.push(WriteData);
			}
		}
		m_cs.Unlock();
		if(qData.size())
			OutputEvent.SetEvent();
	}
}
//--------------------------------------------------------------
//|
//| 내용 삭제
//|
//---------------------------------------------------------------
void CTraceWinDebug::Clear()
{
	if(_visible == FALSE || _window == NULL) return;
	// 리스트 박스에 문자열을 추가한다.
	SendMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) 0L);
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

// sTime이란 ANSI 문자열을 bstr이란 이름의 유니코드(BSTR 타입) 변수로 변환
// char sTime[] = "유니코드 변환 예제";
// BSTR bstr;
// sTime을 유니코드로 변환하기에 앞서 먼저 그 길이를 알아야 한다.
// int nLen = MultiByteToWideChar(CP_ACP, 0, sTime, lstrlen(sTime), NULL, NULL);
// 얻어낸 길이만큼 메모리를 할당한다.
// bstr = SysAllocStringLen(NULL, nLen);
// 이제 변환을 수행한다.
// MultiByteToWideChar(CP_ACP, 0, sTime, lstrlen(sTime), bstr, nLen);
// 필요없어지면 제거한다.
// SysFreeString(bstr);
// 
// 
// 
// 유니코드에서 ANSI 문자열로의 변환 방법
// newVal이란 BSTR 타입에 있는 유니코드 문자열을 sTime이라는 ANSI 문자열로 변환
// char *sTime;
// int nLen = WideCharToMultiByte(CP_ACP, 0, newVal, -1, sTime, 0, NULL, NULL);
// sTime = malloc(nLen+1);
// WideCharToMultiByte(CP_ACP, 0, newVal, -1, sTime, 128, NULL, NULL);
// 필요없으면 메모리를 제거한다.
// free(sTime);

