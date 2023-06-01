
#ifndef __GASTRING_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GASTRING_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GASRING  GAString
class GAString
{
public:
	//생성자
	GAString(void);
	//소멸자
	~GAString(void);
	//======================================
	//
	//=======================================
	//STL 알고리즘으로 기본으로 ,(콤마)로 스트링을 분리하여 넘긴다
	//int
	static void GetTokenize(const tstring& str, vector<int>& tokens, const string& delimiters=",");
	//short
	static void GetTokenize(const tstring& str, vector<short>& tokens, const string& delimiters=",");
	//CString
	static void GetTokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters=",");
	//공백으로 토큰 분리
	static void GetTokenize(const tstring& str, vector<tstring>& tokens);
};
#endif// __GASTRING_B7A15A9D01DC403996FC45F7F92F52D3_H__
