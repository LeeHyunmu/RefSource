
#ifndef __NFCCMD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __NFCCMD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
class CAQdeviceCmd
{
	//NFC Port Connect ok
	BOOL bNfcConnectOk;
	int nRecvlen;
	BYTE byteRecv[BUFFSIZE_1024];
	HMODULE  m_module;
	BOOL   bLoadModule;
	HWND	hWndp;

	//Find Card Thread
	CThread FindCardThread;
	//thread 함수
	static DWORD WINAPI FindCardThreadProcess(LPVOID lpArg);
	//Find Card function
	void FindCardFunc();

	//Find Card Event 처리 스래드
	CThread EventProcThread;
	//카드 이벤트 데이터 처리 스래드
	static DWORD WINAPI EventProcThreadProcess(LPVOID lpArg);
	//Find Card Event proc function
	void EventProcFunc();

	//카드를 찾았다는 메시지 이벤트
	NFCErrorCode FindEventFlag;
	//카드 접속 상태 떨어진 상태 판단
	NFCErrorCode CardConnection;

	//콜백 함수(Atype, Mifare type)
	ATYPEEVENTFUNC_ROUTINE atypefunc;
	MTYPEEVENTFUNC_ROUTINE mtypefunc;
	DISCONNECTEVENTFUNC_ROUTINE disconnect;
	LPVOID lpThreadParameter;
	//Card Find Option
	CardFindType findop;
private:
	void *AQInstance;
	//common
	//에러메시지 가져오기
	typedef NFCErrorCode (WINAPI *type_GetErrMsg)(int,char*);
	type_GetErrMsg mp_GetErrMsg;
	//Port 초기화
	typedef NFCErrorCode (WINAPI *type_DE_InitPort)(int,int);
	type_DE_InitPort mp_DE_InitPort;
	//Port 초기화
	typedef NFCErrorCode (WINAPI *type_DE_ClosePort)(int);
	type_DE_ClosePort mp_DE_ClosePort;

	//
	typedef NFCErrorCode (WINAPI *type_DE_BuzzerOn)(int);
	type_DE_BuzzerOn mp_DE_BuzzerOn;
	//
	typedef NFCErrorCode (WINAPI *type_DE_BuzzerOff)(int);
	type_DE_BuzzerOff mp_DE_BuzzerOff;
	//카드를 찾는다
	typedef NFCErrorCode (WINAPI *type_DE_FindCard)(int, BYTE, BYTE, BYTE, BYTE, LPINT, LPBYTE);
	type_DE_FindCard mp_DE_FindCard;

	//Mifare 키 인증 함수
	typedef NFCErrorCode (WINAPI *type_DEA_Authkey)(int, BYTE, LPBYTE, BYTE);
	type_DEA_Authkey mp_DEA_Authkey;
	//Mifare Read 함수
	typedef NFCErrorCode (WINAPI *type_DEA_Read)(int, BYTE, LPINT, LPBYTE);
	type_DEA_Read mp_DEA_Read;
	//Mifare Write 함수
	typedef NFCErrorCode (WINAPI *type_DEA_Write)(int, BYTE, int, LPBYTE);
	type_DEA_Write mp_DEA_Write;


	FARPROC GetProcAddressFunc(LPCTSTR lpProcName);
public:
	//생성자
	CAQdeviceCmd(void);
	//소멸자
	~CAQdeviceCmd(void);
	//
	BOOL initAQdeviceCmd(HWND _hWnd, LPCTSTR dlllib);
	//A 타입의 카드 감지 세팅
	BOOL NFCAtypeConnect(LPVOID _lpThreadParameter, ATYPEEVENTFUNC_ROUTINE _atypefunc, MTYPEEVENTFUNC_ROUTINE _mtypefunc, DISCONNECTEVENTFUNC_ROUTINE _disconnect);
	//NFC 연결 종료
	void NFCDisConnect();
	//에러 검출함수
	NFCErrorCode GetErrMsg(int errcode,char* retmsg);
	//포트 초기화
	NFCErrorCode DE_InitPort(int nBaud=BAUD, int nPort=USBPORT);
	//포트 닫기
	NFCErrorCode DE_ClosePort(int nPort=USBPORT);
	//
	NFCErrorCode DE_BuzzerOn(int nPort=USBPORT);
	//
	NFCErrorCode DE_BuzzerOff(int nPort=USBPORT);
	//카드를 찾는다
	NFCErrorCode DE_FindCard(LPINT outlen, LPBYTE lpRes, BYTE option, BYTE baud=0x00, BYTE cid=0x01, BYTE nad=0x01, int nPort=USBPORT);
	//Mifare 키 인증 함수
	NFCErrorCode DEA_Authkey(LPBYTE keydata, BYTE blockno, BYTE mode=0x00, int nPort=USBPORT);
	//Mifare Read 함수
	NFCErrorCode DEA_Read(BYTE blockno, LPINT outlen, LPBYTE lpRes, int nPort=USBPORT);
	//Mifare write 함수
	NFCErrorCode DEA_Write(BYTE blockno, int datalen, LPBYTE data, int nPort=USBPORT);
};
#ifdef __cplusplus
	}
#endif //__cplusplus
#endif// __NFCCMD_74F736E25AB94b5dA789E4730F58F2A1_H__
