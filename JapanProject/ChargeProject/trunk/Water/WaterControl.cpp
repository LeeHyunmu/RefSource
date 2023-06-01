
#include "stdAfx.h"
#include <tchar.h>
#define CARD_FILEDTIMEOUT 5000
#define P2P_ACTIVETIMEOUT 5000
#define  MAX_REGDATA 4096
#define  MAX_REGDATABIG 65536
#define THREAD_LOOPTIME 200
#define EVENTTHREAD_LOOPTIME 200

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CWaterControl::CWaterControl(void):
NfcReceivelen(0),
readfunc(NULL),
p2pfunc(NULL),
cardfunc(NULL),
lpEventTaget(NULL),
bWaterOpen(FALSE),
RespFuncFlag(CWaterFormat::Kind_None)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CWaterControl::~CWaterControl(void)
{
	Release();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWaterControl::Release(void)
{
	if(NfcThread.IsRunning())
	{
		NfcThread.Release();
	}
	RespEvent.WaitForEvent(200);
	if(CmdEventThread.IsRunning())
	{
		CmdEventThread.Release();
	}
	EventRespEvent.WaitForEvent(200);
	//컴포트 닫기
	nfcserial.PortClose();
}
//--------------------------------------------------------------
//|
//|NFC 기능 생성 메니저
//|
//---------------------------------------------------------------
void CWaterControl::initWater(LPVOID lpThreadParameter)
{
	lpEventTaget=lpThreadParameter;
	//COM 포트 레지스트리 검색
	vector<CString> valueString;
	GetFindRegKeyEnumValue(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), valueString);
	ActiveComPort="";
	for (int i = 0; i <(int) valueString.size(); i++)
	{
		if(valueString[i].Compare(_T("COM1"))!=0&&valueString[i].Compare(_T("COM2"))!=0)
		{
			//시리얼 통신
			if(nfcserial.PortOpen(valueString[i]))
			{
				//스래드 작동
				NfcThread.Create(ThreadProcess, (LPVOID)this);
				CmdEventThread.Create(EventThreadProcess, (LPVOID)this);
				RespEvent.Create();
				EventRespEvent.Create();
			}
		}
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
unsigned short CWaterControl::UpdateCrc(unsigned char ch, unsigned short* lpwCrc)
{
	ch = (ch^(unsigned char)((*lpwCrc) & 0x00FF));
	ch = (ch^(ch<<4));
	*lpwCrc = (*lpwCrc >> 8)^((unsigned short)ch << 8)^((unsigned short)ch<<3)^((unsigned short)ch>>4);
	return(*lpwCrc);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWaterControl::ComputeCrc(byte *Data, int Length, byte& TransmitFirst, byte& TransmitSecond)
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
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Command Data
UINT CWaterControl::makeCommandData(byte stx, byte cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen)
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
	return (CommandDatalen);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
int CWaterControl::GetNfcLoddingData(byte* data, int& datalen)
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
BOOL CWaterControl::NfcLoop()
{
	if(NfcReceivelen==0)
	{
		if(nfcserial.IsOpened())
		{
			memset(&NfcReceiveData[0], NULL, MAX_COMMANDDATA);
			NfcReceivelen=MAX_COMMANDDATA;
			NfcReceivelen = nfcserial.PortRead(NfcReceiveData, NfcReceivelen);
		}
	}
	// 받은 데이터 길이
	int datalen=MAX_COMMANDDATA;
	byte data[MAX_COMMANDDATA];
	datalen = GetNfcLoddingData(&data[0], datalen);
	if(datalen>0)
	{
// 		RespErrorCode = NfcResponseLoop(RespFuncFlag, data, datalen);
// 		switch(RespFuncFlag)
// 		{
// 		case CAQCmdFormat::NfcOpen:
// 		case CAQCmdFormat::NfcClose:
// 		case CAQCmdFormat::PollingLoopStart:
// 		case CAQCmdFormat::PollingLoopStop:
// 		case CAQCmdFormat::NfcApduCmdResp:
// 		case CAQCmdFormat::SessionClose:
// 		case CAQCmdFormat::NdefDataWrite:
// 		case CAQCmdFormat::NdefDataRead:
// 		case CAQCmdFormat::JewelTopazCmdResp:
// 		case CAQCmdFormat::Iso15693CmdResp:
// 		case CAQCmdFormat::Iso7816Open:
// 		case CAQCmdFormat::Iso7816Close:
// 		case CAQCmdFormat::Iso7816ApduCmdResp:
// 		case CAQCmdFormat::NfcIp1Send:
// 		case CAQCmdFormat::NppServer:
// 		case CAQCmdFormat::NppClient:
// 		case CAQCmdFormat::ReadReaderVersion:
// 		case CAQCmdFormat::MifareAuth:
// 		case CAQCmdFormat::MifareRead:
// 		case CAQCmdFormat::MifareWrite:
// 			{
// 				RespEvent.SetEvent();
// 			}break;
// 		case CAQCmdFormat::EventData:
// 			{
// 				EventRespEvent.SetEvent();
// 			}break;
// 		default:
// 			{
// 
// 			}break;
// 		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Serial Polling Loop
CWaterFormat::AQErrorCode CWaterControl::NfcResponseLoop(CWaterFormat::AQCmdKind& func, byte* data, int readlen)
{
	Resplen=MAX_COMMANDDATA;
	byte crc[2];
	//검증 코드
	byte first, second;
	//Crc 검증 코드 stx len Crc len 빼기
	ComputeCrc(&data[1], readlen-3, first, second);
	//Crc 코드 추출
	memcpy(&crc, &data[readlen-2], 2);
	if(crc[0]==first && crc[1]==second)
	{
		memset(RespData, NULL, Resplen);
		//Resp 데이터 길이
		byte lenth[2];
		lenth[0]=data[2];
		lenth[1]=data[1];
		memcpy(&Resplen, &lenth[0], 2);
		//Response command
		func=(CWaterFormat::AQCmdKind)data[3];
		//순수 데이터 길이 cmd len , Resp len 데이터 길이 빼기
		Resplen-=2;
		memcpy(&RespData[0], &data[5], Resplen);
	}
	if(data[4]==CWaterFormat::Any_Ok)
	{
		return (CWaterFormat::AQErrorCode)data[4];
	}
	//에러 코드가 발생할 때 데이터를 모두 출력하게 한다
	memcpy(&RespData[0], &data[0], readlen);
	Resplen=readlen;
	return (CWaterFormat::AQErrorCode)data[4];
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CWaterControl::NfcEventLoop()
{
	if(bWaterOpen==FALSE)
	{
		return FALSE;
	}
	if(RespFuncFlag==CWaterFormat::EventData)
	{
		return TRUE;
	}
	return FALSE;
}

//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CWaterControl::ThreadProcess(LPVOID lpArg)
{
	CWaterControl *aqdevice = (CWaterControl *)lpArg;
	while (aqdevice->NfcThread.IsRunning() && aqdevice->NfcThread.WaitForThread(THREAD_LOOPTIME)==FALSE)
	{
		aqdevice->NfcLoop();
	}
	aqdevice->NfcThread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CWaterControl::EventThreadProcess(LPVOID lpArg)
{
	CWaterControl *aqdevice = (CWaterControl *)lpArg;
	while (aqdevice->CmdEventThread.IsRunning() && aqdevice->CmdEventThread.WaitForThread(EVENTTHREAD_LOOPTIME)==FALSE)
	{
		aqdevice->NfcEventLoop();
	}
	aqdevice->CmdEventThread.SetCloseEvent();
	return 0;
}

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
size_t CWaterControl::GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString)
{
	TCHAR    lpValue[MAX_REGDATA];
	TCHAR*    lpOutput=NULL;
	TCHAR    lpTmpData[10];
	DWORD    i, k;
	LONG     Result;
	DWORD    Size;
	HKEY     key=NULL;

	DWORD    Type;
	BYTE*     Data=NULL;
	TCHAR pBackGegName[MAX_REGDATA];
	ZeroMemory(pBackGegName,MAX_REGDATA);
	_tcscpy_s(pBackGegName, MAX_REGDATA, pRegName);
	if (RegOpenKeyEx(hRKey,pBackGegName,0,KEY_READ,&key) == ERROR_SUCCESS) 
	{
		// 값중에서 찾고자하는 값과 일치하는 것을 리스트에 추가합니다.
		Result=ERROR_SUCCESS;
		for (i=0;Result==ERROR_SUCCESS;i++) 
		{
			Size=MAX_REGDATA;
			Result=RegEnumValue(key,i,lpValue,&Size,NULL,NULL,NULL,NULL);
			if (Result==ERROR_SUCCESS)
			{
				Size=MAX_REGDATABIG;
				Data = new BYTE[Size+1];
				ZeroMemory(Data, (Size+1)*sizeof(BYTE));
				RegQueryValueEx(key,lpValue,NULL,&Type,Data,&Size);
				lpOutput = new TCHAR[(Size*3)+1];
				ZeroMemory(lpOutput, ((Size*3)+1)*sizeof(TCHAR));
				switch (Type) 
				{
				case REG_DWORD:
					wsprintf(lpOutput,TEXT("%d"),*(LPDWORD)Data);
					break;
				case REG_EXPAND_SZ:
				case REG_SZ:
					wsprintf(lpOutput,TEXT("%s"),Data);
					break;
				case REG_BINARY:
					lpOutput[0]=0;
					for(k=0; k < Size; k++)
					{
						if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
						wsprintf(lpTmpData,TEXT("%002X"),Data[k]);
						_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
					}
					break;
				default:
					wsprintf(lpOutput,TEXT("[%d]"),Type);
					lpOutput[0]=0;
					for(k=0; k < Size; k++)
					{
						if (k > 0) _tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), TEXT(" "));
						wsprintf(lpTmpData,TEXT("%02X"),Data[k]);
						_tcscat_s(lpOutput, ((Size*3)+1)*sizeof(TCHAR), lpTmpData);
					}
					break;
				}
				valueString.push_back(lpOutput);
				if(Data)
				{
					delete Data; Data = NULL;
				}
				if(lpOutput)
				{
					delete lpOutput; lpOutput=NULL;
				}
			}
		} 
		RegCloseKey(key);
	}
	return valueString.size();
}