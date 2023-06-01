
#include "stdAfx.h"
#include <tchar.h>
#define CARD_FILEDTIMEOUT 5000
#define P2P_ACTIVETIMEOUT 5000
#define  MAX_REGDATA 4096
#define  MAX_REGDATABIG 65536
#define  RESP_WAITTIMEOUT 700
#define  WATERCHECK_WAITTIMEOUT 2000

//싱글턴 객체
CDeviceCmd* CDeviceCmd::AQdeviceSingle=NULL;
CDeviceCmd* CDeviceCmd::getInstance()
{
	if(AQdeviceSingle==NULL)
	{
		AQdeviceSingle=new CDeviceCmd();
	}
	return AQdeviceSingle;
}
void CDeviceCmd::ReleaseInstance()
{
	if(AQdeviceSingle!=NULL)
	{
		delete AQdeviceSingle; AQdeviceSingle=NULL;
	}
}
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDeviceCmd::CDeviceCmd(void):
RespFuncFlag(CAQCmdFormat::water_none),
NfcReceivelen(0),
watercheckfunc(NULL),
waterdebug(NULL),
lpEventTaget(NULL)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CDeviceCmd::~CDeviceCmd(void)
{
	Release();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::Release(void)
{
	if(NfcThread.IsRunning())
	{
		NfcThread.Release();
	}
	if(CmdEventThread.IsRunning())
	{
		CmdEventThread.Release();
	}
	RespFuncFlag=CAQCmdFormat::water_none;
	//컴포트 닫기
	nfcserial.Close();
}
//--------------------------------------------------------------
//|
//|NFC 기능 생성 메니저
//|
//---------------------------------------------------------------
CAQCmdFormat::WaterErrorCode CDeviceCmd::initAQNFC(LPVOID lpThreadParameter, WATERCHECKFUNC_ROUTINE _watercheckfunc, WATERDEBUGFUNC_ROUTINE _debugfunc)
{
	CAQCmdFormat::WaterErrorCode ErrorCode = CAQCmdFormat::Any_E_None;
	lpEventTaget=lpThreadParameter;
	watercheckfunc= _watercheckfunc;
	waterdebug = _debugfunc;
	//COM 포트 레지스트리 검색
	//에러 출력 데이터 버퍼
	memset(&RespErrorData[0], NULL, MAX_COMMANDDATA);
	RespErrorlen=0;
	vector<CString> valueString;
	GetFindRegKeyEnumValue(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), valueString);
	ActiveComPort="";
	ErrorCode=CAQCmdFormat::Any_E_NotSerialPort;
	for (int i = (int) valueString.size()-1; i >=0; i--)
	{
		if(valueString[i].Compare(_T("COM1"))!=0&&valueString[i].Compare(_T("COM2"))!=0)
		{
			//시리얼 통신
			CString comport = valueString[i];
			if(nfcserial.Open(comport))
			{
				//스래드 작동
				NfcThread.Create(ThreadProcess, (LPVOID)this);
				CmdEventThread.Create(EventThreadProcess, (LPVOID)this);
				RespEvent.Create();
				EventRespEvent.Create();
			}
		}
	}
	return ErrorCode;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::ModeReset_init()
{
	BOOL b=FALSE;
	//컴포트 닫기
	nfcserial.Close();
	//
	NfcReceivelen=0;
	RespFuncFlag=CAQCmdFormat::water_none;
	memset(&NfcReceiveData[0], NULL, MAX_COMMANDDATA);
	memset(&RespData[0], NULL, MAX_COMMANDDATA);
	memset(&RespErrorData[0], NULL, MAX_COMMANDDATA);
	RespErrorlen=0;
	//시리얼 통신
	if(ActiveComPort.GetLength()>3 && nfcserial.Open(ActiveComPort))
	{
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDeviceCmd::ComputeCrc(byte *Data, int Length, byte& chBlock)
{
	chBlock=0x00;
	for (int i = 0; i < Length; i++)
	{
		chBlock^=Data[i];
	}
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Command Data
UINT CDeviceCmd::makeCommandData(CAQCmdFormat::WateAddress cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen)
{
	memset(CommandData, NULL, CommandDatalen);
	//cmd len + data len
	CommandDatalen = 0;
	//STX
	CommandData[CommandDatalen++]=CAQCmdFormat::stx_format;
	//Address PC(0x53), PCB(0x5A)
	CommandData[CommandDatalen++]=cmd;
	//data
	memcpy(&CommandData[CommandDatalen], &data[0], datalen);
	CommandDatalen+=datalen;
	//ETX
	CommandData[CommandDatalen++]=0x03;
	//BBC
	//check bit
	byte checkbit;
	ComputeCrc(&CommandData[0], CommandDatalen, checkbit);
	CommandData[CommandDatalen++]=checkbit;
	//CR
	CommandData[CommandDatalen++]=0x0D;
	return (CommandDatalen);
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
		//데이터 길이 고정
		//stx len + len len 2 +  Crc len 추가
		BYTE checkBool=0xFF;
		if(NfcReceiveData[0]==0x02&& NfcReceiveData[5]==0x0D)
		{
			datalen=6;
			if(checkBool)
			{
				memcpy(&data[0], &NfcReceiveData[0], datalen);
				//
				//남은 데이터 조정
				NfcReceivelen-=datalen;
				if(NfcReceivelen<5) NfcReceivelen=0;
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
		datalen=0;
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
	if(NfcReceivelen==0)
	{
		if(nfcserial.IsOpened())
		{
			memset(&NfcReceiveData[0], NULL, MAX_COMMANDDATA);
			NfcReceivelen=MAX_COMMANDDATA;
			NfcReceivelen = nfcserial.ReadData(NfcReceiveData, NfcReceivelen);
		}
	}
	// 받은 데이터 길이
	int datalen=0;
	byte data[MAX_COMMANDDATA];
	if(NfcReceivelen!=0)
	{
		memset(&data[0], NULL, MAX_COMMANDDATA);
		datalen = GetNfcLoddingData(&data[0], datalen);
	}
	if(datalen>0)
	{
		RespErrorCode = NfcResponseLoop(RespFuncFlag, data, datalen);
		if(CAQCmdFormat::Any_Ok == RespErrorCode)
		{
			switch(RespFuncFlag)
			{
			case CAQCmdFormat::water_ready:
			case CAQCmdFormat::water_readycomplete:
			case CAQCmdFormat::water_run:
			case CAQCmdFormat::water_error:
				{
					EventRespEvent.SetEvent();
				}break;
			default:
				{
					RespEvent.SetEvent();
				}break;
			}
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Serial Polling Loop
CAQCmdFormat::WaterErrorCode CDeviceCmd::NfcResponseLoop(CAQCmdFormat::WaterCheck& func, byte* data, int readlen)
{
	Resplen=MAX_COMMANDDATA;
	//check bit
	byte checkbit;
	ComputeCrc(&data[0], readlen-2, checkbit);

	//
	if((data[readlen-2]^checkbit)==CAQCmdFormat::Any_Ok)
	{
		//Response command
		func=(CAQCmdFormat::WaterCheck)data[2];
		Resplen = readlen;
		memcpy(&RespData[0], &data[0], Resplen);
		//
		return CAQCmdFormat::Any_Ok;
	}
	return CAQCmdFormat::Any_E_None;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CDeviceCmd::NfcEventLoop()
{
	//
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;
	byte SendData[MAX_COMMANDDATA];
	int  SendDatalen = 1;
	SendData[0] = CAQCmdFormat::watercmd_watercheck;
	makeCommandData(CAQCmdFormat::address_pc, &SendData[0], SendDatalen, &CommandData[0], CommandDatalen);
	//디버그 메시지
	if(waterdebug)
		waterdebug(lpEventTaget, CommandData, CommandDatalen, true);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);
	//1초 대기
	EventRespEvent.WaitForEvent(RESP_WAITTIMEOUT);
	switch(RespFuncFlag)
	{
	case CAQCmdFormat::water_ready:
	case CAQCmdFormat::water_readycomplete:
	case CAQCmdFormat::water_run:
	case CAQCmdFormat::water_error:
		{
			//받은 디버그 데이터
			if(waterdebug)
				waterdebug(lpEventTaget, &RespData[0], Resplen, false);
			if(watercheckfunc)
			{
				watercheckfunc(lpEventTaget, RespFuncFlag);
			}
		}break;
	}
	return 0;
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
	aqdevice->NfcThread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CDeviceCmd::EventThreadProcess(LPVOID lpArg)
{
	CDeviceCmd *aqdevice = (CDeviceCmd *)lpArg;
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
size_t CDeviceCmd::GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString)
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
//--------------------------------------------------------------
//|
//| integer 리틀엔디안 빅엔디안 변경 byte type으로
//|
//---------------------------------------------------------------
void CDeviceCmd::integerEndianChangebyte(int _value, byte* _data)
{
	_data[0]=(byte)((_value&0xFF000000)>>24);
	_data[1]=(byte)((_value&0x00FF0000)>>16);
	_data[2]=(byte)((_value&0x0000FF00)>>8);
	_data[3]=(byte)(_value&0x000000FF);
}
//--------------------------------------------------------------
//|
//| byte type 리틀엔디안 빅엔디안 변경 integer type으로
//|
//---------------------------------------------------------------
void CDeviceCmd::byteEndianChangeinteger(byte* _data, int& _value)
{
	_value=0;
	_value |= (((int)_data[0])<<24)&0xFF000000;
	_value |= (((int)_data[1])<<16)&0x00FF0000;
	_value |= (((int)_data[2])<<8)&0x0000FF00;
	_value |= ((int)_data[3])&0x000000FF;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CDeviceCmd::shortEndianChangebyte(int _value, byte* _data)
{
	_data[0]=(byte)((_value&0x0000FF00)>>8);
	_data[1]=(byte)(_value&0x000000FF);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CDeviceCmd::byteEndianChangeshort(byte* _data, int& _value)
{
	_value=0;
	_value |= (((int)_data[0])<<8)&0x0000FF00;
	_value |= ((int)_data[1])&0x000000FF;
}
//--------------------------------------------------------------
//|
//| 명렁어 보내기
//|
//---------------------------------------------------------------
void CDeviceCmd::CommandSend(byte data)
{
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;

	makeCommandData(CAQCmdFormat::address_pc, &data, 1, &CommandData[0], CommandDatalen);
	//디버그 메시지
	if(waterdebug)
		waterdebug(lpEventTaget, CommandData, CommandDatalen, true);
	int sendsize = nfcserial.SendData(CommandData, CommandDatalen);

	//1초 대기
	if(RespEvent.WaitForEvent(RESP_WAITTIMEOUT)==FALSE)
	{
		//
		if(waterdebug)
			waterdebug(lpEventTaget, &RespData[0], Resplen, false);
	}
}