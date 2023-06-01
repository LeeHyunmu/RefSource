
#include "stdAfx.h"
#include "HCExecuter.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
HCExecuter::HCExecuter(void)
{

}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
HCExecuter::~HCExecuter(void)
{
}
//--------------------------------------------------------------
//|
//|Run ���� �����׷�
//|
//---------------------------------------------------------------
BOOL HCExecuter::RunProgram(LPCTSTR lpszImageName)
{
	STARTUPINFO si;	
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	if( !CreateProcess( lpszImageName, 
		NULL,		// Command line. 
		NULL,		// Process handle not inheritable. 
		NULL,		// Thread handle not inheritable. 
		FALSE,		// Set handle inheritance to FALSE. 
		0,		// No creation flags. 
		NULL,		// Use parent's environment block. 
		NULL,		// Use parent's starting directory. 
		&si,		// Pointer to STARTUPINFO structure.
		&pi )		// Pointer to PROCESS_INFORMATION structure.
		) return 0;
	else	return 1;
}
//--------------------------------------------------------------
//|
//|���� ������ �ִ°� �˻�
//|
//---------------------------------------------------------------
BOOL HCExecuter::IsProgram(LPCTSTR lpszImageName)
{
	HANDLE hFile=NULL;
	unsigned int Result = TRUE;
	hFile = CreateFile(lpszImageName, 
		GENERIC_READ, 
		NULL, NULL, 
		OPEN_EXISTING, 
		NULL, 
		NULL);
	if(hFile == INVALID_HANDLE_VALUE)	Result = FALSE;
	CloseHandle(hFile);
	return Result;
}
