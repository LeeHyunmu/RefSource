
#ifndef __FILEHELPSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __FILEHELPSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _SFILE  CFileHelpstatic
class CFileHelpstatic
{
public:
	//������
	CFileHelpstatic(void);
	//�Ҹ���
	~CFileHelpstatic(void);
	//Folder�� ã�´�
	static BOOL FindFileAndFolder(CString findfile);
	//Ư�� ���丮 ���� ����� �����Ѵ�
	static BOOL DirectoryFindFileDelete(CString path, CString deletepath="");
	//���丮 �˻�
	static void FindFiles(char *path);
	//������ �����Ѵ�
	static BOOL RemveFile(CString file);
	////
	//static BOOL CreateReadFile(CString filename, AtlArrayString& file);
	////
	//static BOOL CreateWriteFile(CString filename, AtlArrayString& file);
	//
	static BOOL CreateWriteFile(LPTSTR filename, LPTSTR file);
	//���� �б�
	static DWORD readfile(CString sFName, LPBYTE& szReadFile);
	//
	static void CreateWriteFile(LPCTSTR filename, vecString file);
	static int CreateReadFile(LPCTSTR filename, vecString& file);
	//��ū ��Ʈ�� ����
	//������ �����Ѵ�
	static CString SelectFolder();
	//���� ����
	static CString FileSave (HWND hWnd);
	//���� ����
	static CString FileOpen(HWND hWnd);
};
#endif// __FILEHELPSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
