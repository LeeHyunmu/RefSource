
#include "stdAfx.h"
#include <sstream>
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CTypeChangHelp::CTypeChangHelp(void)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CTypeChangHelp::~CTypeChangHelp(void)
{
}
//--------------------------------------------------------------
//|
//|���ڸ� ���ڷ�
//|
//---------------------------------------------------------------
inline void CTypeChangHelp::NtoA(int &n) 
{ 
	n |= 0x30; 
} 

//--------------------------------------------------------------
//|
//|�빮�� �ҹ��ڷ�
//|
//---------------------------------------------------------------
inline void CTypeChangHelp::UtoL(char &c) 
{ 
	c |= 0x20; 
} 
//--------------------------------------------------------------
//|
//|�ҹ��� �빮�ڷ�
//|
//---------------------------------------------------------------
inline void CTypeChangHelp::LtoU(char &c) 
{ 
	c &= 0xDF; // ~(0x20) 
} 
//--------------------------------------------------------------
//|
//|string Unicode�� ��ȯ
//|
//---------------------------------------------------------------
std::wstring CTypeChangHelp::AnsiToUnicode(const std::string& s)
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
std::string CTypeChangHelp::wstringTostring(const std::wstring& strSrc)
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
//|BSTR => CString �� ��ȯ
//|
//---------------------------------------------------------------
CString CTypeChangHelp::bstrToCString(BSTR bstr)   
{   
	char* pbstr;   
	USES_CONVERSION; //convert�� ���� ��ũ��   
	pbstr=OLE2A(bstr); //bstr�� char�� convert   
	return CString(pbstr);   
}
//--------------------------------------------------------------
//|
//|char�� hex�� ��ȯ
//|
//---------------------------------------------------------------
CString CTypeChangHelp::Changcharbyhex(CString pass)
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
//|BYTE�� hex�� ��ȯ
//|
//---------------------------------------------------------------
CString CTypeChangHelp::Changbytetohex(BYTE* byt, int len)
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
//|Hex��Ʈ�������� ���ڽ�Ʈ�������� ��ȯ�Ѵ�(�� "303041413961"=>"00AA9a"
//|
//---------------------------------------------------------------
CString CTypeChangHelp::HexStringToCharString(CString st)
{
	CString revel="";
	for(int i=0; i < st.GetLength() ; i += 2)
	{
		CString str = "0x";
		char* stop;
		char cha[2];
		str += st.Mid(i,2);
		BYTE ch = (unsigned char)strtol(str, &stop, 16);
		sprintf_s(cha, sizeof(cha), "%c", ch);
		revel += cha;
	}
	return revel;
}
//--------------------------------------------------------------
//|
//|Hex��Ʈ�������� ���ڽ�Ʈ�������� ��ȯ�Ѵ�(�� "303041413961"=>"00AA9a") bk�� ������ ���ڱ��� �д´�
//|
//---------------------------------------------------------------
int CTypeChangHelp::HexStringToCharString(CString st,CString& revel,CString bk)
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
		sprintf_s(cha, sizeof(cha), "%c", ch);
		revel += cha;
	}
	return i;
}
//--------------------------------------------------------------
//|
//|��ũ���� 1����Ʈ ���ڸ� 2����Ʈ HEX�� ��ȯ�Ѵ�
//|
//---------------------------------------------------------------
CString CTypeChangHelp::Chang1Byteby2Hex(CString pass)
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
//|��Ʈ�� �����ڷ� ��Ʈ���� �з��Ͽ� int�� �ѱ�� vector
//|
//---------------------------------------------------------------
void CTypeChangHelp::Tokenize(const tstring& str, vector<int>& tokens, const tstring& delimiters)
{
	//�� ù ���ڰ� �������� ��� ����
	tstring::size_type lastPos = str.find_first_not_of(delimiters,0);
	tstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while(tstring::npos!=pos||tstring::npos!=lastPos)
	{
		tokens.push_back(atoi((str.substr(lastPos, pos-lastPos)).c_str()));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos=str.find_first_of(delimiters, lastPos);
	}
}
void CTypeChangHelp::Tokenize(const tstring& str, vector<short>& tokens, const tstring& delimiters)
{
	//�� ù ���ڰ� �������� ��� ����
	tstring::size_type lastPos = str.find_first_not_of(delimiters,0);
	tstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while(tstring::npos!=pos||tstring::npos!=lastPos)
	{
		tokens.push_back(atoi((str.substr(lastPos, pos-lastPos)).c_str()));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos=str.find_first_of(delimiters, lastPos);
	}
}
//--------------------------------------------------------------
//|
//|��Ʈ�� �����ڷ� ��Ʈ���� �з��Ͽ� CString�� �ѱ�� vector
//|
//---------------------------------------------------------------
void CTypeChangHelp::Tokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters)
{
	//�� ù ���ڰ� �������� ��� ����
	tstring::size_type lastPos = str.find_first_not_of(delimiters,0);
	tstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while(tstring::npos!=pos||tstring::npos!=lastPos)
	{
		tokens.push_back((str.substr(lastPos, pos-lastPos)).c_str());
		lastPos = str.find_first_not_of(delimiters, pos);
		pos=str.find_first_of(delimiters, lastPos);
	}
}
//--------------------------------------------------------------
//|
//|stringstream���� ��ū ����
//|
//---------------------------------------------------------------
void CTypeChangHelp::Tokenize(const tstring& str, vector<tstring>& tokens)
{
	string buf; // ���� string
	stringstream ss(str); // string�� stream�� �ִ´�
	while (ss >> buf)
		tokens.push_back(buf);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CTypeChangHelp::DeleteVector(vector<CString>& str ,int  index)
{
	str.erase (str.begin()+index);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|���ڿ� �߿��� Ư�� ���ڿ��� ã�� ����
//|
//---------------------------------------------------------------
BOOL CTypeChangHelp::findstring(const char* str ,const char*  fstr)
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
BOOL CTypeChangHelp::findCString(CString str ,CString  fstr)
{
	// Search Algorithm
	if (str.Find(fstr)==-1)
	{
		return FALSE;
	}
	return TRUE;
}
LPCTSTR CTypeChangHelp::FindChars(LPCTSTR p1, LPCTSTR p2)
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
CString CTypeChangHelp::byteToHexbyteValue(byte* data, int len) 
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
int  CTypeChangHelp::ChangeHexCord(CString data, BYTE* hex, int maxlen)
{
	memset(hex, 0, maxlen);
	int dwtransLen=data.GetLength()/2;
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