
#include "stdAfx.h"
#include <tchar.h>

#define CARD_FILEDTIMEOUT 5000
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDeviceCmd::CDeviceCmd(void):
nfcopenfd(0x00),
RespFlag(UnKnownCommand),
NFCCommandNum(0),
NFCCommandSettingNum(0),
NfcReceivelen(0),
bCardFieldOn(FALSE),
bNFCOpen(FALSE),
bISO7816Open(FALSE),
FieldOnTime(0),
PollingMode(NfcEventNone)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CDeviceCmd::~CDeviceCmd(void)
{
	PollingStop();
	NFCClose(); RespFlag=None;
	//컴포트 닫기
	nfcserial.Close();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::initDevice(HWND _hWnd, IMyObject* _obj)
{
	BOOL b=FALSE;
	//========================================
	// 오브젝트 관리 생성 관리 클래스
	//========================================
	obj=_obj;
	//========================================
	// 디버그 출력 클래스
	//========================================
	debug=MYDEBUG;
	LoadCommandData();
	hWnd = _hWnd;

	//COM 포트 레지스트리 검색
	vector<CString> valueString;
	CRegistHelp::GetFindRegKeyEnumValue(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), valueString);
	ActiveComPort="";
	for (int i = 0; i <(int) valueString.size(); i++)
	{
		if(valueString[i].Compare(_T("COM1"))!=0&&valueString[i].Compare(_T("COM2"))!=0)
		{
			debug->Write(valueString[i].GetBuffer(0));
			//시리얼 통신
			if(nfcserial.Open(valueString[i]))
			{
				debug->Write(_T("Serial Open ok"));
				//스래드 작동
				NfcThread.Create(ThreadProcess, (LPVOID)this);
				RespEvent.Create();
				//NFC Open
				if(NFCOpen())
				{
					ActiveComPort=valueString[i];
					b=TRUE;
					break;
				}
			}
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::SetNFCCommandNum(int num)
{
	NFCCommandNum=num;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::SetFirstNFCCardModeSetting(int num)
{
	debug->Write(_T("SetFirstNFCCardModeSetting"));
	//처음 세팅 데이터 넘버
	NFCCommandNum=num;
	//카드 NDEF 데이터를 바꿔야 하기 때문에 넘버가 틀릴 때 Card에 Write 하도록 구성(스래드 안에서 감지)
	NFCCommandSettingNum = NFCCommandNum;
	byte sendbuffer[MAX_COMMANDDATA];
	int intlen = commanddata.GetNdefData(NFCCommandNum, &sendbuffer[0], MAX_COMMANDDATA);
	//초기 NDEF 데이터 Write
	if(NDEFDataWrite(sendbuffer, intlen))
	{
		if(PollingSettingStart(CAQCmdFormat::Reader_None, CAQCmdFormat::P2P_None,	CAQCmdFormat::Card_Hostcard))
		{
			PollingMode=NfcEventCardType;
			//debug->Write(_T("Card Mode Start Ok"));
		}
		else
		{
			debug->Write(_T("Card Mode Start Error"));
		}
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::SetFirstNFCReaderModeSetting(int num)
{
	debug->Write(_T("SetFirstNFCReaderModeSetting"));
	//처음 세팅 데이터 넘버
	NFCCommandNum=num;
	//
	NFCCommandSettingNum = NFCCommandNum;
	if(PollingSettingStart(CAQCmdFormat::Reader_TypeA, CAQCmdFormat::P2P_None,	CAQCmdFormat::Card_None))
	{
		PollingMode=NfcEventReaderType;
		//debug->Write(_T("Reader Mode Start Ok"));
	}
	else
	{
		debug->Write(_T("Reader Mode Start Error"));
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NDEFWriteChangeLoop()
{
	BOOL b=FALSE;
	debug->Write(_T("NDEFWriteChangeLoop"));
	byte sendbuffer[MAX_COMMANDDATA];
	int intlen = commanddata.GetNdefData(NFCCommandNum, &sendbuffer[0], MAX_COMMANDDATA);
	if(NDEFDataWrite(sendbuffer, intlen))
	{
		if(PollingReStart())
		{
			b=TRUE;
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::NDEFWrite_init()
{
	debug->Write(_T("NDEFWrite"));
	byte sendbuffer[MAX_COMMANDDATA];
	int intlen = commanddata.GetNdefData(NFCCommandSettingNum, &sendbuffer[0], MAX_COMMANDDATA);
	if(NDEFDataWrite(sendbuffer, intlen))
	{
		PollingReStart();
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::ModeReset_init()
{
	BOOL b=FALSE;
	debug->Write(_T("Mode ReStart"));
	NFCClose();
	//컴포트 닫기
	nfcserial.Close();
	//
	NfcReceivelen=0;
	RespFlag=None;
	bCardFieldOn=FALSE;
	memset(&NfcReceiveData[0], NULL, MAX_COMMANDDATA);
	memset(&RespData[0], NULL, MAX_COMMANDDATA);
	//시리얼 통신
	if(ActiveComPort.GetLength()>3 && nfcserial.Open(ActiveComPort))
	{
		debug->Write(_T("Serial Open ok"));
		//NFC Open
		if(NFCOpen())
		{
			switch(PollingMode)
			{
			case NfcEventCardType:
				{
					debug->Write(_T("Card Mode Reset NDEFWrite"));
					byte sendbuffer[MAX_COMMANDDATA];
					int intlen = commanddata.GetNdefData(NFCCommandSettingNum, &sendbuffer[0], MAX_COMMANDDATA);
					if(NDEFDataWrite(sendbuffer, intlen))
					{
						PollingReStart();
					}
				}break;
			case NfcEventReaderType:
				{

				}
			case NfcEventP2pType:
				{
					PollingReStart();
				}break;
			}
			b=TRUE;
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::LoadCommandData()
{
// 	byte apdutemp[]={0x92, 0x17, 0x0A, 0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69,
// 		0x6F, 0x6E, 0x2F, 0x76, 0x6E, 0x64,0x2E, 0x61, 0x71, 0x2E, 0x64, 0x61, 0x74, 0x61, 0x31, 0x32,
// 		0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x51, 0x01, 0x29, 0x55, 0x00, 0x68, 0x74, 0x74,
// 		0x70, 0x3A, 0x2F, 0x2F, 0x31, 0x39, 0x32, 0x2E, 0x31, 0x36, 0x38, 0x2E, 0x31, 0x30, 0x2E, 0x32,
// 		0x30, 0x33, 0x2F, 0x41, 0x6E, 0x64, 0x72, 0x6F, 0x69, 0x64, 0x4E, 0x66, 0x63, 0x54, 0x65, 0x73,
// 		0x74, 0x2E, 0x61, 0x70, 0x6B};

	byte apdu[MAX_COMMANDDATA];
	int apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:ZenHideAway;http://121.254.234.134/p/01.html", apdu, apdulen);
	int urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(0, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:Hoabin;http://121.254.234.134/p/02.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(1, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:Angelinus;http://121.254.234.134/p/03.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(2, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:CaffeBene;http://121.254.234.134/p/04.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(3, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:FamilyMart;http://121.254.234.134/p/05.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(4, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:Lotteria;http://121.254.234.134/p/06.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(5, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:Totes;http://121.254.234.134/p/07.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(6, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("coupon:Pylones;http://121.254.234.134/p/08.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(7, apdu, apdulen);

// 	CouponRecord::makePayloadData("sid=1:bid=00000000000000000032", apdu, apdulen);
// 	int urilen = MAX_COMMANDDATA-apdulen;
// 	CNfcUriRecord::makePayloadData("http://", "http://1.234.6.207/a.apk", &apdu[apdulen], urilen);
// 	apdulen+=urilen;
// 	commanddata.AddData(0, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("music:http://www.youtube.com/watch?v=9e5UHSc6u0M", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(8, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("music:http://www.youtube.com/watch?v=xqe9nXEyG0o", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(9, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("mp3:http://121.254.234.134/p/mp3/Disco_Lounge.mp3", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(10, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("mp3:http://121.254.234.134/p/mp3/Avant_Jazz.mp3", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(11, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("video:lotteria;http://121.254.234.134/p/lotteria.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(12, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("video:caffebene;http://121.254.234.134/p/caffebene.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(13, apdu, apdulen);

	apdulen = MAX_COMMANDDATA;
	ContentsMallRecord::makePayloadData("video:BaskinRobbins31;http://121.254.234.134/p/BaskinRobbins.html", apdu, apdulen);
	urilen = MAX_COMMANDDATA-apdulen;
	CNfcUriRecord::makePayloadData("http://", "http://market.android.com/details?id=kr.co.aqnfcapp", &apdu[apdulen], urilen);
	apdulen+=urilen;
	commanddata.AddData(14, apdu, apdulen);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NFCOpen()
{
	//NFC Open
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	//NFC Open
	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NfcOpen, data, 0, &CommandData[0], CommandDatalen);
	nfcserial.SendData(&CommandData[0], CommandDatalen);
	debug->Write(_T("NFCOpen sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));

	//1초 대기
	if(RespEvent.WaitForEvent(1000))
	{
		if(RespFlag==NfcOpen)
		{
			// 오픈 fd 값 저장
			nfcopenfd = RespData[0];
			//debug->Write(_T("NFCOpen Ok"));
			bNFCOpen=TRUE;
			return TRUE;
		}
	}
	debug->Write(_T("NFCOpen Faile"));
	return FALSE;
}

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NFCClose()
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	//NFC Open
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	//NFC Close
	data[0]=nfcopenfd;
	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NfcClose, data, 1, &CommandData[0], CommandDatalen);
	debug->Write(_T("NFCClose sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	int sendsize = nfcserial.SendData(&CommandData[0], CommandDatalen);

	//1초 대기
	if(RespEvent.WaitForEvent(1000))
	{
		if(RespFlag==NfcClose)
		{
			//debug->Write(_T("NFCClose Ok"));
			bNFCOpen=FALSE;
			return TRUE;
		}
	}
	debug->Write(_T("NFCClose Faile"));
	return FALSE;
}

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::PollingSettingStart(byte reader, byte p2p, byte card)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	//
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	PollingStartCommand[0]=reader;
	PollingStartCommand[1]=p2p;
	PollingStartCommand[2]=card;
	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::PollingLoopStart, PollingStartCommand, POLLINGSTARTLEN, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("PollingSettingStart sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==PollingLoopStart)
		{
			//debug->Write(_T("Polling Start Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("Polling Start Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::PollingStart(byte reader, byte p2p, byte card)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	//
	byte StartCommand[POLLINGSTARTLEN+1];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	StartCommand[0]=reader;
	StartCommand[1]=p2p;
	StartCommand[2]=card;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::PollingLoopStart, StartCommand, POLLINGSTARTLEN, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("PollingStart sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==PollingLoopStart)
		{
			//debug->Write(_T("Polling Start Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("Polling Start Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::PollingReStart()
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	//
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;
	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::PollingLoopStart, PollingStartCommand, POLLINGSTARTLEN, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("PollingReStart sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==PollingLoopStart)
		{
			//debug->Write(_T("Polling ReStart Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("Polling ReStart Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::PollingStop()
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::PollingLoopStop, data, 0, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==PollingLoopStop)
		{
			//debug->Write(_T("Polling Stop Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("Polling Stop Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::SESSIONClose()
{
	//
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::SessionClose, data, 0, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("SESSIONClose sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(1000))
	{
		if(RespFlag==SessionClose)
		{
			//debug->Write(_T("SESSIONClose Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("SESSIONClose Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NfcApduCommand(byte* data, int len)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NfcApduCmdResp, data, len, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("NfcApduCommand sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==NfcApduCmdResp)
		{
			CString respstring = CTypeChangHelp::byteToHexbyteValue(RespData, Resplen);
			CString ComFind = respstring.Right(4);
			if(dt.GetLength()>=4 && ComFind.Compare("9000")==0)
			{
				//debug->Write(_T("NfcApduCommand Ok"));
				return TRUE;
			}
		}
	}
	debug->Write(_T("NfcApduCommand Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NDEFDataWrite(byte* data, int len)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NdefDataWrite, data, len, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("NDEFDataWrite sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==NdefDataWrite)
		{
			//debug->Write(_T("NDEFDataWrite Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("NDEFDataWrite Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NDEFDataRead()
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	//
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NdefDataRead, data, 0, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("NDEFDataRead sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==NdefDataRead)		{
			//debug->Write(_T("NDEFDataRead Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("NDEFDataRead Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::JEWELTOPAZCmdResp(byte _cmd, byte* _data, int len)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	int sendcmdlen=1;
	//cmd
	memcpy(&data[sendcmdlen], &_cmd, sendcmdlen);
	//payload data
	memcpy(&data[sendcmdlen], &_data[0], len);
	sendcmdlen+=len;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::JewelTopazCmdResp, data, sendcmdlen, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("JEWELTOPAZCmdResp sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==JewelTopazCmdResp)
		{
			CString respstring = CTypeChangHelp::byteToHexbyteValue(RespData, Resplen);
			CString ComFind = respstring.Right(4);
			if(dt.GetLength()>=4 && ComFind.Compare("9000")==0)
			{
				//debug->Write(_T("JewelTopaz Command Ok"));
				return TRUE;
			}
		}
	}
	debug->Write(_T("JewelTopaz Command Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::ISO15693CmdResp(byte _cmd, byte* payload, int len)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	int sendcmdlen=1;
	//cmd
	memcpy(&data[sendcmdlen], &_cmd, sendcmdlen);
	//payload data
	memcpy(&data[sendcmdlen], &payload[0], len);
	sendcmdlen+=len;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::Iso15693CmdResp, data, sendcmdlen, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("ISO15693CmdResp sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==Iso15693CmdResp)
		{
			CString respstring = CTypeChangHelp::byteToHexbyteValue(RespData, Resplen);
			CString ComFind = respstring.Right(4);
			if(dt.GetLength()>=4 && ComFind.Compare("9000")==0)
			{
				//debug->Write(_T("Iso15693 Command Ok"));
				return TRUE;
			}
		}
	}
	debug->Write(_T("Iso15693 Command Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::ISO7816Open(byte samIndex)
{
	//ISO7816 Open
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	int sendcmdlen = 1;
	//SAM INDEX
	memcpy(&data[0], &samIndex, sendcmdlen);
	//NFC Open
	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NfcOpen, data, sendcmdlen, &CommandData[0], CommandDatalen);
	nfcserial.SendData(&CommandData[0], CommandDatalen);
	debug->Write(_T("ISO7816Open sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));

	//1초 대기
	if(RespEvent.WaitForEvent(1000))
	{
		if(RespFlag==NfcOpen)
		{
			// 오픈 fd 값 저장
			memcpy(&Iso7816Opendata[0], &RespData[0], 2);
			//debug->Write(_T("ISO7816Open Ok"));
			bISO7816Open=TRUE;
			return TRUE;
		}
	}
	debug->Write(_T("ISO7816Open Faile"));
	return FALSE;
}

// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::ISO7816Close()
{
	if(bISO7816Open==FALSE)
	{
		return FALSE;
	}
	//
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;
	
	int sendcmdlen = 1;
	//fd
	memcpy(&data[0], &Iso7816Opendata[1], sendcmdlen);

	//NFC Close
	data[0]=nfcopenfd;
	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::Iso7816Close, data, sendcmdlen, &CommandData[0], CommandDatalen);
	debug->Write(_T("Iso7816Close sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	int sendsize = nfcserial.SendData(&CommandData[0], CommandDatalen);

	//1초 대기
	if(RespEvent.WaitForEvent(1000))
	{
		if(RespFlag==Iso7816Close)
		{
			//debug->Write(_T("Iso7816Close Ok"));
			bISO7816Open=FALSE;
			return TRUE;
		}
	}
	debug->Write(_T("Iso7816Close Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::ISO7816CmdResp(byte* cmddata, int len)
{
	if(bISO7816Open==FALSE)
	{
		return FALSE;
	}
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	int sendcmdlen = 1;
	//fd
	memcpy(&data[0], &Iso7816Opendata[1], sendcmdlen);
	//cmddata n Apdu data
	memcpy(&data[sendcmdlen], &cmddata[0], len);
	sendcmdlen+=len;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::Iso7816ApduCmdResp, data, sendcmdlen, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("ISO7816CmdResp sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==Iso7816ApduCmdResp)
		{
			CString respstring = CTypeChangHelp::byteToHexbyteValue(RespData, Resplen);
			CString ComFind = respstring.Right(4);
			if(dt.GetLength()>=4 && ComFind.Compare("9000")==0)
			{
				//debug->Write(_T("Iso7816ApduCmdResp Command Ok"));
				return TRUE;
			}
		}
	}
	debug->Write(_T("Iso7816ApduCmdResp Command Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::NFCIp1Send(byte mi, byte* _data, int len)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	int sendcmdlen = 1;
	//fd
	memcpy(&data[0], &mi, sendcmdlen);
	//cmddata n Apdu data
	memcpy(&data[sendcmdlen], &_data[0], len);
	sendcmdlen+=len;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NfcIp1Send, data, sendcmdlen, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("NfcIp1Send sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==NfcIp1Send)
		{
			//debug->Write(_T("NfcIp1Send Command Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("NfcIp1Send Command Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::NPPServer()
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	//
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NppServer, data, 0, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("NppServer sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==NdefDataRead)
		{
			//debug->Write(_T("NppServer Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("NppServer Faile"));
	return FALSE;
}
// --------------------------------------------------------------
// |
// |
// |
// ---------------------------------------------------------------
BOOL CDeviceCmd::NPPClient(byte* _data, int len)
{
	if(bNFCOpen==FALSE)
	{
		return FALSE;
	}
	byte data[MAX_COMMANDDATA];
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	int sendcmdlen = 4;
	//길이 카피
	data[0] = (byte) ((CommandDatalen >> 24) & 0xFF);
	data[1] = (byte) ((CommandDatalen >> 16) & 0xFF);
	data[2] = (byte) ((CommandDatalen >> 8) & 0xFF);
	data[3] = (byte) (CommandDatalen & 0xFF);

	//cmddata n Apdu data
	memcpy(&data[sendcmdlen], &_data[0], len);
	sendcmdlen+=len;

	CAQCmdFormat::makeCommandData(CAQCmdFormat::stx_aqformat, CAQCmdFormat::NppClient, data, sendcmdlen, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	debug->Write(_T("NppClient sendcommand"));
	CString dt = CTypeChangHelp::byteToHexbyteValue(CommandData, CommandDatalen);
	//debug->Write(dt.GetBuffer(0));
	//1초 대기
	if(RespEvent.WaitForEvent(500))
	{
		if(RespFlag==NfcIp1Send)
		{
			//debug->Write(_T("NppClient Command Ok"));
			return TRUE;
		}
	}
	debug->Write(_T("NppClient Command Faile"));
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CDeviceCmd::GetNfcLoddingData(byte* data, int& datalen)
{
	if(NfcReceivelen>0)
	{
		memset(&data[0], NULL, datalen);
		//Resp 데이터 길이
		byte lenth[2];
		lenth[0]=NfcReceiveData[2];
		lenth[1]=NfcReceiveData[1];
		//데이터 길이 
		memcpy(&datalen, &lenth[0], 2);
		//stx len + len len 2 +  Crc len 추가
		datalen+=5;
		if(NfcReceivelen>=datalen)
		{
			memcpy(&data[0], &NfcReceiveData[0], datalen);
			debug->Write(_T("RespData"));
			CString dt = CTypeChangHelp::byteToHexbyteValue(data, datalen);
			debug->Write(dt.GetBuffer(0));
			//
			//남은 데이터 조정
			NfcReceivelen-=datalen;
			if(NfcReceivelen<4) NfcReceivelen=0;
			if(NfcReceivelen>0)
			{
				byte backdata[MAX_COMMANDDATA];
				memcpy(&backdata[0], &NfcReceiveData[datalen], NfcReceivelen);
				//조정 데이터 저장
				memcpy(&NfcReceiveData[0], &backdata[0], NfcReceivelen);
			}
			return datalen;
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NfcLoop()
{
	switch(PollingMode)
	{
	case NfcEventCardType:
		{
			if(bCardFieldOn==FALSE && NFCCommandSettingNum != NFCCommandNum)
			{
				NFCCommandSettingNum = NFCCommandNum;
				PostMessage(hWnd, WM_DEVICECMDCALLMESSAGE, NdefDataChange, NULL);
			}
			else if(bCardFieldOn==TRUE)
			{
				if(FieldOnTime<=GetTickCount())
				{
					bCardFieldOn=FALSE;
					PostMessage(hWnd, WM_DEVICECMDCALLMESSAGE, FiledOnTimeOut, NULL);
				}
			}
		}break;
	case NfcEventReaderType:
		{

		}
	case NfcEventP2pType:
		{
			if(NFCCommandSettingNum != NFCCommandNum)
			{
				NFCCommandSettingNum = NFCCommandNum;
			}
		}break;
	}

	if(NfcReceivelen==0)
	{
		if(nfcserial.IsOpened())
		{
			memset(&NfcReceiveData[0], NULL, MAX_COMMANDDATA);
			NfcReceivelen = nfcserial.ReadData(NfcReceiveData, MAX_COMMANDDATA);
		}
	}
	// 받은 데이터 길이
	int datalen=MAX_COMMANDDATA;
	byte data[MAX_COMMANDDATA];
	Resplen = GetNfcLoddingData(&data[0], datalen);
	if(Resplen>0)
	{
		byte RespError;
		switch(RespError = NfcResponseLoop(data, Resplen))
		{
		case EventData:
			{
				PostMessage(hWnd, WM_NFCEVENTCALL, EventData, NULL);
			}break;
		case NfcOpen:
			{
				RespFlag=NfcOpen;
				RespEvent.SetEvent();
			}break;
		case NfcClose:
			{
				RespFlag=NfcClose;
				RespEvent.SetEvent();
			}break;
		case PollingLoopStart:
			{
				RespFlag=PollingLoopStart;
				RespEvent.SetEvent();
			}break;
		case PollingLoopStop:
			{
				RespFlag=PollingLoopStop;
				RespEvent.SetEvent();
			}break;
		case NfcApduCmdResp:
			{
				RespFlag=NfcApduCmdResp;
				RespEvent.SetEvent();
			}break;
		case SessionClose:
			{
				RespFlag=SessionClose;
				RespEvent.SetEvent();
			}break;
		case NdefDataWrite:
			{
				RespFlag=NdefDataWrite;
				RespEvent.SetEvent();
			}break;
		case JewelTopazCmdResp:
			{
				RespFlag=JewelTopazCmdResp;
				RespEvent.SetEvent();
			}break;
		case Iso15693CmdResp:
			{
				RespFlag=Iso15693CmdResp;
				RespEvent.SetEvent();
			}break;
		case Iso7816Open:
			{
				RespFlag=Iso7816Open;
				RespEvent.SetEvent();
			}break;
		case Iso7816Close:
			{
				RespFlag=Iso7816Close;
				RespEvent.SetEvent();
			}break;
		case Iso7816ApduCmdResp:
			{
				RespFlag=Iso7816ApduCmdResp;
				RespEvent.SetEvent();
			}break;
		case NfcIp1Send:
			{
				RespFlag=NfcIp1Send;
				RespEvent.SetEvent();
			}break;
		case NppServer:
			{
				RespFlag=NppServer;
				RespEvent.SetEvent();
			}break;
		case NppClient:
			{
				RespFlag=NppClient;
				RespEvent.SetEvent();
			}break;
		case CrcError:
			{
				RespFlag=CrcError;
				PostMessage(hWnd, WM_NFCEVENTCALL, RespError, NULL);
			}break;
		case UnKnownCommand:
			{
				RespFlag=UnKnownCommand;
				PostMessage(hWnd, WM_NFCEVENTCALL, RespError, NULL);
			}break;
		case Timeout:
			{
				RespFlag=Timeout;
				PostMessage(hWnd, WM_NFCEVENTCALL, RespError, NULL);
			}break;
		default:
			{
				RespFlag=None;
				PostMessage(hWnd, WM_NFCEVENTCALL, RespError, NULL);
			}break;
		}
	}
	return FALSE;
}
//Serial Polling Loop
CDeviceCmd::ComKind CDeviceCmd::NfcResponseLoop(byte* data, int readlen)
{
	Resplen=MAX_COMMANDDATA;
	byte RespCommand;
	byte RespError = CAQCmdFormat::RespCommandData(data, readlen, &RespData[0], Resplen, RespCommand);
	if(RespError==CAQCmdFormat::Ok)
	{
		return (CDeviceCmd::ComKind)RespCommand;
	}
	//에러 코드가 발생할 때 데이터를 모두 출력하게 한다
	memcpy(&RespData[0], &data[0], readlen);
	Resplen=readlen;
	return (CDeviceCmd::ComKind)RespError;
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CDeviceCmd::ThreadProcess(LPVOID lpArg)
{
	CDeviceCmd *aqdevice = (CDeviceCmd *)lpArg;
	while (aqdevice->NfcThread.IsRunning() && aqdevice->NfcThread.WaitForThread(THREAD_LOOPTIME)==FALSE)
	{
		aqdevice->NfcLoop();
	}
	return 0;
}
//--------------------------------------------------------------
//|
//| //Event Call
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NfcEventCall(WPARAM wParam)
{
	switch(wParam)
	{
	case EventData:
		{
			switch(RespData[0])
			{
			case NfcEventReaderType:
				{
					ReaderEventMessage(RespData+1, Resplen-1);
				}break;
			case NfcEventP2pType:
				{
					P2pEventMessage(RespData+1, Resplen-1);
				}break;
			case NfcEventCardType:
				{
					CardEventMessage(RespData+1, Resplen-1);
				}break;
			}
		}break;
	case CrcError:
		{
			debug->Write(_T("CrcError"));
			if(ModeReset_init()==FALSE)
			{
				//debug->Write(_T("ReStart Error"));
			}
		}break;
	case UnKnownCommand:
		{
			//debug->Write(_T("UnKnownCommand"));
			if(ModeReset_init()==FALSE)
			{
				//debug->Write(_T("ReStart Error"));
			}
		}break;
	case Timeout:
		{
			//debug->Write(_T("Timeout"));
			if(ModeReset_init()==FALSE)
			{
				//debug->Write(_T("ReStart Error"));
			}
		}break;
	default:
		{
			debug->Write(_T("Error Data Full View"));
			CString dt = CTypeChangHelp::byteToHexbyteValue(RespData, Resplen);
			debug->Write(dt.GetBuffer(0));
			CString errdata;
			errdata.Format(_T("ErrorCode:%02x"), wParam);
			debug->Write(errdata.GetBuffer(0));
			if(ModeReset_init()==FALSE)
			{
				//debug->Write(_T("ReStart Error"));
			}
		}break;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| //Event Call
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::DeviceCmdCallMessage(WPARAM wParam)
{
	switch(wParam)
	{
	case NdefDataChange:
		{
			BOOL bError = TRUE;
			debug->Write(_T("Ndef Change"));
			if(PollingStop())
			{
				if(NDEFWriteChangeLoop())
				{
					bError=FALSE;
				}
			}
			if(bError && ModeReset_init())
			{
				//debug->Write(_T("ReStart Ok"));
			}
		}break;
	case FiledOffMsg:
		{
			bCardFieldOn=FALSE;	
		}break;
	case FiledOnTimeOut:
		{
			ModeReset_init(); 
		}break;
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//| //Event Call
//|
//---------------------------------------------------------------
void CDeviceCmd::ReaderEventMessage(byte* data, int len)
{
	debug->Write(_T("Reader Mode Event"));
	switch(data[0])
	{
	case TargetDiscovered:
		{
			switch(data[1])
			{
			case TypeA:
				{
					byte sendbuffer[MAX_COMMANDDATA];
					int intlen = CTypeChangHelp::ChangeHexCord(CAQCmdFormat::mau, &sendbuffer[0], MAX_COMMANDDATA);
					if(NfcApduCommand(sendbuffer, intlen)==FALSE)
					{
						if(ModeReset_init()==FALSE)
						{
							//debug->Write(_T("ReStart Error"));
						}
						return;
					}
					intlen = commanddata.GetApduCmdData(NFCCommandNum, &sendbuffer[0], MAX_COMMANDDATA);
					if(NfcApduCommand(sendbuffer, intlen))
					{
						PostMessage(hWnd, WM_NFCAQDEVICEMESSAGE, AQDCMSG_COMPLET, NULL);
					}
					else
					{
						if(ModeReset_init()==FALSE)
						{
							//debug->Write(_T("ReStart Error"));
						}
						return;
					}
					if(SESSIONClose()==FALSE)
					{
						if(ModeReset_init()==FALSE)
						{
							//debug->Write(_T("ReStart Error"));
						}
					}
				}break;
			case MifareEtcTag:
				{
					debug->Write(_T("MifareEtcTag"));
				}break;
			case Mifare1k4k:
				{
					debug->Write(_T("Mifare1k4k"));
				}break;
			case TypeB:
				{
					debug->Write(_T("TypeB"));
				}break;
			case Felica:
				{
					debug->Write(_T("Felica"));
				}break;
			case JewelTopaz:
				{
					debug->Write(_T("JewelTopaz"));
				}break;
			case Iso15693:
				{
					debug->Write(_T("Iso15693"));
				}break;
			}
		}break;
	case TargetEventLost:
		{
			debug->Write(_T("TargetEventLost"));
			if(PollingReStart()==FALSE)
			{
				if(ModeReset_init()==FALSE)
				{
					//debug->Write(_T("ReStart Error"));
				}
			}

		}break;
	}
}
//--------------------------------------------------------------
//|
//| //Event Call
//|
//---------------------------------------------------------------
void CDeviceCmd::P2pEventMessage(byte* data, int len)
{
	debug->Write(_T("P2P Mode Event"));
	switch(data[0])
	{
	case NfcActivated:
		{

		}break;
	case NfcDeactivated:
		{

		}break;
	case NfcReciveData:
		{

		}break;
	case NfcNppData:
		{

		}break;
	case Llcp_Activated:
		{
		}break;
	case Llcp_DeActivated:
		{
		}break;
	}
}
//--------------------------------------------------------------
//|
//| //Event Call
//|
//---------------------------------------------------------------
void CDeviceCmd::CardEventMessage(byte* data, int len)
{
	debug->Write(_T("Card Mode Event"));
	switch(data[0])
	{
	case FieldOn:
		{
			debug->Write(_T("FieldOn"));
			bCardFieldOn=TRUE;
			FieldOnTime=GetTickCount()+CARD_FILEDTIMEOUT;
		}break;
	case CardDeactivated:
		{
			debug->Write(_T("CardDeactivated"));
			FieldOnTime=GetTickCount()+CARD_FILEDTIMEOUT;
		}break;
	case CardActivated:
		{
			debug->Write(_T("CardActivated"));
		}break;
	case FieldOff:
		{
			debug->Write(_T("FieldOff"));
			PostMessage(hWnd, WM_DEVICECMDCALLMESSAGE, FiledOffMsg, NULL);
		}break;
	case Transaction:
		{
			debug->Write(_T("NdefComplete"));
			FieldOnTime=GetTickCount()+CARD_FILEDTIMEOUT;
			PostMessage(hWnd, WM_NFCAQDEVICEMESSAGE, AQDCMSG_COMPLET, NULL);
		}break;
	}
}