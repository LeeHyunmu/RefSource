
#ifndef __WATERFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __WATERFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWaterFormat
{
public:
	//持失切
	CWaterFormat(void){};
	//社瑚切
	~CWaterFormat(void){};

	static enum AQCmdKind{
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
		Kind_None = 0xFF
	};
	static enum AQErrorCode{
		Any_Ok =  0x00,
		Any_E_NotConnected=0x01,
		Any_E_CmdParUnknown=0x02,
		Any_E_Nok=0x03,
		Any_E_NoPipeAvailable=0x04,
		Any_E_RegParUnknown=0x05,
		Any_E_PipeNotOpened=0x06,
		Any_E_CmdNotSupported=0x07,
		Any_E_Inhibited=0x08,
		Any_E_Timeout=0x09,
		Any_E_RegAccessDenied=0x0A,
		Any_E_PipeAccessDenied=0x0B,
		Any_E_CrcError=0x80,
		Any_E_NotNfcOpen=0xF1,
		Any_E_NotNfc7816Open=0xF2,
		Any_E_NotSerialPort=0xF3,
		Any_E_None=0xFF
	};
};
#endif //__WATERFORMAT_74F736E25AB94b5dA789E4730F58F2A1_H__

