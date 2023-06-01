
#ifndef __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IServer
{
	virtual BOOL initServer()=0;
	virtual void Release()=0;
};

#endif //__ISERVER_74F736E25AB94b5dA789E4730F58F2A1_H__