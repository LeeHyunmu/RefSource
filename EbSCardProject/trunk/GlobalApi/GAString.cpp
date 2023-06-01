
#include "stdAfx.h"
#include <sstream>
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
GAString::GAString(void)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
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
//|��Ʈ�� �����ڷ� ��Ʈ���� �з��Ͽ� int�� �ѱ�� vector
//|
//---------------------------------------------------------------
void GAString::GetTokenize(const tstring& str, vector<int>& tokens, const tstring& delimiters)
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
void GAString::GetTokenize(const tstring& str, vector<short>& tokens, const tstring& delimiters)
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
void GAString::GetTokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters)
{
	//�� ù ���ڰ� �������� ��� ����
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
//|stringstream���� ��ū ����
//|
//---------------------------------------------------------------
void GAString::GetTokenize(const tstring& str, vector<tstring>& tokens)
{
	string buf; // ���� string
	stringstream ss(str); // string�� stream�� �ִ´�
	while (ss >> buf)
		tokens.push_back(buf);
}