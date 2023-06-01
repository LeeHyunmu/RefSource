
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
//|�̱��� �����
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
			//���콺 ��ǥ�� ��´�. 
			point.x= LOWORD(lparam); 
			point.y= HIWORD(lparam); 
			//��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ 
			ClientToScreen(hWnd,&point); 
		}
		break; 
	case WM_CONTEXTMENU: 
		{
			//���� ���콺 ��ġ�� �˰� 
			GetCursorPos(&point); 
			//Floating �޴� ��� 
			CMenu menuPopup;
			menuPopup.CreatePopupMenu();
			menuPopup.AppendMenu(MF_STRING, ID_MENUCOPY, _T("����"));
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
//|������
//|
//---------------------------------------------------------------
CTraceWinDebug::CTraceWinDebug(TCHAR* name, DWORD width, DWORD height)
{
	debug=this;
	// ������ ����
	OutputThread();
	bClose=FALSE;
	_width = width;
	hwinicon = AtlLoadIconImage(IDR_MAINFRAME);
	//DestroyIcon();
	//SHGetFileInfo()
	//==========================================
	//������ ����
	// �ν��Ͻ��� ��� ����� ���ؼ� �����Ѵ�.
	//============================================
	instance = GetModuleHandle( NULL );
	WNDCLASS wc;
	// ������ Ŭ���� �ۼ�.
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
	// ������ Ŭ���� ���.
	RegisterClass(&wc);
	//============================================
	// �����츦 �����Ѵ�.
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
	// ������ �������� Ŭ���̾�Ʈ ������ ũ�⸦ ���Ѵ�.
	//=============================================
	RECT rc;
	GetClientRect( _window, &rc );
	//============================================
	// ����Ʈ�ڽ��� �����Ѵ�
	//=============================================
	_listbox = CreateWindow(_T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL | LBS_NOTIFY, 0, 0, rc.right, rc.bottom, _window, (HMENU)LIST_1, instance, NULL );
	if(_listbox == NULL)
	{
		MessageBox(NULL, _T("cannot create LISTBOX for TraceWindow"), _T("Alert"), MB_OK);
		return;
	}
	// �����츦 ȭ�鿡 ���̰��Ѵ�.
	SetVisible(TRUE);
	//ShowWindow( _window, SW_SHOW );
	// ����Ʈ�ڽ��� ȭ�鿡 ���̰��Ѵ�.
	ShowWindow( _listbox, SW_SHOW );
	bDebugRunning=TRUE;
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CTraceWinDebug::~CTraceWinDebug()
{
}
//--------------------------------------------------------------
//|
//|�޸� ����
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
//|������ ����
//|
//---------------------------------------------------------------
void CTraceWinDebug::OutputThread()
{
	////////////////////////////////////////////////////////////////
	//���� ������ ����
	OutputEvent.Create();
	Outputthread.Create(OutputProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|ī�� ���� �� ó�� �Լ�
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
				// ����Ʈ �ڽ��� ���ڿ��� �߰��Ѵ�.
				int pdtlen = ::SendMessage(_listbox, LB_ADDSTRING, 0, (LPARAM)pdt);
				//if(SUCCEEDED(SendMessageTimeout( _listbox, LB_ADDSTRING, 0, (LPARAM) pdt,SMTO_ABORTIFHUNG,500,&dwresult)))
				//{
					// ����Ʈ �ڽ��� ������ ������ŭ Ŀ���� �Ʒ��� �̵���Ų��.
					if(bListClick==FALSE)
					{
						// ����Ʈ �ڽ��� ������ ������ ���Ѵ�.
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
//|�ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CTraceWinDebug::Write(TCHAR* format,...)
{
	if(GetVisible()&&GetbRunning())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// �������ڷκ��� ���ڿ� ���
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
		// ��¥ �ð� ���
		//=============================================
		//COleDateTime oletime;
		//oletime = COleDateTime::GetCurrentTime();
		//CString strtime;
		//strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
		//	,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		//_stprintf_s(fullText, _T("%s> %s"), strtime, buffer);
		_stprintf_s(fullText, _T(">> %s"), buffer);
		//============================================
		// ���
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
				//����
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
//| ���� ����
//|
//---------------------------------------------------------------
void CTraceWinDebug::Clear()
{
	if(_visible == FALSE || _window == NULL) return;
	// ����Ʈ �ڽ��� ���ڿ��� �߰��Ѵ�.
	SendMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) 0L);
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

// sTime�̶� ANSI ���ڿ��� bstr�̶� �̸��� �����ڵ�(BSTR Ÿ��) ������ ��ȯ
// char sTime[] = "�����ڵ� ��ȯ ����";
// BSTR bstr;
// sTime�� �����ڵ�� ��ȯ�ϱ⿡ �ռ� ���� �� ���̸� �˾ƾ� �Ѵ�.
// int nLen = MultiByteToWideChar(CP_ACP, 0, sTime, lstrlen(sTime), NULL, NULL);
// �� ���̸�ŭ �޸𸮸� �Ҵ��Ѵ�.
// bstr = SysAllocStringLen(NULL, nLen);
// ���� ��ȯ�� �����Ѵ�.
// MultiByteToWideChar(CP_ACP, 0, sTime, lstrlen(sTime), bstr, nLen);
// �ʿ�������� �����Ѵ�.
// SysFreeString(bstr);
// 
// 
// 
// �����ڵ忡�� ANSI ���ڿ����� ��ȯ ���
// newVal�̶� BSTR Ÿ�Կ� �ִ� �����ڵ� ���ڿ��� sTime�̶�� ANSI ���ڿ��� ��ȯ
// char *sTime;
// int nLen = WideCharToMultiByte(CP_ACP, 0, newVal, -1, sTime, 0, NULL, NULL);
// sTime = malloc(nLen+1);
// WideCharToMultiByte(CP_ACP, 0, newVal, -1, sTime, 128, NULL, NULL);
// �ʿ������ �޸𸮸� �����Ѵ�.
// free(sTime);

