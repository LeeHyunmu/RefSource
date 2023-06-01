
#ifndef __DUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_P2PBUF 64
class CDualiP2pMg : public IDualiP2pMg
{
public:
	//생성자
	CDualiP2pMg();
	//소멸자
	~CDualiP2pMg();
//=================스래드 함수===========================//
	static DWORD WINAPI P2pProcess(LPVOID lpArg);
//=======================================================//
//			변수 선언
//======================================================//
public:
	CThread  p2pthread;
	CEvent  p2pEvent;
//======================================================//
//			인터페이스 객체 선언
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IDualiCardMg* dualicard;
//======================================================//
	//=======================================================//
	//			멤버변수 선언
	//======================================================//
	//Read Event
	HANDLE hReadEvent;
	//Wirte Event
	HANDLE hWriteEvent;
	//송신 데이타
	CString SendData;
	//수신 데이타
	CString RecvData;
	//보내는 사이즈
	DWORD dwSendflage;
	//받는 사이즈
	DWORD dwRecvSize;
	//종료 플래그
	DWORD dwLoop;
	//송수신이 잘 되었는가를 판단한다
	BOOL bOk;

//======================================================//
//			일반 함수
//======================================================//
	void P2pObject();
//=======================================================//
//======================================================//
//			변수
//======================================================//

//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void initInterfaceObject(IMyObject* _obj);
	virtual void Release();
	//스래드에서 호출되는 함수
	virtual void P2pFunc();
	//데이터 전송
	virtual BOOL SendDatafn(DWORD _dwSendflage, CString data, DWORD dwTimeout = INFINITE);
//=======================================================//
};
#endif// __DUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__
