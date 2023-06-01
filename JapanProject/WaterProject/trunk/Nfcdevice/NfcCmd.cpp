
#include "stdAfx.h"

#define THREAD_LOOPTIME 200
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CAQdeviceCmd::CAQdeviceCmd(void):
m_module(NULL),
bLoadModule(FALSE),
AQInstance(NULL),
mp_GetErrMsg(NULL),
mp_DE_InitPort(NULL),
mp_DE_ClosePort(NULL),
mp_DE_BuzzerOn(NULL),
mp_DE_BuzzerOff(NULL),
mp_DE_FindCard(NULL),
mp_DEA_Authkey(NULL),
mp_DEA_Read(NULL),
mp_DEA_Write(NULL),
nRecvlen(0),
FindEventFlag(DE_NO_EVENTMSG),
CardConnection(DE_NO_EVENTMSG),
bNfcConnectOk(FALSE),
atypefunc(NULL),
mtypefunc(NULL),
lpThreadParameter(NULL),
findop(NFCCONNECTCARD_OTHER)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CAQdeviceCmd::~CAQdeviceCmd(void)
{
}
//--------------------------------------------------------------
//|
//|초기화
//|
//---------------------------------------------------------------
BOOL CAQdeviceCmd::initAQdeviceCmd(HWND _hWnd, LPCTSTR dlllib)
{
	hWndp=_hWnd;
	bLoadModule=FALSE;
	if(dlllib && *dlllib)
	{
		if(m_module!=NULL)
		{
			::FreeLibrary(m_module); m_module=NULL;
		}
		m_module = ::LoadLibrary(dlllib);
		if(m_module) 
		{
			//스래드 작동
			FindCardThread.Create(FindCardThreadProcess, (LPVOID)this);
			EventProcThread.Create(EventProcThreadProcess, (LPVOID)this);
			bLoadModule = TRUE;
		}
	}
	return bLoadModule;
}
//--------------------------------------------------------------
//|
//|A 타입의 카드 감지 세팅
//|
//---------------------------------------------------------------
BOOL CAQdeviceCmd::NFCAtypeConnect(LPVOID _lpThreadParameter, ATYPEEVENTFUNC_ROUTINE _atypefunc, MTYPEEVENTFUNC_ROUTINE _mtypefunc, DISCONNECTEVENTFUNC_ROUTINE _disconnect)
{
	//Atype 카드 감지 세팅
	findop = NFCCONNECTCARD_ATYPE;
	//콜백 함수 전달 인수
	lpThreadParameter = _lpThreadParameter;
	//atype 콜백 함수
	atypefunc = _atypefunc;
	//mifare 콜백 함수
	mtypefunc = _mtypefunc;
	//연결이 끈긴 상태 판단
	disconnect = _disconnect;
	//연결 상태
	bNfcConnectOk=FALSE;
	//dll 로딩 되었을 때
	if(bLoadModule)
	{
		//포트 연결
		int port = DE_InitPort();
		//연결 포트와 리턴값이 같을 때
		if(port==USBPORT)
		{
			bNfcConnectOk=TRUE;
		}
	}
	return bNfcConnectOk;
}
//--------------------------------------------------------------
//|
//|NFC 연결 종료
//|
//---------------------------------------------------------------
void CAQdeviceCmd::NFCDisConnect()
{
	if( bLoadModule && m_module!=NULL)
	{
		if(FindCardThread.IsRunning())
		{
			FindCardThread.Release();
		}
		if(EventProcThread.IsRunning())
		{
			EventProcThread.Release();
		}
		bLoadModule=FALSE;
	}
	if(bNfcConnectOk)
	{
		DE_ClosePort(); ::FreeLibrary(m_module); m_module=NULL; bNfcConnectOk=FALSE;
	}
}
//--------------------------------------------------------------
//|
//|AQ NFc 객체 생성
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::GetErrMsg(int errcode,char* retmsg)
{
	if(mp_GetErrMsg==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_GetErrMsg = (type_GetErrMsg) GetProcAddressFunc(_T("GetErrMsg"));
		}
	}
	if(mp_GetErrMsg)
		return mp_GetErrMsg(errcode,retmsg);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DE_InitPort(int nBaud, int nPort)
{
	if(mp_DE_InitPort==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DE_InitPort = (type_DE_InitPort) GetProcAddressFunc(_T("DE_InitPort"));
		}
	}
	if(mp_DE_InitPort)
		return mp_DE_InitPort(nPort, nBaud);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DE_ClosePort(int nPort)
{
	if(mp_DE_ClosePort==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DE_ClosePort = (type_DE_ClosePort) GetProcAddressFunc(_T("DE_ClosePort"));
		}
	}
	if(mp_DE_ClosePort)
		return mp_DE_ClosePort(nPort);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DE_BuzzerOn(int nPort)
{
	if(mp_DE_BuzzerOn==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DE_BuzzerOn = (type_DE_BuzzerOn) GetProcAddressFunc(_T("DE_BuzzerOn"));
		}
	}
	if(mp_DE_BuzzerOn)
		return mp_DE_BuzzerOn(nPort);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DE_BuzzerOff(int nPort)
{
	if(mp_DE_BuzzerOff==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DE_BuzzerOff = (type_DE_BuzzerOff) GetProcAddressFunc(_T("DE_BuzzerOff"));
		}
	}
	if(mp_DE_BuzzerOff)
		return mp_DE_BuzzerOff(nPort);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DE_FindCard(LPINT outlen, LPBYTE lpRes, BYTE option, BYTE baud, BYTE cid, BYTE nad, int nPort)
{
	if(mp_DE_FindCard==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DE_FindCard = (type_DE_FindCard) GetProcAddressFunc(_T("DE_FindCard"));
		}
	}
	if(mp_DE_FindCard)
		return mp_DE_FindCard(nPort, baud, cid, nad, option, outlen, lpRes);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DEA_Authkey(LPBYTE keydata, BYTE blockno, BYTE mode, int nPort)
{
	if(mp_DEA_Authkey==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DEA_Authkey = (type_DEA_Authkey) GetProcAddressFunc(_T("DEA_Authkey"));
		}
	}
	if(mp_DEA_Authkey)
		return mp_DEA_Authkey(nPort, mode, keydata, blockno);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DEA_Read(BYTE blockno, LPINT outlen, LPBYTE lpRes, int nPort)
{
	if(mp_DEA_Read==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DEA_Read = (type_DEA_Read) GetProcAddressFunc(_T("DEA_Read"));
		}
	}
	if(mp_DEA_Read)
		return mp_DEA_Read(nPort, blockno, outlen, lpRes);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
NFCErrorCode CAQdeviceCmd::DEA_Write(BYTE blockno, int datalen, LPBYTE data, int nPort)
{
	if(mp_DEA_Write==NULL && bLoadModule)
	{
		if( m_module )
		{	
			mp_DEA_Write = (type_DEA_Write) GetProcAddressFunc(_T("DEA_Write"));
		}
	}
	if(mp_DEA_Write)
		return mp_DEA_Write(nPort, blockno, datalen, data);
	return  APP_LIBLOAD_ERROR;
}
//--------------------------------------------------------------
//|
//|dll 함수 객체
//|
//---------------------------------------------------------------
FARPROC CAQdeviceCmd::GetProcAddressFunc(LPCTSTR lpProcName)
{
	if(m_module==NULL) return NULL;
	TCHAR ProcName[MAX_PATH];
	ZeroMemory(ProcName, MAX_PATH*sizeof(TCHAR));
	memcpy(ProcName, lpProcName, lstrlen(lpProcName)*sizeof(TCHAR));
	FARPROC Farproc = NULL;
	Farproc = ::GetProcAddress(m_module,ProcName);
	if(Farproc==NULL)
	{
		//DebugDlg->Write(_T("FARPROC NULL"));
	}
	return Farproc;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CAQdeviceCmd::FindCardThreadProcess(LPVOID lpArg)
{
	CAQdeviceCmd *cmd = (CAQdeviceCmd *)lpArg;
	while (cmd->FindCardThread.IsRunning() && cmd->FindCardThread.WaitForThread(THREAD_LOOPTIME)==FALSE)
	{
		cmd->FindCardFunc();
	}
	cmd->FindCardThread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CAQdeviceCmd::FindCardFunc()
{
	if(bNfcConnectOk==FALSE) return;
	if(nRecvlen==0)
		FindEventFlag = DE_FindCard(&nRecvlen, &byteRecv[0], findop);
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CAQdeviceCmd::EventProcThreadProcess(LPVOID lpArg)
{
	CAQdeviceCmd *cmd = (CAQdeviceCmd *)lpArg;
	while (cmd->EventProcThread.IsRunning() && cmd->EventProcThread.WaitForThread(THREAD_LOOPTIME)==FALSE)
	{
		cmd->EventProcFunc();
	}
	cmd->EventProcThread.SetCloseEvent();
	return 0;
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
void CAQdeviceCmd::EventProcFunc()
{
	if(bNfcConnectOk==FALSE) return;
	switch(FindEventFlag)
	{
	case DE_OK:
		{
			if(CardConnection!=FindEventFlag)
			{
				CardConnection=FindEventFlag;
				int datalen=0;
				BYTE recvdata[BUFFSIZE_1024];
				datalen=nRecvlen-2;  
				memcpy(&recvdata[0], &byteRecv[2], datalen);
				CardFindType cardtype =(CardFindType) byteRecv[1];
				switch (cardtype)
				{
				case NFCCONNECTCARD_ATYPE:
					{
						if(atypefunc)
						{
							atypefunc(lpThreadParameter, recvdata, datalen);
							nRecvlen=0;
						}
					}break;
				case NFCCONNECTCARD_BTYPE:
					{

					}break;
				case NFCCONNECTCARD_MTYPE:
					{
						if(mtypefunc)
						{
							mtypefunc(lpThreadParameter, recvdata, datalen);
							nRecvlen=0;
						}
					}break;
				}
			}
			else
			{
				nRecvlen=0;
			}

		}break;
	case DE_NO_TAG_ERROR:
		{
			if(CardConnection!=FindEventFlag)
			{
				CardConnection=FindEventFlag;
				int datalen=0;
				BYTE recvdata[BUFFSIZE_1024];
				datalen=nRecvlen;  
				memcpy(&recvdata[0], &byteRecv[0], datalen);
				disconnect(lpThreadParameter, recvdata, nRecvlen);
			}
			else
			{
				nRecvlen=0;
			}
		}break;
	}
}