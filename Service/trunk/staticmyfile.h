
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
	//������
	CStaticMyFile(void);
	//�Ҹ���
	~CStaticMyFile(void);
	//����׿����� �ܼ�â�� �ƴҶ��� �α׸� ��� �Լ�
	//�Լ� �� �÷��׸� �ּ�ó���ϰ� ���ϰ� �ؼ� ������� �������� �����Ѵ�
	static int logfile(char* Format,...);
	//Ŭ���� �����Լ� ���� �κ�
	//���� ���̾�α׸� ����
	static CString Filedialog(BOOL bOpSa, CString strFilter,DWORD dwListType=0, DWORD dwFlag=OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT);
	//������ �д´�
	static CFile FileRead(CString szFilePath);
	//���μ����� ���δ�
	static BOOL ProcessKill(CString strProcessName);
	//���μ����� ���������� ���̴� �Լ�
	static BOOL KillProcessModule(DWORD dwPID, CString sProcessName);
	// ���ݽ��� �ǰ��ִ� ���μ����ΰ�??
	static BOOL GetProcessModule(DWORD dwPID, CString sProcessName);

	//�������� ���μ��� �ڵ��� ���� �ڵ��� �� �����Ұ�
	static HANDLE ProcessHandle(CString strProcessName);

	//������ ī���Ѵ�
	static void	FileCopy(HWND hwnd, CString frompath, CString topath);
	//������ ī���Ѵ�(
	static BOOL CopyFolder(CString strFromFolder, CString strToFolder);
	//���丮�� �����Ѵ�
	static int RemoveDirectoryS(const char* FileName, HWND hwnd=NULL);
	//������ �����Ѵ�
	static BOOL RemveFile(CString file);
	//������ ������ ����� ���ο������� ������ ���� �̸����� �ٲ۴�
	static BOOL FileRemoveandRename(CString path,CString oldfile, CString newfile);
	//Folder�� ã�´�
	static BOOL FindFileAndFolder(CString findfile);
	static void	CreateRegRun(LPCTSTR regValueName, const char* pBuffer);
	//Applet�� �н����带 �����Ҷ� hexƮ���ڷ� ��ȯ�ϱ� ���� �Լ�
	static CString Changcharbyhex(CString pass);
	//1Byte���ڸ� Hex2Byte��
	static CString Chang1Byteby2Hex(CString pass);
	//������Ʈ�� Ű���� �ش��ϴ� ��Ʈ���� �о�´�
	static CString GetHkeyReadStringData(HKEY LKey, LPCTSTR subkey, LPCTSTR valuename);
	//������Ʈ ���� ���� Ű����
	static HKEY CreateWriteKey(HKEY KeyOp, LPCTSTR subkey, BYTE opt=1);
	//�� ������ Ű���ϰ����� ��Ʈ��(����)�� �߰��Ѵ�
	static void SetHkeyWriteStringData(HKEY Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0);
	//�� ���Լ��� ��ģ�Լ���
	static HKEY CStaticMyFile::CreateKeyWriteString(HKEY& Key, LPCTSTR valuename, LPBYTE lpData, BYTE keyclose=0, HKEY KeyOp=NULL, LPCTSTR subkey=NULL,BYTE opt=1);
	//���μ����� ����ǰ� �ִ°��� �˾ƺ��� ���� �Լ�
	static bool ExistProcess(CString strProcessName);
	//���μ����� ����ǰ� ������ 
	static bool ExistKillProcess(std::deque<CString> dat);
	//����ǰ� �ִ� ���μ����� �ִ°�
	static bool FindProcess(std::deque<CString> dat);

	static BOOL LaunchAppIntoDifferentSessionAll(tstring strPath);
	static BOOL EnablePrivilege(LPCTSTR szPrivilege);
	//Ư�� ���丮 ���� ����� �����Ѵ�
	static BOOL DirectoryFindFileDelete(CString path, CString deletepath="");

	//�ڽ��� ���� ��Ų ���� Process�� Ȯ���ؼ� �׿� ���� �ڵ� �Ǵ� Ŭ������ �ѱ��
	static BOOL CALLBACK EnumWindowsProc(HWND hWindow, PIDANDHWND *lParam);
	static CWnd* WindowFromProcessID(DWORD pID,tstring sClsname);

	//�ϵ���� �˻� �Լ�(��ġ������)
	static int _tmain(CString DeviceName, BOOL bSmartCard=1);
	static HANDLE CreateDevice( );

	//PID VID�� ��ġ ã��(����̽��� ���ε��;��̵�� �����Ƶ�� ã�´�)
	static BOOL FindVIDPIDToDevice(LPCTSTR VENDORID, LPCTSTR PRODUCTID);
	static BOOL FindExistingDevice(LPTSTR pcDeviceId);

	//STL �˰������� ,(�޸�)�� ��Ʈ���� �и��Ͽ� �ѱ��
	static void Tokenize(const tstring& str, vector<int>& tokens, const string& delimiters=",");

	//����Ʈī�� ����̽� �޽��� �ޱ� ���ؼ� ����ϴ� �Լ�
	static void RegisterForDeviceNotifications(HWND hWnd);
	//���������� ����
	static BOOL SetMyLayeredWindowAttrbutes(HWND hWnd, COLORREF crKey, BYTE Alpha, DWORD flag);

	//ShellExecuteEx  //���α׷��� ���� ��Ų�� �ڵ��� �ѱ�� , �ѱ��ڵ��� �ٷ� ���� �ϴ��� �ٸ����� ������ ������ ���� �����Ѵ�
	static HANDLE ExeShell(LPCTSTR FileName, LPCTSTR Params, INT Flag);
	//�������� ���μ��� �ڵ�� ���� ����������� ���� ���������� �˻��� ���μ����� �׾����� �������´� CloseHandle�� ȣ������ �ʾƵ� �ȴ�
	static BOOL ExitExe(HANDLE& hprocess);
	static DWORD WINAPI ThreadProc(LPVOID param);
};
#endif//#ifndef __STATICMYFILE_H__