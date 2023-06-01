#ifndef __IADOEXCEPTION_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IADOEXCEPTION_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------


//--------------------------------------------------------------
//|
//|IADOException 인터페이스
//|
//---------------------------------------------------------------
interface IADOException
{
	virtual void Release()=0;
	//서버메니저 초기화
	virtual BOOL initAdoException(IMyObject* _obj)=0;
};

#endif //__IADOEXCEPTION_74F736E25AB94b5dA789E4730F58F2A1_H__