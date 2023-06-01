
#ifndef __IDUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IDualiP2pMg
{
	virtual ~IDualiP2pMg()=0;
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	virtual void Release()=0;
	//스래드에서 호출되는 함수
	virtual void CallThreadP2pFunc(int flag)=0;
	//데이터 전송
	virtual BOOL SendDatafn(DWORD _dwSendflage, CString data, DWORD dwTimeout = INFINITE)=0;
};

#endif //__IDUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__