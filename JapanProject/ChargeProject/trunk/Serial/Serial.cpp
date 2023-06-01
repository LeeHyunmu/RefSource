#include "stdafx.h"

#define MAX_BUFF_SIZE 1024

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CSerial::CSerial(void):
hCom(INVALID_HANDLE_VALUE)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
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
	HANDLE uhd = INVALID_HANDLE_VALUE;  // �⺻��
	   uhd=CreateFile(szPort ,                           // ��Ʈ���� ����
		GENERIC_READ | GENERIC_WRITE,    // ��ġ ���� ��� ����
		0,                                 // ����ġ ��� ����
		NULL,                              // NULL
		OPEN_EXISTING,                     // ��ġ ���� ���
		0,                                 // ��ġ �Ӽ� ����
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
	//DCB ������ ����� ��Ʈ ������ ������
	DCB cdb;
	if(GetCommState(hCom,&cdb)==FALSE) return FALSE;
	//��Ʈ ���� �κ�
	cdb.BaudRate = dwBaudRate; // ���� ����Ʈ
	cdb.ByteSize = 8;          // ������ ��Ʈ 8
	cdb.Parity  = NOPARITY;   // 0 no...
	cdb.StopBits = ONESTOPBIT; // Stop Bit 1
	cdb.fBinary  = TRUE;       // �׻� 1
	cdb.fParity  = TRUE;       // �и�Ƽ �˻�
	//��Ʈ �¾�
	if(SetCommState(hCom,&cdb)== FALSE) return FALSE;
	//Ÿ�Ӿƿ� ���� ������
	COMMTIMEOUTS cto;
	if(GetCommTimeouts(hCom,&cto) == FALSE) return FALSE;
	//Ÿ�� �ƿ� ����
	cto.ReadIntervalTimeout    = 1000*dwTimeOutInSec;
	cto.ReadTotalTimeoutConstant  = 1000*dwTimeOutInSec;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 1000*dwTimeOutInSec;
	cto.WriteTotalTimeoutMultiplier = 0;
	//���� ����
	if(SetCommTimeouts(hCom,&cto) == FALSE) return FALSE;
		return TRUE;
}
//�����͸� ����
BOOL CSerial::PortWrite(const TCHAR *szData)
{
	//Check Port
	if(hCom == INVALID_HANDLE_VALUE) return FALSE;
	
		//�����͸� ������ �������� �޾� �´�.
		DWORD dwBytes;
	BOOL bRes = WriteFile(hCom ,      // ��ġ �ڵ�
		szData,     // �۽ŵ�����
		lstrlen(szData)*sizeof(TCHAR),  // �۽ŵ����� ����Ʈ�� 
		&dwBytes,    // ���� ����Ʈ ��
		NULL);          // NULL
		//���۵� ������ ���� ���� Ȯ��
		if(!(bRes == TRUE && lstrlen(szData)*sizeof(TCHAR) == dwBytes) ) return FALSE;

	return TRUE;
}

//����� ���� �ϳ� �׳� ������ �Լ��� �ִµ� ������ ����. ���°��� ����.
//BOOL TransmitCommChar(  HANDLE hFile,  char cChar  );


// data read from UART port
BOOL CSerial::PortRead(byte *RespData, int& Resplen)
{
	memset(&RespData[0], NULL, Resplen);
	Resplen=0;
	//1����Ʈ �б� �Լ�
	//char ch;
	//TransmitCommChar(hCom, ch);
	//��Ʈ�� üũ �Ѵ�.
	if(hCom == INVALID_HANDLE_VALUE) return FALSE;
	//��� Mask�� �����Ѵ�.
	if(SetCommMask(hCom, EV_RXCHAR) == FALSE) return FALSE;
	//�̺�Ʈ�� ��ٸ���.
	//��ٸ��� �����Ͱ� ���� �������� �Ѿ
	DWORD dwEventMask; // � �̺�Ʈ�� �ɷ��� �ѿ��� ���� ����� ��°�
	//�����͸� �о� �´�
	CHAR  cData[MAX_BUFF_SIZE] = "";
	DWORD dwBytes;
	BOOL  bRes;
	//EV_RXCHAR ����ũ �÷��װ� �����ϴ��� ���� ����.
		if(WaitCommEvent(hCom, &dwEventMask, NULL))
		{
			switch(dwEventMask)
			{
			case EV_RXCHAR://�����Ͱ� ��������

				dwBytes  = 0; //���� ����Ʈ�� �ʱ�ȭ
				 bRes     = ReadFile(hCom,      //��ġ �ڵ�
					cData ,        //���� ������ 
					sizeof(cData), //���� ����Ʈ��
					&dwBytes,      //���� ����Ʈ��
					NULL);         //NULL

				if (bRes)  //����� �����͸� ����
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
// //������ ����
// BOOL PortThreadStart()
// {
// 	//��ġ �ڵ� üũ
// 	if(g_hCom == INVALID_HANDLE_VALUE) return FALSE;
// 
// 	//������ ����
// 	g_hThread = CreateThread(NULL, 0, PortThreadProc, NULL, 0, NULL);
// 	//������ ������ �ڵ� üũ
// 		if(g_hThread== INVALID_HANDLE_VALUE || INVALID_HANDLE_VALUE == NULL) return FALSE;
// 	 return TRUE;
// 
// }
// //������ ����
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
// //��Ʈ���� �����͸� �о� ���̴� ���� ���������� �����ϱ� ���� �ۼ��� PortRead �Լ��� �����忡��
// //�����ϱ� ���� ������� ����Ǵ� �������� ������ �Լ��� �����. ��Ʈ�� ������ �ʴ� �� ��� ����
// //�ǵ���
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