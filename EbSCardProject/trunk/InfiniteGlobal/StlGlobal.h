
#ifndef __STLGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __STLGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//유니코드
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif 

	//CString을 vector 저장하는 STL 단축 definition
	typedef vector<CString> vecString;
	typedef vector<CString>::iterator vecStringIt;

#endif //__STLGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__