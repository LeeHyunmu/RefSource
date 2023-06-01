
#ifndef __IMYSQLDB_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYSQLDB_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//| 매크로
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//| typedef
//|
//---------------------------------------------------------------
typedef ADODB::_RecordsetPtr	RecPtr;
typedef ADODB::_ConnectionPtr	CnnPtr; 

//--------------------------------------------------------------
//|
//|IMysqlDb 인터페이스
//|
//---------------------------------------------------------------
interface IMysqlDb
{
	//초기 설정
	virtual BOOL initMysqlDb(IMyObject* _obj)=0;
	//종료
	virtual void Release()=0;
};

#endif //__IMYSQLDB_74F736E25AB94b5dA789E4730F58F2A1_H__