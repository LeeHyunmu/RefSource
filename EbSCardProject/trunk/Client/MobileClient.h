
#ifndef __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//	
//======================================================//
class CMobileClient : public CThreadImpl<CMobileClient>, public IClient
{

public:
	//생성자
	CMobileClient();
	//소멸자
	~CMobileClient();
//======================================================//
//			일반함수 선언
//======================================================//
	//스래드 런함수
	DWORD Run();
	//Recv 사용자 정의 함수
	int recvn(SOCKET s, CHAR *buf, int len, int flags, BOOL bfull=FALSE);
	//스래드 에서 호출 함수
	BOOL ClientMain();
	//스래드 종료
	void ThreadClose();
	//Recv Send 완료 이벤트 함수
	BOOL WaitForReadEvent(DWORD dwTimeout = INFINITE) const;
//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			멤버변수 선언
//======================================================//
	SOCKET sock;
	//Read Event
	HANDLE hReadEvent;
	//Wirte Event
	HANDLE hWriteEvent;
	//송수신 버퍼
	CHAR buf[MAX_SOCKBUF];
	//보내는 사이즈
	DWORD dwSendSize;
	//받는 사이즈
	DWORD dwRecvSize;
	//종료 플래그
	DWORD dwLoop;
	//포트
	u_short port;
	//접속 서버 주소
	CString ip;
	//사용내역 조회 구조체
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	//객체 초기화
	virtual BOOL initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void ReleaseClient();
	//서버 접속(ip , port)
	virtual	BOOL ServerConnect(u_short _port, CString _ip);
	//데이터 전송
	virtual BOOL SendDatafn(DWORD _dwSendSize, void* data, DWORD dwTimeout = INFINITE);
	//데이터 받기
	virtual BOOL RecvDatafn(DWORD _dwRecvSize, void* data, DWORD dwTimeout = INFINITE);
	//클라이언트가 접속상태인가?
	virtual BOOL ActiveClient();
//=======================================================//

};
#endif// __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
