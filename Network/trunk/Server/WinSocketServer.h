
#ifndef __WINSOCKETSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __WINSOCKETSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWinSocketServer : public IWinSocket
{
	struct RecvThreadInfo 
	{
		SOCKET sock;
		IMyObject* obj;
		CThread  RecvThread;
	};
	typedef vector<RecvThreadInfo*> vecThreadInfo;
public:
	//생성자
	CWinSocketServer();
	//소멸자
	~CWinSocketServer();
	//=================스래드 함수===========================//
	static DWORD WINAPI AcceptProcess(LPVOID lpArg);
	static DWORD WINAPI RecvProcess(LPVOID lpArg);
	//=======================================================//
	//			변수 선언
	//======================================================//
	CThread  AcceptThread;
	BOOL bClose;
	SOCKET listen_sock;

	vecThreadInfo RecvThread;
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//======================================================//
	//			인터페이스 포인터 
	//======================================================//

	//======================================================//
	//			일반 함수
	//======================================================//
	//윈속 초기화
	void initWinSocket();
	//도메인 네임으로 IP addr 가져오기
	CString GetIPAddr(TCHAR* name, IN_ADDR* addr);
	//IP addr로 도메인 네임 가져오기
	CString GetDomainName(IN_ADDR addr, TCHAR* name);
	//클라이언트 리시브 스래드를 생성한다
	void MakeRecvThread(SOCKET sock);
	//소겟 정보 vector 삭제
	void DeleteRecvThread(SOCKET sock);
	//=======================================================//
	//======================================================//
	//			변수
	//======================================================//
	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void Release();
	//Accept  함수
	virtual void Acceptfunc();
	//서버를 만든다
	virtual BOOL MakeServer(u_short _port, CString _ip);
	//데이터 리시브 대기
	virtual void Recvfunc(SOCKET client_sock);
	//=======================================================//
};
#endif// __WINSOCKETSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
