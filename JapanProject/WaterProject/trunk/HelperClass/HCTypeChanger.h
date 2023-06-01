
#ifndef __HCTYPECHANGER_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __HCTYPECHANGER_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif 

class HCTypeChanger
{
public:
	//������
	HCTypeChanger(void);
	//�Ҹ���
	~HCTypeChanger(void);
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
	// Hex string ������ ��ȯ
	static CString byteToHexbyteValue(byte* data, int len);
	//Hex string �� hex byte�� ��ȯ
	static int  ChangeHexCord(CString data, BYTE* hex, int& hexlen);
	//ó������ �ɸ��� ���ڵ��� �˻��Ѵ�
	static LPCTSTR FindChars(LPCTSTR p1, LPCTSTR p2);
};
#endif// __HCTYPECHANGER_B7A15A9D01DC403996FC45F7F92F52D3_H__
