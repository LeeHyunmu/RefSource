
#include "stdAfx.h"
#include "DualiCardMg.h"
#include "DualCardDllHeader.h"
#pragma comment(lib, "DualDlleMbedded.lib")

#define SENDFLAG	0
#define RECVFLAG	1
#define ERRORFLAG	2

#define MAX_BUF		255
#define BAUD		115200
//스래드 타임아웃
#define DETECTEVENTTIME 500

// Mode 플래그
#define DEVICEMODE_INITIATOR	0x00000001 //initiator Mode
#define DEVICEMODE_TARGET		0x00000002 //Target Mode
#define DEVICEMODE_NONE			0x00000004 //None

// Mode 플래그
#define THREADLOOP_IDLING		0x00000001 //idling
#define THREADLOOP_DETECTING	0x00000002 //detecting
#define THREADLOOP_NONE			0x00000004 //None
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CDualiCardMg::CDualiCardMg():
debug(NULL),
m_bConnect(FALSE),
m_nPort(0),
nSlotno(-1),
bSAMOn(FALSE),
cardID(_T("")),
uLoopMode(DEVICEMODE_TARGET),
uThreadLoop(THREADLOOP_DETECTING)
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CDualiCardMg::~CDualiCardMg()
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDualiCardMg::DetectObject()
{
	////////////////////////////////////////////////////////////////
	// 스레드 생성
	DetectEvent.Create();
	Detectthread.Create(DetectProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|카드 감지 및 처리 함수
//|
//---------------------------------------------------------------
int bTargetLoop=0;
void CDualiCardMg::CardDetectFunc()
{
	Sleep(10);
	//========================================
	// MainDlg Gdi 클래스
	//========================================
	IMainGdiDlg* maingdi =obj->GetMainGdiDlgObject();
	while(!DetectEvent.WaitForEvent(DETECTEVENTTIME))
	{
		if(uThreadLoop&THREADLOOP_IDLING)
		{
			if(uLoopMode&DEVICEMODE_TARGET)
			{
				maingdi->ImageChange(0, FALSE); uThreadLoop=THREADLOOP_DETECTING;
			}
			else if(uLoopMode&DEVICEMODE_INITIATOR)
			{
				if(IdleReqa())
				{
					if(debug)
						debug->Write(_T("Card_Is"));
				}
				else
				{
					maingdi->ImageChange(0, FALSE); uThreadLoop=THREADLOOP_DETECTING;
					if(debug)
						debug->Write(_T("Card_Non"));
				}
			}
		}
		else if(uThreadLoop&THREADLOOP_DETECTING)
		{
			if(uLoopMode&DEVICEMODE_TARGET)
			{
				if(PollingDetectTarget())
				{
					//uThreadLoop=THREADLOOP_NONE;
					if(debug)
						debug->Write(_T("Target_Detect"));
					maingdi->ImageChange(1);
// 					CString recvlen;
// 					BOOL b = PollingSend(_T("97313233"), recvlen);
// 					if(b)
// 					{
// 						if(debug)
// 							debug->Write(_T("성공데이터 보내기"));
// 						//Sleep(10);
// 						while(TRUE)
// 						{
// 							 b = PollingSend(_T("9731323335353535353535"), recvlen);
// 							if(b)
// 							{
// 								if(debug)
// 									debug->Write(_T("성공데이터 보내기"));
// 							}
// 							else
// 							{	
// 								if(debug)
// 									debug->Write(_T("보내기에러"));
// 							}
// 							if(++bTargetLoop>20)
// 								break;
// 							//Sleep(10);
// 						}
// 						Sleep(1000);
// 						if(debug)
// 							debug->Write(_T("완료"));
// 					}
// 					else
// 					{	
// 						if(debug)
// 							debug->Write(_T("보내기에러"));
// 					}
				}
			}
			else if(uLoopMode&DEVICEMODE_INITIATOR)
			{
				if(PollingDetectInitiator())
				{
					uThreadLoop=THREADLOOP_NONE;
					if(debug)
						debug->Write(_T("Initiator_Detect"));
					if(maingdi->ImageChange(1)==FALSE)
					{
						uThreadLoop=THREADLOOP_DETECTING;	
					}
				}
			}
			else
			{
				if(debug)
					debug->Write(_T("None Mode"));
			}
		}
		else
		{
			if(debug)
				debug->Write(_T("None Thread"));
		}
	}
	CloseEvent.SetEvent();
}
//--------------------------------------------------------------
//|
//| 
//|
//---------------------------------------------------------------
DWORD WINAPI CDualiCardMg::DetectProcess(LPVOID lpArg)
{
	IDualiCardMg *dualimg = (IDualiCardMg *)lpArg;
	dualimg->CardDetectFunc();
	return 0;
}

//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CDualiCardMg::Release()
{
	//|=============================================
	//|샘카드 끊기
	//|==================================================
	if(bSAMOn)
		SAMPwoff();
	//|=============================================
	//|카드 포트 끊기
	//|==================================================
	CardConnent("USB");
	//|=============================================
	//|스래드 종료 이벤트
	//|==================================================
	if(DetectEvent.IsOpen())
	{
		DetectEvent.SetEvent();
		CloseEvent.Create();
		Sleep(10);
	}
	CloseEvent.WaitForEvent();
	Sleep(10);
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CDualiCardMg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// 오브젝트 관리 생성 관리 클래스
	//========================================
	obj=_obj;
	//========================================
	// 디버그 출력 클래스
	//========================================
	debug=obj->GetOutputObject();
	CardConnent("USB");
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CDualiCardMg::CardConnent(CString nPort)
{
	CString strport,strtmp;
	int ret;
	if(nPort == _T("USB"))
	{
		m_nPort = 100;
	}
	else
	{
	}

	if(m_bConnect) //닫기
	{
		ret = DE_BuzzerOn(m_nPort);
		Sleep(30);
		ret = DE_BuzzerOff(m_nPort);
		DE_ClosePort(m_nPort);
		m_nPort = 0;
		m_bConnect = FALSE;
	}
	else// 연결
	{
		ret = DE_InitPort(m_nPort,BAUD);
		if(m_nPort == ret)
		{
			ret = DE_BuzzerOn(m_nPort);
			Sleep(30);
			ret = DE_BuzzerOff(m_nPort);
			Sleep(20);
			ret = DE_BuzzerOn(m_nPort);
			Sleep(30);
			ret = DE_BuzzerOff(m_nPort);

			m_bConnect = TRUE;
			if(debug)
				debug->Write(_T("연결 성공"));
			//========================================
			// 카드 감지 스래드 생성
			//========================================
			DetectObject();
			//========================================
			// 샘카드 ON
			//========================================
			SAMPwon();
		}
		else
		{
			strtmp.Format(_T("[ERROR]DE_InitPort:%d"),ret);
			m_nPort = 0;
			if(debug)
				debug->Write(_T("FAIL"));
		}
	}	
}
//--------------------------------------------------------------
//|////////////////////////////////
//|//MiFare
//|////////////////////////////////
//---------------------------------------------------------------
//--------------------------------------------------------------
//|카드가 있는가를 묻는 함수
//---------------------------------------------------------------
BOOL CDualiCardMg::IdleReqa() 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	CString str;
	str = ShowString("DETECT_IDLE 아이들",SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DEA_Idle_Req(m_nPort,&nRecv, byteRecv);
	if(ret != DE_OK)
		ret = DEA_Idle_Req(m_nPort,&nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		b=TRUE;
	}
	if(debug)
		debug->Write(_T("initiator 에러코드:%d"),ret);
	return b;
}
BOOL CDualiCardMg::PollingDetectInitiator() 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	CString str;
	str = ShowString("POLLING_INITIATOR", SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));
	int ret;
	int nRecv = 0;
	BYTE PollBuf[5];
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	PollBuf[0] = 0x94;
	PollBuf[1] = 0x41;
	//PollBuf[1] = 0x50;
	PollBuf[2] = 0x02;
	PollBuf[3] = 0x00;
	PollBuf[4] = 0x00;
	ret = DE_Polling(m_nPort, 5,  PollBuf, &nRecv, byteRecv, 50);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		b=TRUE;
	}
	if(debug)
		debug->Write(_T("initiator 에러코드:0x%x"),ret);
	return b;
}
BOOL CDualiCardMg::PollingDetectTarget() 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	CString str;
	str = ShowString("POLLINT_TARGET",SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));
	int ret;
	int nRecv = 0;
	BYTE PollBuf[5];
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	PollBuf[0] = 0x95;
	PollBuf[1] = 0x54;
	PollBuf[2] = 0x00;
	PollBuf[3] = 0x00;
	PollBuf[4] = 0x0F;
	ret = DE_Polling(m_nPort, 5,  PollBuf, &nRecv, byteRecv, 50);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		b=TRUE;
	}
	if(debug)
		debug->Write(_T("Target 에러코드:0x%x"),ret);
	return b;
}
//--------------------------------------------------------------
//|간단한 데이타 전송
//---------------------------------------------------------------
BOOL  CDualiCardMg::OneDataSend(CString apdu, CString& recv, int nCount, BOOL brecv)
{
	BOOL b = FALSE;
	CString start = _T("96");
	CString Hex = Changcharbyhex(apdu);
	b = PollingSend(start+Hex, recv);
	Sleep(1000);
	uThreadLoop=THREADLOOP_IDLING;
	return b;
}
//--------------------------------------------------------------
//|파일 데이타 전송
//---------------------------------------------------------------
#define MAXSENDSIZE 76
BOOL  CDualiCardMg::FileDataSend(CString FileName)
{
	BOOL b = FALSE;
	LPBYTE bytecap;
	DWORD readlen;
	if(_GAFILESTREAM::CreateReadFile(FileName,bytecap,readlen))
	{
		CString repass;
		if(ChangBytebyhex(bytecap, readlen,  repass))
		{
			delete[] bytecap;
			//타켓 모드로 전송하는 값
			CString start = _T("97");
			UINT midstart = 0;
			UINT Filesize = repass.GetLength();
			//데이타 길이를 먼저 보낸다
			CString lenHex;
 			CString recvlen;
			CString filelen;
 			filelen.Format(_T("%u"),readlen);
			lenHex = Changcharbyhex(filelen);
			Sleep(20);
			b = PollingSend(start+lenHex, recvlen);
			if(b==FALSE) 
				b = PollingSend(start+lenHex, recvlen);
			if(b)
			{
				while (TRUE)
				{
					Sleep(20);
					UINT afsize;
					CString recv;
					if(midstart+MAXSENDSIZE>=Filesize)
					{
						afsize=	Filesize-midstart;
					}
					else
					{
						afsize=MAXSENDSIZE;
					}
					CString Hex = repass.Mid(midstart,afsize);
					if(PollingSend(start+Hex, recv)==FALSE)
					{

						if(PollingSend(start+Hex, recv)==FALSE)
						{
							b= FALSE; break;
						}
					}
					midstart+=afsize;
					if(midstart==Filesize)
					{
						b= TRUE; break;
					}
				}
			}
		}
	}
	Sleep(1000);
	uThreadLoop=THREADLOOP_IDLING;
	return b;
}
//--------------------------------------------------------------
//|카드 충돌 방지(하나의 카드를 선택 UI를 가지고 온다)
//---------------------------------------------------------------
CString CDualiCardMg::Anticol() 
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return "";
	}
	CString str;
	str = ShowString("ANTICOLLISION",SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DEA_Anticoll(m_nPort,&nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		return showdata;
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	return "";
}
//--------------------------------------------------------------
//|선택된 카드를 연결한다
//---------------------------------------------------------------
void CDualiCardMg::Select(BYTE* Recv, int len) 
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return ;
	}
	CString str;
	str = ShowString("SELECT",SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));

	BYTE byteSend[MAX_BUF];
	memset(byteSend, 0, sizeof(byteSend)/sizeof(byteSend[0]));
	memcpy(byteSend,Recv+1,len); // len = 4
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DEA_Select(m_nPort, byteSend, &nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}

}
//--------------------------------------------------------------
//|카드를 찾는다
//---------------------------------------------------------------
BOOL CDualiCardMg::FindCard() 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	CString str;
	str = ShowString("FINDCARD",SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	BYTE baud = 0x00;
	BYTE cid = 0x01;
	BYTE nad = 0x01;
	BYTE opt = 0x00;
	int ret = DE_FindCard(m_nPort, baud, cid, nad, opt, &nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CString CDualiCardMg::ShowString(int len,BYTE* data,BYTE flag)
{
	int i;
	CString msg="",msg2="";
	char ctmp[255];
	memset(ctmp,0x00,sizeof(ctmp)/sizeof(ctmp[0]));

	if(flag == SENDFLAG)
	{
		for(i = 0; i < len; i++)
		{
			msg2.Format(_T("%02X"),data[i]);
			msg += msg2;
		}
	}
	else if(flag == 1)	
	{
		for(i = 0; i < len; i++)
		{
			msg2.Format(_T("%02X"),data[i]);
			msg += msg2;
		}
	}
	else
	{
		GetErrMsg(len,ctmp);
		msg.Format(_T("[ERROR] 0x%02X(%s)"),len,ctmp);		
	}
	return msg;
}
//--------------------------------------------------------------
//|
//|flag => 0:send  1:receive 2:error
//|
//---------------------------------------------------------------
CString CDualiCardMg::ShowString(CString data,BYTE flag)
{
 	CString msg="";
	if(flag == SENDFLAG)	
		msg += _T("=>") + data;	
	else if(flag == 1)	
		msg += _T("<=") + data;
	else
		msg += _T("[ERROR]") + data;
	return msg;
}
//--------------------------------------------------------------
//|
//|Read
//|
//---------------------------------------------------------------
BOOL CDualiCardMg::DeviceInfoRead()
{
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DEA_DeviceInfo(m_nPort,&nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
int  CDualiCardMg::ChangeHexCord(CString data, BYTE hex[])
{
	BYTE buffer[MAX_BUF];
	memset(buffer, 0, sizeof(buffer)/sizeof(buffer[0]));
	int dwtransLen=data.GetLength()/2;
	for(int i = 0 ; i < dwtransLen ; i++ )
	{
		buffer[i] = (((BYTE)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			buffer[i] += 0x90;

		buffer[i] |= ((BYTE)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			buffer[i] += 0x09;
	}
	memcpy(hex,buffer,MAX_BUF);
	return dwtransLen;
}
//--------------------------------------------------------------
//|
//|Apud로 값을 가지오는 함수 T0통신
//|
//---------------------------------------------------------------
BOOL CDualiCardMg::ApduCommand(CString apdu, CString& recv, int nCount, BOOL brecv)
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return FALSE;
	}
	if(debug)
		debug->Write(_T("[SCARD_APDU]%s"),apdu);
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer)/sizeof(sendbuffer[0]));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	BYTE sendlen = (BYTE)(intlen&0xFF);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DE_APDU(m_nPort,sendlen,sendbuffer,&nRecv,byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		if(debug)
			debug->Write(_T("[SCARD_RECV]%s"),showdata.GetBuffer(0));
		if(showdata.Right(4)=="9000")
		{
			if(brecv)
			{
				if(nCount)
				{
					int midr = nCount*2;
					int midf = showdata.GetLength()-(midr+4);
					recv = showdata.Mid(midf, midr);
				}
				else
				{
					recv = showdata.Left(showdata.GetLength()-4);
				}
			}
			return TRUE;
		}
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|ISO 7816
//|
//---------------------------------------------------------------
//====================================================
// Power off
//====================================================
void CDualiCardMg::SAMPwoff() 
{
	if(nSlotno==-1)
	{
		if(debug)
			debug->Write(_T("No Pow on"));
		return ;
	}
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		nSlotno=-1;
		return ;
	}
	CString str;
	str = ShowString(_T("Power off"),SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));

	CString msg;
	int nret = DE_IC_PowerOff(m_nPort, nSlotno);
	msg.Format(_T("%02X"),nret);

	CString showdata;
	showdata = ShowString(msg,RECVFLAG);
	if(debug)
		debug->Write(showdata.GetBuffer(0));
	nSlotno=-1;
}
//====================================================
// Power on
//====================================================
void CDualiCardMg::SAMPwon(int _nSlotno) 
{
	if(nSlotno==_nSlotno)
	{
		if(debug)
			debug->Write(_T("Pow on 상태"));
		return ;
	}
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return ;
	}
	nSlotno = _nSlotno;
	CString str;
	str = ShowString(_T("Power on"),SENDFLAG);
	if(debug)
		debug->Write(str.GetBuffer(0));

	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int nret = DE_IC_PowerOn(m_nPort,nSlotno, &nRecv, byteRecv);
	if(nret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		bSAMOn=TRUE;
	}
	else
	{
		CString showdata;
		showdata = ShowString(nret,NULL,ERRORFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
		bSAMOn=FALSE;
	}
}
//====================================================
// SAM apdu
//====================================================
BOOL CDualiCardMg::SAMApduSend(CString apdu, CString& recv, int nCount, BOOL brecv) 
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return FALSE;
	}
	if(debug)
		debug->Write(_T("[SAM_APDU]%s"),apdu);
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer)/sizeof(sendbuffer[0]));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	//BYTE sendlen = (BYTE)(intlen&0xFF);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DE_CARD_APDU(m_nPort,nSlotno, intlen, sendbuffer,&nRecv,byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		if(debug)
			debug->Write(_T("[SAM_RECV]%s"),showdata.GetBuffer(0));
		if(showdata.Right(4)=="9000")
		{
			if(brecv)
			{
				if(nCount)
				{
					int midr = nCount*2;
					int midf = showdata.GetLength()-(midr+4);
					recv = showdata.Mid(midf, midr);
				}
				else
				{
					recv = showdata.Left(showdata.GetLength()-4);
				}
			}
			return TRUE;
		}
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	return FALSE;
}
BOOL CDualiCardMg::PollingSend(CString apdu, CString& recv, int nCount, BOOL brecv) 
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return FALSE;
	}
	if(debug)
		debug->Write(_T("[POLLING_DATA]%s"),apdu);
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer)/sizeof(sendbuffer[0]));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv)/sizeof(byteRecv[0]));
	int ret = DE_Polling(m_nPort, intlen, sendbuffer, &nRecv,byteRecv);
	if(ret == DE_OK)
	{
		if(nRecv)
		{
			CString showdata;
			showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
			if(debug)
				debug->Write(_T("[POLLING_RECV]%s"),showdata.GetBuffer(0));

			if(brecv&&showdata.GetLength()>nCount)
			{
				if(nCount)
				{
					int midr = nCount*2;
					int midf = showdata.GetLength()-(midr);
					recv = showdata.Mid(midf, midr);
				}
				else
				{
					recv = showdata.Left(showdata.GetLength());
				}
			}
		}
		return TRUE;
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		if(debug)
			debug->Write(showdata.GetBuffer(0));
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|char를 hex로 변환
//|
//---------------------------------------------------------------
CString CDualiCardMg::Changcharbyhex(CString pass)
{
	CString retvalue="";
	CString temp;
	TCHAR *a = pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format(_T("%02X"), a[i]);
		retvalue += temp;
	}
	return retvalue;
}
//--------------------------------------------------------------
//|
//|카드로 부터 받은 데이타를 변환 한다(0x00~0xFF까지 이므로 스트링형으로 변환해서 판단한다)
//|
//---------------------------------------------------------------
BOOL CDualiCardMg::ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass)
{
	CString temp;
	for( int i = 0; i < (int)len; i++ )
	{
		temp.Format(_T("%02X"), pass[i]);
		repass += temp;
	}
	if(len==(repass.GetLength()/2))
		return TRUE;
	return FALSE;
}