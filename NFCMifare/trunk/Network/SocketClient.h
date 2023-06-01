
#ifndef __SOCKETECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SOCKETECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSocketClient
{
	HWND hWndp;
	//
	CEvent RespEvent;
	//
	CThread NfcThread;
public:
	//생성자
	CSocketClient();
	//소멸자
	~CSocketClient();
//======================================================//
//		 디폴트 클라이언트 접속 값
//======================================================//
	static const UINT DEFAULTPORT;
    static const CString DEFAULTIP;
//======================================================//
//			일반함수 선언
//======================================================//
	int recvn(SOCKET s, char *buf, int len, int flags, BOOL bfull=FALSE);
	void ClientMain();
	//thread 함수
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
//=======================================================//
//			객체 포인터 변수
//======================================================//

//=======================================================//
//			멤버변수 선언
//======================================================//
	SOCKET sock;
	//수신 버퍼
	byte recvbuf[BUFFSIZE_4096];
	UINT recvbuflen;
	//송수신 임시버퍼
	char tempbuf[BUFFSIZE_1024];
	UINT tempbuflen;
	//포트
	u_short port;
	//접속 서버 주소
	CString ip;
	//사용내역 조회 구조체
//======================================================//
//			
//======================================================//
	void ReleaseClient();
	BOOL initClient(HWND _hwnd);
	BOOL ServerConnect(u_short _port, CString _ip);
	BOOL SendData(int code, CString type, byte* data, UINT datalen);
//=======================================================//
	//디버그 출력 호출
	void DebugEditCall(CString str);
};
#endif// __SOCKETECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
