
#ifndef __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//

class CIocpServer : public IServer
{
public:
	//생성자
	CIocpServer();
	//소멸자
	~CIocpServer();
//======================================================//
//			일반함수 선언
//======================================================//
//=================스래드 함수===========================//
	static unsigned int __stdcall CompletionThread(void* pComPort);
//=================서버 소켓 생성 함수===================//
	BOOL CreateServerSocket();
//=======================================================//
//			객체 선언
//======================================================//

//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initServer();
//=======================================================//

};
#endif// __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
