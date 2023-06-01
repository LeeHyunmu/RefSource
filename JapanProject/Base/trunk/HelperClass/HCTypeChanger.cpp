
#include "stdAfx.h"
#include "HCTypeChanger.h"
#include <sstream>
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
HCTypeChanger::HCTypeChanger(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
HCTypeChanger::~HCTypeChanger(void)
{
}
//--------------------------------------------------------------
//|
//|숫자를 문자로
//|
//---------------------------------------------------------------
inline void HCTypeChanger::NtoA(int &n) 
{ 
	n |= 0x30; 
} 

//--------------------------------------------------------------
//|
//|대문자 소문자로
//|
//---------------------------------------------------------------
inline void HCTypeChanger::UtoL(char &c) 
{ 
	c |= 0x20; 
} 
//--------------------------------------------------------------
//|
//|소문자 대문자로
//|
//---------------------------------------------------------------
inline void HCTypeChanger::LtoU(char &c) 
{ 
	c &= 0xDF; // ~(0x20) 
} 
//--------------------------------------------------------------
//|
//|string Unicode로 변환
//|
//---------------------------------------------------------------
std::wstring HCTypeChanger::AnsiToUnicode(const std::string& s)
{
	std::wstring ret(s.length(),' ');
	std::copy(s.begin(), s.end(), ret.begin());
	return ret;
}
//--------------------------------------------------------------
//|
//|wstring strng로 변환
//|
//---------------------------------------------------------------
std::string HCTypeChanger::wstringTostring(const std::wstring& strSrc)
{
	char* szRet = NULL;
	int len = ::WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, szRet, 0, NULL, NULL);
	if(0 < len)
	{
		szRet = new char[len];
		::ZeroMemory(szRet, len);
		::WideCharToMultiByte(CP_ACP, 0, strSrc.c_str(), -1, szRet, len, NULL, NULL);
		std::string strRet(szRet);
		delete [] szRet;
		return strRet;
	}
	return std::string(""); 
}
//--------------------------------------------------------------
//|
//|char를 hex로 변환
//|
//---------------------------------------------------------------
CString HCTypeChanger::Changcharbyhex(CString pass)
{
	CString retvalue="";
	CString temp;
	TCHAR *a = pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format(_T("%02X"), a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|BYTE를 hex로 변환
//|
//---------------------------------------------------------------
CString HCTypeChanger::Changbytetohex(BYTE* byt, int len)
{
	CString temp;
	CString rest="";
	for( int i = 0; i < (int)len; i++ )
	{
		temp.Format(_T("%02X"), byt[i]);
		rest += temp;
	}
	if(len==(rest.GetLength()/2))
		return rest;
	return "";
}

//--------------------------------------------------------------
//|
//|Hex스트링을값을 문자스트링값으로 변환한다(예 "303041413961"=>"00AA9a"
//|
//---------------------------------------------------------------
CString HCTypeChanger::HexStringToCharString(CString st)
{
	CString revel="";
	for(int i=0; i < st.GetLength() ; i += 2)
	{
		CString str = "0x";
		TCHAR cha[2];
		str += st.Mid(i,2);
#if !defined(UNICODE)
		BYTE ch = (BYTE)strtol( str, NULL, 16 ); // 유니코드가 정의되어 있지 않은 경우
#else
		BYTE ch = (BYTE)wcstol( str, NULL, 16 ); // 유니코드가 정의된 경우
#endif
		_stprintf_s(cha, sizeof(cha), _T("%c"), ch);
		revel += cha;
	}
	return revel;
}
//--------------------------------------------------------------
//|
//|Hex스트링을값을 문자스트링값으로 변환한다(예 "303041413961"=>"00AA9a") bk가 설정한 문자까지 읽는다
//|
//---------------------------------------------------------------
int HCTypeChanger::HexStringToCharString(CString st,CString& revel,CString bk)
{
	revel="";
	int i;
	for(i=0; i < st.GetLength() ; i += 2)
	{
		CString str = "0x";
		TCHAR cha[2];
		str += st.Mid(i,2);
		if(str==bk)
			break;
#if !defined(UNICODE)
		BYTE ch = (BYTE)strtol( str, NULL, 16 ); // 유니코드가 정의되어 있지 않은 경우
#else
		BYTE ch = (BYTE)wcstol( str, NULL, 16 ); // 유니코드가 정의된 경우
#endif
		_stprintf_s(cha, sizeof(cha), _T("%c"), ch);
		revel += cha;
	}
	return i;
}
//--------------------------------------------------------------
//|
//|스크링을 1바이트 문자를 2바이트 HEX로 변환한다
//|
//---------------------------------------------------------------
CString HCTypeChanger::Chang1Byteby2Hex(CString pass)
{
	CString retvalue="";
	CString temp;
	BYTE *a = (BYTE*)pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format(_T("%02X"), a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL HCTypeChanger::DeleteVector(vector<CString>& str ,int  index)
{
	str.erase (str.begin()+index);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|문자열 중에서 특정 문자열을 찾아 본다
//|
//---------------------------------------------------------------
BOOL HCTypeChanger::findstring(const char* str ,const char*  fstr)
{
	vector<char> _chVec(str, str + strlen(str));
	vector<char> _CchVec(fstr, fstr + strlen(fstr));

	vector<char>::iterator ppnt;
	ppnt = search(_chVec.begin(), _chVec.end(), _CchVec.begin(), _CchVec.end());						// Search Algorithm
	if ( ppnt == _chVec.end())
	{
		return FALSE;
	}
	return TRUE;
}
BOOL HCTypeChanger::findCString(CString str ,CString  fstr)
{
	// Search Algorithm
	if (str.Find(fstr)==-1)
	{
		return FALSE;
	}
	return TRUE;
}
LPCTSTR HCTypeChanger::FindChars(LPCTSTR p1, LPCTSTR p2)
{
	while (*p1 != NULL)
	{
		LPCTSTR p = p2;
		while (*p != NULL)
		{
			if (*p1 == *p)
				return CharNext(p1);
			p = CharNext(p);
		}
		p1++;
	}
	return NULL;
}

static char hexChar[] = {
	0x30 , 0x31 , 0x32 , 0x33 ,
	0x34 , 0x35 , 0x36 , 0x37 ,
	0x38 , 0x39 , 0x41 , 0x42 ,
	0x43 , 0x44 , 0x45 , 0x46 
};
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
CString HCTypeChanger::byteToHexbyteValue(byte* data, int len) 
{
	CString hex="";
	for (int i = 0, h = 0; i < len; i++, h+=2) {
		hex += hexChar[(data[i]&0xF0)>>4];
		hex += hexChar[(data[i]&0x0F)];
	}
	return hex;
}
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
int  HCTypeChanger::ChangeHexCord(CString data, BYTE* hex, int& hexlen)
{
	memset(hex, 0, hexlen);
	hexlen=data.GetLength()/2;
	for(int i = 0 ; i < hexlen ; i++ )
	{
		hex[i] = (((BYTE)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			hex[i] += 0x90;

		hex[i] |= ((BYTE)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			hex[i] += 0x09;
	}
	return hexlen;
}