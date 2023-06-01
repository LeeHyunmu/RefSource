
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
//VM 요청 종류
//카드 사용 승인
#define RECVCODE_CARDENABLE            0x00000002
//권종 변경
#define RECVCODE_USERCHANGE            0x00000004
//소득 등록
#define RECVCODE_DEDUCTION             0x00000008
//사용 내역 조회
#define RECVCODE_USECHECK              0x00000010

//에러
//CODE가 에러가 났다
#define RECVCODE_WHERECORDERROR	   	   0x01000000
//(어떤 데이터 처리할 지 알 수 없을 때)DATA처리 코드 에러 났다
#define RECVCODE_WHATDATAERROR     	   0x02000000
//DATA가 에러가 났다
#define RECVCODE_DATAERROR      	   0x04000000
//에러(에러 종류가 불 분명하다)
#define RECVCODE_BASEERROR  		   0x08000000

//통신 개체
//VM => 관리서버
#define RECVCODE_VMCLIENTRECV		   0x10000000
//MYBI => 관리서버
#define RECVCODE_MYBISERVERRECV		   0x20000000
//관리서버 => VM
#define RECVCODE_VMCLIENTSEND	       0x40000000
//관리서버 => MYBI
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
	//Iocp Server 보낸 데이터 이후 처리 함수
	virtual void IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)=0;
	//Iocp Server 받은 데이터 처리 함수
	virtual void IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag)=0;
	//Iocp Client 보낸 데이터 이후 처리 함수
	virtual void IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)=0;
	//Iocp Client 받은 데이터 처리 함수
	virtual void IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag)=0;
	//서버 종료
	virtual BOOL GetEndServer()=0;
};

#endif //__ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__