
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
//|������
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
	//������ ����
	// �ν��Ͻ��� ��� ����� ���ؼ� �����Ѵ�.
	//============================================
	HINSTANCE instance = GetModuleHandle( NULL );
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
	_listbox = CreateWindow( _T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL, 
		0, 0, rc.right, rc.bottom, _window, NULL, instance, NULL );
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
	// ������ ����
	OutputThread();
}
//--------------------------------------------------------------
//|
//|�Ҹ���
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
	while(OutputEvent.WaitForEvent())
	{
		if(bClose)
			break;
		while (!qData.empty())
		{
			TCHAR* pdt = qData.front().GetBuffer(0);
			// ����Ʈ �ڽ��� ���ڿ��� �߰��Ѵ�.
			SendMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) pdt);
			// ����Ʈ �ڽ��� ������ ������ ���Ѵ�.
			DWORD n = SendMessage( _listbox, LB_GETCOUNT, 0, 0L ) - 1;
			// ����Ʈ �ڽ��� ������ ������ŭ Ŀ���� �Ʒ��� �̵���Ų��.
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
//|�ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CTraceWinDebug::Write(TCHAR* format,...)
{
	if(GetVisible())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// �������ڷκ��� ���ڿ� ���
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
		// ��¥ �ð� ���
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf(fullText, _T("%s> %s"), strtime, buffer);
		//============================================
		// ���
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