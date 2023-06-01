
#ifndef __STRINGSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __STRINGSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _STRING  CStringstatic
class CStringstatic
{
public:
	//생성자
	CStringstatic(void);
	//소멸자
	~CStringstatic(void);

	//문자열 중에서 특정 문자열을 찾는 함수
	static BOOL findstring(const char* str ,const char*  fstr);
	static BOOL findCString(CString str ,CString  fstr);

	//STL 알고리즘으로 ,(콤마)로 스트링을 분리하여 넘긴다
	static void Tokenize(const tstring& str, vecInt& tokens, const std::string& delimiters=",");
	static void Tokenize(const tstring& str, vecShort& tokens, const std::string& delimiters=",");
	//STL 알고리즘으로 ,(콤마)로 스트링을 분리하여 넘긴다
	static void Tokenize(const tstring& str, vecString& tokens, const tstring& delimiters=",");
	//공백으로 토큰 분리
	static void Tokenize(const tstring& str, std::vector<tstring>& tokens);
	//처음부터 케릭터 문자들을 검색한다
	static LPCTSTR FindChars(LPCTSTR p1, LPCTSTR p2);
	//특정문자를 삭제한다
	static CString DeleteChar(LPCSTR szStr, char ch);
	//
	static int BinaryToken( int nPos , BYTE Source[] , LPSTR& FindData , int nCmpLen , int nTotal );
	//토큰 스트링을 넘긴다(스트링, 넘길 토큰의 순번, 분활토큰
	static CString GetTokenString(CString link,int toknum, const tstring& delimiters=",");
};
#endif// __STRINGSTATIC_H__
