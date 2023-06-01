#ifndef __SERIAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SERIAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerial
{
	HANDLE hCom;
public:
	//생성자
	CSerial(void);
	//소멸자
	~CSerial(void);
	BOOL PortOpen(CONST WCHAR *szPort );
	BOOL PortClose();
	// Set Up Serial Port
	// default BaudRate is 115200, and Time out 5s
	BOOL PortSetUp(DWORD dwBaudRate = CBR_9600, DWORD dwTimeOutInSec= 5);
	//데이터를 보냄
	BOOL PortWrite(const byte *szData, int szDatalen);
	//Read 함수, // data read from UART port
	BOOL PortRead(byte *RespData, int& Resplen);
	BOOL IsOpened();
};

#endif //__SERIAL_B7A15A9D01DC403996FC45F7F92F52D3_H__