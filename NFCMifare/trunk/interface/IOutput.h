
#ifndef __IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IOutput
{
	//상속 클래스소멸을위한 소멸자
	virtual ~IOutput()=0;
	//출력 호출 함수
	virtual void Write(TCHAR* format,...)=0;
	//종료
	virtual void Clear()=0;
	//디버그창 보이고 안보이고 설정
	virtual void SetVisible(BOOL visible)=0;
	//디버그 창이 보이는가?
	virtual BOOL GetVisible()=0;
	//스래드 출력함수
	virtual void OutputString()=0;
	//메모리 해제 
	virtual void Release()=0;
};

#endif //__IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__