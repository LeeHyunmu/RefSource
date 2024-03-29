
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
uLoopMode(DEVICEMODE_INITIATOR),
uThreadLoop(THREADLOOP_DETECTING),
uShowMode(0)
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
//TCHAR MobileSendData[][40]={_T("Sin.jpg"),_T("02http://www.ajussi2010.co.kr"), _T("0101094323475")};
DWORD idleTime = 0;
#define  IDLETIME_VALUE 3000
DWORD idlecount = 0;
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
				if(PollingDetectTarget(FALSE))
				{
					idleTime = GetTickCount();
				}
			}
			else if(uLoopMode&DEVICEMODE_INITIATOR)
			{
				if(PollingDetectInitiator(FALSE))
				{
					idleTime = GetTickCount();
				}
			}
			if(GetTickCount()>idleTime+IDLETIME_VALUE)
			{
				uThreadLoop=THREADLOOP_DETECTING;
				if(uShowMode==2)
				{
					maingdi->SetMode3Text(_T(""), NULL);
				}
				maingdi->ImageChange(0);
			}
			if(debug)
				debug->Write(_T("Idle_count:%d"),idlecount++);
		}
		else if(uThreadLoop&THREADLOOP_DETECTING)
		{
			if(uLoopMode&DEVICEMODE_TARGET)
			{
				if(PollingDetectTarget(FALSE))
				{
					SetThreadState(THREADLOOP_NONE,0);
					if(FileDataSend())
					{
						maingdi->ImageChange(1);
						idleTime = GetTickCount();
					}
				}
// 				if(SendFile())
// 				{
// 					maingdi->ImageChange(1);
// 					idleTime = GetTickCount();
// 				}
			}
			else if(uLoopMode&DEVICEMODE_INITIATOR)
			{
				if(PollingDetectInitiator(FALSE))
				{
					if(maingdi->GetModeException())
					{
						SetThreadState(THREADLOOP_NONE,0);
						CString recv;
						if(OneDataSend(MobileSendData[uShowMode], recv))	
						{
							if(uShowMode==2)
							{
								CString strtemp = MobileSendData[uShowMode];
								CString strformat;
								strformat.Format(_T("%s-%s-%s"),strtemp.Mid(2,3),strtemp.Mid(5,4), strtemp.Mid(9,4));
								maingdi->SetMode3Text(strformat, &CRect(200,220,560,260));
							}
							maingdi->ImageChange(1);
							idleTime = GetTickCount();
						}
					}
				}
			}
			else
			{
				if(debug)
					debug->Write(_T("None Mode"));
			}
			if(idlecount)
				idlecount = 0;
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
//| 스래드 상태 설정 함수
//|
//---------------------------------------------------------------
void  CDualiCardMg::SetThreadState(UINT uThreadState, UINT uModeState)
{
	if(uThreadState)
		uThreadLoop=uThreadState;
	if(uModeState)
		uLoopMode=uModeState;
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
	delete[] l_pbuf; l_pbuf=NULL;
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
	//========================================
	// 포트 연결
	//========================================
	CardConnent("USB");
	//========================================
	// 카드 감지 스래드 생성
	//========================================
	DetectObject();
	//========================================
	// 샘카드 ON
	//========================================
	//SAMPwon();
	//========================================
	// 파일 전송 정보 로드
	//========================================
	//정보로드
	CString str;
	if(_GAFILESTREAM::CreateLineReadFile(_T("P2pInfo.txt"),MobileSendData))
	{
		str = MobileSendData[0];
	}
	//========================================
	// 전송 파일 미리 로딩
	//========================================
	//파일 로딩
	l_pbuf=NULL;
	if(_GAFILESTREAM::CreateReadFile(MobileSendData[0],l_pbuf,read_size))
	{
		if(ChangBytebyhex(l_pbuf, read_size,  repass))
		{
			if(debug)
				debug->Write(_T("파일로드"));
		}
	}
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
BOOL CDualiCardMg::IdleReqa(BOOL bDebug) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_Idle_Req(m_nPort,&nRecv, byteRecv);
	if(ret != DE_OK)
		ret = DEA_Idle_Req(m_nPort,&nRecv, byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		b=TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[IDLECARDREAD_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
}
BOOL CDualiCardMg::PollingDetectInitiator(BOOL bDebug) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		CardConnent("USB");
		if(!m_bConnect)
		{
			if(debug)
				debug->Write(_T("The Device is not connected"));
			return b;
		}
	}
	int ret;
	int nRecv = 0;
	BYTE PollBuf[5];
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	PollBuf[0] = 0x94;
	PollBuf[1] = 0x41;
	//PollBuf[1] = 0x50;
	PollBuf[2] = 0x00;
	PollBuf[3] = 0x00;
	PollBuf[4] = 0x00;
	ret = DE_Polling(m_nPort, 5,  PollBuf, &nRecv, byteRecv, 50);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		b=TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[POLLING_INITIATOR_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	if(ret != DE_OK)
	{
		if(debug)
		{
			debug->Write(_T("에러코드:0x%x"),ret);
		}
		if(ret==0x3F0)
		{

			CardConnent("USB");		CardConnent("USB");
		}
	}
	return b;
}
BOOL CDualiCardMg::PollingDetectRelease(BOOL bDebug) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	int ret;
	int nRecv = 0;
	BYTE PollBuf[5];
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	PollBuf[0] = 0x98;
	PollBuf[1] = 0x01;
	ret = DE_Polling(m_nPort, 2,  PollBuf, &nRecv, byteRecv, 50);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		b=TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[POLLING_RELEASE_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
}
BOOL CDualiCardMg::PollingDetectTarget(BOOL bDebug) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		CardConnent("USB");
		if(!m_bConnect)
		{
			if(debug)
				debug->Write(_T("The Device is not connected"));
			return b;
		}
	}
	int ret;
	int nRecv = 0;
	BYTE PollBuf[5];
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	PollBuf[0] = 0x95;
	PollBuf[1] = 0x54;
	PollBuf[2] = 0x00;
	PollBuf[3] = 0x00;
	PollBuf[4] = 0xF0;
	ret = DE_Polling(m_nPort, 5,  PollBuf, &nRecv, byteRecv, 50);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		b=TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[POLLINT_TARGET_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	if(ret != DE_OK)
	{
		if(debug)
		{
			debug->Write(_T("에러코드:0x%x"),ret);
		}
		if(ret==0x3F0)
		{

			CardConnent("USB");		CardConnent("USB");
		}
	}
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
	SetThreadState(THREADLOOP_IDLING,0);
	return b;
}
//--------------------------------------------------------------
//|파일 데이타 전송
//---------------------------------------------------------------
#define MAXSENDSIZE 200
BOOL  CDualiCardMg::FileDataSend()
{
	BOOL b = FALSE;
	//타켓 모드로 전송하는 값
	CString start = _T("97");
	UINT midstart = 0;
	UINT Filesize = repass.GetLength();
	//데이타 길이를 먼저 보낸다
	CString lenHex;
	CString recvlen;
	CString filelen;
	filelen.Format(_T("%u"),read_size);
	lenHex = Changcharbyhex(filelen);
	b = PollingSend(start+lenHex, recvlen, FALSE);
	if(b)
	{
		while (TRUE)
		{
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
			if(PollingSend(start+Hex, recv, FALSE)==FALSE)
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
	SetThreadState(THREADLOOP_IDLING,0);
	return b;
}
#define	NFCIP_BUF_SIZE		100
BOOL CDualiCardMg::SendFile()
{
	BOOL b = FALSE;
	int oLen;
	int ret;
	CString	str;
	BYTE oBuf[256];
	BYTE iBuf[256];
	DWORD idx;
	iBuf[0] = 0x95;iBuf[1] = 0x54;
	iBuf[2] = 00; iBuf[3] = 00;
	iBuf[4] = 0xF0;

	ret = DE_Polling(m_nPort, 5,  iBuf, &oLen, oBuf, 6000);
	if(ret == DE_OK)
	{
		SetThreadState(THREADLOOP_NONE,0);
		int l_copy_size;
		//check = true;
		//iBuf[0] = 0x96;			
		iBuf[0] = 0x97;			
		// 사이즈를 먼저 보내자...
		l_copy_size = sprintf_s((char*)&iBuf[1],sizeof(iBuf),"%d", read_size);				

		memset(oBuf, 0, 256);
		oLen = 0;
		ret = DE_Polling(m_nPort, l_copy_size+1, iBuf, &oLen,oBuf);
		if(ret == DE_OK)	
		{
			idx = 0;
			while(read_size > idx)
			{
				if(idx+NFCIP_BUF_SIZE < read_size)
					l_copy_size = NFCIP_BUF_SIZE;
				else
					l_copy_size = read_size - idx;
				memcpy(&iBuf[1], l_pbuf+idx, l_copy_size);
				memset(oBuf, 0, 256);
				oLen = 0;
				ret = DE_Polling(m_nPort, l_copy_size+1, iBuf, &oLen,oBuf);
				if(ret != DE_OK)
				{					
					break;
				}
				idx += l_copy_size;						
			}
			if(ret == DE_OK)
			{					
				b=TRUE;
			}
		}
		SetThreadState(THREADLOOP_IDLING,0);
	}
	if(ret==0x3F0)
	{
		if(debug)
		{
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
}
//--------------------------------------------------------------
//|카드 충돌 방지(하나의 카드를 선택 UI를 가지고 온다)
//---------------------------------------------------------------
CString CDualiCardMg::Anticol(BOOL bDebug) 
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return "";
	}
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_Anticoll(m_nPort,&nRecv, byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[ANTICOLLISION_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return showdata;
}
//--------------------------------------------------------------
//|선택된 카드를 연결한다
//---------------------------------------------------------------
void CDualiCardMg::Select(BYTE* Recv, int len, BOOL bDebug) 
{
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return ;
	}
	BYTE byteSend[MAX_BUF];
	memset(byteSend, 0, sizeof(byteSend));
	memcpy(byteSend,Recv+1,len); // len = 4
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_Select(m_nPort, byteSend, &nRecv, byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[SELECT_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
}
//--------------------------------------------------------------
//|카드를 찾는다
//---------------------------------------------------------------
BOOL CDualiCardMg::FindCard(BOOL bDebug) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	BYTE baud = 0x00;
	BYTE cid = 0x01;
	BYTE nad = 0x01;
	BYTE opt = 0x00;
	int ret = DE_FindCard(m_nPort, baud, cid, nad, opt, &nRecv, byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		b=TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[FINDCARD_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
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
	memset(ctmp,0x00,sizeof(ctmp));

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
BOOL CDualiCardMg::DeviceInfoRead(BOOL bDebug)
{
	BOOL b = FALSE;
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_DeviceInfo(m_nPort,&nRecv, byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		b = TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[DEVICEINFOREAD_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
int  CDualiCardMg::ChangeHexCord(CString data, BYTE hex[])
{
	BYTE buffer[MAX_BUF];
	memset(buffer, 0, sizeof(buffer));
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
BOOL CDualiCardMg::ApduCommand(CString apdu, CString& recv, BOOL bDebug, int nCount, BOOL brecv)
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	BYTE sendlen = (BYTE)(intlen&0xFF);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DE_APDU(m_nPort,sendlen,sendbuffer,&nRecv,byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
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
			b = TRUE;
		}
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[SCARDCOMMAND_APDU]%s"),apdu);
			debug->Write(_T("[SCARDCOMMAND_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
}
//--------------------------------------------------------------
//|
//|ISO 7816
//|
//---------------------------------------------------------------
//====================================================
// Power off
//====================================================
void CDualiCardMg::SAMPwoff(BOOL bDebug) 
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
	int nret = DE_IC_PowerOff(m_nPort, nSlotno);
	nSlotno=-1;
	if(bDebug)
	{
		if(debug)
		{
			CString str;
			str = ShowString(_T("Power off"),SENDFLAG);
			debug->Write(str.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),nret);
		}
	}
}
//====================================================
// Power on
//====================================================
void CDualiCardMg::SAMPwon(int _nSlotno, BOOL bDebug) 
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
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int nret = DE_IC_PowerOn(m_nPort,nSlotno, &nRecv, byteRecv);
	CString showdata;
	if(nret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		bSAMOn=TRUE;
	}
	else
	{
		showdata = ShowString(nret,NULL,ERRORFLAG);	
		bSAMOn=FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			CString str;
			str = ShowString(_T("Power on"),SENDFLAG);
			debug->Write(str.GetBuffer(0));
			debug->Write(_T("[SAMPwOn_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),nret);
		}
	}
}
//====================================================
// SAM apdu
//====================================================
BOOL CDualiCardMg::SAMApduSend(CString apdu, CString& recv, BOOL bDebug, int nCount, BOOL brecv) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	//BYTE sendlen = (BYTE)(intlen&0xFF);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DE_CARD_APDU(m_nPort,nSlotno, intlen, sendbuffer,&nRecv,byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
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
			b = TRUE;
		}
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[SAM_APDU]%s"),apdu);
			debug->Write(_T("[SAM_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
}
BOOL CDualiCardMg::PollingSend(CString apdu, CString& recv, BOOL bDebug, int nCount, BOOL brecv) 
{
	BOOL b=FALSE;
	if(!m_bConnect)
	{
		if(debug)
			debug->Write(_T("The Device is not connected"));
		return b;
	}
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DE_Polling(m_nPort, intlen, sendbuffer, &nRecv,byteRecv);
	CString showdata;
	if(ret == DE_OK)
	{
		if(nRecv)
		{
			showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
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
		b = TRUE;
	}
	else
	{
		showdata = ShowString(ret,NULL,ERRORFLAG);	
		b = FALSE;
	}
	if(bDebug)
	{
		if(debug)
		{
			debug->Write(_T("[POLLING_DATA]%s"),apdu);
			debug->Write(_T("[POLLING_RECV]%s"),showdata.GetBuffer(0));
			debug->Write(_T("에러코드:0x%x"),ret);
		}
	}
	return b;
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
// 모드 체인지 함수(시연 진행 모드)
void  CDualiCardMg::ChangeModeShow(UINT _uShowMode)
{
	uShowMode = _uShowMode;
	if(uShowMode==0)
	{
		uLoopMode=DEVICEMODE_TARGET;
		uThreadLoop=THREADLOOP_DETECTING;
	}
	else
	{
		uLoopMode=DEVICEMODE_INITIATOR;
		uThreadLoop=THREADLOOP_DETECTING;
	}
}