
#include "stdAfx.h"
#include "HCTypeChanger.h"
#include <sstream>
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
HCTypeChanger::HCTypeChanger(void)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
HCTypeChanger::~HCTypeChanger(void)
{
}
//--------------------------------------------------------------
//|
//|���ڸ� ���ڷ�
//|
//---------------------------------------------------------------
inline void HCTypeChanger::NtoA(int &n) 
{ 
	n |= 0x30; 
} 

//--------------------------------------------------------------
//|
//|�빮�� �ҹ��ڷ�
//|
//---------------------------------------------------------------
inline void HCTypeChanger::UtoL(char &c) 
{ 
	c |= 0x20; 
} 
//--------------------------------------------------------------
//|
//|�ҹ��� �빮�ڷ�
//|
//---------------------------------------------------------------
inline void HCTypeChanger::LtoU(char &c) 
{ 
	c &= 0xDF; // ~(0x20) 
} 
//--------------------------------------------------------------
//|
//|string Unicode�� ��ȯ
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
//|wstring strng�� ��ȯ
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
//|char�� hex�� ��ȯ
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
//|BYTE�� hex�� ��ȯ
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
//|Hex��Ʈ�������� ���ڽ�Ʈ�������� ��ȯ�Ѵ�(�� "303041413961"=>"00AA9a"
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
		BYTE ch = (BYTE)strtol( str, NULL, 16 ); // �����ڵ尡 ���ǵǾ� ���� ���� ���
#else
		BYTE ch = (BYTE)wcstol( str, NULL, 16 ); // �����ڵ尡 ���ǵ� ���
#endif
		_stprintf_s(cha, sizeof(cha), _T("%c"), ch);
		revel += cha;
	}
	return revel;
}
//--------------------------------------------------------------
//|
//|Hex��Ʈ�������� ���ڽ�Ʈ�������� ��ȯ�Ѵ�(�� "303041413961"=>"00AA9a") bk�� ������ ���ڱ��� �д´�
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
		BYTE ch = (BYTE)strtol( str, NULL, 16 ); // �����ڵ尡 ���ǵǾ� ���� ���� ���
#else
		BYTE ch = (BYTE)wcstol( str, NULL, 16 ); // �����ڵ尡 ���ǵ� ���
#endif
		_stprintf_s(cha, sizeof(cha), _T("%c"), ch);
		revel += cha;
	}
	return i;
}
//--------------------------------------------------------------
//|
//|��ũ���� 1����Ʈ ���ڸ� 2����Ʈ HEX�� ��ȯ�Ѵ�
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
//|���ڿ� �߿��� Ư�� ���ڿ��� ã�� ����
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
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
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
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
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