
#ifndef __ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|서버용 구조체 선언
//|
//---------------------------------------------------------------
typedef vector<HANDLE> vechThread;

//--------------------------------------------------------------
//|
//|Recv Code
//|
//---------------------------------------------------------------
#define RECVCODE_CARDENABLE            0x00000002
#define RECVCODE_USERCHANGE            0x00000004
#define RECVCODE_DEDUCTION             0x00000008
#define RECVCODE_USECHECK              0x00000010
#define RECVCODE_VMCLIENTRECV		   0x10000000
#define RECVCODE_MYBISERVERRECV		   0x20000000
#define RECVCODE_VMCLIENTSEND	       0x40000000
#define RECVCODE_MYBISERVERSEND		   0x80000000
//--------------------------------------------------------------
//|
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IServerMg
{
	virtual void Release()=0;
	//서버메니저 초기화
	virtual BOOL initServerMg(IMyObject* _obj)=0;
	//컴플리션 포트 핸들을 가져 온다
	virtual HANDLE GetCompletionPort()=0;
	virtual HANDLE GetClntCompletionPort()=0;
	//서버를 시작한다
	virtual BOOL ServerStart(u_short nPort)=0;
	//서버를 중지한다(아직 안쓰고 있음)
	virtual void ServerStop()=0;
	//비동기 데이터 recv용 클라이언트 소켓 생성 등록
	virtual SOCKET MakeCompetionPortClient(SOCKET hSock)=0;
	//보낸 데이터 이후 처리 함수
	virtual void SendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)=0;
	//받은 데이터 처리 함수
	virtual void RecvHandling(SOCKETINFO* _info, DWORD flag)=0;
	//서버 종료
	virtual BOOL GetEndServer()=0;
};

#endif //__ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__