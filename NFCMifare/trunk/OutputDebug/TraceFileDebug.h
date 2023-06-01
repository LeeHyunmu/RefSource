
#ifndef __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
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
	//생성자
	CTraceFileDebug(TCHAR* name);
	//소멸자
	~CTraceFileDebug();
//=================스래드 함수===========================//
	static DWORD WINAPI OutputProcess(LPVOID lpArg);
//=======================================================//
//			변수 선언
//======================================================//
//======================================================//
//			싱글톤 함수
//======================================================//
	static IOutput* GetDebug();
public:
	CThread  Outputthread;
	CEvent  OutputEvent;
	CEvent  CloseWinDebugEvent;
//======================================================//
//			일반함수
//======================================================//
	//출력 스래드 함수
	void OutputThread();
	// 파일 출력을 위해 만들기 함수
	void SetFileName(TCHAR* name);
//======================================================//
//			인터페이스 재 정의 함수
//======================================================//
public:
	//출력 호출 함수
	virtual void Write(TCHAR* format,...);
	//종료
	virtual void Clear();
	//스래드 출력함수
	virtual void OutputString();
	//디버그창 보이고 안보이고 설정
	virtual void SetVisible(BOOL visible);
	//디버그 창이 보이는가?
	virtual BOOL GetVisible();
	//메모리 해제 
	virtual void Release();
//=======================================================//
//			객체 선언
//======================================================//
private:
	TCHAR filename[MAX_PATH];
	static CString crlf;
	HANDLE hFile;
//=======================================================//

};
#endif// __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
