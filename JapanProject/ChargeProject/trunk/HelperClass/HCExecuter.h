
#ifndef __HCEXECUTER_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __HCEXECUTER_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class HCExecuter
{
public:
	//持失切
	HCExecuter(void);
	//社瑚切
	~HCExecuter(void);
	//
	static BOOL RunProgram(LPCTSTR lpszImageName);
	//
	static BOOL IsProgram(LPCTSTR lpszImageName);
};
#endif// __HCEXECUTER_B7A15A9D01DC403996FC45F7F92F52D3_H__
