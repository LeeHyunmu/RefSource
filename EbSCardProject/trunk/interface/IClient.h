
#ifndef __ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//--------------------------------------------------------------
//|
//|IClient 인터페이스
//|
//---------------------------------------------------------------
interface IClient
{
	//객체 초기화
	virtual BOOL initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void ReleaseClient()=0;
	//서버 접속(ip , port)
	virtual	BOOL ServerConnect(u_short _port, CString _ip)=0;
	//데이터 전송
	virtual BOOL SendDatafn(DWORD _dwSendSize, void* data, DWORD dwTimeout = INFINITE)=0;
	//데이터 받기
	virtual BOOL RecvDatafn(DWORD _dwRecvSize, void* data, DWORD dwTimeout = INFINITE)=0;
	//클라이언트가 접속상태인가?
	virtual BOOL ActiveClient()=0;
};

#endif //__ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__