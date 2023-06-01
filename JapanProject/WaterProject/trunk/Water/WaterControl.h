
#ifndef __WATERCONTROL_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __WATERCONTROL_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Reader Event Func
typedef DWORD (WINAPI *READEREVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef READEREVENTFUNC READEREVENTFUNC_ROUTINE;
//P2P Event Func
typedef DWORD (WINAPI *P2PEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef P2PEVENTFUNC P2PEVENTFUNC_ROUTINE;
//Card Event Func
typedef DWORD (WINAPI *CARDEVENTFUNC)(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen);
typedef CARDEVENTFUNC CARDEVENTFUNC_ROUTINE;
class CWaterControl
{
	BOOL bWaterOpen;
	CWaterFormat::AQCmdKind RespFuncFlag;
	//Reader Func
	READEREVENTFUNC_ROUTINE readfunc;
	//P2P Func
	P2PEVENTFUNC_ROUTINE p2pfunc;
	//Reader Func
	CARDEVENTFUNC_ROUTINE cardfunc;
	//�̺�Ʈ�� ���� Ŭ���� ��ü
	LPVOID lpEventTaget;

	CSerial nfcserial;
	// ���� ������ ����
	byte NfcReceiveData[MAX_COMMANDDATA];
	//���� ������ ����
	int NfcReceivelen;
	// ���� ������ ����
	byte RespData[MAX_COMMANDDATA];
	//���� ������ ����
	int Resplen;
	//Resp �̺�Ʈ�� �ޱ� ���� ������
	//NFC Event thread���� �̺�Ʈ ���(���� �̺�Ʈ Open, ��� �Լ� Ÿ������ ���� ��ü���� ��� ��Ȳ���� ȣ��)
	CEvent RespEvent;
	//NFC Event thread Command�� ���� ��� �̺�Ʈ�� �޴´�
	CThread NfcThread;
	//AQ ����̽��� Target �� �������� �� �̺�Ʈ ó���� ���� ������
	//NfcThread �����带 ����� �ȵǹǷ� ������ ������� ó���ϱ� ���� ��ü
	CThread CmdEventThread;
	//CmdEventThread ��ü���� ����ϴ� �̺�Ʈ ��ü
	CEvent EventRespEvent;

	//�޽��� �̺�Ʈ ������ �ڵ�
	//HWND hWnd;
	
	//�ø��� ����Ʈ ����
	CString ActiveComPort;
	//�ø��� ��Ʈ ã��
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);
	//Crc c �ڵ�
	unsigned short UpdateCrc(unsigned char ch, unsigned short* lpwCrc);
	void ComputeCrc(byte *Data, int Length, byte& TransmitFirst, byte& TransmitSecond);
	//Command Data
	UINT makeCommandData(byte stx, byte cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen);
	//���� �ļ��� ������ ���� �Լ�
	int GetNfcLoddingData(byte* data, int& datalen);
	//�̺�Ʈ ����
	BOOL NfcLoop();
	//�̺�Ʈ ����
	BOOL NfcEventLoop();
	//Serial Polling Loop
	CWaterFormat::AQErrorCode CWaterControl::NfcResponseLoop(CWaterFormat::AQCmdKind& func, byte* data, int readlen);


	//thread �Լ�
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//Event thread �Լ�
	static  DWORD WINAPI EventThreadProcess(LPVOID lpArg);

	void Release();
public:
	void initWater(LPVOID lpThreadParameter);
	//������
	CWaterControl(void);
	//�Ҹ���
	~CWaterControl(void);
};
#endif// __WATERCONTROL_74F736E25AB94b5dA789E4730F58F2A1_H__
