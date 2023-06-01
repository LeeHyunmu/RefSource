
#ifndef __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|서버용 구조체 선언
//|
//---------------------------------------------------------------
//Recv, Send 구조체 설정
//
#define  MAX_SOCKBUF	8192

//베이스 구조체
// code - 전송된 코드 분류
// card number - 카드 넘버
// total len - data 전체 길이
// 20byte 고정
// data[1] 데이터
// dwdatalen = data[1] 데이터 길이 + 유효성 체크코드(4byte) 포함
// CRC_32 유효성 체크 후 사용
//--------------------------------------------------------------
//|
//| 프로토콜 데이터 구조체
//|
//---------------------------------------------------------------
#pragma pack(push,1)
struct baseDATA20
{
	DWORD dwBaseCode;
	CHAR szCardNum[12];
	DWORD dwdatalen;
	CHAR  data[1];
};
struct DATA36
{
	CHAR  info[32];
	DWORD dwcheckorlen;
	CHAR  afterdata[1];
};
struct DATA162
{
	DWORD  dwnum;
	DWORD  dwcode;
	DWORD  dwusemoney;
	CHAR usename[21];
	CHAR usecardnum[11];
	CHAR useplace[21];
	CHAR useadress[61];
	CHAR usetime[32];
	DWORD dwcheckorlen;
	CHAR  afterdata[1];
};
#pragma pack(pop)
//--------------------------------------------------------------
//|
//| 소켓 정보 구조체
//|
//---------------------------------------------------------------
struct SENDDATAINFO
{
	HANDLE		   hSendEvent;
	DWORD		   nTotalSendBytes;
	DWORD		   nSendBytes;
	BOOL		   bafterSenddata;
};
struct SOCKLIST
{
	SOCKET         hSock;
	SOCKET         hlinkSock;
};
// 소켓 정보 저장을 위한 구조체
struct SOCKETINFO
{
	WSAOVERLAPPED  wsaOv;
	DWORD          wType[2];
	WSABUF         wsaBuf[2];  //wsaBuf[0] : Recv, wsaBuf[1] : Send
	SOCKLIST       socklist;
	SENDDATAINFO   sendinfo;
	DWORD		   overlappedcode;
	CHAR		   data[MAX_SOCKBUF];
};
typedef vector<SOCKETINFO*> vecSocketInfo;


// 오버 랩 I/O 컨텍스트
#define IO_TYPE_RECV              0x10000000
#define IO_TYPE_SEND              0x20000000
//--------------------------------------------------------------
//|
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IServer
{
	//초기 설정
	virtual BOOL initServer(IMyObject* _obj)=0;
	//소켓 초기화
	virtual BOOL Initialize()=0;
	//서버 Listen
	virtual BOOL ServerListen(u_short _nport)=0;
	//소켓 닫기
	virtual void fnCloseSocket()=0;
	//받는 함수
	virtual DWORD fnWSARecv(SOCKET hSock, DWORD dwFlags = 0)=0;
	//보내는 함수
	virtual DWORD fnWSASend(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;
	// 전체 보내기
	virtual DWORD fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;

	//소켓 정보 추가
	virtual BOOL fnAddSocketInfo(SOCKET hSock, SOCKET hlinkSock=0)=0;
	//소켓정보 삭제
	virtual BOOL fnRemoveSocketInfo(SOCKET hSock)=0;
	//소켓정보 가져오기
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock)=0;
	//종료
	virtual void Release()=0;
	//클라이언트 접속
	virtual SOCKET fnAccept()=0;
	//비동기 서버 소켓 접속
	virtual HANDLE CompletionPortServerSocket(HANDLE hIocp)=0;
	//비동기 클라이언트용 소켓 만들기 함수
	virtual SOCKET CompletionPortClientSocket(HANDLE hIocp, SOCKET hSock)=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__