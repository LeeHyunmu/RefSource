
#ifndef __CHANGETYPE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __CHANGETYPE_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _TYPE  CChangeType
class CChangeType
{
public:
	//생성자
	CChangeType(void);
	//소멸자
	~CChangeType(void);

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

	//Applet에 패스워드를 설정할때 hex트문자로 변환하기 위한 함수
	static CString Changcharbyhex(CString pass);
	//
	static CString Changbytetohex(BYTE* byt, int len);
	//1Byte문자를 Hex2Byte로
	static CString Chang1Byteby2Hex(CString pass);
	//핵사 스트링값을 문자 스트링값으로 변환
	static CString HexStringToCharString(CString st);
	//bk스트링 까지 읽어서 변환하고 bk점을 리턴한다
	static int HexStringToCharString(CString st,CString& revel,CString bk);
	//케릭터를 헥사로 변환
	static void StrToBCD(LPSTR&StrData, int BCDNum, BYTE* Result);
	//BYTE 헥사를 BYTE 케릭터로 변환
	static void BCDToStr(BYTE* BCDData, int BCDNum, BYTE* Result);
	//헥사 스트링을 헥사 바이트로
	static int  ChangeHexCord(CString data, LPBYTE hex);
	static CString  ChangeHexCord(BYTE* data, int len, CString& hex);
};
#endif// __CHANGETYPE_B7A15A9D01DC403996FC45F7F92F52D3_H__
