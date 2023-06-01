
#include "stdAfx.h"
#include <io.h>
#include <time.h>
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <conio.h>
using namespace std;

#define HKM_BUF 1024
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CFileHelpstatic::CFileHelpstatic(void)
{

}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CFileHelpstatic::~CFileHelpstatic(void)
{
}
//--------------------------------------------------------------
//|
//|파일이나 폴더 이름이 있는가를 찾는다
//|
//---------------------------------------------------------------
BOOL CFileHelpstatic::FindFileAndFolder(CString findfile)
{
	//파일 유무 체크
	if(PathFileExists(findfile))
	{
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|특정 디렉토리에 있는 파일를 삭제한다
//|
//---------------------------------------------------------------
BOOL CFileHelpstatic::DirectoryFindFileDelete(CString path, CString deletepath)
{
	WIN32_FIND_DATA w32fd;
	CString strPath; 
	//아래의 path + "\\" + "*.*" 은 path 라는 경로 안의 모든 디렉토리와 파일을 얻겠다는 뜻 
	HANDLE hFindFile = FindFirstFile(path + "\\" + "*.*", &w32fd);
	if(hFindFile == INVALID_HANDLE_VALUE)
		return false;
	do{
		// 아래의 두 경우 다 w32fd.cFileName 이 이름이 된다.
		// 다시말하면 이것은 디렉토리인 경우에는 디렉토리의 이름,
		// 파일인 경우에는 파일의 이름이 되는 문자열이다.
		strPath=w32fd.cFileName;
		if(w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
			// 디렉토리인 경우
		}
		else
		{
			// 파일인 경우
			if((strPath.Left(deletepath.GetLength())==deletepath)||(deletepath==""))
				RemveFile(path+"\\"+ strPath);
		}

	}while(FindNextFile(hFindFile, &w32fd) != FALSE);
	FindClose(hFindFile);
	return TRUE;
}
void CFileHelpstatic::FindFiles(char *path)
{
	HANDLE hSearch = NULL;
	WIN32_FIND_DATAA wfd = {0};
	BOOL bResult = TRUE;
	char spath[MAX_PATH] = {0};
	char File[MAX_PATH] = {0};

	char Drive[_MAX_DRIVE];
	char Dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	hSearch = FindFirstFileA(path,&wfd);

	if(hSearch == INVALID_HANDLE_VALUE)    return;

	_splitpath_s(path,Drive,Dir,fname,ext);

	while(bResult)
	{
		if( wfd.dwFileAttributes &    FILE_ATTRIBUTE_DIRECTORY )
		{
			if(strcmp(wfd.cFileName,".") && strcmp(wfd.cFileName,".."))
			{
				wsprintf(spath,"%s%s%s\\*.*",Drive,Dir,wfd.cFileName);
				FindFiles(spath);
			}
		}
		else
		{
			// wsprintfA(File,wfd.cFileName);
			
		}
		bResult = FindNextFile(hSearch,&wfd);
	}
	FindClose(hSearch);
}
//--------------------------------------------------------------
//|
//|파일을 삭제한다
//|
//---------------------------------------------------------------
BOOL CFileHelpstatic::RemveFile(CString file)
{
	TCHAR *lpStr = (LPTSTR)(LPCTSTR)file;
	if(PathFileExists(lpStr))
	{
		SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
		BOOL b = DeleteFile(lpStr);
		if(b) return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|파일을 읽는다
//|
//---------------------------------------------------------------
//BOOL CFileHelpstatic::CreateReadFile(CString filename, AtlArrayString& file)
//{
//	TCHAR buf[MAX_PATH];
//	int len;
//	FILE *fp = _tfopen(filename,_T("rt"));
//	if(fp)
//	{
//		while(1)
//		{
//			_fgetts(buf,MAX_PATH,fp);
//			len  = _tcslen(buf);
//			if(len==0) break;
//			buf[len-2]='\0';
//			if(FindFileAndFolder(buf))
//			{
//				file.Add(buf);
//			}
//			buf[0]= '\0';
//		}
//		fclose(fp);
//	}
//	return FALSE;
//}
//--------------------------------------------------------------
//|
//|파일을 쓴다
//|
//---------------------------------------------------------------
//BOOL CFileHelpstatic::CreateWriteFile(CString filename, AtlArrayString& file)
//{
//	if(file.GetCount())
//	{
//		FILE *fp = _tfopen(filename,_T("wt"));
//		if(fp)
//		{
//			for(int i=0; i<(int)file.GetCount(); i++)
//			{
//				int len=0;
//				if(len = file[i].GetLength())
//				{
//					if(FindFileAndFolder(file[i]))
//					{
//						_fputts(file[i].GetBuffer(0),fp);
//						_fputts("\r\n",fp);
//					}
//				}
//			}
//			fclose(fp);
//		}
//	}
//	return FALSE;
//}
//--------------------------------------------------------------
//|
//|파일을 읽는다
//|
//---------------------------------------------------------------
BOOL CFileHelpstatic::CreateWriteFile(LPTSTR filename, LPTSTR file)
{
	DWORD dwWrite;
	HANDLE hFile = CreateFile(filename,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	DWORD sz = GetFileSize(hFile,NULL);
	DWORD dwW = SetFilePointer(hFile,0,NULL,FILE_END);
	if(dwW==0)
	{
		WORD wd = 0xFEFF;
		WriteFile(hFile,&wd,2,&dwWrite,NULL);
	}
	else
	{
		WriteFile(hFile,_T("\r\n"),4,&dwWrite,NULL);
	}
	if (hFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile,file,(DWORD)_tcslen(file)*sizeof(TCHAR),&dwWrite,NULL);
		CloseHandle(hFile);
		return TRUE;
	}
	return FALSE;
}
DWORD CFileHelpstatic::readfile(CString sFName, LPBYTE& szReadFile)
{
	HANDLE hFile; 
	hFile = CreateFile(sFName,		   // open .geb 					
		GENERIC_READ,              // open for reading 
		FILE_SHARE_READ,           // share for reading 
		NULL,                      // no security 
		OPEN_ALWAYS,             // existing file only 
		FILE_ATTRIBUTE_NORMAL,     // normal file 
		NULL);                     // no attr. template 
	if (hFile == INVALID_HANDLE_VALUE) 
	{ 
		return false;
	} 
	DWORD dwRead;
	DWORD dwSize=GetFileSize(hFile,NULL);
	szReadFile=new BYTE[dwSize+1];
	ReadFile(hFile,szReadFile,dwSize,&dwRead,NULL);
	CloseHandle(hFile);
	return dwRead;
}
//--------------------------------------------------------------
//|
//|파일을 읽고 쓰기
//|
//---------------------------------------------------------------
void CFileHelpstatic::CreateWriteFile(LPCTSTR filename, vecString file)
{
	FILE* fp;
	fopen_s(&fp, filename,"w");
	if(fp)
	{
		for(int fsize = 0; fsize< (int)file.size(); fsize++)
		{
			fprintf(fp,"%s\n",file[fsize]);
		}
		fclose(fp);
	}
}

int CFileHelpstatic::CreateReadFile(LPCTSTR filename, vecString& file)
{
	FILE* fp;
	fopen_s(&fp, filename,"r");
	if(fp)
	{
		TCHAR keyvalue[MAX_PATH];
		while(fgets(keyvalue,MAX_PATH,fp))
		{
			keyvalue[strlen(keyvalue)-1]='\0';
			file.push_back(keyvalue);
		}
		fclose(fp);
	}
	return(int) file.size();
}
//--------------------------------------------------------------
//|
//|폴더를 선택한다
//|
//---------------------------------------------------------------
CString CFileHelpstatic::SelectFolder()
{
	BROWSEINFO   bi;
	LPITEMIDLIST pidl;
	TCHAR        path[MAX_PATH] = { 0 };
	ZeroMemory (&bi,sizeof(BROWSEINFO));
	bi.ulFlags        = 0;
	bi.lpszTitle      = NULL;
	bi.pszDisplayName = path;
	pidl              = SHBrowseForFolder ( &bi );
	if ( pidl != 0 )
	{
		if (SHGetPathFromIDList (pidl, path)) 
		{
			path;
		}
		// free memory used
		IMalloc * imalloc = 0;
		if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
		{
			imalloc->Free ( pidl );		imalloc->Release ( );
		}
	}
	return path;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CFileHelpstatic::FileSave (HWND hWnd)
{
	CFileDialog FileOpenDilaog (FALSE,
		_T("C:\\Key.Dat"),
		_T("default.txt"),          
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0"),
		hWnd);
	char path[MAX_PATH] = {0}; 
	//현재의 작업경로를 얻어와 저장 한다. 
	GetCurrentDirectory(MAX_PATH, path); 
	if (FileOpenDilaog.DoModal() == IDOK)
	{
		SetCurrentDirectory(path); 
 		return FileOpenDilaog.m_szFileName;
	}
	return "";
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CFileHelpstatic::FileOpen(HWND hWnd)
{
	CFileDialog FileOpenDilaog (TRUE,
		_T("bin"),
		NULL,
		OFN_HIDEREADONLY,
		_T("Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0"),
		hWnd);
	char path[MAX_PATH] = {0}; 
	//현재의 작업경로를 얻어와 저장 한다. 
	GetCurrentDirectory(MAX_PATH, path); 
	if (FileOpenDilaog.DoModal() == IDOK)
	{
		SetCurrentDirectory(path); 
		return FileOpenDilaog.m_szFileName;
	}

	return "";
}

