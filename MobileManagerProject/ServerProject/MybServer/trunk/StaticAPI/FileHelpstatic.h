
#ifndef __FILEHELPSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __FILEHELPSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _SFILE  CFileHelpstatic
class CFileHelpstatic
{
public:
	//생성자
	CFileHelpstatic(void);
	//소멸자
	~CFileHelpstatic(void);
	//Folder를 찾는다
	static BOOL FindFileAndFolder(CString findfile);
	//특정 디렉토리 파일 목록을 삭제한다
	static BOOL DirectoryFindFileDelete(CString path, CString deletepath="");
	//디렉토리 검색
	static void FindFiles(char *path);
	//파일을 삭제한다
	static BOOL RemveFile(CString file);
	////
	//static BOOL CreateReadFile(CString filename, AtlArrayString& file);
	////
	//static BOOL CreateWriteFile(CString filename, AtlArrayString& file);
	//
	static BOOL CreateWriteFile(LPTSTR filename, LPTSTR file);
	//파일 읽기
	static DWORD readfile(CString sFName, LPBYTE& szReadFile);
	//
	static void CreateWriteFile(LPCTSTR filename, vecString file);
	static int CreateReadFile(LPCTSTR filename, vecString& file);
	//토큰 스트링 리턴
	//폴더를 선택한다
	static CString SelectFolder();
	//파일 저장
	static CString FileSave (HWND hWnd);
	//파일 열기
	static CString FileOpen(HWND hWnd);
};
#endif// __FILEHELPSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
