
#ifndef __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CServerMg : public IServerMg
{
public:
	//생성자
	CServerMg();
	//소멸자
	~CServerMg();
//======================================================//
//			일반함수 선언
//======================================================//

//=================스래드 함수===========================//
	static DWORD WINAPI WorkerThread(LPVOID lpArg);
	static DWORD WINAPI ProcessServer(LPVOID lpArg);
//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IServer* iocp;
//=======================================================//
//			변수 선언
//======================================================//
public:
	HANDLE   m_hIocp;
	HANDLE  mthread;
	vechThread workerthread;
private:
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initServerMg(IMyObject* _obj);
	virtual HANDLE GetCompletionPort();
	virtual BOOL ServerStart(u_short nPort);
	virtual void ServerStop();
//=======================================================//

};
#endif// __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
