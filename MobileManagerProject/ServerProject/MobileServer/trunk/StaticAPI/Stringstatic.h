
#ifndef __STRINGSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __STRINGSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _STRING  CStringstatic
class CStringstatic
{
public:
	//������
	CStringstatic(void);
	//�Ҹ���
	~CStringstatic(void);

	//���ڿ� �߿��� Ư�� ���ڿ��� ã�� �Լ�
	static BOOL findstring(const char* str ,const char*  fstr);
	static BOOL findCString(CString str ,CString  fstr);

	//STL �˰������� ,(�޸�)�� ��Ʈ���� �и��Ͽ� �ѱ��
	static void Tokenize(const tstring& str, vecInt& tokens, const std::string& delimiters=",");
	static void Tokenize(const tstring& str, vecShort& tokens, const std::string& delimiters=",");
	//STL �˰������� ,(�޸�)�� ��Ʈ���� �и��Ͽ� �ѱ��
	static void Tokenize(const tstring& str, vecString& tokens, const tstring& delimiters=",");
	//�������� ��ū �и�
	static void Tokenize(const tstring& str, std::vector<tstring>& tokens);
	//ó������ �ɸ��� ���ڵ��� �˻��Ѵ�
	static LPCTSTR FindChars(LPCTSTR p1, LPCTSTR p2);
	//Ư�����ڸ� �����Ѵ�
	static CString DeleteChar(LPCSTR szStr, char ch);
	//
	static int BinaryToken( int nPos , BYTE Source[] , LPSTR& FindData , int nCmpLen , int nTotal );
	//��ū ��Ʈ���� �ѱ��(��Ʈ��, �ѱ� ��ū�� ����, ��Ȱ��ū
	static CString GetTokenString(CString link,int toknum, const tstring& delimiters=",");
};
#endif// __STRINGSTATIC_H__
