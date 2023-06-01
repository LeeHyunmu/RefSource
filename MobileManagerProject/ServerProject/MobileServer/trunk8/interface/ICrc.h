
#ifndef __ICRC_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ICRC_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|ICrc 인터페이스
//|
//---------------------------------------------------------------
interface ICrc
{
	//초기 설정
	virtual BOOL initCrc(IMyObject* _obj)=0;
	//종료
	virtual void Release()=0;
	//체크 값 생성
	virtual unsigned long GetResult(char *dataptr,int datasize)=0;
	//값 체크하기
	virtual BOOL GetCheck(char *dataptr,int datasize)=0;
};

#endif //__ICRC_74F736E25AB94b5dA789E4730F58F2A1_H__