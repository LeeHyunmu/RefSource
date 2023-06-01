
#include "StdAfx.h"
#include <afxdlgs.h>
#include <deque>
#include <windows.h>
#include <atlstr.h>
#include <Shlwapi.h>
#include <WtsApi32.h>
#include <TlHelp32.h>
#include <io.h>
#include <UserEnv.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <fcntl.h>
#include <algorithm>
#include <Dbt.h>
#include "ExFileDialog.h"
#include "staticmyfile.h"
using namespace std;

#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "userenv.lib")
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "winmm.lib")
//#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "winscard.lib")
#pragma comment(lib, "setupapi.lib")

///GUID 링크
#include <initguid.h>
//#pragma comment(lib, "rpcrt4.lib")  

DEFINE_GUID(  USB_GUID, 0x36FC9E60, 0xC465, 0x11CF, 0x80, 0x56,0x44, 0x45,0x53,0x54,0x00,0x00);
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CStaticMyFile::CStaticMyFile(void)
{

}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CStaticMyFile::~CStaticMyFile(void)
{
}
//--------------------------------------------------------------
//|
//| WindowFromProcessID 함수의 콜백
//|
//---------------------------------------------------------------
BOOL CALLBACK CStaticMyFile::EnumWindowsProc(HWND hWindow, PIDANDHWND *lParam)
{
	char sClass[513] = { 0, };
	char sCaption[1025] = { 0, };

	DWORD pId;

	GetClassName( hWindow, sClass, 512 );
	if( strcmp( sClass, lParam->clsname.c_str() ) == 0 ) //class이름이 같은지 확인
	{
		GetWindowThreadProcessId(hWindow,&pId);
		if(pId==lParam->pID)                            // 같은 process인지 확인
		{
			lParam->hWindow = hWindow;
			return FALSE;
		}
	}

	return true;

}
//--------------------------------------------------------------
//|
//| 클래스네임과 프로세서 id로 프로세서를 찾아서 CWnd* 형을 넘겨준다
//|
//---------------------------------------------------------------
CWnd* CStaticMyFile::WindowFromProcessID(DWORD pID,string sClsname)
{
	PIDANDHWND Param;
	Param.pID     = pID;
	Param.hWindow = 0;
	Param.clsname=sClsname;
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&Param);
	return CWnd::FromHandle(Param.hWindow);

}
//--------------------------------------------------------------
//|
//|로그 출력 함수
//|
//---------------------------------------------------------------
//디버그로 출력 또는 로그파일로  상황을 출력 해준다
#define  _LOGFILE // <=======출력 디파인 값을 설정해야 출력한다
//#define  _MODULENAME_
int CStaticMyFile::logfile(char* Format,...)
{
#ifdef _LOGFILE
	va_list		ap;
	char		sBuf[4096];

	va_start(ap, Format); 
	vsprintf(sBuf, Format, ap);
	va_end(ap);
	char CurrentName[256];
	GetModuleFileName(0,CurrentName,sizeof(CurrentName));
	char * ActiveName =strrchr(CurrentName,'\\');
	ActiveName++;

#ifndef _DEBUG
	int		file;
	time_t  t;
	struct  tm* tm;
	t = time(NULL);
	memset(&tm,0,sizeof(struct  tm));
	tm = localtime(&t);
	char	wrt_buff[4096+1];
	char	wrt_file[100];
	memset(wrt_file, 0x00, sizeof(wrt_file));
#ifndef _MODULENAME_
	sprintf(wrt_file, "C:\\Anyq\\log\\%s_LOG%04d%02d%02d.log",ActiveName, 1900+tm->tm_year, tm->tm_mon + 1, tm->tm_mday);
#else
	sprintf(wrt_file, "%s_LOG%04d%02d%02d.log",CurrentName, 1900+tm->tm_year, tm->tm_mon + 1, tm->tm_mday);
#endif

	if(!FindFileAndFolder(wrt_file))
	{
		if(!FindFileAndFolder("C:\\Anyq\\log"))
		{
			CreateDirectory("C:\\Anyq",NULL);
			CreateDirectory("C:\\Anyq\\log",NULL);
		}
		else
			DirectoryFindFileDelete("C:\\Anyq\\log",ActiveName);
	}
	if((file = _open((char *)wrt_file, _O_WRONLY | _O_CREAT, _S_IREAD | _S_IWRITE | _O_TEXT)) < 0)
	{
		return (-1);
	}
	_lseek(file, 0L, SEEK_END);
	memset(wrt_buff, 0x00, sizeof(wrt_buff));
	sprintf(wrt_buff,"%04d-%02d-%02d %02d:%02d:%02d %s\n", 1900+tm->tm_year, tm->tm_mon + 1, tm->tm_mday, 
		tm->tm_hour, tm->tm_min, tm->tm_sec,
		sBuf);

	_write(file, wrt_buff, strlen(wrt_buff));
	_close(file);
#else
	printf("%s_%s\n",ActiveName, sBuf);
#endif
#endif
	return 0;
}
//--------------------------------------------------------------
//|
//|파일다이얼로그로 선택된 파일의 이름 을 가지고 온다
//|
//---------------------------------------------------------------
CString CStaticMyFile::Filedialog(BOOL bOpSa, CString strFilter,DWORD dwListType, DWORD dwFlag) 
{
	CString returndata="";
	CExFileDialog::LISTVIEWCMD type;
	switch(dwListType)
	{
	case 0: type=CExFileDialog::LISTVIEWCMD::ODM_VIEW_THUMBS;
		break;
	case 1:type=CExFileDialog::LISTVIEWCMD::ODM_VIEW_ICONS;
		break;
	case 2:type=CExFileDialog::LISTVIEWCMD::ODM_VIEW_LIST;
		break;
	case 3:type=CExFileDialog::LISTVIEWCMD::ODM_VIEW_TILES;
		break;
	default:type=CExFileDialog::LISTVIEWCMD::ODM_VIEW_DETAIL;
		break;
	}
	CExFileDialog OpenFileDialog(TRUE, NULL, NULL, dwFlag, strFilter,NULL, type);
	if(OpenFileDialog.DoModal() == IDOK)
	{
		returndata = OpenFileDialog.GetPathName();  
	}
	return returndata;
}
//--------------------------------------------------------------
//|
//|다시 
//|
//---------------------------------------------------------------
CFile CStaticMyFile::FileRead(CString szFilePath)
{
	CFile hFile = CreateFile(szFilePath, GENERIC_READ, 0, NULL,OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	HANDLE hFileMapping  = CreateFileMapping(hFile, NULL, PAGE_WRITECOPY, 0, dwFileSize, NULL); //=>//메모리에 쓰여진 파일 내용을 확인 하자
	//함수 MapviewOfFile()는 리턴 타임이 LPVOID이며 (char*)szFileViewer캐스팅
	char* szFileViewer = (char*)MapViewOfFile(hFileMapping, FILE_MAP_COPY, 0,0, 0);
	return hFile;
}
//--------------------------------------------------------------
//|
//|char를 hex로 변환
//|
//---------------------------------------------------------------
CString CStaticMyFile::Changcharbyhex(CString pass)
{
	CString retvalue="";
	CString temp;
	char *a = pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format("%X", a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|스크링을 1바이트 문자를 2바이트 HEX로 변환한다
//|
//---------------------------------------------------------------
CString CStaticMyFile::Chang1Byteby2Hex(CString pass)
{
	CString retvalue="";
	CString temp;
	BYTE *a = (BYTE*)pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format("%02X", a[i]);
		retvalue += temp;
	}
	return retvalue;
}
#include <tlhelp32.h>
#include ".\staticmyfile.h"
//--------------------------------------------------------------
//|
//|프로세서를 죽인다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::ProcessKill(CString strProcessName)
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (hProcessSnap == (HANDLE)-1) 
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32); 
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32)) 
	{ 
		BOOL          bCurrent = FALSE; 
		MODULEENTRY32 me32       = {0}; 

		do 
		{ 
			bCurrent = KillProcessModule(pe32.th32ProcessID,strProcessName);
			if(bCurrent) 
			{ 
				if(bCurrent==2)
				{
					Sleep(1000); KillProcessModule(pe32.th32ProcessID,strProcessName);
				}
				CloseHandle (hProcessSnap); 
				return TRUE;
			} 
		} 
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	} 
	CloseHandle (hProcessSnap); 
	return FALSE;
}
//--------------------------------------------------------------
//|
//|프로세스모듈을 찾는다(동일한 프로세스가 있으면 TRUE, 아니면 FALSE)
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::GetProcessModule(DWORD dwPID, CString sProcessName)
{
	HANDLE        hModuleSnap = NULL; 
	MODULEENTRY32 me32        = {0}; 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 
	
	if (hModuleSnap == (HANDLE)-1) 
		return FALSE; 
	me32.dwSize = sizeof(MODULEENTRY32); 
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면 
	//true를 리턴한다.
	if(Module32First(hModuleSnap, &me32)) 
	{ 
		do 
		{ 
			if(me32.szModule == sProcessName)
			{ 
				CloseHandle (hModuleSnap); 
				return TRUE;
			} 
		} 
		while(Module32Next(hModuleSnap, &me32)); 
	} 
	CloseHandle (hModuleSnap); 
	return FALSE;
}
//--------------------------------------------------------------
//|
//|프로세스를 죽이는 TerminateProcess
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::KillProcessModule(DWORD dwPID, CString sProcessName)
{
	HANDLE        hModuleSnap = NULL; 
	MODULEENTRY32 me32        = {0}; 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 

	if (hModuleSnap == (HANDLE)-1) 
		return (FALSE); 

	me32.dwSize = sizeof(MODULEENTRY32); 
	//해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면 
	//true를 리턴한다.
	if(Module32First(hModuleSnap, &me32)) 
	{ 
		do 
		{ 
			if(me32.szModule == sProcessName)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID); 
				if(hProcess)
				{
					if(TerminateProcess(hProcess, 0))
					{
						ExitExe(hProcess);
						CloseHandle (hModuleSnap); 			return TRUE;
					}					
				}
				CloseHandle (hModuleSnap); 		return 2;
			} 
		} 
		while(Module32Next(hModuleSnap, &me32)); 
	} 
	CloseHandle (hModuleSnap); 
	return FALSE;
}
//--------------------------------------------------------------
//|
//|파일묵음을 복사한다
//|
//---------------------------------------------------------------
void CStaticMyFile::FileCopy(HWND hwnd, CString frompath, CString topath)
{
	SHFILEOPSTRUCT    shfo;
	TCHAR        pszTo[1024] = {0};
	TCHAR        pszFrom[1024] = {0};
	TCHAR        pszTitle[MAX_PATH] = {0};

	sprintf(pszTo,"%s\0\0",topath);
	sprintf(pszFrom,"%s\0\0",frompath);

	ZeroMemory(&shfo, sizeof shfo);
	shfo.hwnd = hwnd; 
	shfo.wFunc = FO_COPY;
	shfo.fFlags = FOF_SIMPLEPROGRESS|FOF_MULTIDESTFILES|FOF_NOCONFIRMATION ;//FOF_NOCONFIRMATION//노메시지 
	shfo.lpszProgressTitle=pszTitle;
	shfo.pTo=pszTo;
	shfo.pFrom=pszFrom;
	SHFileOperation(&shfo);
}
//--------------------------------------------------------------
//|
//|폴더전체를 카피한다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::CopyFolder(CString strFromFolder, CString strToFolder)
{
	int nLenFrom = strFromFolder.GetLength()+1;
	int nLenTo = strToFolder.GetLength()+1;
	LPCSTR lpszBufFrom = strFromFolder.GetBuffer(nLenFrom);
	LPCSTR lpszBufTo = strToFolder.GetBuffer(nLenTo);
	SHFILEOPSTRUCT fo = {0};
	fo.wFunc = FO_COPY;
	fo.pFrom = lpszBufFrom;
	fo.pTo = lpszBufTo;
	fo.fFlags = FOF_SILENT|FOF_NOERRORUI;
	int nRtn = SHFileOperation(&fo);
	strFromFolder.ReleaseBuffer();
	strToFolder.ReleaseBuffer();
	return (nRtn==0);
}
//--------------------------------------------------------------
//|
//|파일 디렉토리 전체를 삭제한다
//|
//---------------------------------------------------------------
int CStaticMyFile::RemoveDirectoryS(const char* FileName, HWND hwnd)
{
	int nResult;
	SHFILEOPSTRUCT FileOp = {0};
	char szTemp[MAX_PATH];
	strcpy(szTemp, FileName);
	szTemp[strlen(FileName)+1]=NULL;
	FileOp.hwnd = hwnd;
	FileOp.wFunc = FO_DELETE;
	FileOp.pFrom = szTemp;
	FileOp.pTo = NULL;
	FileOp.fFlags = FOF_NOCONFIRMATION|FOF_NOERRORUI;
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = NULL;
	nResult = SHFileOperation(&FileOp);
	return nResult;
}
//--------------------------------------------------------------
//|
//|파일이나 폴더 이름이 있는가를 찾는다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::FindFileAndFolder(CString findfile)
{
	//파일 유무 체크 및 Copy
	if(PathFileExists(findfile))
	{
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|파일을 삭제한다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::RemveFile(CString file)
{
	char *lpStr = (LPSTR)(LPCTSTR)file;
	if(PathFileExists(lpStr))
	{
		SetFileAttributes(file, FILE_ATTRIBUTE_NORMAL);
		CStdioFile::Remove(file);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|Rename될 파일이 있으면 삭제하고 Rename시킨다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::FileRemoveandRename(CString path,CString oldfile, CString newfile)
{
	CString lpStrnew = path+newfile;
	CString lpStrold = path+oldfile;
	if(PathFileExists(lpStrnew))
	{
		if(PathFileExists(lpStrold))
		{
			SetFileAttributes(lpStrold, FILE_ATTRIBUTE_NORMAL);		CStdioFile::Remove(lpStrold);	
		}
		CStdioFile::Rename(lpStrnew, lpStrold); 		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|프로그램 실행시 자동 실행할수 있도록 레지스트리 등록한다
//|
//---------------------------------------------------------------
void CStaticMyFile::CreateRegRun(LPCTSTR regValueName, const char* pBuffer)
{
	HKEY hkey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),0,KEY_ALL_ACCESS,&hkey);
	RegSetValueEx(hkey, regValueName, 0, REG_SZ, (const unsigned char*)pBuffer, strlen(pBuffer));
	RegCloseKey(hkey);
}
//--------------------------------------------------------------
//|
//|레지스트리 저장소를 만들거나 연다
//|
//---------------------------------------------------------------
HKEY CStaticMyFile::CreateWriteKey(HKEY KeyOp, LPCTSTR subkey, BYTE opt)
{

	HKEY key;
	DWORD dwDisp;
	DWORD sam;
	switch(opt)
	{
	case 0: sam=REG_OPTION_RESERVED;	break;
	case 1:sam=REG_OPTION_NON_VOLATILE;		break;
	case 2:sam=REG_OPTION_VOLATILE;		break;
	case 3:sam=REG_OPTION_CREATE_LINK;		break;
	case 4: sam=REG_OPTION_BACKUP_RESTORE;	break;
	case 5:sam=REG_OPTION_OPEN_LINK;		break;
	case 6:sam=REG_LEGAL_OPTION;		break;
	}
	if(KeyOp)
		RegCreateKeyEx( KeyOp, subkey, 0, NULL,	sam, KEY_WRITE, NULL,	&key, &dwDisp );
	return key;
}
//--------------------------------------------------------------
//|
//|열린 리지스트리에 쓴다
//|
//---------------------------------------------------------------
void CStaticMyFile::SetHkeyWriteStringData(HKEY Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose)
{
	if(Key)
	{
		RegSetValueEx(Key, valuename, 0, REG_SZ, (LPBYTE)lpData, strlen(valuename)+1);
	}
	if(keyclose) RegCloseKey(Key);
	return;
}
//--------------------------------------------------------------
//|
//|위 두함수를 합친함수 이다 사용방법은 잘...
//|
//---------------------------------------------------------------
HKEY CStaticMyFile::CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose, HKEY KeyOp, LPCTSTR subkey,BYTE opt)
{
	HKEY createkey=Key;
	DWORD dwDisp;
	DWORD sam;
	
	if(KeyOp)
	{
		RegCloseKey(createkey); createkey=NULL;;
		switch(opt)
		{
		case 0: sam=REG_OPTION_RESERVED;	break;
		case 1:sam=REG_OPTION_NON_VOLATILE;		break;
		case 2:sam=REG_OPTION_VOLATILE;		break;
		case 3:sam=REG_OPTION_CREATE_LINK;		break;
		case 4: sam=REG_OPTION_BACKUP_RESTORE;	break;
		case 5:sam=REG_OPTION_OPEN_LINK;		break;
		case 6:sam=REG_LEGAL_OPTION;		break;
		}
		RegCreateKeyEx( KeyOp, subkey, 0, NULL,	sam, KEY_WRITE, NULL, &createkey, &dwDisp );
	}
	if(createkey)
	{
		RegSetValueEx(createkey, valuename, 0, REG_SZ, (LPBYTE)lpData, strlen(valuename)+1);
	}
	if(keyclose){	RegCloseKey(createkey); createkey=NULL;}
	Key=createkey;
	return createkey;
}
//--------------------------------------------------------------
//|
//|레지스트리에 등록된 스트링값을 가지고 온다
//|
//---------------------------------------------------------------
CString CStaticMyFile::GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename)
{
	TCHAR keydata[256];
	CString returnvalue;
	DWORD dwType = REG_SZ;
	DWORD dwSize=sizeof(keydata);
	HKEY hKey;
	LONG lResult = RegOpenKeyEx(LKey,subkey, 0 , KEY_READ , &hKey);
	if (lResult == ERROR_SUCCESS )
		RegQueryValueEx(hKey , valuename, NULL , &dwType, (LPBYTE)keydata, &dwSize);

	returnvalue.Format(_T("%s"), keydata);
	RegCloseKey(hKey);
	return returnvalue;
}
//--------------------------------------------------------------
//|
//|프로세스가 있는가
//|
//---------------------------------------------------------------
bool CStaticMyFile::ExistProcess(CString strProcessName)
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (hProcessSnap == (HANDLE)-1) 
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32); 
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32)) 
	{ 
		BOOL          bCurrent = FALSE; 
		MODULEENTRY32 me32       = {0}; 

		do 
		{ 
			bCurrent =(BOOL) GetProcessModule(pe32.th32ProcessID,strProcessName);
			if(bCurrent) 
			{ 
				CloseHandle (hProcessSnap); 
				return true;
			} 
		} 
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	} 
	CloseHandle (hProcessSnap); 
	return false;
}
//--------------------------------------------------------------
//|
//|실행중인 프로세스가핸들 리턴
//|
//---------------------------------------------------------------
HANDLE CStaticMyFile::ProcessHandle(CString strProcessName)
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (hProcessSnap == (HANDLE)-1) 
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32); 
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	HANDLE hnd=NULL;
	if (Process32First(hProcessSnap, &pe32)) 
	{ 
		BOOL          bCurrent; 
		MODULEENTRY32 me32       = {0}; 

		do 
		{ 
			bCurrent = GetProcessModule(pe32.th32ProcessID,strProcessName);
			if(bCurrent) 
			{
				hnd=OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
				CloseHandle (hProcessSnap); 
				return hnd;
			} 
		} 
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	} 
	CloseHandle (hProcessSnap); 
	return NULL;
}
//--------------------------------------------------------------
//|
//|프로세스 목록 중에 있으면 모두 죽인다
//|
//---------------------------------------------------------------
bool CStaticMyFile::ExistKillProcess(std::deque<CString> dat)
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	BOOL          bCurrent = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (hProcessSnap == (HANDLE)-1) 
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32); 
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32)) 
	{ 
		
		MODULEENTRY32 me32       = {0}; 

		do 
		{ 
			for(int i=0; i< (int)dat.size(); i++)
			{
				bCurrent = FALSE;
				bCurrent = KillProcessModule(pe32.th32ProcessID,dat[i]);
				if(bCurrent) 
				{ 
					if(bCurrent==2)
					{
						Sleep(1000); KillProcessModule(pe32.th32ProcessID,dat[i]);
					}
				} 
			}
		} 
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	} 
	CloseHandle (hProcessSnap); 
	return false;
}
//--------------------------------------------------------------
//|
//|프로세스 목록이 하나라도 있으면 true, 아니면 false
//|
//---------------------------------------------------------------
bool CStaticMyFile::FindProcess(std::deque<CString> dat)
{
	HANDLE         hProcessSnap = NULL; 
	BOOL           bRet      = FALSE; 
	BOOL          bCurrent = FALSE; 
	PROCESSENTRY32 pe32      = {0}; 
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

	if (hProcessSnap == (HANDLE)-1) 
		return false;

	pe32.dwSize = sizeof(PROCESSENTRY32); 
	//프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
	if (Process32First(hProcessSnap, &pe32)) 
	{ 

		MODULEENTRY32 me32       = {0}; 

		do 
		{
			for(int i=0; i< (int)dat.size(); i++)
			{
				bCurrent = FALSE;
				bCurrent = (BOOL)GetProcessModule(pe32.th32ProcessID,dat[i]);
				if(bCurrent) 
				{ 
					CloseHandle (hProcessSnap); 
					return true;
				} 
			}
		} 
		while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.
	} 
	CloseHandle (hProcessSnap); 
	return false;
}
//--------------------------------------------------------------
//|
//|User 권한으로 실행 하는 함수.. 구현중
//|
//---------------------------------------------------------------
// Winlogon.exe 실행파일의 세션을 검색하여 같은 세션에서 실행시킴, 2000 ~ Vista
BOOL CStaticMyFile::LaunchAppIntoDifferentSessionAll(tstring strPath)
{
	if( strPath.empty() )
		return FALSE;

	tstring strSearch;
	OSVERSIONINFO osi;
	ZeroMemory(&osi,sizeof(OSVERSIONINFO));
	osi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osi); 
	// 변수 선언
	BOOL bResult = FALSE;
	DWORD dwSessionId = 0; // 세션 ID
	DWORD dwWinlogonPID = 0; // Winlogon PID 
	typedef DWORD (*pfnWTSGetActiveConsoleSessionId)(void);
	HMODULE hModule = NULL;
	pfnWTSGetActiveConsoleSessionId fnWTSGetActiveConsoleSessionId = NULL; 
	if( osi.dwMajorVersion == 6 )
	{
		OutputDebugString(_T("Vista, 라이브러리 로딩 시도"));
		hModule = ::LoadLibrary(_T("Kernel32.dll"));
		if( hModule != NULL )
		{
			OutputDebugString(_T("Vista, 함수 로딩 시도"));
			fnWTSGetActiveConsoleSessionId = (pfnWTSGetActiveConsoleSessionId) GetProcAddress(hModule, "WTSGetActiveConsoleSessionId");
			if( fnWTSGetActiveConsoleSessionId == NULL )
				OutputDebugString(_T("WTSGetActiveConsoleSessionId 함수 로딩 실패")); 
			dwSessionId = fnWTSGetActiveConsoleSessionId();
		}
	}

	// 같은 세션으로 실행할 프로세스 찾기(Winlogon.exe)
	BOOL IsExistsWinlogon = FALSE;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if( hSnap == INVALID_HANDLE_VALUE )
	{
		bResult = FALSE;
		goto CleanUP;
	} 
	if( !Process32First(hSnap, &pe32) )
	{
		bResult = FALSE;
		goto CleanUP;
	} 
	if( osi.dwMajorVersion == 6 )
		strSearch = _T("winlogon.exe");
	else if( osi.dwMajorVersion == 5 )
		strSearch = _T("explorer.exe"); 
	do
	{
		if( lstrcmpi(pe32.szExeFile, strSearch.c_str()) == 0 )
		{
			OutputDebugString(_T("WinLogon 프로세스 검색"));
			DWORD dwWLSessionID = 0;
			BOOL bRet = ProcessIdToSessionId(pe32.th32ProcessID, &dwWLSessionID); 
			if(  bRet && (osi.dwMajorVersion == 6) && (dwSessionId == dwWLSessionID) )
			{
				// 비스타
				OutputDebugString(_T("Vista"));
				dwWinlogonPID = pe32.th32ProcessID;
				IsExistsWinlogon = TRUE;
				break;
			}
			else if( bRet && (osi.dwMajorVersion == 5) )
			{
				// 2000 ~ XP
				OutputDebugString(_T("2000/XP"));
				dwSessionId = dwWLSessionID;
				dwWinlogonPID = pe32.th32ProcessID;
				IsExistsWinlogon = TRUE;
				break;
			}
		}
	} while ( Process32Next(hSnap, &pe32) ); 
	if( IsExistsWinlogon == FALSE )
	{
		OutputDebugString(_T("Winlogon.exe 검색 실패"));
		bResult = FALSE;
		goto CleanUP;
	} 
	// WINLOGON 프로세스 열고 토큰 복사하여 특정 세션 지정
	HANDLE hProcess = ::OpenProcess(MAXIMUM_ALLOWED, FALSE, dwWinlogonPID);
	HANDLE hToken = NULL;
	HANDLE hTokenDup = NULL;
	if( hProcess == NULL )
		goto CleanUP;
	if( !::OpenProcessToken(hProcess,/* TOKEN_ADJUST_PRIVILEGES|*/TOKEN_QUERY|TOKEN_QUERY_SOURCE|TOKEN_ASSIGN_PRIMARY|TOKEN_DUPLICATE/*|TOKEN_ASSIGN_PRIMARY|TOKEN_ADJUST_SESSIONID|TOKEN_READ|TOKEN_WRITE*/, &hToken))
	{
		getchar();
		OutputDebugString(_T("OpenProcessToken Failed"));
		bResult = FALSE;
		goto CleanUP;
	}
	LUID luid;
	if( !LookupPrivilegeValue(NULL,SE_DEBUG_NAME, &luid) )
	{
		OutputDebugString(_T("LookupPrivilegeValue Failed"));
		bResult = FALSE;
		goto CleanUP;
	}
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if( !DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hTokenDup) )
	{
		OutputDebugString(_T("DuplicateTokenEx Failed"));
		bResult = FALSE;
		goto CleanUP;
	}
	SetTokenInformation(hTokenDup, TokenSessionId, (LPVOID)&dwSessionId, sizeof(DWORD));
	if( !AdjustTokenPrivileges(hTokenDup, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL,NULL) )
	{
		OutputDebugString(_T("AdjustTokenPrivileges Failed"));
		bResult = FALSE;
		goto CleanUP;
	}
	if( GetLastError() == ERROR_NOT_ALL_ASSIGNED )
	{
		OutputDebugString(_T("AdjustTokenPrivileges Assign Failed"));
		bResult = FALSE;
		goto CleanUP;
	} 
	// CreateProcessAsUser 준비

	SECURITY_ATTRIBUTES sa;
    HANDLE hReadPipe, hWritePipe;

    sa.nLength = sizeof( SECURITY_ATTRIBUTES );
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = true;
    // create I/O pipe
    CreatePipe( &hReadPipe, &hWritePipe, &sa, 1024 );
	SetHandleInformation(hWritePipe, HANDLE_FLAG_INHERIT,
             HANDLE_FLAG_INHERIT);
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.lpDesktop = _T("WinSta0\\Default");
	si.dwFlags          = STARTF_USESHOWWINDOW|STARTF_USESTDHANDLES; 
	si.hStdOutput = hWritePipe;  // standard output (stdout) redirection
    si.hStdError = hWritePipe;   // standard error (stderr) redirection
	si.wShowWindow = SW_SHOW; 
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	DWORD dwCreationFlags = CREATE_NEW_PROCESS_GROUP | CREATE_NEW_CONSOLE;
	LPVOID pEnv = NULL;
	if( CreateEnvironmentBlock(&pEnv, hTokenDup, TRUE) )
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	else
		pEnv = NULL; 
	LPTSTR lpszPath = _tcsdup(strPath.c_str());
	if( lpszPath == NULL )
	{
		OutputDebugString(_T("Path String Assign Failed"));
		bResult = FALSE;
		goto CleanUP;
	} 
	OutputDebugString(lpszPath); 
	bResult = ::CreateProcessAsUser(
		hTokenDup,
		lpszPath,
		NULL,
		&sa,
		&sa, 
		FALSE,
		dwCreationFlags,
		pEnv,
		NULL,
		&si,
		&pi); 
	free(lpszPath); 
	if( !bResult )
		OutputDebugString(_T("CreateProcessAsUser Failed"));

	if( (bResult) && (pi.hProcess != INVALID_HANDLE_VALUE) )
		CloseHandle(pi.hProcess);
	if( (bResult) && (pi.hThread != INVALID_HANDLE_VALUE) )
		CloseHandle(pi.hThread); 
CleanUP:
	if( hModule )
		FreeLibrary(hModule);
	if( hSnap != INVALID_HANDLE_VALUE )
		CloseHandle(hSnap);
	if( hProcess != NULL )
		CloseHandle(hProcess);
	if( hToken )
		CloseHandle(hToken);
	if( hTokenDup )
		CloseHandle(hTokenDup); 
	return bResult;
} 
//--------------------------------------------------------------
//|
//|권한 부여 구현중
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::EnablePrivilege(LPCTSTR szPrivilege)
{
	BOOL bResult = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tpOld, tpCurrent; 
	if( !OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken) )
		return bResult; 
	tpCurrent.PrivilegeCount = 1;
	tpCurrent.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	if( ::LookupPrivilegeValue(NULL, szPrivilege, &tpCurrent.Privileges[0].Luid) )
	{
		DWORD dwOld = sizeof(TOKEN_PRIVILEGES);
		if( ::AdjustTokenPrivileges(hToken, FALSE, &tpCurrent, dwOld, &tpOld, &dwOld) )
			bResult = TRUE;
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE; 
	CloseHandle(hToken);
	return bResult;
}
//--------------------------------------------------------------
//|
//|특정 디렉토리에 있는 파일를 삭제한다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::DirectoryFindFileDelete(CString path, CString deletepath)
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
#include <setupapi.h>
int CStaticMyFile::_tmain(CString DeviceName, BOOL bSmartCard)
{
	int returnvalue=0;
	HDEVINFO hDevInfo;
	DWORD   LastError =0;
	SP_DEVINFO_DATA DeviceInfoData;
	DWORD i;
	CString buff="";
	if(bSmartCard)
	{
		//스마트카드 GUID
		GUID UDEVICE_GUID = {0x50DD5230, 0xBA8A, 0x11D1, 0xBF, 0x5D, 0x00, 0x00, 0xF8, 0x05, 0xF5, 0x30};
		// Create a HDEVINFO with all present devices.
		hDevInfo = SetupDiGetClassDevs(
			(LPGUID)&UDEVICE_GUID, /* CONST GUID * ClassGuid - USB class GUID */
			NULL, /* PCTSTR Enumerator */
			NULL, /* HWND hwndParent */
			DIGCF_PRESENT | DIGCF_DEVICEINTERFACE|DIGCF_DEVICEINTERFACE /* DWORD Flags */
			); 
	}
	else
	{
		//전체 장치 중에서 검색할때
		hDevInfo = SetupDiGetClassDevs(NULL,
			0, // Enumerator
			0,
			DIGCF_PRESENT | DIGCF_ALLCLASSES );
	}
//	HDEVINFO DeviceInfo = SetupDiGetClassDevs(NULL, _T("USB"),

//		0, DIGCF_ALLCLASSES | DIGCF_PRESENT);


	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		// Insert error handling here.
		return returnvalue;
	}

	// Enumerate through all devices in Set.

	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,
		&DeviceInfoData);i++)
	{
		DWORD DataT;
		LPTSTR buffer = NULL;
		DWORD buffersize = 0;
		while (!SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
		{
			LastError=GetLastError();
			if (LastError ==
				ERROR_INSUFFICIENT_BUFFER)
			{
				// Change the buffer size.
				if (buffer) LocalFree(buffer);
				buffer = (LPTSTR)LocalAlloc(LPTR,buffersize);
			}
			else if (GetLastError() == ERROR_INVALID_DATA) 
			{
				break;
			} 
			else
			{
				// Insert error handling here.
				goto Cleanup;
			}
		}

		// _tprintf(TEXT("Device: %s\n"),buffer);
		if(LastError==ERROR_INVALID_DATA)
		{
			continue;
		}
		buff = buffer;
		if(buff == DeviceName)
		{
			logfile("%s찾았다",buff);
			returnvalue=1;
		}
		if (buffer) 
		{
			LocalFree(buffer); buffer=NULL;
		}
		if(returnvalue)
			break;
	}
	if(DeviceName==""&&i!=0)
	{
		logfile("%s라는 스마트카드가 감지되었습니다",buff);
		returnvalue=1;
	}
Cleanup:
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return returnvalue;
}

//참고로 하드웨어 아이디 찾는 소스는 아래입니다..


/*
HDEVINFO DevInfoSet;
SP_DEVINFO_DATA DevInfoData;
DWORD i, dwErrCode;
// BOOL bFound;
LPTSTR lpHWID;// = "PCI\\VEN_9914&DEV_0001&SUBSYS_04404985&REV_00";
char * INF_FILENAME;// = "..\\3.2\\OD_16240_WXP.inf";

DevInfoSet = SetupDiGetClassDevs(NULL, 0, 0, DIGCF_ALLCLASSES | DIGCF_PRESENT);

if(DevInfoSet == INVALID_HANDLE_VALUE)
MessageBox("GetClassDevs");


DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

BOOL bRebootRequired = FALSE;

for(i=0;SetupDiEnumDeviceInfo(DevInfoSet, i, &DevInfoData);i++)
{
	DWORD dwDataType;
	LPTSTR lpBuf = NULL;
	DWORD dwBufSize = 0;
	LPTSTR temp = NULL;

	while(!SetupDiGetDeviceRegistryProperty(DevInfoSet, &DevInfoData, SPDRP_HARDWAREID, &dwDataType, (PBYTE)lpBuf, dwBufSize, &dwBufSize))
	{
		MessageBox("1");

		if (GetLastError() == ERROR_INVALID_DATA) 
		{
			break;
		} 

		else if(GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			MessageBox("2");
			if(lpBuf)LocalFree(lpBuf);
			lpBuf = (char*)LocalAlloc(LPTR, dwBufSize);==================>현재 내 컴퓨터의 하드웨어 아이디 찾는 목록변수
		}
		else
		{
			MessageBox("GetDeviceResitryProperty");
			goto cleanup_DevInfo;
		}
	}
*/

HANDLE CStaticMyFile::CreateDevice( )

{
	// Get handle to relevant device information set
	HDEVINFO info = SetupDiGetClassDevs(&USB_GUID,NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
	/*  요기가 이상하긴 한데....*/
	SP_INTERFACE_DEVICE_DATA ifdata;
	DWORD ReqLen;
	HANDLE rv;
	PSP_INTERFACE_DEVICE_DETAIL_DATA ifDetail;
	if(info==INVALID_HANDLE_VALUE)
	{
		return NULL;
	}
	// Get interface data for the requested instance
	ifdata.cbSize = sizeof(ifdata);
	if(!SetupDiEnumDeviceInterfaces(info, NULL/*&dd*/, &USB_GUID, 0, &ifdata))
	{
		SetupDiDestroyDeviceInfoList(info);
		return NULL;
	}
	ReqLen=1024;
	ifDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)(malloc(ReqLen));
	if( ifDetail==NULL)
	{
		SetupDiDestroyDeviceInfoList(info);
		return NULL;
	}

	ifDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
	if( !SetupDiGetDeviceInterfaceDetail(info, &ifdata, ifDetail, ReqLen, NULL, NULL))
	{
		SetupDiDestroyDeviceInfoList(info);
		free(ifDetail);
		return NULL;
	}
	// Open file
	rv = CreateFile( ifDetail->DevicePath, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( rv==INVALID_HANDLE_VALUE) rv = NULL;
	free(ifDetail);
	SetupDiDestroyDeviceInfoList(info);

	return rv;

}
//--------------------------------------------------------------
//|
//| PID와 VID로 장치를 찾는다
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::FindVIDPIDToDevice(LPCTSTR VENDORID, LPCTSTR PRODUCTID)
{
	BOOL b=FALSE;
	CString strDeviceId;
	TCHAR* pcDeviceId =NULL;

	strDeviceId.Format("USB\\Vid_%s&Pid_%s", VENDORID, PRODUCTID);

	pcDeviceId =strDeviceId.GetBuffer(strDeviceId.GetLength());
	if(FindExistingDevice(pcDeviceId))
	{
		b=TRUE;
	}
	strDeviceId.ReleaseBuffer();
	return b;
}
//--------------------------------------------------------------
//|
//|PID와 VID로 장치를 찾는 함수
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::FindExistingDevice(LPTSTR pcDeviceId)
{
	HDEVINFO  hdi  =NULL;
	SP_DEVINFO_DATA did  ={0,};
	DWORD   nError =0;
	BOOL   bFound =FALSE;

	hdi =SetupDiGetClassDevs(NULL, 0, 0, DIGCF_ALLCLASSES|DIGCF_PRESENT);
	if(hdi==INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	did.cbSize =sizeof(SP_DEVINFO_DATA);
	for(DWORD i=0; SetupDiEnumDeviceInfo(hdi, i, &did); i++)
	{
		DWORD nType;        
		LPTSTR pcBuffer =NULL;
		DWORD nBufferSize =0;
		while(!SetupDiGetDeviceRegistryProperty(hdi, &did, SPDRP_HARDWAREID, &nType, (PBYTE)pcBuffer, nBufferSize, &nBufferSize))
		{
			nError =GetLastError();
			if(nError==ERROR_INVALID_DATA) 
			{
				break;
			} else if(nError==ERROR_INSUFFICIENT_BUFFER) 
			{
				if(pcBuffer)
				{
					LocalFree(pcBuffer);
				}
				pcBuffer =(TCHAR*)LocalAlloc(LPTR, nBufferSize);
			} else 
			{
				goto GOTO_END_FINDEXISTINGDEVICE;
			}
		}
		if(nError==ERROR_INVALID_DATA)
		{
			continue;
		}
		for(LPTSTR p=pcBuffer; *p&&(p<&pcBuffer[nBufferSize]); p+=lstrlen(p)+sizeof(TCHAR))
		{
			logfile(p);
			if(!_tcscmp(pcDeviceId, p))
			{
				bFound =TRUE;
				break;
			}
		}
		if(pcBuffer) 
		{
			LocalFree(pcBuffer);
			pcBuffer =NULL;
		}
		if(bFound)
		{
			break;
		}
	}
GOTO_END_FINDEXISTINGDEVICE:
	nError =GetLastError();
	SetupDiDestroyDeviceInfoList(hdi);
	SetLastError(nError);
   	return (nError==NO_ERROR);
}
//--------------------------------------------------------------
//|
//|스트링 구분자로 스트링을 분류하여 int로 넘긴다 vector
//|
//---------------------------------------------------------------
void CStaticMyFile::Tokenize(const tstring& str, vector<int>& tokens, const tstring& delimiters)
{
	//맨 첫 글자가 구분자인 경우 무시
	tstring::size_type lastPos = str.find_first_not_of(delimiters,0);
	tstring::size_type pos = str.find_first_of(delimiters, lastPos);
	while(tstring::npos!=pos||tstring::npos!=lastPos)
	{
		tokens.push_back(atoi((str.substr(lastPos, pos-lastPos)).c_str()));
		lastPos = str.find_first_not_of(delimiters, pos);
		pos=str.find_first_of(delimiters, lastPos);
	}
}
/*#define NULLCHARACTER '\0' 
char token[SIZE][SIZE]; 
int tokenize(char *str)
{

// cnt는 리턴할 총 단어의 개수이고

// j는 2차원배열 token의 가로좌표입니다. 가로로 하나씩 증가하면서

// 문자를 삽입하고 공백이나오면 세로로 하나 증가, 가로좌표는 0으로 셋팅해줘야겠죠.
int cnt=0,j=0;
do{ // do 블럭 안의 내용을 반복합니다.
if(' ' ==*str || NULLCHARACTER==*str) // 현재 가리키고 있는 문자가 공백이거나 0이면
{
cnt++; // 단어 개수를 증가시키고
j=0; // 가로좌표는 다시 0으로
}
if(isalnum(*str)) // 해당 문자가 알파벳 또는 숫자이면
{
token[cnt][j]=*str; // 문자를 결과배열(token)에 복사하고
j++; // 가로좌표 증가
}
}while(*str++); // str값이 0이라면 문자열의 끝에 다다른것이므로 false가 되서

// while문을 나오게됩니다.
return cnt;
}

*/
//--------------------------------------------------------------
//|
//|스마트카드 OnDeviceChange 메시지 받기 위해 등록하는 부분
//|
//---------------------------------------------------------------
void CStaticMyFile::RegisterForDeviceNotifications(HWND hWnd)
{
	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;
	GUID UDEVICE_GUID = {0x50DD5230, 0xBA8A, 0x11D1, 0xBF, 0x5D, 0x00, 0x00, 0xF8, 0x05, 0xF5, 0x30};

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = UDEVICE_GUID;
	DeviceNotificationHandle = RegisterDeviceNotification(hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
}
//--------------------------------------------------------------
//|
//|투명윈도우 설정
//|
//---------------------------------------------------------------
BOOL CStaticMyFile::SetMyLayeredWindowAttrbutes(HWND hWnd, COLORREF crKey, BYTE Alpha, DWORD flag)
{
	SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE)|WS_EX_LAYERED);
	BOOL b=SetLayeredWindowAttributes(hWnd, crKey, Alpha, flag);
	return b;
}
//--------------------------------------------------------------
//|
//|ShellExecuteEx함수로 프로그램 실행 시킨다
//|
//---------------------------------------------------------------
HANDLE CStaticMyFile::ExeShell(LPCTSTR FileName, LPCTSTR Params, INT Flag)
{
	SHELLEXECUTEINFO execinfo;

	// 실행을 위해 구조체 세트
	ZeroMemory( &execinfo, sizeof(execinfo) );
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = "open";
	execinfo.lpFile = FileName;
	execinfo.lpParameters = Params;
	execinfo.fMask = SEE_MASK_FLAG_NO_UI|SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;
	// 프로그램을 실행한다.
	int r = (int)ShellExecuteEx( &execinfo );
	if ( r == 0 ) return NULL;

	// 만약 Sync 플랙이 세트되었으면, 핸들을 넘긴다
	if ( Flag == 1 )
	{
		return execinfo.hProcess;
	}
	CloseHandle(execinfo.hProcess);
	return NULL;
}
//--------------------------------------------------------------
//|
//|ShellExecuteEx함수로 프로그램 실행 시킨다
//|
//---------------------------------------------------------------
struct ExitParam{HANDLE hProcess; HANDLE hEvent;};
BOOL CStaticMyFile::ExitExe(HANDLE& hprocess)
{
	ExitParam pm;
	pm.hProcess=hprocess;
	pm.hEvent= CreateEvent(NULL, FALSE, FALSE, NULL);
	DWORD ThreadId=NULL;HANDLE  hThread=NULL;
	hThread = CreateThread(NULL, 0,CStaticMyFile::ThreadProc, (PVOID)&pm, 0, &ThreadId);
	WaitForSingleObject(pm.hEvent,INFINITE);
	CloseHandle(hprocess); hprocess=NULL;
	return TRUE;
}
DWORD WINAPI CStaticMyFile::ThreadProc(LPVOID param)
{
	CStaticMyFile::logfile("Process종료감시쓰래드-------------------------------------------------시작");
	ExitParam* pm=(ExitParam*)param;
	DWORD ec=0;
	HANDLE hEvent= CreateEvent(NULL, FALSE, FALSE, NULL);
	do
	{
		WaitForSingleObject(hEvent,100);
		GetExitCodeProcess(pm->hProcess, &ec);
	}
	while ( ec == STILL_ACTIVE );
	CloseHandle(hEvent);
	SetEvent(pm->hEvent);
	CStaticMyFile::logfile("Process종료감시쓰래드-------------------------------------------------종료");
	return 0;
}