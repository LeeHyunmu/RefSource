#include "stdafx.h"

#define MAX_BUFF_SIZE 1024

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSerial::CSerial(void):
hCom(INVALID_HANDLE_VALUE)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSerial::~CSerial(void)
{
}
//Serial Port Open Fucntion
//
//return is the device handle
BOOL CSerial::PortOpen(CONST TCHAR *szPort )
{
	HANDLE uhd = INVALID_HANDLE_VALUE;  // 기본값
	   uhd=CreateFile(szPort ,                           // 포트명을 지정
		GENERIC_READ | GENERIC_WRITE,    // 장치 접근 모드 지정
		0,                                 // 현장치 사용 권한
		NULL,                              // NULL
		OPEN_EXISTING,                     // 장치 생성 모드
		0,                                 // 장치 속성 지정
		NULL);                             // NULL
	if (uhd == INVALID_HANDLE_VALUE)  return FALSE;
		hCom  = uhd;
	 return TRUE; 
}

BOOL CSerial::PortClose()
{
	//Check Port
	if(hCom == INVALID_HANDLE_VALUE) return FALSE;
	if(!CloseHandle(hCom))
	{
		return FALSE;
	}
	hCom = INVALID_HANDLE_VALUE;
	return TRUE;
}

// Set Up Serial Port
// default BaudRate is 115200, and Time out 5s
BOOL CSerial::PortSetUp(DWORD dwBaudRate, DWORD dwTimeOutInSec)
{
	//check the Port state
	if(hCom== INVALID_HANDLE_VALUE ) return FALSE;
	//Setting in, out QUEUE size
	if(SetupComm(hCom,MAX_BUFF_SIZE,MAX_BUFF_SIZE)== FALSE) return FALSE;
	//DCB 변수를 만들고 포트 정보를 가져옴
	DCB cdb;
	if(GetCommState(hCom,&cdb)==FALSE) return FALSE;
	//포트 설정 부분
	cdb.BaudRate = dwBaudRate; // 보우 레이트
	cdb.ByteSize = 8;          // 데이터 비트 8
	cdb.Parity  = NOPARITY;   // 0 no...
	cdb.StopBits = ONESTOPBIT; // Stop Bit 1
	cdb.fBinary  = TRUE;       // 항상 1
	cdb.fParity  = TRUE;       // 패리티 검사
	//포트 셋업
	if(SetCommState(hCom,&cdb)== FALSE) return FALSE;
	//타임아웃 설정 가져옴
	COMMTIMEOUTS cto;
	if(GetCommTimeouts(hCom,&cto) == FALSE) return FALSE;
	//타임 아웃 설정
	cto.ReadIntervalTimeout    = 1000*dwTimeOutInSec;
	cto.ReadTotalTimeoutConstant  = 1000*dwTimeOutInSec;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 1000*dwTimeOutInSec;
	cto.WriteTotalTimeoutMultiplier = 0;
	//설정 적용
	if(SetCommTimeouts(hCom,&cto) == FALSE) return FALSE;
		return TRUE;
}
//데이터를 보냄
BOOL CSerial::PortWrite(const TCHAR *szData)
{
	//Check Port
	if(hCom == INVALID_HANDLE_VALUE) return FALSE;
	
		//데이터를 보내고 보낸양을 받아 온다.
		DWORD dwBytes;
	BOOL bRes = WriteFile(hCom ,      // 장치 핸들
		szData,     // 송신데이터
		lstrlen(szData)*sizeof(TCHAR),  // 송신데이터 바이트수 
		&dwBytes,    // 보낸 바이트 수
		NULL);          // NULL
		//전송된 데이터 성공 여부 확인
		if(!(bRes == TRUE && lstrlen(szData)*sizeof(TCHAR) == dwBytes) ) return FALSE;

	return TRUE;
}

//참고로 문자 하나 그냥 보내는 함수가 있는데 다음과 같다. 쓰는것은 쉽다.
//BOOL TransmitCommChar(  HANDLE hFile,  char cChar  );


// data read from UART port
BOOL CSerial::PortRead(byte *RespData, int& Resplen)
{
	memset(&RespData[0], NULL, Resplen);
	Resplen=0;
	//1바이트 읽기 함수
	//char ch;
	//TransmitCommChar(hCom, ch);
	//포트를 체크 한다.
	if(hCom == INVALID_HANDLE_VALUE) return FALSE;
	//통신 Mask를 설정한다.
	if(SetCommMask(hCom, EV_RXCHAR) == FALSE) return FALSE;
	//이벤트를 기다린다.
	//기다리던 데이터가 오면 다음으로 넘어감
	DWORD dwEventMask; // 어떤 이벤트가 걸려서 넘여져 리턴 됬는지 담는곳
	//데이터를 읽어 온다
	CHAR  cData[MAX_BUFF_SIZE] = "";
	DWORD dwBytes;
	BOOL  bRes;
	//EV_RXCHAR 마스크 플래그가 도착하는지 지켜 본다.
		if(WaitCommEvent(hCom, &dwEventMask, NULL))
		{
			switch(dwEventMask)
			{
			case EV_RXCHAR://데이터가 도착했음

				dwBytes  = 0; //읽은 바이트수 초기화
				 bRes     = ReadFile(hCom,      //장치 핸들
					cData ,        //수신 데이터 
					sizeof(cData), //읽을 바이트수
					&dwBytes,      //읽은 바이트수
					NULL);         //NULL

				if (bRes)  //제대로 데이터를 받음
				{
					//Converts a sequence of multibyte characters to
					//a corresponding sequence of wide characters.
					 
						//mbstowcs(wData, cData, dwBytes) ;
						//mbstowcs_s(0, wData,0, cData, dwBytes, _TRUNCATE) ;

					//OutputDebugString(wData);
					memcpy(&RespData[0], cData, dwBytes);
					return TRUE;
				}
				default:
				return FALSE;
			}
		}
		return FALSE;
}
BOOL CSerial::IsOpened()
{
	if(hCom != INVALID_HANDLE_VALUE) return TRUE;
	return FALSE;
}
// HANDLE g_hCom;
// HANDLE g_hThread;
// DWORD WINAPI PortThreadProc(LPVOID lpParameter);
// //쓰레드 생성
// BOOL PortThreadStart()
// {
// 	//장치 핸들 체크
// 	if(g_hCom == INVALID_HANDLE_VALUE) return FALSE;
// 
// 	//쓰레드 생성
// 	g_hThread = CreateThread(NULL, 0, PortThreadProc, NULL, 0, NULL);
// 	//생성된 쓰레드 핸들 체크
// 		if(g_hThread== INVALID_HANDLE_VALUE || INVALID_HANDLE_VALUE == NULL) return FALSE;
// 	 return TRUE;
// 
// }
// //쓰레드 해제
// void PortThreadFinish()
// {
// 	if(g_hThread != NULL)
// 	{
// 		TerminateThread(g_hThread,0) ;
// 		CloseHandle(g_hThread);
// 		g_hThread = NULL;
// 	}
// }
// 
// 
// //포트에서 데이터를 읽어 들이는 것을 지속적으로 감시하기 위해 작성한 PortRead 함수를 스레드에서
// //실행하기 위해 스레드로 실행되는 형식으로 스레드 함수를 만든다. 포트가 닫히지 않는 한 계속 실행
// //되도록
// DWORD WINAPI PortThreadProc(LPVOID lpParameter)
// {   
// 	while( g_hCom != INVALID_HANDLE_VALUE)
// 	{
// 		WCHAR wRead[MAX_BUFF_SIZE] = L"";
// 		BOOL bRes = PortRead(g_hCom, wRead);
// 		if(bRes) OutputDebugString(wRead);
// 	}
// 	return 0;
// }