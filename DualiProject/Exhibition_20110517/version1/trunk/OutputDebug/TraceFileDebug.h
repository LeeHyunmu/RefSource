
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
public:
	CThread  Outputthread;
	CEvent  OutputEvent;
	CEvent  CloseEvent;
	BOOL bClose;
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
//======================================================//
//			일반함수
//======================================================//
	//출력 스래드 함수
	void OutputThread();
	// 파일 출력을 위해 만들기 함수
	void CreateTraceFile(TCHAR* name, BOOL is_truc=FALSE);
	//스래드 종료함수
	void DebugCloseEvent();
//======================================================//
//			인터페이스 재 정의 함수
//======================================================//
public:
	virtual void initInterfaceObject(IMyObject* _obj);
	//출력 호출 함수
	virtual void Write(TCHAR* format,...);
	//종료
	virtual void Clear();
	//디버그 파일 출력 세팅
	virtual void SetVisible(BOOL visible);
	//디버그 파일 출력 실행 상태
	virtual BOOL GetVisible();
	//스래드 출력함수
	virtual void OutputString();
	//메모리 해제 
	virtual void Release();
//=======================================================//
//			객체 선언
//======================================================//
private:
	BOOL _visible;
	//파일로 출력
	_tofstream _filestream;
//=======================================================//

};
#endif// __TRACEFILEDEBUG_74F736E25AB94b5dA789E4730F58F2A1_H__
