
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
	BOOL fnBind();
	BOOL fnListen();
	//윈속 종료
	void Cleanup();
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			변수 선언
//======================================================//
public:
	//소켓 정보 저장
	vecSocketInfo  SockInfo;
	// 서버 소켓
	SOCKET      m_hSocket;
	//로컬 IP
	CString		Local_IP;
private:
	u_short     m_nPort;
	//
	static BOOL bWinsockinitialize;
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();

	virtual BOOL initServer(IMyObject* _obj);
	//초기화
	virtual BOOL WinSockInitialize();
	//서버 Listen
	virtual BOOL ServerListen(u_short _nport);
	//소켓을 닫는다
	virtual void fnCloseSocket();
	//받는 함수
	virtual DWORD fnWSARecv(SOCKET hSock, DWORD dwFlags = 0);
	//보내는 함수
	virtual DWORD fnWSASend(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0);
	// 전체 보내기
	virtual DWORD fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0);

	//소켓 정보 추가
	virtual BOOL fnAddSocketInfo(SOCKET hSock, SOCKET hlinkSock=0);
	//소켓정보 삭제
	virtual BOOL fnRemoveSocketInfo(SOCKET hSock);
	//소켓정보 가져오기(소켓 으로)
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock);
	//클라이언트 접속
	virtual SOCKET fnAccept();
	//비동기 서버 소켓 접속
	virtual HANDLE CompletionPortServerSocket(HANDLE hIocp);
	//비동기 클라이언트용 소켓 만들기 함수
	virtual SOCKET CompletionPortClientSocket(HANDLE hIocp, SOCKET hSock, CString _ip, u_short _port);
//=======================================================//

};
#endif// __IOCPSERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
