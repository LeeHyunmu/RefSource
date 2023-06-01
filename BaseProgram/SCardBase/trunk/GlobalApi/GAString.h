
#ifndef __GASTRING_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GASTRING_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GASRING  GAString
class GAString
{
public:
	//������
	GAString(void);
	//�Ҹ���
	~GAString(void);
	//======================================
	//
	//=======================================
	//STL �˰������� �⺻���� ,(�޸�)�� ��Ʈ���� �и��Ͽ� �ѱ��
	//int
	static void GetTokenize(const tstring& str, vector<int>& tokens, const string& delimiters=",");
	//short
	static void GetTokenize(const tstring& str, vector<short>& tokens, const string& delimiters=",");
	//CString
	static void GetTokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters=",");
	//�������� ��ū �и�
	static void GetTokenize(const tstring& str, vector<tstring>& tokens);
};
#endif// __GASTRING_B7A15A9D01DC403996FC45F7F92F52D3_H__
