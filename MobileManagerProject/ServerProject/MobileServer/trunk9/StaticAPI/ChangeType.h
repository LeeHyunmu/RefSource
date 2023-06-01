
#ifndef __CHANGETYPE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __CHANGETYPE_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _TYPE  CChangeType
class CChangeType
{
public:
	//������
	CChangeType(void);
	//�Ҹ���
	~CChangeType(void);

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

	//Applet�� �н����带 �����Ҷ� hexƮ���ڷ� ��ȯ�ϱ� ���� �Լ�
	static CString Changcharbyhex(CString pass);
	//
	static CString Changbytetohex(BYTE* byt, int len);
	//1Byte���ڸ� Hex2Byte��
	static CString Chang1Byteby2Hex(CString pass);
	//�ٻ� ��Ʈ������ ���� ��Ʈ�������� ��ȯ
	static CString HexStringToCharString(CString st);
	//bk��Ʈ�� ���� �о ��ȯ�ϰ� bk���� �����Ѵ�
	static int HexStringToCharString(CString st,CString& revel,CString bk);
	//�ɸ��͸� ���� ��ȯ
	static void StrToBCD(LPSTR&StrData, int BCDNum, BYTE* Result);
	//BYTE ��縦 BYTE �ɸ��ͷ� ��ȯ
	static void BCDToStr(BYTE* BCDData, int BCDNum, BYTE* Result);
	//��� ��Ʈ���� ��� ����Ʈ��
	static int  ChangeHexCord(CString data, LPBYTE hex);
	static CString  ChangeHexCord(BYTE* data, int len, CString& hex);
};
#endif// __CHANGETYPE_B7A15A9D01DC403996FC45F7F92F52D3_H__
