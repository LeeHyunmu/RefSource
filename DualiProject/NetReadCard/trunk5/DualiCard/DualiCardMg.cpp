
#include "stdAfx.h"
#include "DualiCardMg.h"
#include "DualCardDllHeader.h"
#pragma comment(lib, "DualDlleMbedded.lib")

#define SENDFLAG	0
#define RECVFLAG	1
#define ERRORFLAG	2

#define MAX_BUF		255
#define BAUD		115200
//������ Ÿ�Ӿƿ�
#define DETECTEVENTTIME 1000
//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
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
	//���� ������ ����
	DetectEvent.Create();
	Detectthread.Create(DetectProcess, (LPVOID)this);
}
//--------------------------------------------------------------
//|
//|ī�� ���� �� ó�� �Լ�
//|
//---------------------------------------------------------------
#define GCARD _T("A3EDC9BF")
#define PHONE_1 _T("4B453418")
#define PHONE_2 _T("9B853318")
#define MODESTRING_1 _T("���Ҹ� ������ ����!!")
#define MODESTRING_2 _T("ī�޶� ������ ����!!")
#define MODESTRING_3 _T("���Ҹ� ������� ��������!!")
#define MODESTRING_4 _T("ī�޶� ������� ��������!!")

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
							maingdi->SetListString(_T("ī�� ����!!"));
							BOOL b;
							CString scardRecv="";
							CString SamRecv="";
							CString Recvback="";
							b = ApduCommand("00A404000C616E79717569746F75733031",scardRecv,13,TRUE);
							maingdi->SetListString(_T("ī�� ������ �б�"));
							if(b)
							{
								b = SAMApduSend("00A404000C616E79717569746F75733032",SamRecv,8,TRUE);
								maingdi->SetListString(_T("��ī�� ������ �б�"));
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
								maingdi->SetListString(_T("����������!!"));
							}
							if(b)
							{
								CString recv1="";
								b = ApduCommand(Recvback+SamRecv,recv1);
							}
							CString cardString = b?_T("������ ���� ����") : _T("������ ���� ����");
							maingdi->SetListString(cardString);
							maingdi->SetStaticString(cardString);
						}
						else
						{
							maingdi->SetListString(_T("�� ����!!"));
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
				maingdi->SetListString(_T("���� ����!!"));
				maingdi->SetStaticString(_T("���� ����!!"));
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
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CDualiCardMg::Release()
{
	//|=============================================
	//|��ī�� ����
	//|==================================================
	SAMPwoff();
	//|=============================================
	//|ī�� ��Ʈ ����
	//|==================================================
	CardConnent("USB");
	//|=============================================
	//|������ ���� �̺�Ʈ
	//|==================================================
	if(DetectEvent.IsOpen())
	{
		DetectEvent.SetEvent();
	}
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CDualiCardMg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
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

	if(m_bConnect) //�ݱ�
	{
		ret = DE_BuzzerOn(m_nPort);
		Sleep(30);
		ret = DE_BuzzerOff(m_nPort);
		DE_ClosePort(m_nPort);
		m_nPort = 0;
		m_bConnect = FALSE;
		//debug->Write(_T("���� ����"));
	}
	else // ����
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
			debug->Write(_T("���� ����"));
			//========================================
			// ī�� ���� ������ ����
			//========================================
			DetectObject();
			//========================================
			// ��ī�� ON
			//========================================
			SAMPwon();
			IGdiDlg* maingdi=obj->GetMainGdiObject();
			maingdi->SetListString(_T("========================="));
			maingdi->SetListString(_T("ī�� �ʱ�ȭ"));
			maingdi->SetListString(_T("SAM ī�� ON"));
			maingdi->SetListString(_T("ī�� ���� ��� ����"));
			maingdi->SetDrawItem(0,RGB(0x00, 0xff, 0x00));
			maingdi->SetDrawItem(1,RGB(0x00, 0xff, 0x00));
			maingdi->SetListString(_T("�ܸ��� �ʱ�ȭ!!"));
			maingdi->SetStaticString(_T("�ܸ��� �ʱ�ȭ!!"));
			maingdi->SetListString(_T("========================="));
		}
		else
		{
			strtmp.Format(_T("[ERROR]DE_InitPort:%d"),ret);
			m_nPort = 0;
			debug->Write(_T("���� ����"));
		}
	}	
}
//--------------------------------------------------------------
//|////////////////////////////////
//|//MiFare
//|////////////////////////////////
//---------------------------------------------------------------
//--------------------------------------------------------------
//|ī�尡 �ִ°��� ���� �Լ�
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
//|ī�� �浹 ����(�ϳ��� ī�带 ���� UI�� ������ �´�)
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
//|���õ� ī�带 �����Ѵ�
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
//|ī�带 ã�´�
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
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
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
//|Apud�� ���� �������� �Լ� T0���
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
		debug->Write(_T("Pow on ����"));
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
//|��� ����
//|
//---------------------------------------------------------------
void CDualiCardMg::SetControlMode(int _Mode)
{
	Mode = _Mode;
	CString ModeString = _Mode==0?_T("�� �����б� �������"):(_Mode==1?_T("�� ���Ҹ����� �������!!"):_T("�� ī�޶����� �������!!"));
	debug->Write(ModeString.GetBuffer());
	IGdiDlg* maingdi=obj->GetMainGdiObject();
	maingdi->SetListString(_T("========================="));
	maingdi->SetListString(_T("�ܸ��� ��� ����!!"));
	maingdi->SetListString(ModeString);
	maingdi->SetListString(_T("========================="));
}