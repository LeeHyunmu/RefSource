
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
	virtual BOOL initClient(IMyObject* _obj)=0;
	virtual void ReleaseClient()=0;
	virtual	BOOL ServerConnect(u_short _port, CString _ip)=0;
	virtual void SendData(int code, TCHAR* CardNum, TCHAR* data)=0;
};

#endif //__ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__