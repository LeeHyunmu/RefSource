
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
// 전송 자료
struct DATA
{
	DWORD wCode;
	CHAR szCardNum[20];
	CHAR szData[1024];
};

// 소켓 정보 저장을 위한 구조체
struct SOCKETINFO
{
	WSAOVERLAPPED  wsaOv;
	WORD           wType[2];
	WSABUF         wsaBuf[2];  //wsaBuf[0] : Recv, wsaBuf[1] : Send
	SOCKET         hSock;
	CHAR           szCardNum[20];
	DATA		   data;
};
struct SOCKETLINK
{
	SOCKET hVMSock;
	SOCKET hMBSock;
};
typedef vector<SOCKETINFO*> vecSocketInfo;
typedef vector<SOCKETLINK> vecSoLk;

// 오버 랩 I/O 컨텍스트
#define IO_TYPE_RECV              0x1000
#define IO_TYPE_SEND              0x2000
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
	virtual BOOL Initialize(u_short _nport)=0;
	//소켓 닫기
	virtual void fnCloseSocket()=0;
	//받는 함수
	virtual DWORD fnWSARecv(SOCKET hSock, DWORD dwFlags = 0)=0;
	//보내는 함수
	virtual DWORD fnWSASend(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;
	// 전체 보내기
	virtual DWORD fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags = 0)=0;

	//소켓 정보 추가
	virtual BOOL fnAddSocketInfo(SOCKET hSock)=0;
	//소켓정보 삭제
	virtual BOOL fnRemoveSocketInfo(SOCKET hSock)=0;
	//소켓정보 가져오기
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock)=0;
	virtual BOOL fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock, TCHAR *szCardNum)=0;
	//종료
	virtual void Release()=0;
	//클라이언트 접속
	virtual SOCKET fnAccept()=0;
	//비동기 클라이언트용 소켓 만들기 함수
	virtual BOOL CompletionPortClientSocket(CString _ip, u_short _port, SOCKET& hClientSock)=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__