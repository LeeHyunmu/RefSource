
#ifndef __HCREGISTER_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __HCREGISTER_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class HCRegister
{
public:
	//
	typedef CAtlMap<CString,CString, CStringElementTraits<CString> > RegData;
	//������
	HCRegister(void);
	//�Ҹ���
	~HCRegister(void);
	//
	static int RegAccessChange(HKEY hRKey,LPCTSTR pReg);
	static BOOL FindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindKeyValue);
	//����Ű ã�� �Լ�
	static BOOL FindSubKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindSubKey);

	//����Ű�� ��Ʈ������ �����´�
	static size_t GetSubKeyEnum(HKEY hRKey, TCHAR *pRegName, vector<CString>* KeyString);
	//Ű�� ���� ���������� ��ƿ´�
	static size_t GetFindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, RegData& ValueString);
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);

	//�� ���Լ��� ��ģ�Լ���
	static HKEY CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0, HKEY KeyOp=NULL, LPCTSTR subkey=NULL,BYTE opt=1);
	//������Ʈ�� Ű���� �ش��ϴ� ��Ʈ���� �о�´�
	static CString GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename);
};
#endif// __HCREGISTER_B7A15A9D01DC403996FC45F7F92F52D3_H__
