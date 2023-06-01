
#include "stdAfx.h"
#include "TraceWinDebug.h"

#define  DEBUG_MAXLEN 53
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
	_listbox = CreateWindow( _T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL, 
		0, 0, rc.right, rc.bottom, _window, NULL, instance, NULL );
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
	if(OutputEvent.IsSignalled())
	{
		bClose=TRUE;
		OutputEvent.SetEvent();
	}
	if(_window) DestroyWindow(_window);
	if(hwinicon)  DestroyIcon(hwinicon);
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
			break;
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
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CTraceWinDebug::OutputProcess(LPVOID lpArg)
{
	CTraceWinDebug *debug = (CTraceWinDebug *)lpArg;
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
		TCHAR buffer[256];
		va_start(args, format);
		//int len = vswprintf(NULL, format, args) + 1; 
		//buffer = new TCHAR[len];
		vswprintf(buffer, format, args);
		va_end(args);
		TCHAR fullText[1024];
		//============================================
		// 날짜 시간 얻기
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf(fullText, _T("%s> %s"), strtime, buffer);
		//============================================
		// 출력
		//=============================================
		DWORD textlen = _tcslen(fullText);
		DWORD textpoint = 0;
		if(_listbox)
		{
			while(textlen != textpoint)
			{
				TCHAR WriteData[DEBUG_MAXLEN+1];
				memset(WriteData, 0, sizeof(WriteData));
				int offset = textlen-textpoint;
				if(offset > DEBUG_MAXLEN)
				{
					memcpy(WriteData,fullText+textpoint,sizeof(TCHAR)*DEBUG_MAXLEN); textpoint+=DEBUG_MAXLEN;
				}
				else
				{
					memcpy(WriteData,fullText+textpoint,sizeof(TCHAR)*offset); textpoint+=offset;
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