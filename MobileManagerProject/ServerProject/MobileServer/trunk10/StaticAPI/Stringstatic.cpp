
#include "stdAfx.h"
#include <sstream>
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CStringstatic::CStringstatic(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CStringstatic::~CStringstatic(void)
{
}
//--------------------------------------------------------------
//|
//|스트링 구분자로 스트링을 분류하여 int로 넘긴다 vector
//|
//---------------------------------------------------------------
void CStringstatic::Tokenize(const tstring& str, vecInt& tokens, const tstring& delimiters)
{
	//맨 첫 글자가 구분자인 경우 무시
	tstring::size_type lastPos = str.find_first_not_of(delimiters,0);
	tstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while(tstring::npos!=pos||tstring::npos!=lastPos)
	{
		tokens.push_back(atoi((str.substr(lastPos, pos-lastPos)).c_str()));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos=str.find_first_of(delimiters, lastPos);
	}
}
void CStringstatic::Tokenize(const tstring& str, vecShort& tokens, const tstring& delimiters)
{
	//맨 첫 글자가 구분자인 경우 무시
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
//|스트링 구분자로 스트링을 분류하여 CString로 넘긴다 vector
//|
//---------------------------------------------------------------
void CStringstatic::Tokenize(const tstring& str, vecString& tokens, const tstring& delimiters)
{
	//맨 첫 글자가 구분자인 경우 무시
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
//|stringstream으로 토큰 구분
//|
//---------------------------------------------------------------
void CStringstatic::Tokenize(const tstring& str, vector<tstring>& tokens)
{
	string buf; // 버퍼 string
	stringstream ss(str); // string을 stream에 넣는다
	while (ss >> buf)
		tokens.push_back(buf);
}

//--------------------------------------------------------------
//|
//|문자열 중에서 특정 문자열을 찾아 본다
//|
//---------------------------------------------------------------
BOOL CStringstatic::findstring(const char* str ,const char*  fstr)
{
	vecChar _chVec(str, str + strlen(str));
	vecChar _CchVec(fstr, fstr + strlen(fstr));

	vecChar::iterator ppnt;
	ppnt = search(_chVec.begin(), _chVec.end(), _CchVec.begin(), _CchVec.end());						// Search Algorithm
	if ( ppnt == _chVec.end())
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CStringstatic::findCString(CString str ,CString  fstr)
{
	// Search Algorithm
	if (str.Find(fstr)==-1)
	{
		return FALSE;
	}
	return TRUE;
}
LPCTSTR CStringstatic::FindChars(LPCTSTR p1, LPCTSTR p2)
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
CString CStringstatic::DeleteChar(LPCSTR szStr, char ch)
{
	CString value="";
	TCHAR szTmp[256]={0,};
	int d=0;
	for( int i = 0; szStr[i] != '\0'; i++ )
	{
		if( ch == szStr[i] )
			continue;
		szTmp[d++] = szStr[i];
	}
	szTmp[d] = '\0';
	value=szTmp;
	return value;
}
int CStringstatic::BinaryToken( int nPos , BYTE Source[] , LPSTR& FindData , int nCmpLen , int nTotal )
{
	int nNewPos = 0 ; 
	int nSearch = -1 ;
	int j = 0 ;
	for ( int i = nPos ; i < nTotal ; i++ )
	{
		nSearch = memcmp( &Source[i] , &FindData[0] , nCmpLen ) ;
		if ( nSearch == 0 ) // Find 
		{
			nNewPos = i + nCmpLen ;
			return nNewPos ;
		}
	}
	return -1 ;
}
CString CStringstatic::GetTokenString(CString link,int toknum, const tstring& delimiters)
{
	vecString vecItem;
	Tokenize((LPSTR)(LPCTSTR)link, vecItem,delimiters);
	return vecItem[toknum];
}