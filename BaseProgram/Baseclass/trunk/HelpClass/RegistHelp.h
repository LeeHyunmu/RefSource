
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
	//생성자
	CRegistHelp(void);
	//소멸자
	~CRegistHelp(void);
	//시작 프로그램 레지스트리 등록
	static void	CreateRegRun(LPCTSTR regValueName, const char* pBuffer);
	//
	static int RegAccessChange(HKEY hRKey,LPCTSTR pReg);
	static BOOL FindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindKeyValue);
	//서브키 찾는 함수
	static BOOL FindSubKeyEnum(HKEY hRKey, TCHAR *pRegName, TCHAR *pFindSubKey);
	static int RegDeleteKeyRecurse(HKEY hKey, LPCTSTR pRegName, char *svKeyBuf);
	static void DeleteRegKey(HKEY hRKey,TCHAR *pRegName);
	//서브키를 스트링으로 가지온다
	static size_t GetSubKeyEnum(HKEY hRKey, TCHAR *pRegName, vector<CString>* KeyString);
	//키에 대한 벨류값들을 담아온다
	static size_t GetFindRegKeyEnum(HKEY hRKey, TCHAR *pRegName, RegData& ValueString);
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);

	//레지스트 쓰기 위한 키생성
	static HKEY CreateWriteKey(HKEY KeyOp, LPCTSTR subkey, BYTE opt=1);
	//위 생성한 키리턴값으로 스트링(문자)을 추가한다
	static void SetHkeyWriteStringData(HKEY Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0);
	//위 두함수를 합친함수다
	static HKEY CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0, HKEY KeyOp=NULL, LPCTSTR subkey=NULL,BYTE opt=1);
	//레지스트리 키값에 해당하는 스트링을 읽어온다
	static CString GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename);
	//스마트카드 다중 접속 레지스트리 등록
	static void SetRegMultiSmartCard();
};
#endif// __REGISTHELP_B7A15A9D01DC403996FC45F7F92F52D3_H__
