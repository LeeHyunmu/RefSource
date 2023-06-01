
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
//|생성자
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
//|소멸자
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
//|콘솔, 윈도우 창 SHOW, HIDE
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
//|SHOW HIDE 상태 리턴
//|
//---------------------------------------------------------------
BOOL CTraceOutput::GetVisible()
{
	return _visible;
}
//--------------------------------------------------------------
//|
//|내용 지우기
//|
//---------------------------------------------------------------
void CTraceOutput::Clear()
{
	if(_target & CTraceOutput::CONSOLE)
	{
		//콘솔 초기화....
		system("cls");
	}

	if(_target & CTraceOutput::FILE)
	{
		if(_filestream.is_open())
		{
			//파일내용 삭제
			//쉽게 기존 파일을 지우고 다시 생성함
			_filestream.close();
			CreateFile(NULL,TRUE);
		}
	}
}
//--------------------------------------------------------------
//|
//|콘솔 출력 생성
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
//|파일출력 생성
//|
//---------------------------------------------------------------
void CTraceOutput::CreateFile(TCHAR* name, BOOL is_truc)
{
	//텍스트파일로 만들기 위해 이름뒤에 ".txt"를 붙인다.
	static TCHAR filename[MAX_PATH];
	if(name) _stprintf_s(filename, _countof(filename), _T("%s.txt"), name);

	//한글 출력이 가능하게 한다.
	_tsetlocale(LC_ALL, _T("Korean"));
	_filestream.imbue(std::locale("kor"));

	//삭제모드(기존 파일지우기)로 텍스트 파일을 연다.
	if(is_truc) _filestream.open(filename, std::ios_base::trunc);
	// 추가 모드(기존파일에 연결)로 파일을 연다.
	else _filestream.open(filename, std::ios_base::app);
	if(!_filestream.is_open())
		MessageBox(NULL, _T("traceMessage Class can not open file."), _T("Alert"), MB_OK);
}
//--------------------------------------------------------------
//|
//|윈도우 출력 생성
//|
//---------------------------------------------------------------
void CTraceOutput::CreateTraceWindow(TCHAR* name, DWORD width, DWORD height)
{
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
	// 생성한 윈도우의 클라이언트 영역의 크기를 구한다.
	RECT rc;
	GetClientRect( _window, &rc );
	// 리스트박스를 생성한다.
	_listbox = CreateWindow( _T("LISTBOX"), _T(""), WS_CHILD | WS_VSCROLL, 
		0, 0, rc.right, rc.bottom, _window, NULL, instance, NULL );
	if(_listbox == NULL)
	{
		MessageBox(NULL, _T("cannot create LISTBOX for TraceWindow"), _T("Alert"), MB_OK);
		return;
	}
	// 윈도우를 화면에 보이게한다.
	ShowWindow( _window, SW_SHOW );
	// 리스트박스를 화면에 보이게한다.
	ShowWindow( _listbox, SW_SHOW );
}
//--------------------------------------------------------------
//|
//|텍스트 출력
//|
//---------------------------------------------------------------
void CTraceOutput::Write(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//타겟이 없으면 함수를 종료 한다.
	if(_target == CTraceOutput::NONE) return;
	//가변인자로부터 문자열 얻기
	va_list args;
	TCHAR* buffer;
	//시작
	va_start(args, format);
	//가변인자로 이루어진 문자열의 크기를 구한다.
	INT len = _vsctprintf(format, args) + sizeof(TCHAR); //+1 널문자 추가
	// 위해서 구한 크기 +1 만큼 buffer에 메모리를 할당한다.
	buffer = new TCHAR[len];
	//문자열을 버퍼에 입력한다.
	_vstprintf_s(buffer, len, format, args);
	//끝
	va_end(args);
	TCHAR date[16];
	TCHAR time[16];
	TCHAR fullText[1024];
	//날짜 데이터를 문자열로 구한다.
	_tstrdate_s(date, _countof(date));
	//시간 데이터를 문자열로 구한다.
	_tstrtime_s(time,_countof(time));
	//날짜 시간>내용 향식으로 문자열을 구한다.
	_stprintf_s(fullText, _countof(fullText), _T("%s %s> %s"), date, time, buffer);
	//콘솔에 출력
	if(_target & CTraceOutput::CONSOLE)
	{
		WriteConsole(_console, fullText, _tcslen(fullText), NULL, NULL);
		WriteConsole(_console, _T("\n"), _tcslen(_T("\n")), NULL, NULL);
	}
	//파일에 출력
	if(_target & CTraceOutput::FILE)
	{
		if(_filestream.is_open())
		{
			_filestream << fullText << std::endl;
		}
	}
	// 윈도우에 출력.
	if( _target & CTraceOutput::WINDOW )
	{
		if(_listbox)
		{
			// 리스트 박스에 문자열을 추가한다.
			SendMessage( _listbox, LB_ADDSTRING, 0, (LPARAM) fullText );
			// 리스트 박스의 컨텐츠 개수를 구한다.
			DWORD n = SendMessage( _listbox, LB_GETCOUNT, 0, 0L ) - 1;
			// 리스트 박스의 컨텐츠 개수만큼 커서를 아래로 이동시킨다.
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
//|소켓함수 오류 에디트 출력
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