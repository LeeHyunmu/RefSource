#include "stdAfx.h"
#include "SCardTm.h"

//--------------------------------------------------------------
//|
//|SCrad 디바이스 사용하기 위한 클래스 라이브러리
//|
//---------------------------------------------------------------
#pragma comment(lib, "winscard.lib")
//--------------------------------------------------------------
//|
//|SCard 버퍼 크기 멕스
//|
//---------------------------------------------------------------
#define  MAXSCARD_HEXSTRING  256
#define  MAXSCARD_RECVDATA	255
#define  MAXSCARD_SCARDLISTBUF 1024

//--------------------------------------------------------------
//|
//|알고 있는 스마트카드 VID_PID
//|
//---------------------------------------------------------------
CString AQVIDPID_1 = _T("VID_0471&PID_040F");
CString AQGNERICVIDPID_1 = _T("VID_058F&PID_9520");
CString OMNIVIDPID_1 = _T("VID_08E6&PID_3437");
//--------------------------------------------------------------
//|
//|알고 있는 스마트카드 Display Name
//|
//---------------------------------------------------------------
CString AQSCARDDEVICENAME_1 = _T("USB Smart Card Reader (AnyQuitous Co.,Ltd.)");
CString AQSCARDDEVICENAME_2 = _T("NXP Semiconductors JCOP41V231");
CString OMNIKEY_1 = _T("OMNIKEY CardMan 5x21-CL");
CString GENERIC_1 = _T("GENERIC2KXP USB Smart Card Reader");
CString GENERIC_2 = _T("Generic Usb Smart Card Reader");
//--------------------------------------------------------------
//|
//|생성자
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
//|복사생성자
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
//|소멸자
//|
//---------------------------------------------------------------
CSCardTm::~CSCardTm()
{
	SCardList.RemoveAll();
	ReleaseinitHandle();
}
//--------------------------------------------------------------
//|
//|기본 사용 클래스
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|char를 hex로 변환(입력받은 값이다, apdu아님)
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
//|카드로 부터 받은 데이타를 변환 한다(0x00~0xFF까지 이므로 스트링형으로 변환해서 판단한다)
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
//|스트링을 BYTE형으로 변환 넘겨준다
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
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
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
//|받은 데이타가 정상인가를 판단하는 코드 분석함수
//|
//---------------------------------------------------------------
//61XX 데이타 확인 APDU
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
//|스마트카드 리더를 ContextHandle 얻기
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
			MessageBox(NULL,_T("스마트카드 초기화 실패"), _T("스마트카드"),MB_OK);
			return false;
		}
	}
	ConnectSCardList("","");
	return true;
}
//--------------------------------------------------------------
//|
//|스마트카드 리더를 ContextHandle 반납
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
//|컨넥션 스마트카드 리스트 탐색
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
			//스마트카드의 갯수가 변화 될 때 호출
			SendSCardChangMsg();
			return SCardList.GetCount();
		}
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|T0 통신용 함수(BYTE 용)
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
//|T1 통신용 함수(BYTE 통신)
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
//|디바이스가 컨넥되었을 때
//|
//---------------------------------------------------------------
BOOL CSCardTm::AddSCardList(CString PidVid, CString Usbid)
{
	ConnectSCardList(PidVid, Usbid);
	return FALSE;
}
//--------------------------------------------------------------
//|
//|디바이스가 제거 되었을 때
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
				//스마트카드 접속이 끊어진 사실을 알리기 위한 함수
				SendSCardDisconnectMsg(posback);
				//리스트에서 삭제
				SCardList.RemoveAtPos(posback);		break;
			}
		}
		posback = pos;
	}
	//삭제할 목록을 찾지 못했을 때(프로그램 시작 전에 로드된 스마트카드 삭제)
	if(posback==NULL)
	{
		RemoveSCardList();
	}
	//스마트카드의 갯수가 변화 될 때 호출
	SendSCardChangMsg();
	return FALSE;
}
//--------------------------------------------------------------
//|
//|스마트카드 디바이스 LOID 정보(프로그램 시작 전 이미 장착된 스마트카드)
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
//|디바이스가 제거 되었을 때
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
//|디바이스가 제거 되었을 때
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
				//스마트카드 접속이 끊어진 사실을 알리기 위한 함수
				SendSCardDisconnectMsg(posback);
				//리스트에서 삭제
				SCardList.RemoveAtPos(posback); bDeleteOk=TRUE; break;
			}
			posback = pos;
		}
	}
	else
	{
		//스마트카드 접속이 끊어진 사실을 알리기 위한 함수
		POSITION pos = SCardList.GetStartPosition();
		SendSCardDisconnectMsg(pos);
		//목록 전체 삭제
		SCardList.RemoveAll();bDeleteOk=TRUE;
	}
	return bDeleteOk;
}
//--------------------------------------------------------------
//|
//|디바이스 컨텍션 변화가 있을 때 통지 함수
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
//|통신을 위해 스마트카드가 Disconnect 되었을 때
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
//|인터페이스 함수
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CSCardTm::initInterfaceObject(IMyObject* _obj)
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
	// 스마트카드 초기화
	//========================================
	SCardinit();
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CSCardTm::Release()
{
	//========================================
	// 스마트카드 ContextHandle 핸들 반납
	//========================================
	ReleaseinitHandle();
}
//--------------------------------------------------------------
//|
//|스마트카드 디바이스 CONNECTION 정보
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
//|스마트카드 리스트 목록을 외부로 준다
//|
//---------------------------------------------------------------
//======================================================//
//		VIDPID 로 스마트카드 Name 정보를 준다
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
//		스마트카드의 정보를 넘긴다
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
//|이름으로 스마트카드 핸들과 프로토콜을 넘긴다
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
//|사용이 끝난 스마트카드 핸들을 반납한다
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
//|T0통신용 함수(사용안함)
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
//|T1통신 함수(사용안함)
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
//|프로토콜을 받아서 T0,T1 통신 함수(프로토콜로 구분하여 통신)
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
//|제일 마지막 통신한 에러 검출코드를 넘긴다
//|
//---------------------------------------------------------------
CString CSCardTm::GetLastErrorCode()
{
	return ErrorCode;
}
//--------------------------------------------------------------
//|
//|디바이스 변화에 따른 통지 등록함수
//|
//---------------------------------------------------------------
void CSCardTm::SetSCardMsg(ISCardMsg* obj)
{
	ObjectMsg.push_back(obj);
}
