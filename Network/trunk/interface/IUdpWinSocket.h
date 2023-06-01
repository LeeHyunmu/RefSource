
#ifndef __IUDPWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IUDPWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IUdpWinSocket
{
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void Release()=0;
	//UDP 통신 스래드 호출  함수
	virtual void TeleUdpfunc()=0;
};

#endif //__IUDPWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__