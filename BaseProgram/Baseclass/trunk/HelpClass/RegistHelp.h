
#ifndef __REGISTHELP_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __REGISTHELP_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRegistHelp
{
public:
	//
	typedef CAtlMap<CString,CString, CStringElementTraits<CString> > RegData;
	//������
	CRegistHelp(void);
	//�Ҹ���
	~CRegistHelp(void);
	//���� ���α׷� ������Ʈ�� ���
	static void	CreateRegRun(LPCTSTR regValueName, const char* pBuffer);
	//
	static int RegAccessChange(HKEY hRKey,LPCTSTR pReg);
	static BOOL FindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindKeyValue);
	//����Ű ã�� �Լ�
	static BOOL FindSubKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindSubKey);
	static int RegDeleteKeyRecurse(HKEY hKey, LPCTSTR pRegName, char *svKeyBuf);
	static void DeleteRegKey(HKEY hRKey,TCHAR *pRegName);
	//����Ű�� ��Ʈ������ �����´�
	static size_t GetSubKeyEnum(HKEY hRKey, TCHAR *pRegName, vector<CString>* KeyString);
	//Ű�� ���� ���������� ��ƿ´�
	static size_t GetFindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, RegData& ValueString);
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);

	//������Ʈ ���� ���� Ű����
	static HKEY CreateWriteKey(HKEY KeyOp, LPCTSTR subkey, BYTE opt=1);
	//�� ������ Ű���ϰ����� ��Ʈ��(����)�� �߰��Ѵ�
	static void SetHkeyWriteStringData(HKEY Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0);
	//�� ���Լ��� ��ģ�Լ���
	static HKEY CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0, HKEY KeyOp=NULL, LPCTSTR subkey=NULL,BYTE opt=1);
	//������Ʈ�� Ű���� �ش��ϴ� ��Ʈ���� �о�´�
	static CString GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename);
	//����Ʈī�� ���� ���� ������Ʈ�� ���
	static void SetRegMultiSmartCard();
};
#endif// __REGISTHELP_B7A15A9D01DC403996FC45F7F92F52D3_H__
