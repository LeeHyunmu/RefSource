
#ifndef __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//
#include <fstream>
//=======================================================//
typedef std::basic_ofstream<TCHAR> _tofstream;
//=======================================================//
//======================================================//
//			
//=======================================================//
class CTraceFileDebug : public IOutput
{
	static IOutput* debug;
	typedef deque<CString, allocator<CString> > CHARDEQUE;
	typedef queue<CString,CHARDEQUE> CHARQUEUE;
	CHARQUEUE qData;
public:
	//������
	CTraceFileDebug(TCHAR* name);
	//�Ҹ���
	~CTraceFileDebug();
//=================������ �Լ�===========================//
	static DWORD WINAPI OutputProcess(LPVOID lpArg);
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
//======================================================//
//			�Ϲ��Լ�
//======================================================//
	//��� ������ �Լ�
	void OutputThread();
	// ���� ����� ���� ����� �Լ�
	void SetFileName(TCHAR* name);
//======================================================//
//			�������̽� �� ���� �Լ�
//======================================================//
public:
	//��� ȣ�� �Լ�
	virtual void Write(TCHAR* format,...);
	//����
	virtual void Clear();
	//������ ����Լ�
	virtual void OutputString();
	//�����â ���̰� �Ⱥ��̰� ����
	virtual void SetVisible(BOOL visible);
	//����� â�� ���̴°�?
	virtual BOOL GetVisible();
	//�޸� ���� 
	virtual void Release();
//=======================================================//
//			��ü ����
//======================================================//
private:
	TCHAR filename[MAX_PATH];
	static CString crlf;
	HANDLE hFile;
//=======================================================//

};
#endif// __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
