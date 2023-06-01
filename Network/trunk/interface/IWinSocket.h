
#ifndef __IWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IWinSocket
{
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void Release()=0;
	//Accept  함수
	virtual void Acceptfunc()=0;
	//서버를 만든다
	virtual BOOL MakeServer(u_short _port, CString _ip)=0;
	//데이터 리시브 대기
	virtual void Recvfunc(SOCKET client_sock)=0;
};

#endif //__IWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__