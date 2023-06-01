
#ifndef __GARESFILE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GARESFILE_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GARes  GAResFile
class GAResFile
{
public:
	//持失切
	GAResFile(void);
	//社瑚切
	~GAResFile(void);
	static BOOL ImageLoad(HMODULE hModule,UINT ResourceName, LPCTSTR ResourceType, CBitmapHandle& bit, HWND hWnd=NULL);
};
#endif// __GARESFILE_B7A15A9D01DC403996FC45F7F92F52D3_H__
