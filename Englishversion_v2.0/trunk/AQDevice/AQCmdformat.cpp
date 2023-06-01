#include "stdAfx.h"

//생성자
CAQCmdFormat::CAQCmdFormat(void)
{

}
//소멸자
CAQCmdFormat::~CAQCmdFormat(void)
{

}

//STX
const byte CAQCmdFormat::stx_aqformat = 0x02;

//Command Value
const byte CAQCmdFormat::NfcOpen = 0x10;
const byte CAQCmdFormat::NfcClose = 0x11;
const byte CAQCmdFormat::PollingLoopStart = 0x12;
const byte CAQCmdFormat::PollingLoopStop = 0x13;
const byte CAQCmdFormat::SetParameter = 0x14;
const byte CAQCmdFormat::GetParameter = 0x15;
const byte CAQCmdFormat::NdefDataWrite = 0x20;
const byte CAQCmdFormat::NdefDataRead = 0x21;
const byte CAQCmdFormat::NfcApduCmdResp = 0x22;
const byte CAQCmdFormat::SessionClose = 0x23;
const byte CAQCmdFormat::MifareAuth = 0x30;
const byte CAQCmdFormat::MifareRead = 0x31;
const byte CAQCmdFormat::MifareWrite = 0x32;
const byte CAQCmdFormat::MifareInc = 0x33;
const byte CAQCmdFormat::MifareDec = 0x34;
const byte CAQCmdFormat::MifareTranser = 0x35;
const byte CAQCmdFormat::MifareRestore = 0x36;
const byte CAQCmdFormat::FelicaCmdResp = 0x37;
const byte CAQCmdFormat::JewelTopazCmdResp = 0x38;
const byte CAQCmdFormat::Iso15693CmdResp = 0x39;
const byte CAQCmdFormat::Iso7816Open = 0x40;
const byte CAQCmdFormat::Iso7816Close = 0x41;
const byte CAQCmdFormat::Iso7816ApduCmdResp = 0x42;
const byte CAQCmdFormat::NfcIp1Send = 0x50;
const byte CAQCmdFormat::NfcIp1Recv = 0x51;
const byte CAQCmdFormat::NppServer = 0x52;
const byte CAQCmdFormat::NppClient = 0x53;
const byte CAQCmdFormat::EventData = 0x80;
const byte CAQCmdFormat::ReadReaderVersion = 0x99;

//Response Value
const byte CAQCmdFormat::Ok = 0x00;
const byte CAQCmdFormat::CrcError = 0x01;
const byte CAQCmdFormat::UnknownCommand = 0x02;
const byte CAQCmdFormat::Timeout = 0x03;

//폴링 스타트 Data
//Tyep Value
const byte CAQCmdFormat::PollingEventType_Reader = 0x01;
const byte CAQCmdFormat::PollingEventType_P2p = 0x02;
const byte CAQCmdFormat::PollingEventType_Card = 0x03;
//Reader Mode Setting
const byte CAQCmdFormat::Reader_None = 0x00;
const byte CAQCmdFormat::Reader_TypeA = 0x01;
const byte CAQCmdFormat::Reader_TypeB = 0x02;
const byte CAQCmdFormat::Reader_Felica212 = 0x04;
const byte CAQCmdFormat::Reader_Felica424 = 0x08;
const byte CAQCmdFormat::Reader_Iso15693 = 0x10;

//P2P Mode Setting
const byte CAQCmdFormat::P2P_None = 0x00;
const byte CAQCmdFormat::P2P_106Kpassive = 0x01;
const byte CAQCmdFormat::P2P_212Kpassive = 0x02;
const byte CAQCmdFormat::P2P_424Kpassive = 0x04;
const byte CAQCmdFormat::P2P_106Kactive = 0x08;
const byte CAQCmdFormat::P2P_212Kactive = 0x10;
const byte CAQCmdFormat::P2P_424Kactive = 0x20;

//Card Emulation Mode Setting
const byte CAQCmdFormat::Card_None = 0x00;
const byte CAQCmdFormat::Card_Hostcard = 0x01;
const byte CAQCmdFormat::Card_Swpcard = 0x02;
const byte CAQCmdFormat::Card_Wicard = 0x04;

//Set Parameter
//Reader Mode
const byte CAQCmdFormat::ReaderParameter_base = 0x01;
//P2P Mode
const byte CAQCmdFormat::P2PParameter_base = 0x02;
//Card Mode
const byte CAQCmdFormat::CardParameter_base = 0x04;

//NDEF Data Write
//Card Mode에 사용할 Tag Type 4의 NDEF 데이터를 Write한다

//Event Data
//Reader Mode
const byte CAQCmdFormat::ReaderEvent_base = 0x01;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered = 0x10;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_TypeA=0x01;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_MifareEtcTag=0x02;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_Mifare1k4k=0x04;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_TypeB=0x08;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_Felica=0x10;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_JewelTopaz=0x20;
const byte CAQCmdFormat::ReaderEvent_TargetDiscovered_Iso15693=0x40;
const byte CAQCmdFormat::ReaderEvent_TargetEventLost = 0x11;
//P2P Mode
const byte CAQCmdFormat::P2PEvent_base = 0x02;
const byte CAQCmdFormat::P2PEvent_NfcActivated=0x10;
const byte CAQCmdFormat::P2PEvent_NfcActivated_PassiveMode=0x00;
const byte CAQCmdFormat::P2PEvent_NfcActivated_ActiveMode=0x01;
const byte CAQCmdFormat::P2PEvent_NfcDeactivated=0x11;
const byte CAQCmdFormat::P2PEvent_NfcReciveData=0x12;
const byte CAQCmdFormat::P2PEvent_NfcNppData=0x13;
const byte CAQCmdFormat::P2PEvent_Llcp_Activated=0x14;
const byte CAQCmdFormat::P2PEvent_Llcp_DeActivated=0x15;
//Card Mode
const byte CAQCmdFormat::CardEvent_base = 0x03;
const byte CAQCmdFormat::CardEvent_FieldOn = 0x11;
const byte CAQCmdFormat::CardEvent_CardDeactivated = 0x12;
const byte CAQCmdFormat::CardEvent_CardActivated = 0x13;
const byte CAQCmdFormat::CardEvent_FieldOff = 0x14;
const byte CAQCmdFormat::CardEvent_Transaction = 0x15;

//
const CString CAQCmdFormat::mau = "00A404000BD4100000004151436F0001";
const CString CAQCmdFormat::mode_w_cmd = "00F10100020202";

unsigned short CAQCmdFormat::UpdateCrc(unsigned char ch, unsigned short* lpwCrc)
{
	ch = (ch^(unsigned char)((*lpwCrc) & 0x00FF));
	ch = (ch^(ch<<4));
	*lpwCrc = (*lpwCrc >> 8)^((unsigned short)ch << 8)^((unsigned short)ch<<3)^((unsigned short)ch>>4);
	return(*lpwCrc);
}
void CAQCmdFormat::ComputeCrc(byte *Data, int Length, byte& TransmitFirst, byte& TransmitSecond)
{
	unsigned char chBlock;
	unsigned short wCrc;
	wCrc = 0xFFFF; /* ISO/IEC 13239 (formerly ISO/IEC 3309) */
	do {
		chBlock = *Data++;
		UpdateCrc(chBlock, &wCrc);
	} while (--Length);
	wCrc = ~wCrc; /* ISO/IEC 13239 (formerly ISO/IEC 3309) */
	TransmitFirst = (byte) (wCrc & 0xFF);
	TransmitSecond = (byte) ((wCrc >> 8) & 0xFF);
}
//Command Data
byte* CAQCmdFormat::makeCommandData(byte stx, byte cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen)
{
	memset(CommandData, NULL, CommandDatalen);
	//cmd len + data len
	CommandDatalen = (1+datalen)&0xFFFF;
	CommandData[0]=stx;
	//길이 카피
	CommandData[1] = (byte) ((CommandDatalen >> 8) & 0xFF);
	CommandData[2] = (byte) (CommandDatalen & 0xFF);
	
	//memcpy(&CommandData[1], &CommandDatalen, 2);
	//명령
	CommandData[3]=cmd;
	//검증 코드
	byte first, second;
	
	//data 카피
	memcpy(&CommandData[4], &data[0], datalen);
	//stx len + len len
	CommandDatalen+=3;
	//Crc 검증 코드
	ComputeCrc(&CommandData[1], CommandDatalen-1, first, second);
	//Serial Data Total len
	//검증 코드 추가
	CommandData[CommandDatalen++]=first;
	CommandData[CommandDatalen++]=second;
	return (CommandData);
}
//Response Data
byte CAQCmdFormat::RespCommandData(byte* data, int datalen, byte* RespCommandData, int& RespCommandDatalen, byte& RespCommand)
{
	byte crc[2];
	//검증 코드
	byte first, second;
	//Crc 검증 코드 stx len Crc len 빼기
	ComputeCrc(&data[1], datalen-3, first, second);
	//Crc 코드 추출
	memcpy(&crc, &data[datalen-2], 2);
	if(crc[0]==first && crc[1]==second)
	{
		memset(RespCommandData, NULL, RespCommandDatalen);
		//Resp 데이터 길이
		byte lenth[2];
		lenth[0]=data[2];
		lenth[1]=data[1];
		memcpy(&RespCommandDatalen, &lenth[0], 2);
		//Response command
		RespCommand=data[3];
		//순수 데이터 길이 cmd len , Resp len 데이터 길이 빼기
		RespCommandDatalen-=2;
		memcpy(&RespCommandData[0], &data[5], RespCommandDatalen);
		//Resp Error 체크
		return data[4];
	}
	memset(RespCommandData, NULL, RespCommandDatalen);
	RespCommandDatalen=datalen;
	memcpy(&RespCommandData[0], &data[0], RespCommandDatalen);
	return 0xFF;
}

