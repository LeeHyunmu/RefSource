
#ifndef __AQCMDFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __AQCMDFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAQCmdFormat
{
public:
	//생성자
	CAQCmdFormat(void);
	//소멸자
	~CAQCmdFormat(void);

	//STX
	static const byte stx_aqformat;
	//Command Value
	static const byte NfcOpen;
	static const byte NfcClose;
	static const byte PollingLoopStart;
	static const byte PollingLoopStop;
	static const byte SetParameter;
	static const byte GetParameter;
	static const byte NdefDataWrite;
	static const byte NdefDataRead;
	static const byte NfcApduCmdResp;
	static const byte SessionClose;
	static const byte MifareAuth;
	static const byte MifareRead;
	static const byte MifareWrite;
	static const byte MifareInc;
	static const byte MifareDec;
	static const byte MifareTranser;
	static const byte MifareRestore;
	static const byte FelicaCmdResp;
	static const byte JewelTopazCmdResp;
	static const byte Iso15693CmdResp;
	static const byte Iso7816Open;
	static const byte Iso7816Close;
	static const byte Iso7816ApduCmdResp;
	static const byte NfcIp1Send;
	static const byte NfcIp1Recv;
	static const byte NppServer;
	static const byte NppClient;
	static const byte EventData;
	static const byte ReadReaderVersion;

	//Response Value
	static const byte Ok;
	static const byte CrcError;
	static const byte UnknownCommand;
	static const byte Timeout;

	//폴링 스타트 Data
	//Tyep Value
	static const byte CAQCmdFormat::PollingEventType_Reader;
	static const byte CAQCmdFormat::PollingEventType_P2p;
	static const byte CAQCmdFormat::PollingEventType_Card;
	//Reader Mode Setting
	static const byte Reader_None;
	static const byte Reader_TypeA;
	static const byte Reader_TypeB;
	static const byte Reader_Felica212;
	static const byte Reader_Felica424;
	static const byte Reader_Iso15693;

	//P2P Mode Setting
	static const byte P2P_None;
	static const byte P2P_106Kpassive;
	static const byte P2P_212Kpassive;
	static const byte P2P_424Kpassive;
	static const byte P2P_106Kactive;
	static const byte P2P_212Kactive;
	static const byte P2P_424Kactive;

	//Card Emulation Mode Setting
	static const byte Card_None;
	static const byte Card_Hostcard;
	static const byte Card_Swpcard;
	static const byte Card_Wicard;

	//Set Parameter
	//Reader Mode
	static const byte ReaderParameter_base;
	//P2P Mode
	static const byte P2PParameter_base;
	//Card Mode
	static const byte CardParameter_base;

	//NDEF Data Write
	//Card Mode에 사용할 Tag Type 4의 NDEF 데이터를 Write한다

	//Event Data
	//Reader Mode
	static const byte ReaderEvent_base;
	static const byte ReaderEvent_TargetDiscovered;
	static const byte ReaderEvent_TargetDiscovered_TypeA;
	static const byte ReaderEvent_TargetDiscovered_MifareEtcTag;
	static const byte ReaderEvent_TargetDiscovered_Mifare1k4k;
	static const byte ReaderEvent_TargetDiscovered_TypeB;
	static const byte ReaderEvent_TargetDiscovered_Felica;
	static const byte ReaderEvent_TargetDiscovered_JewelTopaz;
	static const byte ReaderEvent_TargetDiscovered_Iso15693;
	static const byte ReaderEvent_TargetEventLost;
	//P2P Mode
	static const byte P2PEvent_base;
	static const byte P2PEvent_NfcActivated;
	static const byte P2PEvent_NfcActivated_PassiveMode;
	static const byte P2PEvent_NfcActivated_ActiveMode;
	static const byte P2PEvent_NfcDeactivated;
	static const byte P2PEvent_NfcReciveData;
	static const byte P2PEvent_NfcNppData;
	static const byte P2PEvent_Llcp_Activated;
	static const byte P2PEvent_Llcp_DeActivated;
	//Card Mode
	static const byte CardEvent_base;
	static const byte CardEvent_FieldOn;
	static const byte CardEvent_CardDeactivated;
	static const byte CardEvent_CardActivated;
	static const byte CardEvent_FieldOff;
	static const byte CardEvent_Transaction;

	//카드 연결 커맨드
	static const CString mau;
	//상대(휴대폰) Card Change Command
	static const CString mode_w_cmd;

	//Crc c 코드
	static unsigned short UpdateCrc(unsigned char ch, unsigned short* lpwCrc);
	static void ComputeCrc(byte *Data, int Length, byte& TransmitFirst, byte& TransmitSecond);
	//Command Data
	static byte* makeCommandData(byte stx, byte cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen);
	//Response Data
	static byte RespCommandData(byte* data, int datalen, byte* RespCommandData, int& RespCommandDatalen, byte& RespCommand);
};

#endif //__AQCMDFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__

