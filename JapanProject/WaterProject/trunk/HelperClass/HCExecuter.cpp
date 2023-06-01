
#include "stdAfx.h"
#include "HCExecuter.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
HCExecuter::HCExecuter(void)
{

}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
HCExecuter::~HCExecuter(void)
{
}
//--------------------------------------------------------------
//|
//|Run 실행 프르그램
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
//|실행 파일이 있는가 검사
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
