
#include "stdAfx.h"
#include <sstream>
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
GAString::GAString(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
GAString::~GAString(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|스트링 구분자로 스트링을 분류하여 int로 넘긴다 vector
//|
//---------------------------------------------------------------
void GAString::GetTokenize(const tstring& str, vector<int>& tokens, const tstring& delimiters)
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
void GAString::GetTokenize(const tstring& str, vector<short>& tokens, const tstring& delimiters)
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
void GAString::GetTokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters)
{
	//맨 첫 글자가 구분자인 경우 무시
	tstring::size_type lastPos = str.find_first_not_of(delimiters,0);
	tstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while(tstring::npos!=pos||tstring::npos!=lastPos)
	{
		tokens.push_back(str.substr(lastPos, pos-lastPos).c_str());
		lastPos = str.find_first_not_of(delimiters, pos);
		pos=str.find_first_of(delimiters, lastPos);
	}
}
//--------------------------------------------------------------
//|
//|stringstream으로 토큰 구분
//|
//---------------------------------------------------------------
void GAString::GetTokenize(const tstring& str, vector<tstring>& tokens)
{
	string buf; // 버퍼 string
	stringstream ss(str); // string을 stream에 넣는다
	while (ss >> buf)
		tokens.push_back(buf);
}