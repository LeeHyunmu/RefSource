
#ifndef __STRINGSTATIC_H__
#define __STRINGSTATIC_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif 

class CTypeChangHelp
{
public:
	//������
	CTypeChangHelp(void);
	//�Ҹ���
	~CTypeChangHelp(void);
	//���� ���ڷ�
	static inline void NtoA(int &n);
	//�빮�� �ҹ��ڷ�
	static inline void UtoL(char &c);
	// �ҹ��� �빮�ڷ�
	static inline void LtoU(char &c);

	//string wstring�� ��ȯ
	static std::wstring AnsiToUnicode(const std::string& s);
	//�����ڵ�
	static std::string wstringTostring(const std::wstring& strSrc);
	//bstr CString����ȯ
	static CString bstrToCString(BSTR bstr);
	//���Ϳ��� ���ؽ� ���� ���� �Ѵ�
	static BOOL DeleteVector(vector<CString>& str ,int  index);
	//���ڿ� �߿��� Ư�� ���ڿ��� ã�� �Լ�
	static BOOL findstring(const char* str ,const char*  fstr);
	static BOOL findCString(CString str ,CString  fstr);

	//Applet�� �н����带 �����Ҷ� hexƮ���ڷ� ��ȯ�ϱ� ���� �Լ�
	static CString Changcharbyhex(CString pass);
	//
	static CString Changbytetohex(BYTE* byt, int len);
	//1Byte���ڸ� Hex2Byte��
	static CString Chang1Byteby2Hex(CString pass);
	//�ٻ� ��Ʈ������ ���� ��Ʈ�������� ��ȯ
	static CString HexStringToCharString(CString st);
	//bk��Ʈ�� ���� �о ��ȯ�ϰ� bk���� �����Ѵ�(
	static int HexStringToCharString(CString st,CString& revel,CString bk);

	//STL �˰������� ,(�޸�)�� ��Ʈ���� �и��Ͽ� �ѱ��
	static void Tokenize(const tstring& str, vector<int>& tokens, const string& delimiters=",");
	static void Tokenize(const tstring& str, vector<short>& tokens, const string& delimiters=",");
	//STL �˰������� ,(�޸�)�� ��Ʈ���� �и��Ͽ� �ѱ��
	static void Tokenize(const tstring& str, vector<CString>& tokens, const tstring& delimiters=",");
	//�������� ��ū �и�
	static void Tokenize(const tstring& str, vector<tstring>& tokens);
	//ó������ �ɸ��� ���ڵ��� �˻��Ѵ�
	static LPCTSTR FindChars(LPCTSTR p1, LPCTSTR p2);

	//
	static CString byteToHexbyteValue(byte* data, int len);
	//
	static int  ChangeHexCord(CString data, BYTE* hex, int maxlen);
};
#endif// __STRINGSTATIC_H__
