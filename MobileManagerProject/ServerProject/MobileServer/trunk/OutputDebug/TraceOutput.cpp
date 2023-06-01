
#include "stdAfx.h"
#include "TraceOutput.h"
#include <time.h>

//--------------------------------------------------------------
//|
//|Window CALLBACK Func
//|
//---------------------------------------------------------------
LRESULT CALLBACK CTraceOutput::WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lparam);
}
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CTraceOutput::CTraceOutput(int type, TCHAR* name, DWORD width,DWORD height):
_target(type),
_window(NULL),
_listbox(NULL),
_console(NULL),
_visible(TRUE)
{
	if(_target == CTraceOutput::NONE) return;
	if(_target & CTraceOutput::FILE) CreateFile(name);
	if(_target & CTraceOutput::WINDOW) CreateTraceWindow(name, width,height);
	if(_target & CTraceOutput::CONSOLE) CreateConsole(name);
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CTraceOutput::~CTraceOutput()
{
	if(_console) FreeConsole();
	if(_filestream.is_open()) _filestream.close();
	if(_window) DestroyWindow(_window);
}
//--------------------------------------------------------------
//|
//|�ܼ�, ������ â SHOW, HIDE
//|
//---------------------------------------------------------------
void CTraceOutput::SetVisible(BOOL visible)
{
	_visible = visible;
	if(_visible == TRUE)
	{
		if(_console) AllocConsole();
		if(_window) ShowWindow( _window, SW_SHOW );
	}
	else
	{
		if(_console) FreeConsole();
		if(_window) ShowWindow( _window, SW_HIDE );
	}
}
//--------------------------------------------------------------
//|
//|SHOW HIDE ���� ����
//|
//---------------------------------------------------------------
BOOL CTraceOutput::GetVisible()
{
	return _visible;
}
//--------------------------------------------------------------
//|
//|���� �����
//|
//---------------------------------------------------------------
void CTraceOutput::Clear()
{
	if(_target & CTraceOutput::CONSOLE)
	{
		//�ܼ� �ʱ�ȭ....
		system("cls");
	}

	if(_target & CTraceOutput::FILE)
	{
		if(_filestream.is_open())
		{
			//���ϳ��� ����
			//���� ���� ������ ����� �ٽ� ������
			_filestream.close();
			CreateFile(NULL,TRUE);
		}
	}
}
//--------------------------------------------------------------
//|
//|�ܼ� ��� ����
//|
//---------------------------------------------------------------
void CTraceOutput::CreateConsole(TCHAR* name)
{
	AllocConsole();
	_console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle(name);
}
//--------------------------------------------------------------
//|
//|������� ����
//|
//---------------------------------------------------------------
void CTraceOutput::CreateFile(TCHAR* name, BOOL is_truc)
{
	//�ؽ�Ʈ���Ϸ� ����� ���� �̸��ڿ� ".txt"�� ���δ�.
	static TCHAR filename[MAX_PATH];
	if(name) _stprintf_s(filename, _countof(filename), _T("%s.txt"), name);

	//�ѱ� ����� �����ϰ� �Ѵ�.
	_tsetlocale(LC_ALL, _T("Korean"));
	_filestream.imbue(std::locale("kor"));

	//�������(���� ���������)�� �ؽ�Ʈ ������ ����.
	if(is_truc) _filestream.open(filename, std::ios_base::trunc);
	// �߰� ���(�������Ͽ� ����)�� ������ ����.
	else _filestream.open(filename, std::ios_base::app);
	if(!_filestream.is_open())
		MessageBox(NULL, _T("traceMessage Class can not open file."), _T("Alert"), MB_OK);
}
//--------------------------------------------------------------
//|
//|������ ��� ����
//|
//---------------------------------------------------------------
void CTraceOutput::CreateTraceWindow(TCHAR* name, DWORD width, DWORD height)
{
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
	// ������ �������� Ŭ���̾�Ʈ ������ ũ�⸦ ���Ѵ�.
	RECT rc;
	GetClientRect( _window, &rc );
	// ����Ʈ�ڽ��� �����Ѵ�.
	_listbox = CreateWindow( _T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL, 
		0, 0, rc.right, rc.bottom, _window, NULL, instance, NULL );
	if(_listbox == NULL)
	{
		MessageBox(NULL, _T("cannot create LISTBOX for TraceWindow"), _T("Alert"), MB_OK);
		return;
	}
	// �����츦 ȭ�鿡 ���̰��Ѵ�.
	ShowWindow( _window, SW_SHOW );
	// ����Ʈ�ڽ��� ȭ�鿡 ���̰��Ѵ�.
	ShowWindow( _listbox, SW_SHOW );
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CTraceOutput::Write(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//Ÿ���� ������ �Լ��� ���� �Ѵ�.
	if(_target == CTraceOutput::NONE) return;
	//�������ڷκ��� ���ڿ� ���
	va_list args;
	TCHAR* buffer;
	//����
	va_start(args, format);
	//�������ڷ� �̷���� ���ڿ��� ũ�⸦ ���Ѵ�.
	INT len = _vsctprintf(format, args) + sizeof(TCHAR); //+1 �ι��� �߰�
	// ���ؼ� ���� ũ�� +1 ��ŭ buffer�� �޸𸮸� �Ҵ��Ѵ�.
	buffer = new TCHAR[len];
	//���ڿ��� ���ۿ� �Է��Ѵ�.
	_vstprintf_s(buffer, len, format, args);
	//��
	va_end(args);
	TCHAR date[16];
	TCHAR time[16];
	TCHAR fullText[1024];
	//��¥ �����͸� ���ڿ��� ���Ѵ�.
	_tstrdate_s(date, _countof(date));
	//�ð� �����͸� ���ڿ��� ���Ѵ�.
	_tstrtime_s(time,_countof(time));
	//��¥ �ð�>���� ������� ���ڿ��� ���Ѵ�.
	_stprintf_s(fullText, _countof(fullText), _T("%s %s> %s"), date, time, buffer);
	//�ֿܼ� ���
	if(_target & CTraceOutput::CONSOLE)
	{
		WriteConsole(_console, fullText, _tcslen(fullText), NULL, NULL);
		WriteConsole(_console, _T("\n"), _tcslen(_T("\n")), NULL, NULL);
	}
	//���Ͽ� ���
	if(_target & CTraceOutput::FILE)
	{
		if(_filestream.is_open())
		{
			_filestream << fullText << std::endl;
		}
	}
	// �����쿡 ���.
	if( _target & CTraceOutput::WINDOW )
	{
		if(_listbox)
		{
			// ����Ʈ �ڽ��� ���ڿ��� �߰��Ѵ�.
			SendMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) fullText );
			// ����Ʈ �ڽ��� ������ ������ ���Ѵ�.
			DWORD n = SendMessage( _listbox, LB_GETCOUNT, 0, 0L ) - 1;
			// ����Ʈ �ڽ��� ������ ������ŭ Ŀ���� �Ʒ��� �̵���Ų��.
			SendMessage( _listbox, LB_SETCURSEL, (WPARAM)n, 0L );
		}
	}
	if(buffer)
	{
		delete [] buffer; buffer=NULL;
	}
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//|�����Լ� ���� ����Ʈ ���
//|
//---------------------------------------------------------------
void CTraceOutput::err_display(char* msg)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,0,NULL);
	Write("[%s] %s",msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
	m_cs.Unlock();
}