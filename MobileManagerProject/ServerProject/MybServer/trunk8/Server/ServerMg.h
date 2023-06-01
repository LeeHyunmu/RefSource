
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
	//소켓 정보 출력
	void Sockinfodisplay(SOCKET _sock);
//=================스래드 함수===========================//
	static DWORD WINAPI WorkerThread(LPVOID lpArg);
	static DWORD WINAPI ProcessServer(LPVOID lpArg);

	static DWORD WINAPI WorkerClntThread(LPVOID lpArg);
	static DWORD WINAPI ProcessClntServer(LPVOID lpArg);
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
	DWORD dwServerThreadCount;
	BOOL  bEndServer;
private:
	BOOL bServerStart;
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	//서버메니저 초기화
	virtual BOOL initServerMg(IMyObject* _obj);
	//컴플리션 포트 핸들을 가져 온다
	virtual HANDLE GetCompletionPort();
	virtual HANDLE GetClntCompletionPort();
	//서버를 시작한다
	virtual BOOL ServerStart(u_short nPort);
	//서버를 중지한다(아직 안쓰고 있음)
	virtual void ServerStop();
	//비동기 데이터 recv용 클라이언트 소켓 생성 등록
	virtual SOCKET MakeCompetionPortClient(SOCKET hSock);
	//비동기 서버 대기 함수
	virtual SOCKET MakeCompetionPortServer();
	//Iocp Server 보낸 데이터 이후 처리 함수
	virtual void IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag);
	//Iocp Server 받은 데이터 처리 함수
	virtual void IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag);
	//Iocp Client 보낸 데이터 이후 처리 함수
	virtual void IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag);
	//Iocp Client 받은 데이터 처리 함수
	virtual void IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag);
	//서버 종료
	virtual BOOL GetEndServer();
	//소켓 연결이 끊겼을 때 처리	
	virtual BOOL DisConnectHandling(SOCKETINFO* _info, DWORD flag);
	//IOCP 멤버를 넘긴다
	virtual IServer* GetIocpServer();
	//데이터 베이스 메니저를 넘긴다
	virtual IDatabasebMg* GetDatabasebMg();
	//Db 데이터 전송 함수
	virtual BOOL DbDataSend();
//=======================================================//

};
#endif// __SERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
