
#ifndef __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_COMMANDDATA 512
#define THREAD_LOOPTIME 200
#define POLLINGSTARTLEN 3
class CDeviceCmd
{
	CNfcUriRecord st;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//NFC ���� �÷���
	enum ComKind{
		Ok =  0x00,
		CrcError = 0x01,
		UnKnownCommand = 0x02,
		Timeout = 0x03,
		NfcOpen = 0x10,
		NfcClose = 0x11,
		PollingLoopStart = 0x12,
		PollingLoopStop = 0x13,
		SetParameter = 0x14,
		GetParameter = 0x15,
		NdefDataWrite = 0x20,
		NdefDataRead = 0x21,
		NfcApduCmdResp = 0x22,
		SessionClose = 0x23,
		MifareAuth = 0x30,
		MifareRead = 0x31,
		MifareWrite = 0x32,
		MifareInc = 0x33,
		MifareDec = 0x34,
		MifareTranser = 0x35,
		MifareRestore = 0x36,
		FelicaCmdResp = 0x37,
		JewelTopazCmdResp = 0x38,
		Iso15693CmdResp = 0x39,
		Iso7816Open = 0x40,
		Iso7816Close = 0x41,
		Iso7816ApduCmdResp = 0x42,
		NfcIp1Send = 0x50,
		NfcIp1Recv = 0x51,
		NppServer = 0x52,
		NppClient = 0x53,
		EventData = 0x80,
		ReadReaderVersion = 0x99,
		None = 0xFF};
		//NFC ReaderMode Event
		enum ReaderModeEvent{
			TargetDiscovered = 0x10,
			TargetEventLost = 0x11
		};
		//NFC P2pMode Event
		enum P2pModeEvent{
			NfcActivated=0x10,
			NfcDeactivated=0x11,
			NfcReciveData=0x12,
			NfcNppData=0x13,
			Llcp_Activated=0x14,
			Llcp_DeActivated=0x15
		};
		//NFC CardMode Event
		enum CardModeEvent{
			FieldOn = 0x11,
			CardDeactivated = 0x12,
			CardActivated = 0x13,
			FieldOff = 0x14,
			Transaction = 0x15
		};
		//NFC ReaderMode Event Discovered TargetType
		enum ReaderModeDiscovered{
			TypeA=0x01,
			MifareEtcTag=0x02,
			Mifare1k4k=0x04,
			TypeB=0x08,
			Felica=0x10,
			JewelTopaz=0x20,
			Iso15693=0x40
		};
		//NFC Mode Ÿ��
		enum EventType{
			NfcEventComposite = 0x00,
			NfcEventReaderType = 0x01,
			NfcEventP2pType = 0x02,
			NfcEventCardType = 0x03,
			NfcEventNone = 0xFF
		};
		//PollingStartMode ���� �÷���
		EventType PollingMode;
		//NFC ���� �÷���
		enum MyCmdCall{
			NdefDataChange = 0x01,
			FiledOffMsg = 0x02,
			FiledOnTimeOut=0x03
		};
	//NFC Open fd
	byte nfcopenfd;
	//�ø��� ��� Ŭ����
	CSerial nfcserial;
	//Resp Flag
	ComKind RespFlag;
	// ���� ������ ����
	byte NfcReceiveData[MAX_COMMANDDATA];
	//���� ������ ����
	int NfcReceivelen;
	// ���� ������ ����
	byte RespData[MAX_COMMANDDATA];
	//���� ������ ����
	int Resplen;
	//
	CEvent RespEvent;
	//
	CThread NfcThread;

	//�޽��� �̺�Ʈ ������ �ڵ�
	HWND hWnd;
	// Nfc data num
	int NFCCommandNum;
	//���� 
	int NFCCommandSettingNum;
	//Command data �����
	CAtlmapNfcData commanddata;
	//�ø��� ����Ʈ ����
	CString ActiveComPort;
	//
	byte PollingStartCommand[POLLINGSTARTLEN+1];
	//
	BOOL bCardFieldOn;
	//CardFieldOnTime Delary
	DWORD FieldOnTime;
	//
	BOOL bNFCOpen;
	//
	BOOL bISO7816Open;
	//
	byte Iso7816Opendata[2];
public:
	//������
	CDeviceCmd(void);
	//�Ҹ���
	~CDeviceCmd(void);
	//���� �ļ��� ������ ���� �Լ�
	int GetNfcLoddingData(byte* data, int& datalen);
	//�̺�Ʈ ����
	BOOL NfcLoop();
	//����̽� �ʱ�ȭ
	BOOL initDevice(HWND _hWnd, IMyObject* _obj);
	//����̽� �ٽ� �ʱ�ȭ(�ø��� ��Ʈ���� �ʱ�ȭ)
	BOOL ModeReset_init();
	//NFC Open
	BOOL NFCOpen();
	//NFC Close
	BOOL NFCClose();
	// 	data[0]=CAQCmdFormat::Reader_TypeA;
	// 	data[1]=CAQCmdFormat::P2P_None;
	// 	data[2]=CAQCmdFormat::Card_None;
	//Polling Start
	BOOL PollingSettingStart(byte reader, byte p2p, byte card);
	//Polling Start
	BOOL PollingStart(byte reader, byte p2p, byte card);
	//Polling ReStart
	BOOL PollingReStart();
	//Polling Stop
	BOOL PollingStop();
	//Session close
	BOOL SESSIONClose();
	//Serial Polling Loop
	ComKind NfcResponseLoop(byte* data, int readlen);
	//Apdu cmdresp
	BOOL NfcApduCommand(byte* data, int len);

	//Ndef data Write
	BOOL NDEFDataWrite(byte* data, int len);
	//Ndef data Read
	BOOL NDEFDataRead();
	//Event Call
	BOOL NfcEventCall(WPARAM wParam);
	//
	//NDEFData Change Message
	BOOL DeviceCmdCallMessage(WPARAM wParam);
	//thread �Լ�
	static DWORD WINAPI ThreadProcess(LPVOID lpArg);

	//Reader Mode Event
	void ReaderEventMessage(byte* data, int len);
	//P2p Mode Event
	void P2pEventMessage(byte* data, int len);
	//Card Mode Event
	void CardEventMessage(byte* data, int len);
	// set NFCCommandNum
	void SetNFCCommandNum(int num);
	//Card Mode ó�� ���� �Լ�
	void SetFirstNFCCardModeSetting(int num);
	//Reader Mode ó�� ���� �Լ�
	void SetFirstNFCReaderModeSetting(int num);
	//NFC ���� ������ NDEF �Ľ� ����
	void LoadCommandData();
	//ī�� ������ Change Loop
	BOOL NDEFWriteChangeLoop();
	//
	BOOL JEWELTOPAZCmdResp(byte _cmd, byte* _data, int len);
	//
	BOOL ISO15693CmdResp(byte _cmd, byte* payload, int len);
	//
	BOOL ISO7816Open(byte samIndex);
	//
	BOOL ISO7816Close();
	//
	BOOL ISO7816CmdResp(byte* cmddata, int len);
	//P2p ���� �Լ�
	//
	BOOL NFCIp1Send(byte mi, byte* _data, int len);
	//
	BOOL NPPServer();
	//
	BOOL NPPClient(byte* _data, int len);
	//
	void NDEFWrite_init();
};
#endif// __DEVICECMD_74F736E25AB94b5dA789E4730F58F2A1_H__
