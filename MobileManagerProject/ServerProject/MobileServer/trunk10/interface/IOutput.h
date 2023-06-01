
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
	virtual void Write(TCHAR* format,...){}
	virtual void err_display(char* msg){}
	virtual void ErrorHandler(TCHAR* place,_com_error &e){}
	virtual void Write(int x, int y, TCHAR * format,...){}
	virtual void Clear() = 0;
	virtual void SetVisible(BOOL visible) = 0;
	virtual BOOL GetVisible() = 0;
	//virtual wsOutput(void);
	virtual ~IOutput(void){}
};

#endif //__IOUTPUT_74F736E25AB94b5dA789E4730F58F2A1_H__