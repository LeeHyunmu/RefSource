
#ifndef __IMYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__

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
//|IMysqlDbtable 인터페이스
//|
//---------------------------------------------------------------
interface IMysqlDbtable
{
	virtual void Release()=0;
	//레코드 포인터 저장
	virtual void SetRecordPtr(RecPtr _rec)=0;
	//서버메니저 초기화
	virtual BOOL initMysqlDbtable(IMyObject* _obj)=0;
};

#endif //__IMYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__