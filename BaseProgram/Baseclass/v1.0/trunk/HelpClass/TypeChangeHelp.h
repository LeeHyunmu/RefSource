
#ifndef __STRINGSTATIC_H__
#define __STRINGSTATIC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif 

class CTypeChangHelp
{
public:
	//생성자
	CTypeChangHelp(void);
	//소멸자
	~CTypeChangHelp(void);
	//숫자 문자로
	static inline void NtoA(int &n);
	//대문자 소문자로
	static inline void UtoL(char &c);
	// 소문자 대문자로
	static inline void LtoU(char &c);

	//string wstring로 변환
	static std::wstring AnsiToUnicode(const std::string& s);
	//유니코드
	static std::string wstringTostring(const std::wstring& strSrc);
	//bstr CString형변환
	static CString bstrToCString(BSTR bstr);
	//벡터에서 인텍스 값을 삭제 한다
	static BOOL DeleteVector(vector<CString>& str ,int  index);
	//문자열 중에서 특정 문자열을 찾는 함수
	static BOOL findstring(const char* str ,const char*  fstr);
	static BOOL findCString(CString str ,CString  fstr);

	//Applet에 패스워드를 설정할때 hex트문자로 변환하기 위한 함수
	static CString Changcharbyhex(CString pass);
	//
	static CString Changbytetohex(BYTE* byt, int len);
	//1Byte문자를 Hex2Byte로
	static CString Chang1Byteby2Hex(CString pass);
	//핵사 스트링값을 문자 스트링값으로 변환
	static CString HexStringToCharString(CString st);
	//bk스트링 까지 읽어서 변환하고 bk점을 리턴한다(
	static int HexStringToCharString(CString st,CString& revel,CString bk);

	//STL 알고리즘으로 ,(콤마)로 스트링을 분리하여 넘긴다
	static void Tokenize(const tstring& str, vector<int>& tokens, const string& delimiters=",");
	static void Tokenize(const tstring& str, vector<short>& tokens, const string& delimiters=",");
	//STL 알고리즘으로 ,(콤마)로 스트링을 분리하여 넘긴다
	static void Tokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters=",");
	//공백으로 토큰 분리
	static void Tokenize(const tstring& str, vector<tstring>& tokens);
	//처음부터 케릭터 문자들을 검색한다
	static LPCTSTR FindChars(LPCTSTR p1, LPCTSTR p2);

	//
	static CString byteToHexbyteValue(byte* data, int len);
	//
	static int  ChangeHexCord(CString data, BYTE* hex, int maxlen);
};
#endif// __STRINGSTATIC_H__
