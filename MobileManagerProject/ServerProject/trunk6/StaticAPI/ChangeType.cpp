
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CChangeType::CChangeType(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CChangeType::~CChangeType(void)
{
}
//--------------------------------------------------------------
//|
//|숫자를 문자로
//|
//---------------------------------------------------------------
inline void CChangeType::NtoA(int &n) 
{ 
	n |= 0x30; 
} 

//--------------------------------------------------------------
//|
//|대문자 소문자로
//|
//---------------------------------------------------------------
inline void CChangeType::UtoL(char &c) 
{ 
	c |= 0x20; 
} 
//--------------------------------------------------------------
//|
//|소문자 대문자로
//|
//---------------------------------------------------------------
inline void CChangeType::LtoU(char &c) 
{ 
	c &= 0xDF; // ~(0x20) 
} 
//--------------------------------------------------------------
//|
//|string Unicode로 변환
//|
//---------------------------------------------------------------
std::wstring CChangeType::AnsiToUnicode(const std::string& s)
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
std::string CChangeType::wstringTostring(const std::wstring& strSrc)
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
//|BSTR => CString 로 변환
//|
//---------------------------------------------------------------
CString CChangeType::bstrToCString(BSTR bstr)   
{   
	char* pbstr;   
	USES_CONVERSION; //convert를 위한 매크로   
	pbstr=OLE2A(bstr); //bstr을 char로 convert   
	return CString(pbstr);   
}
//--------------------------------------------------------------
//|
//|char를 hex로 변환
//|
//---------------------------------------------------------------
CString CChangeType::Changcharbyhex(CString pass)
{
	CString retvalue="";
	CString temp;
	char *a = pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format("%02X", a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|BYTE를 hex로 변환
//|
//---------------------------------------------------------------
CString CChangeType::Changbytetohex(BYTE* byt, int len)
{
	CString temp;
	CString rest="";
	for( int i = 0; i < (int)len; i++ )
	{
		temp.Format("%02X", byt[i]);
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
CString CChangeType::HexStringToCharString(CString st)
{
	CString revel="";
	for(int i=0; i < st.GetLength() ; i += 2)
	{
		CString str = "0x";
		char* stop;
		char cha[2];
		str += st.Mid(i,2);
		BYTE ch = (unsigned char)strtol(str, &stop, 16);
		sprintf_s(cha, "%c", ch);
		revel += cha;
	}
	return revel;
}
//--------------------------------------------------------------
//|
//|Hex스트링을값을 문자스트링값으로 변환한다(예 "303041413961"=>"00AA9a") bk가 설정한 문자까지 읽는다
//|
//---------------------------------------------------------------
int CChangeType::HexStringToCharString(CString st,CString& revel,CString bk)
{
	revel="";
	int i;
	for(i=0; i < st.GetLength() ; i += 2)
	{
		CString str = "0x";
		char* stop;
		char cha[2];
		str += st.Mid(i,2);
		if(str==bk)
			break;
		BYTE ch = (unsigned char)strtol(str, &stop, 16);
		sprintf_s(cha, "%c", ch);
		revel += cha;
	}
	return i;
}
//--------------------------------------------------------------
//|
//|스크링을 1바이트 문자를 2바이트 HEX로 변환한다
//|
//---------------------------------------------------------------
CString CChangeType::Chang1Byteby2Hex(CString pass)
{
	CString retvalue="";
	CString temp;
	BYTE *a = (BYTE*)pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format("%02X", a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CChangeType::StrToBCD(LPSTR&StrData, int BCDNum, BYTE* Result)
{
	BYTE Tmp = 0x00;
	// 31 32 -> 12 , 31 45 -> 1E
	for ( int StrIndex = 0, ResIndex = 0 ; ResIndex < BCDNum ; ResIndex++ )
	{
		Tmp = StrData[StrIndex++] ;
		if ( Tmp > 0x39 )	// A : 0x41 -> 0x0a
			Tmp = Tmp - 0x37 ;
		else
			Tmp = Tmp - 0x30;
		Result[ResIndex] = Tmp << 4;
		Tmp = StrData[StrIndex++] ;
		if ( Tmp > 0x39 )	// A : 0x41 -> 0x0a
			Tmp = Tmp - 0x37 ;
		else
			Tmp = Tmp - 0x30;
		Result[ResIndex] |= Tmp;
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CChangeType::BCDToStr(BYTE* BCDData, int BCDNum, BYTE* Result)
{
	BYTE Tmp = 0x00;
	int BCDIndex = 0, ResIndex = 0 ;
	for ( BCDIndex = 0, ResIndex = 0 ; BCDIndex < BCDNum ; BCDIndex++ )
	{
		Tmp = BCDData[BCDIndex] >> 4;
		if ( Tmp > 9 )
			Tmp += 0x37 ;
		else
			Tmp += 0x30 ;
		Result[ResIndex++] = Tmp;
		Tmp = BCDData[BCDIndex] & 0x0F;
		if ( Tmp > 9 )
			Tmp += 0x37 ;
		else
			Tmp += 0x30 ;
		Result[ResIndex++] = Tmp;
	}
	Result[ResIndex] = 0x00;
}
//--------------------------------------------------------------
//|
//|헥사 스트링2바이트의값을 BYTE핵사 값으로 변환한다 반드시 메모리 해제할것
//|
//---------------------------------------------------------------
int  CChangeType::ChangeHexCord(CString data, LPBYTE hex)
{
	int dwtransLen=data.GetLength()/2;
	memset(hex,0x00,dwtransLen+1);
	for(int i = 0 ; i < dwtransLen ; i++ )
	{
		hex[i] = (((BYTE)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			hex[i] += 0x90;

		hex[i] |= ((BYTE)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			hex[i] += 0x09;
	}
	return dwtransLen;
}
//--------------------------------------------------------------
//|
//|바이트 헥사를 스트링 핵사 값으로 변환한다
//|
//---------------------------------------------------------------
static char hexChar[] = {
	'0' , '1' , '2' , '3' ,
	'4' , '5' , '6' , '7' ,
	'8' , '9' , 'A' , 'B' ,
	'C' , 'D' , 'E' , 'F' 
};
CString  CChangeType::ChangeHexCord(BYTE* data, int len, CString& hex)
{
	for(int i = 0 ; i < len ; i++ )
	{
		hex+= hexChar[(data[i]&0xF0)>>4];
		hex+= hexChar[data[i]&0x0F];
	}
	return hex;
}