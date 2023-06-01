#include "stdAfx.h"
#include "SCardTm.h"

//--------------------------------------------------------------
//|
//|SCrad ����̽� ����ϱ� ���� Ŭ���� ���̺귯��
//|
//---------------------------------------------------------------
#pragma comment(lib, "winscard.lib")
//--------------------------------------------------------------
//|
//|SCard ���� ũ�� �߽�
//|
//---------------------------------------------------------------
#define  MAXSCARD_HEXSTRING  256
#define  MAXSCARD_RECVDATA	255
#define  MAXSCARD_SCARDLISTBUF 1024

//--------------------------------------------------------------
//|
//|�˰� �ִ� ����Ʈī�� VID_PID
//|
//---------------------------------------------------------------
CString AQVIDPID_1 = _T("VID_0471&PID_040F");
CString AQGNERICVIDPID_1 = _T("VID_058F&PID_9520");
CString OMNIVIDPID_1 = _T("VID_08E6&PID_3437");
//--------------------------------------------------------------
//|
//|�˰� �ִ� ����Ʈī�� Display Name
//|
//---------------------------------------------------------------
CString AQSCARDDEVICENAME_1 = _T("USB Smart Card Reader (AnyQuitous Co.,Ltd.)");
CString AQSCARDDEVICENAME_2 = _T("NXP Semiconductors JCOP41V231");
CString OMNIKEY_1 = _T("OMNIKEY CardMan 5x21-CL");
CString GENERIC_1 = _T("GENERIC2KXP USB Smart Card Reader");
CString GENERIC_2 = _T("Generic Usb Smart Card Reader");
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CSCardTm::CSCardTm():
debug(NULL),
obj(NULL),
hContextHandle(NULL),
ErrorCode(_T(""))
{}
//--------------------------------------------------------------
//|
//|���������
//|
//---------------------------------------------------------------
CSCardTm::CSCardTm(CSCardTm &v):
debug(NULL),
obj(NULL),
hContextHandle(NULL)
{
	memcpy(this,&v,sizeof(CSCardTm));
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CSCardTm::~CSCardTm()
{
	SCardList.RemoveAll();
	ReleaseinitHandle();
}
//--------------------------------------------------------------
//|
//|�⺻ ��� Ŭ����
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|char�� hex�� ��ȯ(�Է¹��� ���̴�, apdu�ƴ�)
//|
//---------------------------------------------------------------
BOOL CSCardTm::Changcharbyhex(CString pass, CString& repass)
{
	CString temp;
	if(repass.GetLength())
		repass="";
	TCHAR *a = pass.GetBuffer(0);
	for( int i = 0; i < pass.GetLength(); i++ )
	{
		temp.Format(_T("%02X"), a[i]);
		repass += temp;
	}
	if(pass.GetLength()==(repass.GetLength()/2))
		return TRUE;
	return FALSE;
}
//--------------------------------------------------------------
//|
//|ī��� ���� ���� ����Ÿ�� ��ȯ �Ѵ�(0x00~0xFF���� �̹Ƿ� ��Ʈ�������� ��ȯ�ؼ� �Ǵ��Ѵ�)
//|
//---------------------------------------------------------------
BOOL CSCardTm::ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass)
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
//--------------------------------------------------------------
//|
//|��Ʈ���� BYTE������ ��ȯ �Ѱ��ش�
//|
//---------------------------------------------------------------
void CSCardTm::ChangStringtoChar(CString pass, char repass[])
{
	DWORD len=pass.GetLength(); 
	for( int i = 0; i < (int)len; i++ )
	{
		repass[i]=(char)pass.GetAt(i);
	}
}
//--------------------------------------------------------------
//|
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
//|
//---------------------------------------------------------------
int  CSCardTm::ChangeHexCord(CString data, BYTE hex[])
{
	BYTE buffer[MAXSCARD_HEXSTRING];
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
	memcpy(hex,buffer,MAXSCARD_HEXSTRING);
	return dwtransLen;
}
//--------------------------------------------------------------
//|
//|���� ����Ÿ�� �����ΰ��� �Ǵ��ϴ� �ڵ� �м��Լ�
//|
//---------------------------------------------------------------
//61XX ����Ÿ Ȯ�� APDU
CString  APDU_61XX = _T("00C00000");
BOOL CSCardTm::Analysis(SCARDHANDLE scard,DWORD protocol,CString cord, CString& recv, int nCount, BOOL brecv)
{
	ErrorCode=_T("");
	ErrorCode=cord.Right(4);
	if(ErrorCode.Compare(_T("9000"))==0)
	{
		if(brecv)
		{
			if(nCount)
				recv=cord.Left(nCount);
			else
				recv=cord.Left(cord.GetLength()-4);
		}
		else
		{
			recv="";
		}
		return TRUE;
	}
	else
	{
		CString data61=cord.Right(4);
		if(data61.Left(2)=="61")
		{
			CString  recvdata = "";
			CString apducord= APDU_61XX+data61.Right(2);
			BYTE sendbuffer[MAXSCARD_HEXSTRING];
			CString data = "";
			DWORD dwtransLen = ChangeHexCord(apducord,sendbuffer);
			BYTE Req[MAXSCARD_RECVDATA];
			DWORD dwReqLen=0;
			if(protocol == SCARD_PROTOCOL_T0)
			{
				dwReqLen=CommandT0(scard,sendbuffer,dwtransLen,Req,MAXSCARD_RECVDATA);
			}
			else if(protocol == SCARD_PROTOCOL_T1)
			{
				dwReqLen=CommandT1(scard,sendbuffer,dwtransLen,Req,MAXSCARD_RECVDATA);
			}
			if(dwReqLen)
			{
				ChangBytebyhex(Req,dwReqLen, data);
				ErrorCode=_T("");
				ErrorCode=data.Right(4);
				if(ErrorCode.Compare(_T("9000"))==0)
				{
					if(brecv)
					{
						if(nCount)
							recv=data.Left(nCount);
						else
							recv=data.Left(data.GetLength()-4);
					}
					else
					{
						recv="";
					}
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|����Ʈī�� ������ ContextHandle ���
//|
//---------------------------------------------------------------
bool CSCardTm::SCardinit()
{
	if(hContextHandle==NULL)
	{
		long  lResult=0 ;
		lResult = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hContextHandle);
		if( SCARD_S_SUCCESS != lResult )
		{
			MessageBox(NULL,_T("����Ʈī�� �ʱ�ȭ ����"), _T("����Ʈī��"),MB_OK);
			return false;
		}
	}
	ConnectSCardList("","");
	return true;
}
//--------------------------------------------------------------
//|
//|����Ʈī�� ������ ContextHandle �ݳ�
//|
//---------------------------------------------------------------
void CSCardTm::ReleaseinitHandle(void)
{
	if(hContextHandle)
	{
		SCardReleaseContext(hContextHandle);	hContextHandle=NULL;
	}
}
//--------------------------------------------------------------
//|
//|���ؼ� ����Ʈī�� ����Ʈ Ž��
//|
//---------------------------------------------------------------
size_t CSCardTm::ConnectSCardList(CString PidVid, CString Usbid)
{
	if(hContextHandle)
	{
		long  lResult=0 ;
		TCHAR bReaderListBuffer[MAXSCARD_SCARDLISTBUF];
		unsigned long dwReaderListLength ;
		dwReaderListLength = sizeof(bReaderListBuffer);
		memset((TCHAR*)bReaderListBuffer, 0x00, dwReaderListLength );	
		lResult = SCardListReaders(hContextHandle, NULL, (TCHAR*)bReaderListBuffer, &dwReaderListLength);
		if(lResult == SCARD_S_SUCCESS)
		{
			TCHAR buf[MAX_PATH];
			int count = 0;
			int len=0;
			while(count < (int)dwReaderListLength-1)
			{
				memset(buf,0,sizeof(buf));
				len=(int)lstrlen(((TCHAR*)bReaderListBuffer)+count)+1;
				memcpy(buf ,(TCHAR*)( bReaderListBuffer + count),len*sizeof(TCHAR));
				count = count + len;
				CString CardName;
				CardName=buf;
				if(SCardList.Lookup(CardName)==NULL)
				{
					SCARDINFOSTRUCT info;
					info.object=NULL;
					if(PidVid.GetLength())
					{
						info.USB_id = Usbid;
						info.VP_id = PidVid;
						SCardList[CardName]=info;
						break;
					}
					else
					{
						info.USB_id = _T("None");
						info.VP_id = GetDevicePidVid(CardName);
						SCardList[CardName]=info;
					}
				}
			}
			//����Ʈī���� ������ ��ȭ �� �� ȣ��
			SendSCardChangMsg();
			return SCardList.GetCount();
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|T0 ��ſ� �Լ�(BYTE ��)
//|
//---------------------------------------------------------------
int  CSCardTm::CommandT0(SCARDHANDLE scard,LPBYTE sendbuffer, unsigned long dwtransLen, BYTE recvbuffer[],int len)
{
	if(scard)
	{
		BYTE buffer[MAXSCARD_HEXSTRING];
		DWORD RecvLen = sizeof(buffer);
		long  lResult=0 ;
		memset(buffer,0x00,RecvLen);

		lResult = SCardTransmit(scard, SCARD_PCI_T0, sendbuffer, dwtransLen, NULL, (LPBYTE)buffer, &RecvLen);
		if( lResult == SCARD_S_SUCCESS )
		{
			memcpy(recvbuffer,buffer, len);
			return RecvLen;
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|T1 ��ſ� �Լ�(BYTE ���)
//|
//---------------------------------------------------------------

int  CSCardTm::CommandT1(SCARDHANDLE scard,LPBYTE sendbuffer, unsigned long dwtransLen, BYTE recvbuffer[],int len)
{
	if(scard)
	{
		BYTE buffer[MAXSCARD_HEXSTRING];
		DWORD RecvLen = sizeof(buffer);
		long  lResult=0 ;
		memset(buffer,0x00,RecvLen);

		lResult = SCardTransmit(scard, SCARD_PCI_T1, sendbuffer, dwtransLen, NULL, (LPBYTE)buffer, &RecvLen);
		if( lResult == SCARD_S_SUCCESS )
		{
			memcpy(recvbuffer,buffer, len);
			return RecvLen;
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|����̽��� ���صǾ��� ��
//|
//---------------------------------------------------------------
BOOL CSCardTm::AddSCardList(CString PidVid, CString Usbid)
{
	ConnectSCardList(PidVid, Usbid);
	return FALSE;
}
//--------------------------------------------------------------
//|
//|����̽��� ���� �Ǿ��� ��
//|
//---------------------------------------------------------------
BOOL CSCardTm::RemoveSCardList(CString PidVid, CString Usbid)
{
	POSITION pos = SCardList.GetStartPosition();
	POSITION posback = pos;
	while (pos)
	{
		ATLPAIRSCARD* piar=NULL;
		piar = SCardList.GetNext(pos);
		if(piar)
		{
			SCARDINFOSTRUCT info=piar->m_value;
			if(info.VP_id==PidVid&&info.USB_id==Usbid)
			{
				//����Ʈī�� ������ ������ ����� �˸��� ���� �Լ�
				SendSCardDisconnectMsg(posback);
				//����Ʈ���� ����
				SCardList.RemoveAtPos(posback);		break;
			}
		}
		posback = pos;
	}
	//������ ����� ã�� ������ ��(���α׷� ���� ���� �ε�� ����Ʈī�� ����)
	if(posback==NULL)
	{
		RemoveSCardList();
	}
	//����Ʈī���� ������ ��ȭ �� �� ȣ��
	SendSCardChangMsg();
	return FALSE;
}
//--------------------------------------------------------------
//|
//|����Ʈī�� ����̽� LOID ����(���α׷� ���� �� �̹� ������ ����Ʈī��)
//|
//---------------------------------------------------------------
CString CSCardTm::GetDevicePidVid(CString DeviceName)
{
	if(AQSCARDDEVICENAME_1.CompareNoCase(DeviceName.Left(AQSCARDDEVICENAME_1.GetLength()))==0 ||
		AQSCARDDEVICENAME_2.CompareNoCase(DeviceName.Left(AQSCARDDEVICENAME_2.GetLength()))==0)
		return AQVIDPID_1;
	else if(GENERIC_1.CompareNoCase(DeviceName.Left(GENERIC_1.GetLength()))==0 ||
		GENERIC_2.CompareNoCase(DeviceName.Left(GENERIC_2.GetLength()))==0)
		return AQGNERICVIDPID_1;
	else if(OMNIKEY_1.CompareNoCase(DeviceName.Left(OMNIKEY_1.GetLength()))==0)
		return OMNIVIDPID_1;
	return _T("None");
}
//--------------------------------------------------------------
//|
//|����̽��� ���� �Ǿ��� ��
//|
//---------------------------------------------------------------
size_t CSCardTm::GetSCardList(vecString& vecSCard)
{
	if(hContextHandle)
	{
		long  lResult=0 ;
		TCHAR bReaderListBuffer[MAXSCARD_SCARDLISTBUF];
		unsigned long dwReaderListLength ;
		dwReaderListLength = sizeof(bReaderListBuffer);
		memset((TCHAR*)bReaderListBuffer, 0x00, dwReaderListLength );	
		lResult = SCardListReaders(hContextHandle, NULL, (TCHAR*)bReaderListBuffer, &dwReaderListLength);
		if(lResult == SCARD_S_SUCCESS)
		{
			vecSCard.clear();
			TCHAR buf[MAX_PATH];
			int count = 0;
			int len=0;
			while(count < (int)dwReaderListLength-1)
			{
				memset(buf,0,sizeof(buf));
				len=(int)lstrlen(((TCHAR*)bReaderListBuffer)+count)+1;
				memcpy(buf ,(TCHAR*)( bReaderListBuffer + count),len*sizeof(TCHAR));
				count = count + len;
				vecSCard.push_back(buf);
			}
			return vecSCard.size();
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|����̽��� ���� �Ǿ��� ��
//|
//---------------------------------------------------------------
BOOL CSCardTm::RemoveSCardList()
{
	BOOL bDeleteOk=FALSE;
	vecString vecSCard;
	if(GetSCardList(vecSCard))
	{
		POSITION pos = SCardList.GetStartPosition();
		POSITION posback = pos;
		while (pos)
		{
			CString SCardName="";
			SCardName = SCardList.GetNextKey(pos);
			vecStringIt it = find(vecSCard.begin(), vecSCard.end(), SCardName);
			if(it==vecSCard.end())
			{
				//����Ʈī�� ������ ������ ����� �˸��� ���� �Լ�
				SendSCardDisconnectMsg(posback);
				//����Ʈ���� ����
				SCardList.RemoveAtPos(posback); bDeleteOk=TRUE; break;
			}
			posback = pos;
		}
	}
	else
	{
		//����Ʈī�� ������ ������ ����� �˸��� ���� �Լ�
		POSITION pos = SCardList.GetStartPosition();
		SendSCardDisconnectMsg(pos);
		//��� ��ü ����
		SCardList.RemoveAll();bDeleteOk=TRUE;
	}
	return bDeleteOk;
}
//--------------------------------------------------------------
//|
//|����̽� ���ؼ� ��ȭ�� ���� �� ���� �Լ�
//|
//---------------------------------------------------------------
void CSCardTm::SendSCardChangMsg()
{
	for (int i=0; i<(int)ObjectMsg.size(); i++)
	{
		ObjectMsg[i]->DeviceListChangeMsg(ISCARDMSG_NEW);
	}
}
//--------------------------------------------------------------
//|
//|����� ���� ����Ʈī�尡 Disconnect �Ǿ��� ��
//|
//---------------------------------------------------------------
void CSCardTm::SendSCardDisconnectMsg(POSITION pos)
{
	CString sckey;
	SCARDINFOSTRUCT scvalue;
	SCardList.GetAt(pos,sckey,scvalue);
	if(scvalue.object)
	{
		scvalue.object->DeviceListChangeMsg(ISCARDMSG_DISCONNECT, sckey);
	}
}
//--------------------------------------------------------------
//|
//|�������̽� �Լ�
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CSCardTm::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
	//========================================
	debug=obj->GetOutputObject();
	//========================================
	// ����Ʈī�� �ʱ�ȭ
	//========================================
	SCardinit();
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CSCardTm::Release()
{
	//========================================
	// ����Ʈī�� ContextHandle �ڵ� �ݳ�
	//========================================
	ReleaseinitHandle();
}
//--------------------------------------------------------------
//|
//|����Ʈī�� ����̽� CONNECTION ����
//|
//---------------------------------------------------------------
BOOL CSCardTm::DeviceConnect(LPARAM lParam, BOOL bConnect)
{
	PDEV_BROADCAST_DEVICEINTERFACE pp = (PDEV_BROADCAST_DEVICEINTERFACE)(DWORD_PTR)lParam;
	CString name=pp->dbcc_name;
	vecString Deviecinfo;
	_GASRING::GetTokenize(name.GetBuffer(name.GetLength()), Deviecinfo,"#");
	if(bConnect)
		AddSCardList(Deviecinfo[1].MakeUpper(),Deviecinfo[2].MakeUpper());
	else
		RemoveSCardList(Deviecinfo[1].MakeUpper(),Deviecinfo[2].MakeUpper());
	Deviecinfo.clear();
	return 0;
}
//--------------------------------------------------------------
//|
//|����Ʈī�� ����Ʈ ����� �ܺη� �ش�
//|
//---------------------------------------------------------------
//======================================================//
//		VIDPID �� ����Ʈī�� Name ������ �ش�
//======================================================//
BOOL CSCardTm::GetSCardListinfo(CString VidPid, vecString& vecSCardName)
{
	POSITION pos = SCardList.GetStartPosition();
	while (pos)
	{
		ATLPAIRSCARD* piar=NULL;
		piar = SCardList.GetNext(pos);
		if(piar)
		{
			CString sckey = piar->m_key;
			SCARDINFOSTRUCT scvalue = piar->m_value;
			if(VidPid.GetLength())
			{
				if(VidPid.CompareNoCase(scvalue.VP_id)==0)
				{
					vecSCardName.push_back(sckey.GetBuffer(sckey.GetLength()));
				}
			}
			else
			{
				vecSCardName.push_back(sckey.GetBuffer(sckey.GetLength()));
			}
		}
	}
	return vecSCardName.size()!=0;
}
//======================================================//
//		����Ʈī���� ������ �ѱ��
//======================================================//
BOOL CSCardTm::GetSCardListinfo(vecString& vecVidPid, vecString& vecUsbid, vecString& vecSCardName)
{
	POSITION pos = SCardList.GetStartPosition();
	while (pos)
	{
		ATLPAIRSCARD* piar=NULL;
		piar = SCardList.GetNext(pos);
		if(piar)
		{
			CString sckey = piar->m_key;
			SCARDINFOSTRUCT scvalue = piar->m_value;
			vecVidPid.push_back(scvalue.VP_id.GetBuffer(scvalue.VP_id.GetLength()));
			vecUsbid.push_back(scvalue.USB_id.GetBuffer(scvalue.USB_id.GetLength()));
			vecSCardName.push_back(sckey.GetBuffer(sckey.GetLength()));
		}
	}
	return vecSCardName.size()!=0;
}
//--------------------------------------------------------------
//|
//|�̸����� ����Ʈī�� �ڵ�� ���������� �ѱ��
//|
//---------------------------------------------------------------
BOOL CSCardTm::ScardConnect(SCARDHANDLE& hCard,DWORD& ActiveProtocol, CString SCardName, ISCardMsg* _connectobj)
{
	if(hContextHandle)
	{
		SCARDINFOSTRUCT scardinfo;
		ATLPAIRSCARD* piar=NULL;
		if(piar = SCardList.Lookup(SCardName))
		{
			long lResult;
			hCard=0;
			ActiveProtocol=0;
			lResult = SCardConnect(hContextHandle, SCardName, SCARD_SHARE_SHARED,SCARD_PROTOCOL_T0|SCARD_PROTOCOL_T1, &hCard, &ActiveProtocol);
			if(lResult == SCARD_S_SUCCESS)
			{
				if(hCard)
				{
					piar->m_value.object=_connectobj;
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|����� ���� ����Ʈī�� �ڵ��� �ݳ��Ѵ�
//|
//---------------------------------------------------------------
BOOL CSCardTm::ScardDisConnect(SCARDHANDLE hCard)
{
	if(hCard)
	{
		long lResult;
		lResult = SCardDisconnect(hCard, SCARD_RESET_CARD);	hCard=NULL;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|T0��ſ� �Լ�(������)
//|
//---------------------------------------------------------------
BOOL CSCardTm::CommandT0(SCARDHANDLE scard,CString apdu, CString& recv, int nCount, BOOL brecv)
{
	BYTE sendbuffer[MAXSCARD_HEXSTRING];
	CString data = "";
	DWORD dwtransLen = ChangeHexCord(apdu,sendbuffer);
	BYTE Req[MAXSCARD_RECVDATA];
	DWORD dwReqLen = CommandT0(scard,sendbuffer,dwtransLen,Req,MAXSCARD_RECVDATA);
	if(dwReqLen)
	{
		ChangBytebyhex(Req,dwReqLen, data);
		if(Analysis(scard, SCARD_PROTOCOL_T0,data,recv,nCount,brecv))
		{
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|T1��� �Լ�(������)
//|
//---------------------------------------------------------------

BOOL CSCardTm::CommandT1(SCARDHANDLE scard,CString apdu, CString& recv, int nCount, BOOL brecv)
{
	BYTE sendbuffer[MAXSCARD_HEXSTRING];
	CString data = "";
	DWORD dwtransLen = ChangeHexCord(apdu,sendbuffer);
	BYTE Req[MAXSCARD_RECVDATA];
	DWORD dwReqLen = CommandT1(scard,sendbuffer,dwtransLen,Req,MAXSCARD_RECVDATA);
	if(dwReqLen)
	{
		ChangBytebyhex(Req,dwReqLen, data);
		if(Analysis(scard, SCARD_PROTOCOL_T1,data,recv,nCount,brecv))
		{
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|���������� �޾Ƽ� T0,T1 ��� �Լ�(�������ݷ� �����Ͽ� ���)
//|
//---------------------------------------------------------------

BOOL CSCardTm::CommandAll(SCARDHANDLE scard,DWORD protocol,CString apdu, CString& recv, int nCount, BOOL brecv)
{
	BYTE sendbuffer[MAXSCARD_HEXSTRING];
	CString data = "";
	DWORD dwtransLen = ChangeHexCord(apdu,sendbuffer);
	BYTE Req[MAXSCARD_RECVDATA];
	DWORD dwReqLen=0;
	if(protocol == SCARD_PROTOCOL_T0)
	{
		dwReqLen=CommandT0(scard,sendbuffer,dwtransLen,Req,MAXSCARD_RECVDATA);
	}
	else if(protocol == SCARD_PROTOCOL_T1)
	{
		dwReqLen=CommandT1(scard,sendbuffer,dwtransLen,Req,MAXSCARD_RECVDATA);
	}
	if(dwReqLen)
	{
		ChangBytebyhex(Req,dwReqLen, data);
		if(Analysis(scard, protocol,data,recv,nCount,brecv))
		{
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|���� ������ ����� ���� �����ڵ带 �ѱ��
//|
//---------------------------------------------------------------
CString CSCardTm::GetLastErrorCode()
{
	return ErrorCode;
}
//--------------------------------------------------------------
//|
//|����̽� ��ȭ�� ���� ���� ����Լ�
//|
//---------------------------------------------------------------
void CSCardTm::SetSCardMsg(ISCardMsg* obj)
{
	ObjectMsg.push_back(obj);
}
