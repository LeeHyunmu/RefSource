
#include "stdAfx.h"
#include "TraceWinDebug.h"
#include <strsafe.h>

#define  DEBUGWINDOW_MAXLEN 68
#define  ID_MENUCOPY 301
#define  LIST_1 333
#define  DEBUGTHREAD_LOOPTIME 200
IOutput* CTraceWinDebug::debug=NULL;
//--------------------------------------------------------------
//|
//|싱글톤 디버그
//|
//---------------------------------------------------------------
IOutput* CTraceWinDebug::GetDebug()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|Window CALLBACK Func
//|
//---------------------------------------------------------------
HINSTANCE CTraceWinDebug::instance=NULL;
TCHAR CTraceWinDebug::clipString[BUFFSIZE_4096]={0,};
BOOL CTraceWinDebug::bListClick=FALSE;
LRESULT CALLBACK CTraceWinDebug::WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam)
{
	HDC         hdc ; 
	PAINTSTRUCT ps ; 
	POINT point; 
	int wmId, wmEvent;
	static int iSelectList1;
	switch (uMsg) 
	{ 
	case WM_CREATE : 
		return 0 ; 
	case WM_PAINT : 
		hdc = BeginPaint (hWnd, &ps) ; 
		EndPaint (hWnd, &ps) ; 
		return 0 ; 
	case WM_RBUTTONDOWN: 
		{
			//마우스 좌표를 얻는다. 
			point.x= LOWORD(lparam); 
			point.y= HIWORD(lparam); 
			//좌표를 클라이언트 좌표로 전환 
			ClientToScreen(hWnd,&point); 
		}
		break; 
	case WM_CONTEXTMENU: 
		{
			//현재 마우스 위치를 알고 
			GetCursorPos(&point); 
			//Floating 메뉴 출력 
			CMenu menuPopup;
			menuPopup.CreatePopupMenu();
			menuPopup.AppendMenu(MF_STRING, ID_MENUCOPY, _T("복사"));
			menuPopup.AppendMenu(MF_SEPARATOR);
			menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON, point.x, point.y, hWnd);
		}
		return 0; 
	case WM_COMMAND: 
		{
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			switch (wmId)
			{
			case ID_MENUCOPY:
				{
					HGLOBAL hmem;
					TCHAR* ptr;
					int datalen = _tcslen(clipString);
					if(datalen>0)
					{
						hmem=GlobalAlloc(GHND, _tcslen(clipString)+1);
						ptr=(TCHAR*)GlobalLock(hmem);
						memcpy(ptr, clipString, _tcslen(clipString)+1);
						GlobalUnlock(hmem);
						if(OpenClipboard(hWnd))
						{
							EmptyClipboard();
							SetClipboardData(CF_TEXT, hmem);
							CloseClipboard();
						}else
						{
							GlobalFree(hmem);
						}
					}
				}
				break;
			case LIST_1:
				{
					bListClick=TRUE;
					switch (wmEvent)
					{
					case LBN_SELCHANGE:
						{
							iSelectList1=SendMessage(GetDlgItem(hWnd, LIST_1),LB_GETCURSEL,0,0);
							memset(clipString, NULL, MAX_PATH);
							SendMessage(GetDlgItem(hWnd, LIST_1), LB_GETTEXT, iSelectList1,(LPARAM) clipString);
						}
						break;
					case LBN_DBLCLK:
						{
							bListClick=FALSE;
						}break;
					}
				}break;
			}
		}
		break; 
	case WM_DESTROY : 
		//PostQuitMessage (0) ; 
		return 0 ; 

	} 
	return DefWindowProc(hWnd, uMsg, wParam, lparam);
}
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CTraceWinDebug::CTraceWinDebug(TCHAR* name, DWORD width, DWORD height)
{
	debug=this;
	// 스래드 생성
	OutputThread();
	bClose=FALSE;
	_width = width;
	hwinicon = AtlLoadIconImage(IDR_MAINFRAME);
	//DestroyIcon();
	//SHGetFileInfo()
	//==========================================
	//윈도우 생성
	// 인스턴스를 얻는 방법에 대해서 설명한다.
	//============================================
	instance = GetModuleHandle( NULL );
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
	_listbox = CreateWindow(_T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL | LBS_NOTIFY, 0, 0, rc.right, rc.bottom, _window, (HMENU)LIST_1, instance, NULL );
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
	bDebugRunning=TRUE;
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CTraceWinDebug::~CTraceWinDebug()
{
}
//--------------------------------------------------------------
//|
//|메모리 해제
//|
//---------------------------------------------------------------
void CTraceWinDebug::Release()
{
	bDebugRunning=FALSE;
	OutputEvent.SetEvent();
	Outputthread.WaitForThread();
	//Outputthread.Release();
	if(hwinicon)  DestroyIcon(hwinicon);
	hwinicon=NULL;
	if(_listbox) DestroyWindow(_listbox);
	_listbox=NULL;
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
	if(OutputEvent.WaitForEvent())
	{
		if(_window&&_listbox)
		{
			while (!qData.empty())
			{
				DWORD dwresult;
				TCHAR* pdt = qData.front().GetBuffer(0);
				// 리스트 박스에 문자열을 추가한다.
				int pdtlen = ::SendMessage(_listbox, LB_ADDSTRING, 0, (LPARAM)pdt);
				//if(SUCCEEDED(SendMessageTimeout( _listbox, LB_ADDSTRING, 0, (LPARAM) pdt,SMTO_ABORTIFHUNG,500,&dwresult)))
				//{
					// 리스트 박스의 컨텐츠 개수만큼 커서를 아래로 이동시킨다.
					if(bListClick==FALSE)
					{
						// 리스트 박스의 컨텐츠 개수를 구한다.
						if(SUCCEEDED(SendMessageTimeout( _listbox, LB_GETCOUNT, 0, 0L ,SMTO_ABORTIFHUNG,500,&dwresult)))
						{
							DWORD n = dwresult-1;
							SendMessageTimeout( _listbox, LB_SETCURSEL, (WPARAM)n, 0L,SMTO_ABORTIFHUNG,500,&dwresult );
						}
					}
					qData.pop();
				//}
			}
		}
	}
}
BOOL CTraceWinDebug::GetbRunning()
{
	return bDebugRunning;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CTraceWinDebug::OutputProcess(LPVOID lpArg)
{
	CTraceWinDebug *debug = (CTraceWinDebug *)lpArg;
	while (debug->GetbRunning() && debug->Outputthread.WaitForThread(DEBUGTHREAD_LOOPTIME)==FALSE)
	{
		if(debug)
		{
			debug->OutputString();
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|텍스트 출력
//|
//---------------------------------------------------------------
void CTraceWinDebug::Write(TCHAR* format,...)
{
	if(GetVisible()&&GetbRunning())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// 가변인자로부터 문자열 얻기
		//=============================================
		va_list args;
		TCHAR buffer[256]={0,};
		va_start(args, format);
		//int len = vswprintf(NULL, format, args) + 1; 
		//buffer = new TCHAR[len];
		_vstprintf_s(buffer, 255, format, args);
		va_end(args);
		TCHAR fullText[1024];
		//============================================
		// 날짜 시간 얻기
		//=============================================
		//COleDateTime oletime;
		//oletime = COleDateTime::GetCurrentTime();
		//CString strtime;
		//strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
		//	,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		//_stprintf_s(fullText, _T("%s> %s"), strtime, buffer);
		_stprintf_s(fullText, _T(">> %s"), buffer);
		//============================================
		// 출력
		//=============================================
		DWORD textlen = _tcslen(fullText);
		int nLen = MultiByteToWideChar(CP_ACP, 0, fullText, textlen, NULL, NULL);
		WCHAR _unText[MAX_PATH]={0,};
		MultiByteToWideChar(CP_ACP, 0, fullText, textlen, _unText, nLen);
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
					memcpy(WriteData,_unText+textpoint,DEBUGWINDOW_MAXLEN*sizeof(WCHAR)); textpoint+=DEBUGWINDOW_MAXLEN;
					//StringCbCopyW(WriteData, DEBUGWINDOW_MAXLEN,fullText+textpoint);textpoint+=DEBUGWINDOW_MAXLEN;
				}
				else
				{
					memcpy(WriteData,_unText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
					//StringCbCopyW(WriteData, offset,fullText+textpoint); textpoint+=offset;
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

