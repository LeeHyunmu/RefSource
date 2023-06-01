
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
	//생성자
	HCRegister(void);
	//소멸자
	~HCRegister(void);
	//
	static int RegAccessChange(HKEY hRKey,LPCTSTR pReg);
	static BOOL FindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindKeyValue);
	//서브키 찾는 함수
	static BOOL FindSubKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindSubKey);

	//서브키를 스트링으로 가지온다
	static size_t GetSubKeyEnum(HKEY hRKey, TCHAR *pRegName, vector<CString>* KeyString);
	//키에 대한 벨류값들을 담아온다
	static size_t GetFindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, RegData& ValueString);
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);

	//위 두함수를 합친함수다
	static HKEY CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0, HKEY KeyOp=NULL, LPCTSTR subkey=NULL,BYTE opt=1);
	//레지스트리 키값에 해당하는 스트링을 읽어온다
	static CString GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename);
};
#endif// __HCREGISTER_B7A15A9D01DC403996FC45F7F92F52D3_H__
