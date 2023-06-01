
#ifndef __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//
//======================================================//
//			
//=======================================================//
class CTraceWinDebug : public IOutput
{
	static IOutput* debug;
	static HINSTANCE instance;
	static TCHAR  clipString[BUFFSIZE_4096];
	static BOOL bListClick;
	typedef deque<CString, allocator<CString> > CHARDEQUE;
	typedef queue<CString,CHARDEQUE> CHARQUEUE;
	CHARQUEUE qData;
public:
	//������
	CTraceWinDebug(TCHAR* name, DWORD width = 600, DWORD height = 400);
	//�Ҹ���
	~CTraceWinDebug();
//=================������ �Լ�===========================//
	static DWORD WINAPI OutputProcess(LPVOID lpArg);
//=================���μ��� �ݹ� �Լ�===========================//
	static LRESULT CALLBACK WndProc(HWND hWnd, DWORD uMsg, WPARAM wParam, LPARAM lparam);
//=======================================================//
//			���� ����
//======================================================//
//======================================================//
//			�̱��� �Լ�
//======================================================//
	static IOutput* GetDebug();
public:
	CThread  Outputthread;
	CEvent  OutputEvent;
	CEvent  CloseWinDebugEvent;
	BOOL bClose;
//======================================================//
//			�Ϲ��Լ�
//======================================================//
	//
	void OutputThread();
//======================================================//
//			�������̽� �� ���� �Լ�
//======================================================//
public:
	//��� ȣ�� �Լ�
	virtual void Write(TCHAR* format,...);
	//����
	virtual void Clear();
	//�����â ���̰� �Ⱥ��̰� ����
	virtual void SetVisible(BOOL visible);
	//����� â�� ���̴°�?
	virtual BOOL GetVisible();
	//������ ����Լ�
	virtual void OutputString();
	//�޸� ���� 
	virtual void Release();
//=======================================================//
//			��ü ����
//======================================================//
private:
	DWORD _width;
	HWND _window;
	BOOL _visible;
	HICON hwinicon;
	//�����쿡�� ����Ʈ�ڽ��� ���
	HWND _listbox;
//=======================================================//

};
#endif// __TRACEWINDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
