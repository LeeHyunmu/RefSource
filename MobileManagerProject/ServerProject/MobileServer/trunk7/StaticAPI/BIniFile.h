// BIniFile.h: interface for the CBIniFile class.
// CBIniFile
// : INI 파일을 쉽게 사용하기 위한 Wrapper Class
// : 복영철
// : 2003. 3. 10
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINIFILE_H__695DB7A2_39EB_4476_A340_43842C207239__INCLUDED_)
#define AFX_BINIFILE_H__695DB7A2_39EB_4476_A340_43842C207239__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBIniFile  
{
public:
	//생성자 full path 파일이름 준다
	CBIniFile(LPCTSTR lpFileName);
	CBIniFile();
	//소멸자
	virtual ~CBIniFile();
	//파일 이름을 가져온다
	inline LPCTSTR GetPathName() { return m_szFileName; };
	//구조체를 저정한다(암호화 효과
	BOOL GetProfileStruct(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPVOID lpStruct, UINT uSizeStruct);
	//정수형 데이타를 가져온다
	UINT GetProfileInt(LPCTSTR lpszSection, LPCTSTR lpKeyName, INT nDefault);
	//
	DWORD GetProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize);
	DWORD GetProfileSection(LPCTSTR lpszSection, LPTSTR lpReturnedString, DWORD nSize);
	DWORD GetProfileSectionNames(LPTSTR lpszReturnBuffer, DWORD nSize);
	BOOL WriteProfileStruct(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPVOID lpStruct, UINT uSizeStruct);
	BOOL WriteProfileString(LPCTSTR lpszSection, LPCTSTR lpKeyName, LPCTSTR lpString);
	BOOL WriteProfileSection(LPCTSTR lpszSection, LPCTSTR lpString);
	void SetFileName(LPCTSTR lpFileName);
	BOOL WriteProfileint(LPCTSTR lpszSection, LPCTSTR lpKeyName, int ndata);
private:
	TCHAR m_szFileName[MAX_PATH];	//INI 파일이 저장되는 풀패스.
							//NULL일 경우 win.ini에 저장된다.
							//경로가 없을 경우 windows폴더에 저장된다.
};

#endif // !defined(AFX_BINIFILE_H__695DB7A2_39EB_4476_A340_43842C207239__INCLUDED_)
