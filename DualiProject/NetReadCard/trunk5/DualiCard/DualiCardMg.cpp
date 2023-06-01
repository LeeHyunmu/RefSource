
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
#define DETECTEVENTTIME 1000
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
cardID(_T("")),
Mode(0)
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
	//종료 스레드 생성
	DetectEvent.Create();
	Detectthread.Create(DetectProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|카드 감지 및 처리 함수
//|
//---------------------------------------------------------------
#define GCARD _T("A3EDC9BF")
#define PHONE_1 _T("4B453418")
#define PHONE_2 _T("9B853318")
#define MODESTRING_1 _T("벨소리 제어모드 상태!!")
#define MODESTRING_2 _T("카메라 제어모드 상태!!")
#define MODESTRING_3 _T("벨소리 제어모드로 설정변경!!")
#define MODESTRING_4 _T("카메라 제어모드로 설정변경!!")

void CDualiCardMg::CardDetectFunc()
{
	while(!DetectEvent.WaitForEvent(DETECTEVENTTIME))
	{
		if(IdleReqa())
		{
			if(cardID.IsEmpty())
			{
				cardID = Anticol();
				if(cardID.GetLength())
				{
					IGdiDlg* maingdi=obj->GetMainGdiObject();
					maingdi->SetListString(_T("========================="));
					maingdi->SetDrawItem(2,RGB(0x00, 0xff, 0x00));
					_GASOUND::SoundPlay(_T("Flash Disk\\ok.wav"));
					if(FindCard())
					{
						if(cardID.CompareNoCase(GCARD)==0)
						{
							maingdi->SetListString(_T("카드 접속!!"));
							BOOL b;
							CString scardRecv="";
							CString SamRecv="";
							CString Recvback="";
							b = ApduCommand("00A404000C616E79717569746F75733031",scardRecv,13,TRUE);
							maingdi->SetListString(_T("카드 데이터 읽기"));
							if(b)
							{
								b = SAMApduSend("00A404000C616E79717569746F75733032",SamRecv,8,TRUE);
								maingdi->SetListString(_T("샘카드 데이터 읽기"));
							}
							if(b)
							{
								CString recv1="";
								b = SAMApduSend("00F1000003"+scardRecv.Left(6),recv1);
							}
							if(b)
							{
								CString recv1="";
								b = SAMApduSend("90CA000008"+scardRecv.Right(16),recv1,0,TRUE);
								Recvback=recv1;
								maingdi->SetListString(_T("데이터전송!!"));
							}
							if(b)
							{
								CString recv1="";
								b = ApduCommand(Recvback+SamRecv,recv1);
							}
							CString cardString = b?_T("데이터 전송 성공") : _T("데이터 전송 실패");
							maingdi->SetListString(cardString);
							maingdi->SetStaticString(cardString);
						}
						else
						{
							maingdi->SetListString(_T("폰 접속!!"));
							BOOL bb;
							CString PhoneRecv="";
							bb = ApduCommand("00A4040007A0000000030000",PhoneRecv);
							BOOL b=FALSE;
							if(cardID.CompareNoCase(PHONE_1)==0)
							{
								b=TRUE;
							}
							else
							{

							}
							CString ModeString = Mode==0 ?(b?MODESTRING_1 : MODESTRING_2):(Mode==1?MODESTRING_3:MODESTRING_4);
							maingdi->SetListString(ModeString);
							maingdi->SetStaticString(ModeString);
						}
					}
					maingdi->SetListString(_T("========================="));
				}
			}
		}
		else
		{
			if(cardID.GetLength())
			{
				IGdiDlg* maingdi=obj->GetMainGdiObject();
				maingdi->SetListString(_T("========================="));
				maingdi->SetListString(_T("접속 해제!!"));
				maingdi->SetStaticString(_T("접속 해제!!"));
				maingdi->SetListString(_T("========================="));
				maingdi->SetDrawItem(2,RGB(0xff, 0x00, 0x00));
				cardID.Empty();
				_GASOUND::SoundPlay(_T("Flash Disk\\error.wav"));
			}
		}
		DE_RFOff(m_nPort);
	}
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
	}
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
		//debug->Write(_T("연결 끊기"));
	}
	else // 연결
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
			debug->Write(_T("연결 성공"));
			//========================================
			// 카드 감지 스래드 생성
			//========================================
			DetectObject();
			//========================================
			// 샘카드 ON
			//========================================
			SAMPwon();
			IGdiDlg* maingdi=obj->GetMainGdiObject();
			maingdi->SetListString(_T("========================="));
			maingdi->SetListString(_T("카드 초기화"));
			maingdi->SetListString(_T("SAM 카드 ON"));
			maingdi->SetListString(_T("카드 접속 대기 상태"));
			maingdi->SetDrawItem(0,RGB(0x00, 0xff, 0x00));
			maingdi->SetDrawItem(1,RGB(0x00, 0xff, 0x00));
			maingdi->SetListString(_T("단말기 초기화!!"));
			maingdi->SetStaticString(_T("단말기 초기화!!"));
			maingdi->SetListString(_T("========================="));
		}
		else
		{
			strtmp.Format(_T("[ERROR]DE_InitPort:%d"),ret);
			m_nPort = 0;
			debug->Write(_T("연결 실패"));
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
		debug->Write(_T("The Device is not connected"));
		return b;
	}
	DE_RFOn(m_nPort);
	CString str;
	str = ShowString("DETECT_IDLE",SENDFLAG);
	debug->Write(str.GetBuffer(0));
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_Idle_Req(m_nPort,&nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
		debug->Write(showdata.GetBuffer(0));
		b=TRUE;
	}
	else
		debug->Write(_T("Er:%d"), ret);
	return b;
}
//--------------------------------------------------------------
//|카드 충돌 방지(하나의 카드를 선택 UI를 가지고 온다)
//---------------------------------------------------------------
CString CDualiCardMg::Anticol() 
{
	if(!m_bConnect)
	{
		debug->Write(_T("The Device is not connected"));
		return "";
	}
	CString str;
	str = ShowString("ANTICOLLISION",SENDFLAG);
	debug->Write(str.GetBuffer(0));
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_Anticoll(m_nPort,&nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		debug->Write(showdata.GetBuffer(0));
		return showdata;
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
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
		debug->Write(_T("The Device is not connected"));
		return ;
	}
	CString str;
	str = ShowString("SELECT",SENDFLAG);
	debug->Write(str.GetBuffer(0));

	BYTE byteSend[MAX_BUF];
	memset(byteSend, 0, sizeof(byteSend));
	memcpy(byteSend,Recv+1,len); // len = 4
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_Select(m_nPort, byteSend, &nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		debug->Write(showdata.GetBuffer(0));
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
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
		debug->Write(_T("The Device is not connected"));
		return b;
	}
	CString str;
	str = ShowString("FINDCARD",SENDFLAG);
	debug->Write(str.GetBuffer(0));
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	BYTE baud = 0x00;
	BYTE cid = 0x01;
	BYTE nad = 0x01;
	BYTE opt = 0x00;
	int ret = DE_FindCard(m_nPort, baud, cid, nad, opt, &nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);	
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
BOOL CDualiCardMg::DeviceInfoRead()
{
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DEA_DeviceInfo(m_nPort,&nRecv, byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		debug->Write(showdata.GetBuffer(0));
	}
	else
	{
		CString showdata;
		showdata = ShowString(ret,NULL,ERRORFLAG);	
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
BOOL CDualiCardMg::ApduCommand(CString apdu, CString& recv, int nCount, BOOL brecv)
{
	if(!m_bConnect)
	{
		debug->Write(_T("The Device is not connected"));
		return FALSE;
	}
	debug->Write(_T("[SCARD_APDU]%s"),apdu);
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	BYTE sendlen = (BYTE)(intlen&0xFF);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DE_APDU(m_nPort,sendlen,sendbuffer,&nRecv,byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
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
		debug->Write(_T("No Pow on"));
		return ;
	}
	if(!m_bConnect)
	{
		debug->Write(_T("The Device is not connected"));
		nSlotno=-1;
		return ;
	}
	CString str;
	str = ShowString(_T("Power off"),SENDFLAG);
	debug->Write(str.GetBuffer(0));

	CString msg;
	int nret = DE_IC_PowerOff(m_nPort, nSlotno);
	msg.Format(_T("%02X"),nret);

	CString showdata;
	showdata = ShowString(msg,RECVFLAG);
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
		debug->Write(_T("Pow on 상태"));
		return ;
	}
	if(!m_bConnect)
	{
		debug->Write(_T("The Device is not connected"));
		return ;
	}
	nSlotno = _nSlotno;
	CString str;
	str = ShowString(_T("Power on"),SENDFLAG);
	debug->Write(str.GetBuffer(0));

	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int nret = DE_IC_PowerOn(m_nPort,nSlotno, &nRecv, byteRecv);
	if(nret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
		debug->Write(showdata.GetBuffer(0));
	}
	else
	{
		CString showdata;
		showdata = ShowString(nret,NULL,ERRORFLAG);	
		debug->Write(showdata.GetBuffer(0));
	}
}
//====================================================
// SAM apdu
//====================================================
BOOL CDualiCardMg::SAMApduSend(CString apdu, CString& recv, int nCount, BOOL brecv) 
{
	if(!m_bConnect)
	{
		debug->Write(_T("The Device is not connected"));
		return FALSE;
	}
	debug->Write(_T("[SAM_APDU]%s"),apdu);
	BYTE sendbuffer[MAX_BUF];
	memset(sendbuffer, 0, sizeof(sendbuffer));
	int intlen = ChangeHexCord(apdu,sendbuffer);
	//BYTE sendlen = (BYTE)(intlen&0xFF);
	int nRecv=0;
	BYTE byteRecv[MAX_BUF];
	memset(byteRecv, 0, sizeof(byteRecv));
	int ret = DE_CARD_APDU(m_nPort,nSlotno, intlen, sendbuffer,&nRecv,byteRecv);
	if(ret == DE_OK)
	{
		CString showdata;
		showdata = ShowString(nRecv-1,byteRecv+1,RECVFLAG);
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
		debug->Write(showdata.GetBuffer(0));
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|모드 설정
//|
//---------------------------------------------------------------
void CDualiCardMg::SetControlMode(int _Mode)
{
	Mode = _Mode;
	CString ModeString = _Mode==0?_T("폰 제어읽기 설정모드"):(_Mode==1?_T("폰 벨소리제어 설정모드!!"):_T("폰 카메라제어 설정모드!!"));
	debug->Write(ModeString.GetBuffer());
	IGdiDlg* maingdi=obj->GetMainGdiObject();
	maingdi->SetListString(_T("========================="));
	maingdi->SetListString(_T("단말기 모드 변경!!"));
	maingdi->SetListString(ModeString);
	maingdi->SetListString(_T("========================="));
}