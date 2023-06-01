
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
//|������
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
//|�Ҹ���
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
	//����Ʈ �ݱ�
	nfcserial.PortClose();
}
//--------------------------------------------------------------
//|
//|NFC ��� ���� �޴���
//|
//---------------------------------------------------------------
void CWaterControl::initWater(LPVOID lpThreadParameter)
{
	lpEventTaget=lpThreadParameter;
	//COM ��Ʈ ������Ʈ�� �˻�
	vector<CString> valueString;
	GetFindRegKeyEnumValue(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"), valueString);
	ActiveComPort="";
	for (int i = 0; i <(int) valueString.size(); i++)
	{
		if(valueString[i].Compare(_T("COM1"))!=0&&valueString[i].Compare(_T("COM2"))!=0)
		{
			//�ø��� ���
			if(nfcserial.PortOpen(valueString[i]))
			{
				//������ �۵�
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
	//���� ī��
	CommandData[1] = (byte) ((CommandDatalen >> 8) & 0xFF);
	CommandData[2] = (byte) (CommandDatalen & 0xFF);

	//memcpy(&CommandData[1], &CommandDatalen, 2);
	//���
	CommandData[3]=cmd;
	//���� �ڵ�
	byte first, second;

	//data ī��
	memcpy(&CommandData[4], &data[0], datalen);
	//stx len + len len
	CommandDatalen+=3;
	//Crc ���� �ڵ�
	ComputeCrc(&CommandData[1], CommandDatalen-1, first, second);
	//Serial Data Total len
	//���� �ڵ� �߰�
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
		//Resp ������ ����
		byte lenth[2];
		lenth[0]=NfcReceiveData[2];
		lenth[1]=NfcReceiveData[1];
		//������ ���� 
		memcpy(&datalen, &lenth[0], 2);
		//stx len + len len 2 +  Crc len �߰�
		datalen+=5;
		if(NfcReceivelen>=datalen)
		{
			memcpy(&data[0], &NfcReceiveData[0], datalen);
			//
			//���� ������ ����
			NfcReceivelen-=datalen;
			if(NfcReceivelen<4) NfcReceivelen=0;
			if(NfcReceivelen>0)
			{
				byte backdata[MAX_COMMANDDATA];
				memcpy(&backdata[0], &NfcReceiveData[datalen], NfcReceivelen);
				//���� ������ ����
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
	// ���� ������ ����
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
	//���� �ڵ�
	byte first, second;
	//Crc ���� �ڵ� stx len Crc len ����
	ComputeCrc(&data[1], readlen-3, first, second);
	//Crc �ڵ� ����
	memcpy(&crc, &data[readlen-2], 2);
	if(crc[0]==first && crc[1]==second)
	{
		memset(RespData, NULL, Resplen);
		//Resp ������ ����
		byte lenth[2];
		lenth[0]=data[2];
		lenth[1]=data[1];
		memcpy(&Resplen, &lenth[0], 2);
		//Response command
		func=(CWaterFormat::AQCmdKind)data[3];
		//���� ������ ���� cmd len , Resp len ������ ���� ����
		Resplen-=2;
		memcpy(&RespData[0], &data[5], Resplen);
	}
	if(data[4]==CWaterFormat::Any_Ok)
	{
		return (CWaterFormat::AQErrorCode)data[4];
	}
	//���� �ڵ尡 �߻��� �� �����͸� ��� ����ϰ� �Ѵ�
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
		// ���߿��� ã�����ϴ� ���� ��ġ�ϴ� ���� ����Ʈ�� �߰��մϴ�.
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