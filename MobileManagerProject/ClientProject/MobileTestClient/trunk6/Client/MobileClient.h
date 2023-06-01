
#ifndef __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//
#define  MAX_SOCKBUF	8192
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
	DWORD Run();
	int recvn(SOCKET s, char *buf, int len, int flags, BOOL bfull=FALSE);
	BOOL ClientMain();
	void ThreadClose();
	int findBluetooth();
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
	TCHAR buf[MAX_SOCKBUF];
	//종료 플래그
	BOOL bEnd;
	//포트
	u_short port;
	//접속 서버 주소
	CString ip;
	//보내는 코드(보내는 코드에서 사용내역 조회는 구조체가 틀려진다)
	DWORD wsendCode;
	//사용내역 조회 구조체
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void ReleaseClient();
	virtual BOOL initClient(IMyObject* _obj);
	virtual	BOOL ServerConnect(u_short _port, CString _ip);
	virtual void SendData(int code, TCHAR* CardNum, TCHAR* data);
//=======================================================//

};
#endif// __MOBILECLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
