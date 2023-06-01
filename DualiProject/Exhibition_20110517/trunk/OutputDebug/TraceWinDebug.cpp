
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
	_listbox = CreateWindow( _T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL, 0, 0, rc.right, rc.bottom, _window, NULL, instance, NULL );
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
	DebugCloseEvent();
	if(hwinicon)  DestroyIcon(hwinicon);
	hwinicon=NULL;
	if(_listbox) DestroyWindow(_listbox);
	_listbox=NULL;
// 	if(_window) DestroyWindow(_window);
// 	_window=NULL;
}
void CTraceWinDebug::DebugCloseEvent()
{
	while(qData.empty()==false)
		qData.pop();
	CloseEvent.Create();
	if(OutputEvent.IsOpen())
	{
		bClose=TRUE;
		OutputEvent.SetEvent();
	}
	CloseEvent.WaitForEvent();
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CTraceWinDebug::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ������ ����
	//========================================
	OutputThread();
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
	Outputthread.Create(OutputProcess, (LPVOID)obj);
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
		{
			break;
		}
		if(_window&&_listbox)
		{
			while (!qData.empty())
			{
				DWORD dwresult;
				BOOL b=FALSE;
				TCHAR* pdt = qData.front().GetBuffer(0);
				// ����Ʈ �ڽ��� ���ڿ��� �߰��Ѵ�.
				if(SUCCEEDED(SendMessageTimeout( _listbox, LB_ADDSTRING, 0, (LPARAM) pdt,SMTO_NORMAL,500,&dwresult)))
				{
					qData.pop();
					// ����Ʈ �ڽ��� ������ ������ ���Ѵ�.
					if(SUCCEEDED(SendMessageTimeout( _listbox, LB_GETCOUNT, 0, 0L ,SMTO_NORMAL,500,&dwresult)))
					{
						DWORD n = dwresult-1;
						// ����Ʈ �ڽ��� ������ ������ŭ Ŀ���� �Ʒ��� �̵���Ų��.
						if(SUCCEEDED(SendMessageTimeout( _listbox, LB_SETCURSEL, (WPARAM)n, 0L,SMTO_NORMAL,500,&dwresult )))
						{
							b=TRUE;
						}
					}
				}
				if(b==FALSE)
					break;
			}
		}
		else
			break;
	}
	CloseEvent.SetEvent();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CTraceWinDebug::OutputProcess(LPVOID lpArg)
{
	IOutput *debug = ((IMyObject *)lpArg)->GetOutputObject();
	if(debug)
	{
		debug->OutputString();
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
	if(GetVisible())
	{
		CStaticDataInitCriticalSectionLock  m_cs;
		m_cs.Lock();
		//============================================
		// �������ڷκ��� ���ڿ� ���
		//=============================================
		va_list args;
		TCHAR buffer[512]={0,};
		va_start(args, format);
		_vstprintf_s(buffer, sizeof(buffer)/sizeof(buffer[0]), format, args);
		va_end(args);
		WCHAR fullText[1024];
		//============================================
		// ��¥ �ð� ���
		//=============================================
		COleDateTime oletime;
		oletime = COleDateTime::GetCurrentTime();
		CString strtime;
		strtime.Format(_T("%04d%02d%02d_%02d:%02d:%02d"),oletime.GetYear(),oletime.GetMonth(), oletime.GetDay()
			,oletime.GetHour(), oletime.GetMinute(), oletime.GetSecond());
		_stprintf_s(fullText, sizeof(fullText)/sizeof(fullText[0]), _T("%s>%s"), strtime, buffer);
		//============================================
		// ���
		//=============================================
		DWORD nLen = _tcslen(fullText);
		DWORD textpoint = 0;
		if(_listbox)
		{
			while(nLen != textpoint)
			{
				WCHAR WriteData[DEBUGWINDOW_MAXLEN+1];
				memset(WriteData, 0x00, sizeof(WriteData));
				int offset = nLen-textpoint;
				if(offset > DEBUGWINDOW_MAXLEN)
				{
					memcpy(WriteData,fullText+textpoint,DEBUGWINDOW_MAXLEN*sizeof(WCHAR)); textpoint+=DEBUGWINDOW_MAXLEN;
					WriteData[DEBUGWINDOW_MAXLEN]=_T('\0');
				}
				else
				{
					memcpy(WriteData,fullText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
					WriteData[offset]=_T('\0');
				}
				//����
				qData.push(WriteData);
			}
		}
		if(qData.size())
		{
			OutputEvent.SetEvent();
		}
		m_cs.Unlock();
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
	PostMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) 0L);
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
