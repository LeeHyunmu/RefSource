
#ifndef __TRACEOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __TRACEOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//
typedef std::basic_ofstream<TCHAR> _tofstream;
//======================================================//
//			
//=======================================================//
class CTraceOutput : public IOutput
{
public:
	//������
	CTraceOutput(int type, TCHAR* name, DWORD width=400,DWORD height=600);
	//�Ҹ���
	~CTraceOutput();
public:
	enum TARGET{NONE=0x00, FILE = 0x01, CONSOLE = 0x02, WINDOW = 0x04};
//======================================================//
//			�������̽� ������
//======================================================//
public:
	void Write(TCHAR* format,...);
	void err_display(char* msg);
	void ErrorHandler(TCHAR* place,_com_error &e);
	void Clear();
	void SetVisible(BOOL visible);
	BOOL GetVisible();
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam);
	void CreateFile(TCHAR* name, BOOL is_truc = false);
	void CreateTraceWindow(TCHAR* name, DWORD width, DWORD height);
	void CreateConsole(TCHAR* name);
public:
//=======================================================//
//			��ü ����
//======================================================//
private:
	DWORD _target;
	BOOL _visible;
	//�����쿡�� ����Ʈ�ڽ��� ���
	HWND _window;
	HWND _listbox;
	//�ܼ�â�� �ٿ�� ���� �ڵ�
	HANDLE _console;
	//���Ϸ� ���
	_tofstream _filestream;
//=======================================================//

};
#endif// __TRACEOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__
