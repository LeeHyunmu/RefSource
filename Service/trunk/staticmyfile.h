
#ifndef __STATICMYFILE_H__
#define __STATICMYFILE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <deque>
#include <vector>
#include <string>
#include <xstring>
using namespace std;

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif 

typedef struct _PIDANDHWND{
	DWORD  pID;
	HWND   hWindow;
	tstring clsname;
} PIDANDHWND;

class CStaticMyFile
{
public:
	//생성자
	CStaticMyFile(void);
	//소멸자
	~CStaticMyFile(void);
	//디버그에서는 콘솔창에 아닐때는 로그를 찍는 함수
	//함수 위 플래그를 주석처리하고 안하고 해서 출력할지 안할지를 경정한다
	static int logfile(char* Format,...);
	//클래스 전역함수 선언 부분
	//파일 다이얼로그를 연다
	static CString Filedialog(BOOL bOpSa, CString strFilter,DWORD dwListType=0, DWORD dwFlag=OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	//파일을 읽는다
	static CFile FileRead(CString szFilePath);
	//프로세서를 죽인다
	static BOOL ProcessKill(CString strProcessName);
	//프로세스를 실질적으로 죽이는 함수
	static BOOL KillProcessModule(DWORD dwPID, CString sProcessName);
	// 지금실행 되고있는 프로세서인가??
	static BOOL GetProcessModule(DWORD dwPID, CString sProcessName);

	//실행중인 프로세서 핸들얻기 얻은 핸들을 꼭 해지할것
	static HANDLE ProcessHandle(CString strProcessName);

	//파일을 카피한다
	static void	FileCopy(HWND hwnd, CString frompath, CString topath);
	//폴더를 카피한다(
	static BOOL CopyFolder(CString strFromFolder, CString strToFolder);
	//디렉토리를 삭제한다
	static int RemoveDirectoryS(const char* FileName, HWND hwnd=NULL);
	//파일을 삭제한다
	static BOOL RemveFile(CString file);
	//기존의 파일을 지우고 새로운파일을 기존의 파일 이름으로 바꾼다
	static BOOL FileRemoveandRename(CString path,CString oldfile, CString newfile);
	//Folder를 찾는다
	static BOOL FindFileAndFolder(CString findfile);
	static void	CreateRegRun(LPCTSTR regValueName, const char* pBuffer);
	//Applet에 패스워드를 설정할때 hex트문자로 변환하기 위한 함수
	static CString Changcharbyhex(CString pass);
	//1Byte문자를 Hex2Byte로
	static CString Chang1Byteby2Hex(CString pass);
	//레지스트리 키값에 해당하는 스트링을 읽어온다
	static CString GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename);
	//레지스트 쓰기 위한 키생성
	static HKEY CreateWriteKey(HKEY KeyOp, LPCTSTR subkey, BYTE opt=1);
	//위 생성한 키리턴값으로 스트링(문자)을 추가한다
	static void SetHkeyWriteStringData(HKEY Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0);
	//위 두함수를 합친함수다
	static HKEY CStaticMyFile::CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0, HKEY KeyOp=NULL, LPCTSTR subkey=NULL,BYTE opt=1);
	//프로세스가 실행되고 있는가를 알아보기 위한 함수
	static bool ExistProcess(CString strProcessName);
	//프로세스가 실행되고 있으면 
	static bool ExistKillProcess(std::deque<CString> dat);
	//실행되고 있는 프로세스가 있는가
	static bool FindProcess(std::deque<CString> dat);

	static BOOL LaunchAppIntoDifferentSessionAll(tstring strPath);
	static BOOL EnablePrivilege(LPCTSTR szPrivilege);
	//특정 디렉토리 파일 목록을 삭제한다
	static BOOL DirectoryFindFileDelete(CString path, CString deletepath="");

	//자신이 실행 시킨 실행 Process를 확인해서 그에 대한 핸들 또는 클래스를 넘긴다
	static BOOL CALLBACK EnumWindowsProc(HWND hWindow, PIDANDHWND *lParam);
	static CWnd* WindowFromProcessID(DWORD pID,tstring sClsname);

	//하드웨어 검색 함수(장치관리자)
	static int _tmain(CString DeviceName, BOOL bSmartCard=1);
	static HANDLE CreateDevice( );

	//PID VID로 장치 찾기(디바이스의 프로덱터아이디와 벤더아디로 찾는다)
	static BOOL FindVIDPIDToDevice(LPCTSTR VENDORID, LPCTSTR PRODUCTID);
	static BOOL FindExistingDevice(LPTSTR pcDeviceId);

	//STL 알고리즘으로 ,(콤마)로 스트링을 분리하여 넘긴다
	static void Tokenize(const tstring& str, vector<int>& tokens, const string& delimiters=",");

	//스마트카드 디바이스 메시지 받기 위해서 등록하는 함수
	static void RegisterForDeviceNotifications(HWND hWnd);
	//투명윈도우 설정
	static BOOL SetMyLayeredWindowAttrbutes(HWND hWnd, COLORREF crKey, BYTE Alpha, DWORD flag);

	//ShellExecuteEx  //프로그램을 실행 시킨고 핸들을 넘긴다 , 넘긴핸들을 바로 해지 하던가 다른곳에 쓸곳이 있으면 쓰고 해지한다
	static HANDLE ExeShell(LPCTSTR FileName, LPCTSTR Params, INT Flag);
	//실행중인 프로세서 핸들로 지금 실행상태인지 죽은 상태인지를 검사해 프로세서가 죽었으면 빠져나온다 CloseHandle를 호출하지 않아도 된다
	static BOOL ExitExe(HANDLE& hprocess);
	static DWORD WINAPI ThreadProc(LPVOID param);
};
#endif//#ifndef __STATICMYFILE_H__