
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|持失切
//|
//---------------------------------------------------------------
GAFileStream::GAFileStream(void)
{
}
//--------------------------------------------------------------
//|
//|社瑚切
//|
//---------------------------------------------------------------
GAFileStream::~GAFileStream(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL GAFileStream::CreateLineReadFile(CString filename, vecFileStreamString& vecfile)
{
	TCHAR fullpath[MAX_PATH];
	_stprintf_s(fullpath,MAX_PATH ,_T("%s\\%s"),_T("\\NandFlash\\PhoneP2p"), filename);
	TCHAR buf[MAX_PATH];
	int len;
	FILE* fp;
	_tfopen_s(&fp, filename, _T("rt"));
	if(fp)
	{
		 while (TRUE)
		 {
			 memset(buf, 0, sizeof(buf));
			 _fgetts(buf, MAX_PATH, fp);
			 len = _tcslen(buf);
			 if(len==0) break;
			 if(buf[len-2]=='\r')
			 {
				 buf[len-2]='\0';
			 }
			 else if(buf[len-1]=='\n')
			 {
				buf[len-1]='\0';
			 }
			 else
			 {
				 buf[len]='\0';
			 }
			 vecfile.push_back(buf);
		 }
		 fclose(fp);
		 return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL GAFileStream::CreateLineWriteFile(CString filename, vecFileStreamString& vecfile)
{
	TCHAR fullpath[MAX_PATH];
	_stprintf_s(fullpath,MAX_PATH ,_T("%s\\%s"),_T("\\NandFlash\\PhoneP2p"), filename);
	FILE* fp;
	_tfopen_s(&fp, filename, _T("wt"));
	if(fp)
	{
		for (int i=0; i<(int)vecfile.size();i++)
		{
			if(vecfile[i].GetLength())
			{
				_fputts(vecfile[i].GetBuffer(vecfile[i].GetLength()),fp);
				_fputts(_T("\r\n"),fp);
			}
		}
		fclose(fp);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
#define GA_CRLP_CODE _T("\r\n")
BOOL GAFileStream::CreateFEFFWriteFile(CString filename, LPTSTR file)
{
	HANDLE hFile;
	hFile = CreateFile(filename.GetBuffer(filename.GetLength()),GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,
		               FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		DWORD dwWritten;
		DWORD dwfilesz = GetFileSize(hFile, NULL);
		if(dwfilesz==0)
		{
			WORD wd = 0xFEFF;
			WriteFile(hFile, &wd, sizeof(WORD),&dwWritten,NULL);
		}
		else
		{
			DWORD dwfilepointer = SetFilePointer(hFile, 0, NULL, FILE_END);
			WriteFile(hFile, GA_CRLP_CODE, _tcslen(GA_CRLP_CODE)*sizeof(TCHAR),&dwWritten,NULL);
		}
		WriteFile(hFile, file, _tcslen(file)*sizeof(TCHAR),&dwWritten,NULL);
		CloseHandle(hFile);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL GAFileStream::CreateByteReadFile(CString filename, LPBYTE& bytefile, DWORD& dwFileSize)
{
	HANDLE hFile;
	hFile = CreateFile(filename.GetBuffer(filename.GetLength()),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		DWORD dwRead;
		DWORD dwfilesz = GetFileSize(hFile, NULL);
		bytefile = new BYTE[dwfilesz+1];
		ReadFile(hFile, bytefile, dwfilesz ,&dwRead,NULL);
		CloseHandle(hFile);
		if(dwfilesz==dwRead)
		{
			dwFileSize=dwRead;	return TRUE;
		}
	}
	return FALSE;
}
