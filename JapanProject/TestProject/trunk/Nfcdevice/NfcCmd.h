
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
	//thread �Լ�
	static DWORD WINAPI FindCardThreadProcess(LPVOID lpArg);
	//Find Card function
	void FindCardFunc();

	//Find Card Event ó�� ������
	CThread EventProcThread;
	//ī�� �̺�Ʈ ������ ó�� ������
	static DWORD WINAPI EventProcThreadProcess(LPVOID lpArg);
	//Find Card Event proc function
	void EventProcFunc();

	//ī�带 ã�Ҵٴ� �޽��� �̺�Ʈ
	NFCErrorCode FindEventFlag;
	//ī�� ���� ���� ������ ���� �Ǵ�
	NFCErrorCode CardConnection;

	//�ݹ� �Լ�(Atype, Mifare type)
	ATYPEEVENTFUNC_ROUTINE atypefunc;
	MTYPEEVENTFUNC_ROUTINE mtypefunc;
	DISCONNECTEVENTFUNC_ROUTINE disconnect;
	LPVOID lpThreadParameter;
	//Card Find Option
	CardFindType findop;
private:
	void *AQInstance;
	//common
	//�����޽��� ��������
	typedef NFCErrorCode (WINAPI *type_GetErrMsg)(int,char*);
	type_GetErrMsg mp_GetErrMsg;
	//Port �ʱ�ȭ
	typedef NFCErrorCode (WINAPI *type_DE_InitPort)(int,int);
	type_DE_InitPort mp_DE_InitPort;
	//Port �ʱ�ȭ
	typedef NFCErrorCode (WINAPI *type_DE_ClosePort)(int);
	type_DE_ClosePort mp_DE_ClosePort;

	//
	typedef NFCErrorCode (WINAPI *type_DE_BuzzerOn)(int);
	type_DE_BuzzerOn mp_DE_BuzzerOn;
	//
	typedef NFCErrorCode (WINAPI *type_DE_BuzzerOff)(int);
	type_DE_BuzzerOff mp_DE_BuzzerOff;
	//ī�带 ã�´�
	typedef NFCErrorCode (WINAPI *type_DE_FindCard)(int, BYTE, BYTE, BYTE, BYTE, LPINT, LPBYTE);
	type_DE_FindCard mp_DE_FindCard;

	//Mifare Ű ���� �Լ�
	typedef NFCErrorCode (WINAPI *type_DEA_Authkey)(int, BYTE, LPBYTE, BYTE);
	type_DEA_Authkey mp_DEA_Authkey;
	//Mifare Read �Լ�
	typedef NFCErrorCode (WINAPI *type_DEA_Read)(int, BYTE, LPINT, LPBYTE);
	type_DEA_Read mp_DEA_Read;
	//Mifare Write �Լ�
	typedef NFCErrorCode (WINAPI *type_DEA_Write)(int, BYTE, int, LPBYTE);
	type_DEA_Write mp_DEA_Write;


	FARPROC GetProcAddressFunc(LPCTSTR lpProcName);
public:
	//������
	CAQdeviceCmd(void);
	//�Ҹ���
	~CAQdeviceCmd(void);
	//
	BOOL initAQdeviceCmd(HWND _hWnd, LPCTSTR dlllib);
	//A Ÿ���� ī�� ���� ����
	BOOL NFCAtypeConnect(LPVOID _lpThreadParameter, ATYPEEVENTFUNC_ROUTINE _atypefunc, MTYPEEVENTFUNC_ROUTINE _mtypefunc, DISCONNECTEVENTFUNC_ROUTINE _disconnect);
	//NFC ���� ����
	void NFCDisConnect();
	//���� �����Լ�
	NFCErrorCode GetErrMsg(int errcode,char* retmsg);
	//��Ʈ �ʱ�ȭ
	NFCErrorCode DE_InitPort(int nBaud=BAUD, int nPort=USBPORT);
	//��Ʈ �ݱ�
	NFCErrorCode DE_ClosePort(int nPort=USBPORT);
	//
	NFCErrorCode DE_BuzzerOn(int nPort=USBPORT);
	//
	NFCErrorCode DE_BuzzerOff(int nPort=USBPORT);
	//ī�带 ã�´�
	NFCErrorCode DE_FindCard(LPINT outlen, LPBYTE lpRes, BYTE option, BYTE baud=0x00, BYTE cid=0x01, BYTE nad=0x01, int nPort=USBPORT);
	//Mifare Ű ���� �Լ�
	NFCErrorCode DEA_Authkey(LPBYTE keydata, BYTE blockno, BYTE mode=0x00, int nPort=USBPORT);
	//Mifare Read �Լ�
	NFCErrorCode DEA_Read(BYTE blockno, LPINT outlen, LPBYTE lpRes, int nPort=USBPORT);
	//Mifare write �Լ�
	NFCErrorCode DEA_Write(BYTE blockno, int datalen, LPBYTE data, int nPort=USBPORT);
};
#ifdef __cplusplus
	}
#endif //__cplusplus
#endif// __NFCCMD_74F736E25AB94b5dA789E4730F58F2A1_H__
