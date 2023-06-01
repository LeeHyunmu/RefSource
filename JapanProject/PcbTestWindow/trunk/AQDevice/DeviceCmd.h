
#ifndef __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//Reader Event Func
typedef DWORD (WINAPI *WATERCHECKFUNC)(LPVOID lpThreadParameter, CAQCmdFormat::WaterCheck check);
typedef WATERCHECKFUNC WATERCHECKFUNC_ROUTINE;
typedef DWORD (WINAPI *WATERDEBUGFUNC)(LPVOID lpThreadParameter, byte* data, int datalen, bool bsend);
typedef WATERDEBUGFUNC WATERDEBUGFUNC_ROUTINE;
class CDeviceCmd
{
	//�̱��� ��ü
	static CDeviceCmd* AQdeviceSingle;

	//watercheck Func
	WATERCHECKFUNC_ROUTINE watercheckfunc;
	//debug func
	WATERDEBUGFUNC_ROUTINE waterdebug;
	//�̺�Ʈ�� ���� Ŭ���� ��ü
	LPVOID lpEventTaget;

	//�ø��� ��� Ŭ����
	CSerial nfcserial;
	//Resp Func Code
	CAQCmdFormat::WaterCheck RespFuncFlag;
	//Resp Error Code
	CAQCmdFormat::WaterErrorCode RespErrorCode;
	// ���� ������ ����
	byte NfcReceiveData[MAX_COMMANDDATA];
	//���� ������ ����
	int NfcReceivelen;
	// ���� ������ ����
	byte RespData[MAX_COMMANDDATA];
	//���� ������ ����
	int Resplen;
	// ���� ������ ����
	byte RespErrorData[MAX_COMMANDDATA];
	int RespErrorlen;
	//Resp �̺�Ʈ�� �ޱ� ���� ������
	//NFC Event thread���� �̺�Ʈ ���(���� �̺�Ʈ Open, ��� �Լ� Ÿ������ ���� ��ü���� ��� ��Ȳ���� ȣ��)
	CLeeEvent RespEvent;
	//NFC Event thread Command�� ���� ��� �̺�Ʈ�� �޴´�
	CThread NfcThread;
	//AQ ����̽��� Target �� �������� �� �̺�Ʈ ó���� ���� ������
	//NfcThread �����带 ����� �ȵǹǷ� ������ ������� ó���ϱ� ���� ��ü
	CThread CmdEventThread;
	//CmdEventThread ��ü���� ����ϴ� �̺�Ʈ ��ü
	CLeeEvent EventRespEvent;

	//�޽��� �̺�Ʈ ������ �ڵ�
	//HWND hWnd;
	
	//�ø��� ����Ʈ ����
	CString ActiveComPort;

	//�ø��� ��Ʈ ã��
	//
	static size_t GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString);

	void ComputeCrc(byte *Data, int Length, byte& chBlock);
	//Command Data
	UINT makeCommandData(CAQCmdFormat::WateAddress cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen);
	//���� �ļ��� ������ ���� �Լ�
	int GetNfcLoddingData(byte* data, int& datalen);
	//�̺�Ʈ ����
	BOOL NfcLoop();
	//�̺�Ʈ ����
	BOOL NfcEventLoop();
	//Serial Polling Loop
	CAQCmdFormat::WaterErrorCode NfcResponseLoop(CAQCmdFormat::WaterCheck& func, byte* data, int readlen);

	//Ŭ���� ��ü�� �ʱ� ���·� ����
	void Release(void);

	//thread �Լ�
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);
	//Event thread �Լ�
	static  DWORD WINAPI EventThreadProcess(LPVOID lpArg);
public:
	//AQ device Ŭ���� �ν��Ͻ� ����
	static CDeviceCmd* getInstance();
	//AQ device Ŭ���� �ν��Ͻ� ��ü �Ҹ�
	static void ReleaseInstance();
	//������
	CDeviceCmd(void);
	//�Ҹ���
	~CDeviceCmd(void);
	//NFC ��� ���� �޴���
	CAQCmdFormat::WaterErrorCode initAQNFC(LPVOID lpThreadParameter, WATERCHECKFUNC_ROUTINE _watercheckfunc, WATERDEBUGFUNC_ROUTINE _debugfunc=NULL);

	//����̽� �ٽ� �ʱ�ȭ(�ø��� ��Ʈ���� �ʱ�ȭ)
	BOOL ModeReset_init();

	//integer ��Ʋ����� �򿣵�� ���� byte type����
	void integerEndianChangebyte(int _value, byte* _data);
	//byte type ��Ʋ����� �򿣵�� ���� integer type����
	void byteEndianChangeinteger(byte* _data, int& _value);
	//short ��Ʋ����� �򿣵�� ���� byte type����
	void shortEndianChangebyte(int _value, byte* _data);
	//byte type ��Ʋ����� �򿣵�� ���� short type����
	void byteEndianChangeshort(byte* _data, int& _value);
	//���� ������
	void CommandSend(byte data);
};
#endif// __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__
