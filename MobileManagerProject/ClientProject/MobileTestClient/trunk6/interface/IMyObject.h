
#ifndef __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IMyObject
{
	virtual BOOL CreateObject()=0;
	virtual void Release()=0;
	//디버그 클래스
	virtual IOutput* GetOutput()=0;
	//Crc32 
	virtual ICrc* GetCrc32()=0;
	//Client class
	virtual IClient* GetClient()=0;
};

#endif //__IMYOBJECT_74F736E25AB94b5dA789E4730F58F2A1_H__