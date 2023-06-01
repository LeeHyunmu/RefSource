
#include "stdAfx.h"
#include <tchar.h>
#define  MAX_REGDATA 4096
#define  MAX_REGDATABIG 65536
#define  RESP_WAITTIMEOUT 700
#define  STATEEVENT_WAITTIMEOUT 1000

//싱글턴 객체
CWaterCmd* CWaterCmd::AQdeviceSingle=NULL;
CWaterCmd* CWaterCmd::getInstance()
{
	if(AQdeviceSingle==NULL)
	{
		AQdeviceSingle=new CWaterCmd();
	}
	return AQdeviceSingle;
}
void CWaterCmd::ReleaseInstance()
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
CWaterCmd::CWaterCmd(void):
RespFuncFlag(CWaterCmdFormat::water_none),
NfcReceivelen(0),
watercheckfunc(NULL),
lpEventTaget(NULL)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CWaterCmd::~CWaterCmd(void)
{
	Release();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWaterCmd::Release(void)
{
	if(NfcThread.IsRunning())
	{
		NfcThread.Release();
	}
	if(CmdEventThread.IsRunning())
	{
		CmdEventThread.Release();
	}
	RespFuncFlag=CWaterCmdFormat::water_none;
	//컴포트 닫기
	nfcserial.PortClose();
}
//--------------------------------------------------------------
//|
//|NFC 기능 생성 메니저
//|
//---------------------------------------------------------------
CWaterCmdFormat::WaterErrorCode CWaterCmd::initAQNFC(LPVOID lpThreadParameter, WATERCHECKFUNC_ROUTINE _watercheckfunc)
{
	CWaterCmdFormat::WaterErrorCode ErrorCode = CWaterCmdFormat::Any_E_None;
	lpEventTaget=lpThreadParameter;
	watercheckfunc= _watercheckfunc;
	//COM 포트 레지스트리 검색
	//에러 출력 데이터 버퍼
	memset(&RespErrorData[0], NULL, MAX_COMMANDDATA);
	RespErrorlen=0;
	ActiveComPort=_T("COM3:");
	ErrorCode=CWaterCmdFormat::Any_E_NotSerialPort;
	//시리얼 통신
	if(nfcserial.PortOpen(ActiveComPort))
	{
		//스래드 작동
		NfcThread.Create(ThreadProcess, (LPVOID)this);
		CmdEventThread.Create(EventThreadProcess, (LPVOID)this);
		RespEvent.Create();
		EventRespEvent.Create();
	}
	return ErrorCode;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CWaterCmd::ModeReset_init()
{
	BOOL b=FALSE;
	//컴포트 닫기
	nfcserial.PortClose();
	//
	NfcReceivelen=0;
	RespFuncFlag=CWaterCmdFormat::water_none;
	memset(&NfcReceiveData[0], NULL, MAX_COMMANDDATA);
	memset(&RespData[0], NULL, MAX_COMMANDDATA);
	memset(&RespErrorData[0], NULL, MAX_COMMANDDATA);
	RespErrorlen=0;
	//시리얼 통신
	if(ActiveComPort.GetLength()>3 && nfcserial.PortOpen(ActiveComPort))
	{
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CWaterCmd::ComputeCrc(byte *Data, int Length, byte& chBlock)
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
UINT CWaterCmd::makeCommandData(CWaterCmdFormat::WateAddress cmd, byte* data, int datalen, byte* CommandData, int& CommandDatalen)
{
	memset(CommandData, NULL, CommandDatalen);
	//cmd len + data len
	CommandDatalen = 0;
	//STX
	CommandData[CommandDatalen++]=CWaterCmdFormat::stx_format;
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
int CWaterCmd::GetNfcLoddingData(byte* data, int& datalen)
{
	if(NfcReceivelen>0)
	{
		//데이터 길이 고정
		//stx len + len len 2 +  Crc len 추가
		BYTE checkBool=0xFF;
		if(NfcReceiveData[0]==0x02&& NfcReceiveData[5]==0x0D)
		{
			datalen=5;
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
BOOL CWaterCmd::NfcLoop()
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
		if(CWaterCmdFormat::Any_Ok == RespErrorCode)
		{
			switch(RespFuncFlag)
			{
			case CWaterCmdFormat::water_ready:
			case CWaterCmdFormat::water_readycomplete:
			case CWaterCmdFormat::water_run:
			case CWaterCmdFormat::water_error:
				{
					EventRespEvent.SetEvent();
				}break;
			default:
				{

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
CWaterCmdFormat::WaterErrorCode CWaterCmd::NfcResponseLoop(CWaterCmdFormat::WaterCheck& func, byte* data, int readlen)
{
	Resplen=MAX_COMMANDDATA;
	//check bit
	byte checkbit;
	ComputeCrc(&data[0], readlen-1, checkbit);

	//
	if((data[readlen-1]^checkbit)==CWaterCmdFormat::Any_Ok)
	{
		//Response command
		func=(CWaterCmdFormat::WaterCheck)data[2];
		//
		return CWaterCmdFormat::Any_Ok;
	}
	return CWaterCmdFormat::Any_E_None;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CWaterCmd::NfcEventLoop()
{
	//
	byte CommandData[MAX_COMMANDDATA];
	int  CommandDatalen=MAX_COMMANDDATA;
	byte SendData[MAX_COMMANDDATA];
	int  SendDatalen = 1;
	SendData[0] = CWaterCmdFormat::watercmd_watercheck;
	makeCommandData(CWaterCmdFormat::address_pc, &SendData[0], SendDatalen, &CommandData[0], CommandDatalen);
	int sendsize = nfcserial.PortWrite(CommandData, CommandDatalen);
	//1초 대기
	EventRespEvent.WaitForEvent(RESP_WAITTIMEOUT);
	switch(RespFuncFlag)
	{
	case CWaterCmdFormat::water_ready:
	case CWaterCmdFormat::water_readycomplete:
	case CWaterCmdFormat::water_run:
	case CWaterCmdFormat::water_error:
		{
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
DWORD WINAPI CWaterCmd::ThreadProcess(LPVOID lpArg)
{
	CWaterCmd *aqdevice = (CWaterCmd *)lpArg;
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
DWORD WINAPI CWaterCmd::EventThreadProcess(LPVOID lpArg)
{
	CWaterCmd *aqdevice = (CWaterCmd *)lpArg;
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
size_t CWaterCmd::GetFindRegKeyEnumValue(HKEY hRKey, TCHAR *pRegName, vector<CString>& valueString)
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
void CWaterCmd::integerEndianChangebyte(int _value, byte* _data)
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
void CWaterCmd::byteEndianChangeinteger(byte* _data, int& _value)
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
void CWaterCmd::shortEndianChangebyte(int _value, byte* _data)
{
	_data[0]=(byte)((_value&0x0000FF00)>>8);
	_data[1]=(byte)(_value&0x000000FF);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CWaterCmd::byteEndianChangeshort(byte* _data, int& _value)
{
	_value=0;
	_value |= (((int)_data[0])<<8)&0x0000FF00;
	_value |= ((int)_data[1])&0x000000FF;
}