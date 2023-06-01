#include "stdAfx.h"
#include "MainGdiDlg.h"

// 출력 텍스트 길이 제한
#define  SCARDDATAFILE_MAXLEN 74
//====================================================
// 스트링 정의
//=====================================================
#define MAINGDISTRING_1 _T("================================================================")
#define MAINGDISTRING_2 _T("GLOBAL_SCARD")

//--------------------------------------------------------------
//|
//|기본 APDU 정의
//|
//---------------------------------------------------------------
#define DEFAULTPORT _T("9000")
#define DEFAULTIP _T("127.0.0.1")
#define DEFAULTMONEY _T("20000")
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CMainGdiDlg::CMainGdiDlg():
debug(NULL),
obj(NULL),
scardtm(NULL),
hWnd(NULL),
bSCardConnect(FALSE),
hCard(0),
ActiveProtocol(0),
ConnectSCardName(_T("None"))
{}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::Release()
{
	comvpid.Detach();
	listscardname.Detach();
	listUsbid.Detach();
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
void CMainGdiDlg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// 오브젝트 관리 생성 관리 클래스
	//========================================
	obj=_obj;
	//========================================
	// 디버그 출력 클래스
	//========================================
	debug=obj->GetOutputObject();
	//=============================================
	// scardtm
	//=============================================
	scardtm = obj->GetSCardTmObject();
	//========================================
	// 스마트카드 변화 통지 등록
	//========================================
	scardtm->SetSCardMsg(this);
}
//--------------------------------------------------------------
//|
//|dlg 초기화 
//|
//---------------------------------------------------------------
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND 저장
	//========================================
	hWnd = _hWnd;
	//========================================
	// 윈도우 이름
	//========================================
	SetWindowText(hWnd, MAINGDISTRING_2);
	//========================================
	// 컨트롤
	//========================================
	comvpid.Attach(GetDlgItem(hWnd, IDC_VIDPID));
	comvpid.AddString(_T("전체"));
	comvpid.SetCurSel(0);
	listscardname.Attach(GetDlgItem(hWnd, IDC_SCARDNAME));
	listUsbid.Attach(GetDlgItem(hWnd, IDC_SCARDUSBID));
	//컨넥션 카드
	staticconnect.Attach(GetDlgItem(hWnd, IDC_CONNECTCARD));
	//APDU 값
	editapdu.Attach(GetDlgItem(hWnd, IDC_APDUSEND));
	//스마트카드 접속 버튼
	butconnect.Attach(GetDlgItem(hWnd, IDC_CONNECTION));
	//스마트카드 접속을 끊는다
	butdisconnect.Attach(GetDlgItem(hWnd, IDC_DISCONNECTION));
	//APDU 전송
	butapud.Attach(GetDlgItem(hWnd, IDC_SEND));
	//APDU RECV 데이터 뷰어
	listapdurecv.Attach(GetDlgItem(hWnd, IDC_RECVDATA));
	//베이스 APDU 콤보박스
	combaseapdu.Attach(GetDlgItem(hWnd, IDC_APDUSHORT));
	LoadBaseApduName();

	//버튼 활성화 비활성화 설정
	butconnect.EnableWindow(TRUE);
	butdisconnect.EnableWindow(FALSE);
	butapud.EnableWindow(FALSE);

	//서버 컨트롤 변수
	Serverip.Attach(GetDlgItem(hWnd, IDC_SERVERIPNUM));
	Serverport.Attach(GetDlgItem(hWnd, IDC_SERVERPORTNUM));
	Serverconnect.Attach(GetDlgItem(hWnd, IDC_SERVERCONNECTION));
	listserverview.Attach(GetDlgItem(hWnd, IDC_SERVERCOMMECTDATA));
	Servertimeout.Attach(GetDlgItem(hWnd, IDC_SERVERTIMEOUT));

	//충전 컨트롤
	chargemoney.Attach(GetDlgItem(hWnd, IDC_CHARGEMONEY));
	chargebut.Attach(GetDlgItem(hWnd, IDC_CLCHARGE));
	//충전버튼 비활성화
	chargebut.EnableWindow(FALSE);

	Serverip.SetWindowText(DEFAULTIP);
	Serverport.SetWindowText(DEFAULTPORT);
	chargemoney.SetWindowText(DEFAULTMONEY);
	CHAR timeout[11];
	memset(timeout,0x00,sizeof(timeout));
	sprintf_s(timeout,_T("%u"),INFINITE);
	Servertimeout.SetWindowText(timeout);
	//========================================
	// 폰트 변경
	//========================================
	fontinfo.lfHeight   = 16;
	fontinfo.lfWeight   = 0;
	fontinfo.lfEscapement  = 0;
	fontinfo.lfOrientation  = 0;
	fontinfo.lfWeight   = FW_BOLD;
	fontinfo.lfItalic   = FALSE;
	fontinfo.lfUnderline  = FALSE;
	fontinfo.lfStrikeOut  = FALSE;
	fontinfo.lfCharSet   = DEFAULT_CHARSET;
	fontinfo.lfOutPrecision = OUT_DEFAULT_PRECIS;
	fontinfo.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	fontinfo.lfQuality   = DEFAULT_QUALITY;
	fontinfo.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName),_T("굴림체"));
	fontcolor=RGB(255,0,0);
}
//--------------------------------------------------------------
//|
//|Dlg 그리기 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
}
//--------------------------------------------------------------
//|
//|콤보박스 IDC_VIDPID 메시지
//|
//---------------------------------------------------------------
void CMainGdiDlg::ComboboxVidPidMessage(UINT uNotifyCode, int nID)
{
	int intsel;
	CString strvpid;
	intsel = comvpid.GetCurSel();
	comvpid.GetLBText(intsel,strvpid);
	listscardname.ResetContent();
	listUsbid.ResetContent();
	for (int i=0;i<(int)vecVidPid.size(); i++)
	{
		if(strvpid.Compare(_T("전체"))==0)
		{
			listscardname.AddString(vecSCardName[i]);
			listUsbid.AddString(vecUsbid[i]);
		}
		else if(vecVidPid[i].Compare(strvpid)==0)
		{
			listscardname.AddString(vecSCardName[i]);
			listUsbid.AddString(vecUsbid[i]);
		}
	}
	//컨넥션 스마트카드 이름 초기화
	if(bSCardConnect==FALSE)
	{
		staticconnect.SetWindowText(_T("None"));
		ConnectSCardName = _T("None");
	}
}
//--------------------------------------------------------------
//|
//|콤보박스 IDC_APDUSHORT 메시지
//|
//---------------------------------------------------------------
void CMainGdiDlg::ComboboxApduShortMessage(UINT uNotifyCode, int nID)
{
	FindBaseApdu();
}
//--------------------------------------------------------------
//|
//|List 박스 IDC_SCARDNAME 메시지
//|
//---------------------------------------------------------------
void CMainGdiDlg::ListboxSCardNameMessage(UINT uNotifyCode, int nID)
{
	//컨넥션 스마트카드 이름 초기화
	if(bSCardConnect==FALSE)
	{
		int intsel;
		CString strscardname;
		intsel = listscardname.GetCurSel();
		listscardname.GetText(intsel,strscardname);
		staticconnect.SetWindowText(strscardname);
		ConnectSCardName = strscardname;
	}
}
//--------------------------------------------------------------
//|
//| 버튼 클릭 이벤트 메시지
//|
//---------------------------------------------------------------
void CMainGdiDlg::ButtonClickedMessage(UINT uNotifyCode, int nID)
{
	switch(nID)
	{
	case IDC_CONNECTION:
	case IDC_DISCONNECTION:
	case IDC_SEND:
		SCardButtonMessage(uNotifyCode, nID);
		break;
	case IDC_SERVERCONNECTION:
	case IDC_CLCHARGE:
		ClientButtonMessage(uNotifyCode, nID);
		break;
	}
}
//--------------------------------------------------------------
//|
//|Static Control CtlColor 변경
//|
//---------------------------------------------------------------
HBRUSH CMainGdiDlg::StaticCtlColorMessage(CDCHandle dc, CStatic wndStatic)
{
	if(wndStatic == GetDlgItem(hWnd, IDC_CONNECTCARD))
	{
		CBrush brush;
		CFont font;
		brush.CreateSolidBrush(RGB(0,0,255));
		font.CreateFontIndirect(&fontinfo);
		int mode = dc.SetBkMode(TRANSPARENT);
		dc.SelectFont(font);
		dc.SetTextColor(RGB(0,0,255));
		dc.SetBkColor(RGB(255,0,255));
		dc.SetBkMode(mode);
		return brush;
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|스마트카드 변화 통지 함수
//|
//---------------------------------------------------------------
//상속 받은 클래스에서 스마트카드 장치의 변화를 알고 싶을 때
//flag값에 따라서 처리 기능이 다르게 쓴다
void CMainGdiDlg::DeviceListChangeMsg(int flag, CString SCardName)
{
	switch (flag)
	{
	case ISCARDMSG_NEW: NewSCardListMsg();	break;
	case ISCARDMSG_DISCONNECT: SCardDiConnectMsg(SCardName); break;
	}
	//컨넥션 스마트카드 이름 초기화
	if(bSCardConnect==FALSE)
	{
		staticconnect.SetWindowText(_T("None"));
		ConnectSCardName = _T("None");
	}
}
//--------------------------------------------------------------
//|
//|일반함수
//|
//---------------------------------------------------------------
//=======================================================
//스마트카드 갯수의 변화가 발생했다
//===========================================================
void CMainGdiDlg::NewSCardListMsg()
{
	if(scardtm)
	{
		vecVidPid.clear();
		vecUsbid.clear();
		vecSCardName.clear();
		comvpid.ResetContent();
		listscardname.ResetContent();
		listUsbid.ResetContent();
		comvpid.AddString(_T("전체"));
		comvpid.SetCurSel(0);
		scardtm->GetSCardListinfo(vecVidPid,vecUsbid,vecSCardName);
		for (int i=0;i<(int)vecSCardName.size(); i++)
		{
			int comboindex = comvpid.FindStringExact(-1,vecVidPid[i]);
			if(comboindex==-1)
			{
				comvpid.AddString(vecVidPid[i]);
			}
			listscardname.AddString(vecSCardName[i]);
			listUsbid.AddString(vecUsbid[i]);
		}
	}
}
//=======================================================
//접속하고 있는 스마트카드가 빠졌다
//===========================================================
void CMainGdiDlg::SCardDiConnectMsg(CString SCardName)
{
	if(bSCardConnect)
	{
		bSCardConnect=scardtm->ScardDisConnect(hCard);
		butconnect.EnableWindow(TRUE);
		butdisconnect.EnableWindow(FALSE);
		butapud.EnableWindow(FALSE);
		Write(_T("%s 이름의 스마트카드를 사용자가 뺐다"),SCardName);
		ViewSmartCardConnectLastError();
	}
}
//--------------------------------------------------------------
//|
//|일반함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::ViewSmartCardConnectLastError()
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM|
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		0,
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
		);
	if(debug)
	{
		debug->Write((LPTSTR)lpMsgBuf);
	}
	LocalFree(lpMsgBuf);
}
//--------------------------------------------------------------
//|
//|텍스트 출력(스마트카드 데이터 출력)
//|
//---------------------------------------------------------------
void CMainGdiDlg::Write(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//============================================
	// 가변인자로부터 문자열 얻기
	//=============================================
	va_list args;
	TCHAR buffer[1024]={0,};
	va_start(args, format);
	//int len = vswprintf(NULL, format, args) + 1; 
	//buffer = new TCHAR[len];
	_vstprintf_s(buffer, 1024, format, args);
	va_end(args);
	//============================================
	// 출력
	//=============================================
	DWORD textlen = _tcslen(buffer);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, textlen, NULL, NULL);
	WCHAR _unText[MAX_PATH]={0,};
	MultiByteToWideChar(CP_ACP, 0, buffer, textlen, _unText, nLen);
	DWORD textpoint = 0;
	while(nLen != textpoint)
	{
		WCHAR WriteData[SCARDDATAFILE_MAXLEN+1];
		memset(WriteData, 0, sizeof(WriteData));
		int offset = nLen-textpoint;
		if(offset > SCARDDATAFILE_MAXLEN)
		{
			//memcpy(WriteData,buffer+textpoint,sizeof(TCHAR)*SCARDDATAFILE_MAXLEN); textpoint+=SCARDDATAFILE_MAXLEN;
			memcpy(WriteData,_unText+textpoint,SCARDDATAFILE_MAXLEN*sizeof(WCHAR)); textpoint+=SCARDDATAFILE_MAXLEN;
		}
		else
		{
			//memcpy(WriteData,buffer+textpoint,sizeof(TCHAR)*offset); textpoint+=offset;
			memcpy(WriteData,_unText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
		}
		CString strData = WriteData;
		//출력
		listapdurecv.AddString(strData);
		int selcount = listapdurecv.GetCount();
		listapdurecv.SetCurSel(selcount-1);
	}
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//|텍스트 출력(서버 데이터 출력)
//|
//---------------------------------------------------------------
void CMainGdiDlg::ServerWrite(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//============================================
	// 가변인자로부터 문자열 얻기
	//=============================================
	va_list args;
	TCHAR buffer[1024]={0,};
	va_start(args, format);
	//int len = vswprintf(NULL, format, args) + 1; 
	//buffer = new TCHAR[len];
	_vstprintf_s(buffer, 1024, format, args);
	va_end(args);
	//============================================
	// 출력
	//=============================================
	DWORD textlen = _tcslen(buffer);
	int nLen = MultiByteToWideChar(CP_ACP, 0, buffer, textlen, NULL, NULL);
	WCHAR _unText[MAX_PATH]={0,};
	MultiByteToWideChar(CP_ACP, 0, buffer, textlen, _unText, nLen);
	DWORD textpoint = 0;
	while(nLen != textpoint)
	{
		WCHAR WriteData[SCARDDATAFILE_MAXLEN+1];
		memset(WriteData, 0, sizeof(WriteData));
		int offset = nLen-textpoint;
		if(offset > SCARDDATAFILE_MAXLEN)
		{
			//memcpy(WriteData,buffer+textpoint,sizeof(TCHAR)*SCARDDATAFILE_MAXLEN); textpoint+=SCARDDATAFILE_MAXLEN;
			memcpy(WriteData,_unText+textpoint,SCARDDATAFILE_MAXLEN*sizeof(WCHAR)); textpoint+=SCARDDATAFILE_MAXLEN;
		}
		else
		{
			//memcpy(WriteData,buffer+textpoint,sizeof(TCHAR)*offset); textpoint+=offset;
			memcpy(WriteData,_unText+textpoint,offset*sizeof(WCHAR)); textpoint+=offset;
		}
		CString strData = WriteData;
		//출력
		listserverview.AddString(strData);
		int selcount = listserverview.GetCount();
		listserverview.SetCurSel(selcount-1);
	}
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//|기본 APDU 네임 리턴
//|
//---------------------------------------------------------------
void CMainGdiDlg::LoadBaseApduName()
{
	vecDefalutAPDU.clear();
	vecString vecFile;
	if(GAFileStream::CreateLineReadFile(_T("BaseApdu.txt"),vecFile))
	{
		for (int i=0; i<(int)vecFile.size(); i++)
		{
			vecString str;
			_GASRING::GetTokenize(vecFile[i].GetBuffer(vecFile[i].GetLength()),str);
			combaseapdu.AddString(vecFile[0]);
			//vecDefalutAPDU.push_back(str[1]);
		}
		combaseapdu.SetCurSel(1);
		FindBaseApdu();
	}
}
//--------------------------------------------------------------
//|
//|단축이름으로 기본 APDU 넘긴다
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::FindBaseApdu()
{
	int intsel;
	intsel = combaseapdu.GetCurSel();
	if((int)vecDefalutAPDU.size()>intsel)
	{
		editapdu.SetWindowText(vecDefalutAPDU[intsel]);
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|스마트카드 버튼 사용 함수
//|
//---------------------------------------------------------------
void CMainGdiDlg::SCardButtonMessage(UINT uNotifyCode, int nID)
{
	Write(MAINGDISTRING_1);
	switch(nID)
	{
	case IDC_CONNECTION:
		{
			if(bSCardConnect==FALSE)
			{
				if(ConnectSCardName.Compare(_T("None"))!=0)
				{
					bSCardConnect=scardtm->ScardConnect(hCard, ActiveProtocol,ConnectSCardName,this);
					if(bSCardConnect)
					{
						//=================================================
						//버튼을 활성화 세팅
						//===================================================
						butconnect.EnableWindow(FALSE);
						butdisconnect.EnableWindow(TRUE);
						butapud.EnableWindow(TRUE);
					}
				}
			}
			if(bSCardConnect)
			{
				Write(_T("스마트카드 접속 성공=>카드이름:%s"),ConnectSCardName);
			}
			else
			{
				Write(_T("스마트카드 접속 실패=>카드이름:%s"),ConnectSCardName);
			}
		}break;
	case IDC_DISCONNECTION:
		{
			if(bSCardConnect)
			{
				scardtm->ScardDisConnect(hCard);
				//=================================================
				//버튼을 활성화 세팅을 다시 한다
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//충전버튼  비 활성화
				chargebut.EnableWindow(FALSE);
			}
			Write(_T("%s의스마트카드 접속을 종료합니다"),ConnectSCardName);
		}break;
	case IDC_SEND:
		{
			CString SendApdu;
			editapdu.GetWindowText(SendApdu);
			CString RecvData="";
			BOOL b = scardtm->CommandAll(hCard, ActiveProtocol, SendApdu, RecvData, 0,TRUE);
			if(b)
			{
				Write(_T("성공"));
				if(RecvData.GetLength())
				{
					Write(_T("받은데이터:%s"),RecvData);
				}
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					Write(_T("에러검출코드:%s"),ErrorCode);
				}
				//충전버튼 활성화
				chargebut.EnableWindow(TRUE);
			}
			else
			{
				Write(_T("실패"));
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					Write(_T("에러검출코드:%s"),ErrorCode);
				}
				//=================================================
				//버튼을 활성화 세팅을 다시 한다
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//충전버튼  비 활성화
				chargebut.EnableWindow(FALSE);
			}
		}break;
	}
	Write(MAINGDISTRING_1);
}
//--------------------------------------------------------------
//|
//|스마트카드 버튼 사용 함수
//|
//---------------------------------------------------------------
//USERCODE(01: 성인, 02: 청소년, 03: 학생, 04:14세미만 어린이)
#define USERCODE_1 _T("01")
#define USERCODE_2 _T("02")
#define USERCODE_3 _T("03")
#define USERCODE_4 _T("04")
//충전 
//init
#define CHARGEAPDU_INIT _T("9040000004")
//Load
#define CHARGEAPDU_LOAD _T("9042000010")
//지불 
//init
#define PAYMONEYAPDU_INIT _T("9040030004")
//Load
#define PAYMONEYAPDU_PURCHASE _T("9046000012")
void CMainGdiDlg::ClientButtonMessage(UINT uNotifyCode, int nID)
{
	ServerWrite(MAINGDISTRING_1);
	debug->Write(_T("============출력 시작 ==============="));
	switch(nID)
	{
	case IDC_SERVERCONNECTION:
		{
			IClient* client = obj->GetClientObject();
			CString ip;
			CString porttemp;
			u_short port;
			Serverip.GetWindowText(ip);
			Serverport.GetWindowText(porttemp);
			port=(u_short)atoi(porttemp.GetBuffer(porttemp.GetLength()));
			if(client->ServerConnect(port, ip)==FALSE)
			{
				ServerWrite(_T("서버 접속 실패"));
			}
			else
			{
				ServerWrite(_T("서버 접속 성공"));
				debug->Write(_T("서버 접속 성공"));
				Serverconnect.EnableWindow(FALSE);
			}
		}break;
	case IDC_CLCHARGE:
		{
			debug->Write(_T("============충전 시작============="));
			//==============================================
			//금액 + 카드 init
			//==============================================
			//충전init 데이타를 쉽게 사용하기 위한 구조체
			ChargeinitCardDataRecv chrc;
			//==============================================
			//입력 금액 변환
			CString strmoney;
			chargemoney.GetWindowText(strmoney);
			CString MoneyHex;
			MoneyHex.Format("%08X",atoi(strmoney));
			//============================================================================================
			CString RecvData="";
			//카드 데이터 가져오기
 			BOOL b = scardtm->CommandAll(hCard, ActiveProtocol, CHARGEAPDU_INIT+MoneyHex, RecvData, 0,TRUE);
			//===============================================================================================
			//에러 검출 코드
 			if(b)
			{
 				ServerWrite(_T("성공"));
				if(RecvData.GetLength())
				{
					ServerWrite(_T("받은데이터:%s"),RecvData);
					//=================================================
					//카드에서 받은 데이타를 구조체에 복사한다
					//===================================================
					memcpy(&chrc,RecvData.GetBuffer(RecvData.GetLength()),sizeof(ChargeinitCardDataRecv));
					chrc.end=0x00;
				}
				//=================================================
				//에러검출코드 가져오는 함수 호출하여 값을 가져온다
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("에러검출코드:%s"),ErrorCode);
				}
			}
 			else
 			{
 				ServerWrite(_T("실패"));
				//=================================================
				//에러검출코드 가져오는 함수 호출하여 값을 가져온다
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("에러검출코드:%s"),ErrorCode);
				}
				//=================================================
				//버튼을 활성화 세팅을 다시 한다
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//충전버튼  비 활성화
				chargebut.EnableWindow(FALSE);
				break;
			}
			//=================================================
			//서버 보내기(init 데이타)
			//===================================================
			//서버로 보낼 데이터를 구조체를 이용하여 값을 세팅한다
			ChargeInitRequest clientinitRq;
			int totalinitRqsize = sizeof(clientinitRq);   //구조체 사이즈 구한다
			memset(&clientinitRq, 0x30, totalinitRqsize); //값을 숫자 0(아스키코드값 0x30)으로 초기화
			clientinitRq.end=0x00; //end를 문자열 NULL(아스키코드 0x00)으로만든다
			//==============================================================
			//거래전 잔액 변환(10진수 스트링으로 변환) , 카드 데이타 8바이트를 10바이트로 만든다
			CHAR befatm16[9];
			memset(befatm16,0x00,sizeof(befatm16));
			memcpy(befatm16, chrc.BEF_AMT, sizeof(chrc.BEF_AMT)); //chrc.BEF_AMT 구조체 크기 만큼 복사 , 8자리
			CHAR befatm10[11];
			memset(befatm10,0x00,sizeof(befatm10));
			sprintf_s(befatm10,_T("%010u"),strtoul(befatm16, NULL, 16)); //16진수 스트링을 10진수 unsinge int 스트링으로 , 10자리
			memcpy(clientinitRq.BEF_AMT,befatm10,sizeof(clientinitRq.BEF_AMT));
			//===============================================================
			//사용 알고리즘
			memcpy(clientinitRq.ALGORI_ID,chrc.ALGORI_ID,sizeof(clientinitRq.ALGORI_ID));
			//전자화폐사 ID
			memcpy(clientinitRq.KSET_VER,chrc.KSET_VER,sizeof(clientinitRq.KSET_VER));
			//전자화폐 ID
			memcpy(clientinitRq.CARD_ID,chrc.CARD_ID,sizeof(clientinitRq.CARD_ID));
			//==============================================================
			//전자화폐 일련번호 변환(10진수 스트링으로 변환)
			CHAR cardtrseq16[9];
			memset(cardtrseq16,0x00,sizeof(cardtrseq16));
			memcpy(cardtrseq16, chrc.CARD_TR_SEQ, sizeof(chrc.CARD_TR_SEQ)); //chrc.CARD_TR_SEQ 구조체 크기 만큼 복사 , 8자리
			CHAR cardtrseq10[11];
			memset(cardtrseq10,0x00,sizeof(cardtrseq10));
			sprintf_s(cardtrseq10,_T("%010u"),strtoul(cardtrseq16, NULL, 16)); //16진수 스트링을 10진수 unsinge int 스트링으로, 10자리
			memcpy(clientinitRq.CARD_TR_SEQ,cardtrseq10,sizeof(clientinitRq.CARD_TR_SEQ));
			//===============================================================
			//전자화폐 생성 난수
			memcpy(clientinitRq.RANDM_NUM,chrc.RANDM_NUM,sizeof(clientinitRq.RANDM_NUM));
			//전저화폐 서명
			memcpy(clientinitRq.SIGN1,chrc.SIGN1,sizeof(clientinitRq.SIGN1));
			//카드 소지자 구분
			memcpy(clientinitRq.CARD_CLASS_CD,USERCODE_1,sizeof(clientinitRq.CARD_CLASS_CD));
			//=============================================================
			// 보낸 데이타 출력(ListBox에 출력)
			//==========================================================
			//CString 형으로 만들어서 출력한다(CAtlString)
			CString view_write=(CHAR*)&clientinitRq;
			ServerWrite(MAINGDISTRING_1);
			ServerWrite(_T("충전 INIT 보낼 데이타 출력"));
			ServerWrite(_T("거래전카드잔액:%s"),view_write.Mid(56,10));
			ServerWrite(_T("알고리즘:%s"),view_write.Mid(76,2));
			ServerWrite(_T("전자화폐사ID:%s"),view_write.Mid(78,2));
			ServerWrite(_T("전자화폐ID:%s"),view_write.Mid(80,16));
			ServerWrite(_T("전자화폐일련번호:%s"),view_write.Mid(96,10));
			ServerWrite(_T("전자화폐난수:%s"),view_write.Mid(106,16));
			ServerWrite(_T("전자화폐서명:%s"),view_write.Mid(122,8));
			ServerWrite(_T("카드소지자구분:%s"),view_write.Mid(130,2));
			//=====================================================================
			//보낼 데이타 디버그 출력(모든 데이타 출력)
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("충전 INIT 보낼 데이타 출력"));
				debug->Write(_T("전체데이타크기:%d"), view_write.GetLength());
				debug->Write(_T("1,전화번호:%s"),view_write.Mid(0,12));
				debug->Write(_T("2,거래요청일시:%s"),view_write.Mid(12,14));
				debug->Write(_T("3,요청금액:%s"),view_write.Mid(26,10));
				debug->Write(_T("4,수수료금액:%s"),view_write.Mid(36,10));
				debug->Write(_T("5,PG결제금액:%s"),view_write.Mid(46,10));
				debug->Write(_T("6,거래전카드잔액:%s"),view_write.Mid(56,10));
				debug->Write(_T("7,거래후카드잔액:%s"),view_write.Mid(66,10));
				debug->Write(_T("8,사용알고리즘:%s"),view_write.Mid(76,2));
				debug->Write(_T("9,전자화폐사ID:%s"),view_write.Mid(78,2));
				debug->Write(_T("10,전자화폐ID:%s"),view_write.Mid(80,16));
				debug->Write(_T("11,전자화폐거래일련번호:%s"),view_write.Mid(96,10));
				debug->Write(_T("12,전자화폐가생성한난수:%s"),view_write.Mid(106,16));
				debug->Write(_T("13,전자화폐가생성한서명:%s"),view_write.Mid(122,8));
				debug->Write(_T("14,카드소지자구분:%s"),view_write.Mid(130,2));
				debug->Write(_T("15,결제사코드:%s"),view_write.Mid(132,10));
				debug->Write(_T("16,결제사거래번호:%s"),view_write.Mid(142,30));
				debug->Write(_T("17,결제수단:%s"),view_write.Mid(172,2));
				debug->Write(_T("18,Padding:%s"),view_write.Mid(174,10));
			}
			//=============================================================
			//클라이언트 객체
			IClient* client = obj->GetClientObject();
			//=================================================================
			//클라이언트가 서버에 접속되어 있는가를 확인한다
			if(client->ActiveClient()==FALSE)
			{
				ServerWrite(_T("서버와 연결이 끊어졌거나 연결하지 않음"));
				break;
			}
			//==================================================================
			//타임아웃 읽기
			DWORD timeout;
			CString timestr;
			Servertimeout.GetWindowText(timestr);
			timeout= strtoul(timestr.GetBuffer(timestr.GetLength()), NULL, 10);
			//==============================================================
			//INIT 서버로 데이터를 보냄
			client->SendDatafn(sizeof(clientinitRq)-1, &clientinitRq, timeout); //사이즈에서 -1은 end(CHAR)은 전송에서 제외
			//==============================================================
			//INIT 서버로부터 데이터를 받음
			ChargeInitResponse clientinitRp;
			client->RecvDatafn(sizeof(clientinitRp)-1, &clientinitRp,timeout); //사이즈에서 -1은 end(CHAR)은 수신에서 제외
			clientinitRp.end=0x00; //받은 데이터 끝
			//=====================================================================
			//INIT 받은 데이타 출력
			//=======================================================================
			view_write="";
			view_write=(CHAR*)&clientinitRp;
			ServerWrite(MAINGDISTRING_1);
			ServerWrite(_T("충전 INIT 받은 데이타 출력"));
			ServerWrite(_T("거래전카드잔액:%s"),view_write.Mid(100,10));
			ServerWrite(_T("알고리즘:%s"),view_write.Mid(110,2));
			ServerWrite(_T("전자화폐ID:%s"),view_write.Mid(112,16));
			ServerWrite(_T("전자화폐일련번호:%s"),view_write.Mid(128,10));
			ServerWrite(_T("전자화폐난수:%s"),view_write.Mid(138,16));
			ServerWrite(_T("전자화폐서명:%s"),view_write.Mid(122,8));
			ServerWrite(_T("카드소지자구분:%s"),view_write.Mid(154,2));
			ServerWrite(_T("센터 SAM ID:%s"),view_write.Mid(156,16));
			ServerWrite(_T("센터 LSAM 일련번호:%s"),view_write.Mid(172,10));
			ServerWrite(_T("센터 서명:%s"),view_write.Mid(182,8));
			ServerWrite(_T("센터 거래일련번호:%s"),view_write.Mid(190,10));
			//=====================================================================
			//받은 데이터 디버그 모두 출력
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("충전 INIT 받은 데이타 출력"));
				debug->Write(_T("전체데이타크기:%d"), view_write.GetLength());
				debug->Write(_T("1,응답코드:%s"),view_write.Mid(0,10));
				debug->Write(_T("2,응답메시지:%s"),view_write.Mid(10,50));
				debug->Write(_T("3,충전요청금액:%s"),view_write.Mid(60,10));
				debug->Write(_T("4,수수료금액:%s"),view_write.Mid(70,10));
				debug->Write(_T("5,PG결제금액:%s"),view_write.Mid(80,10));
				debug->Write(_T("6,거래후카드잔액:%s"),view_write.Mid(90,10));
				debug->Write(_T("7,거래전카드잔액:%s"),view_write.Mid(100,10));
				debug->Write(_T("8,사용알고리즘:%s"),view_write.Mid(110,2));
				debug->Write(_T("9,전자화폐ID:%s"),view_write.Mid(112,16));
				debug->Write(_T("10,전자화폐거래일련번호:%s"),view_write.Mid(128,10));
				debug->Write(_T("11,전자화폐가생성한난수:%s"),view_write.Mid(138,16));
				debug->Write(_T("12,카드소지자구분%s"),view_write.Mid(154,2));
				debug->Write(_T("13,센터SMID:%s"),view_write.Mid(156,16));
				debug->Write(_T("14,센터LSAM일련번호:%s"),view_write.Mid(172,10));
				debug->Write(_T("15,센터서명:%s"),view_write.Mid(182,8));
				debug->Write(_T("16,센터거래일련번호:%s"),view_write.Mid(190,10));
				debug->Write(_T("17,결제사코드:%s"),view_write.Mid(200,10));
				debug->Write(_T("18,결제사거래번호:%s"),view_write.Mid(210,30));
				debug->Write(_T("19,결제수단:%s"),view_write.Mid(240,2));
				debug->Write(_T("20,결제사고유정보:%s"),view_write.Mid(242,20));
				debug->Write(_T("21,결제사거래일시:%s"),view_write.Mid(262,20));
				debug->Write(_T("22,Padding:%s"),view_write.Mid(282,6));
			}
			//=====================================================================
			//받은 데이터 카드 입력
			//=======================================================================
			//==============================================
			//카드 서버 데이타 LOAD
			//==============================================
			//서버 데이타를 카드에 기록하기 위한 구조체
			ChargeLoadCardDataSend scardlosd;
			int losdsize = sizeof(scardlosd);
			memset(&scardlosd, 0x30, losdsize);
			scardlosd.end=0x00;
			//=================================================================
			//카드 거래 (충전 LOAD APDU) 복사 
			//==================================================================
			memcpy(scardlosd.CARDCHARGELOAD,CHARGEAPDU_LOAD,sizeof(scardlosd.CARDCHARGELOAD));

			//========================================================================
			//SAM 데이터 복사
			//충전 SAMID
			memcpy(scardlosd.SAM_ID,clientinitRp.SAM_ID,sizeof(scardlosd.SAM_ID));
			//==============================================================
			//충전SAM거래일련번호 변환(16진수 스트링으로 변환)
			CHAR samseq10[11]; 
			memset(samseq10,0x00,sizeof(samseq10));
			memcpy(samseq10, clientinitRp.SAM_SEQ, sizeof(clientinitRp.SAM_SEQ)); //clientinitRp.SAM_SEQ 구조체 크기 만큼 복사 ,10자리
			CHAR samseq16[9];
			memset(samseq16,0x00,sizeof(samseq16));
			sprintf_s(samseq16,_T("%08X"),strtoul(befatm10, NULL, 10)); //10진수 스트링을 16진수 unsinge int 스트링으로, 8자리
			memcpy(scardlosd.SAM_SEQ,samseq16,sizeof(scardlosd.SAM_SEQ));

			//===============================================================
			//카드 데이터 LOAD 용 구조체
			ChargeLoadCardDataRecv lorc;
			RecvData="";
			//================================================================================================
			//카드 데이터 가져오기
			b = scardtm->CommandAll(hCard, ActiveProtocol, (CHAR*)&scardlosd, RecvData, 0,TRUE);
			//===================================================================================================
			//에러 검출 코드
			if(b)
			{
				ServerWrite(_T("성공"));
				if(RecvData.GetLength())
				{
					ServerWrite(_T("받은데이터:%s"),RecvData);
					//=================================================
					//카드에서 받은 데이타를 구조체에 복사한다
					//===================================================
					memcpy(&lorc,RecvData.GetBuffer(RecvData.GetLength()),sizeof(ChargeLoadCardDataRecv));
					lorc.end=0x00; //NULL 종료 문자 추가
				}
				//=================================================
				//에러검출코드 가져오는 함수 호출하여 값을 가져온다
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("에러검출코드:%s"),ErrorCode);
				}
			}
			else
			{
				ServerWrite(_T("실패"));
				//=================================================
				//에러검출코드 가져오는 함수 호출하여 값을 가져온다
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("에러검출코드:%s"),ErrorCode);
				}
				//=================================================
				//버튼을 활성화 세팅을 다시 한다
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//충전버튼  비 활성화
				chargebut.EnableWindow(FALSE);
				break;
			}
			//=================================================
			//서버 보내기(Load 데이타 서버 보낼 구조체에 세팅)
			//===================================================
			ChargeResultRequest clientloadRq;
			int totalloadRqsize = sizeof(clientloadRq); //구조체 크기
			memset(&clientloadRq, 0x30, totalloadRqsize); //0x30 (char 0)으로 초기화
			clientloadRq.end=0x00; //NULL  종료 문자 삽입
			//==============================================================
			//거래전 잔액 변환(10진수 스트링으로 변환) //카드에서 받은 데이터
			CHAR loadbefatm16[9];
			memset(loadbefatm16,0x00,sizeof(loadbefatm16));
			memcpy(loadbefatm16, lorc.BEF_AMT, sizeof(lorc.BEF_AMT)); //lorc.BEF_AMT 구조체 크기 만큼 복사 , 8자리
			CHAR loadbefatm10[11];
			memset(loadbefatm10,0x00,sizeof(loadbefatm10));
			sprintf_s(loadbefatm10,_T("%010u"),strtoul(loadbefatm16, NULL, 16)); //16진수 스트링을 10진수 unsinge int 스트링으로, 10자리
			memcpy(clientloadRq.BEF_AMT,loadbefatm10,sizeof(clientloadRq.BEF_AMT));

			//===============================================================
			//전자화폐 ID         //서버에서 받은 데이터 그래로 복사
			memcpy(clientloadRq.CARD_ID,clientinitRp.CARD_ID,sizeof(clientloadRq.CARD_ID));
			//전자화폐 일련번호 변환(10진수 스트링으로 변환)  //서버에서 받은 데이터 그대로 복사
			memcpy(clientloadRq.CARD_TR_SEQ,clientinitRp.CARD_TR_SEQ,sizeof(clientloadRq.CARD_TR_SEQ));

			//=================chrc==========================================
			//전자화폐사 ID  //init 카드 데이터에서 얻은 값
			memcpy(clientloadRq.ID_CENTER,chrc.KSET_VER,sizeof(clientloadRq.ID_CENTER));

			//사용 알고리즘  //서버에서 받은 데이터 그대로 복사
			memcpy(clientloadRq.ALGORI_ID,clientinitRp.ALGORI_ID,sizeof(clientloadRq.ALGORI_ID));
			//==========================================================================
			//샘카드 넣기
			//SAM 거래일련번호 //서버에서 받은 데이터 그대로 복사
			memcpy(clientloadRq.SAM_SEQ,clientinitRp.SAM_SEQ,sizeof(clientloadRq.SAM_SEQ));
			//SAM ID  //서버에서 받은 데이터 그래로 복사
			memcpy(clientloadRq.SAM_ID,clientinitRp.SAM_ID,sizeof(clientloadRq.SAM_ID));
			//센터 SAM 거래일련번호  //서버에서 받은 데이터 그대로 복사
			memcpy(clientloadRq.SAM_NT,clientinitRp.SAM_NT,sizeof(clientloadRq.SAM_NT));

			//S3             //카드에서 받은 데이타
			memcpy(clientloadRq.SIGN3,lorc.SIGN3,sizeof(clientloadRq.SIGN3));

			//카드 소지자 구분  //서버에서 받은 데이터 그대로 복사
			memcpy(clientloadRq.CARD_CLASS_CD,clientinitRp.CARD_CLASS_CD,sizeof(clientloadRq.CARD_CLASS_CD));
			//=============================================================
			// 보낼 데이타 출력
			//==========================================================
			view_write="";
			view_write=(CHAR*)&clientloadRq;
			ServerWrite(MAINGDISTRING_1);
			ServerWrite(_T("충전 LOAD 보낼 데이타 출력"));
			ServerWrite(_T("거래전카드잔액:%s"),view_write.Mid(56,10));
			ServerWrite(_T("전자화폐ID:%s"),view_write.Mid(76,16));
			ServerWrite(_T("카드일련번호:%s"),view_write.Mid(92,10));
			ServerWrite(_T("전자화폐사ID:%s"),view_write.Mid(102,2));
			ServerWrite(_T("알고리즘ID:%s"),view_write.Mid(104,2));
			ServerWrite(_T("SAM거래일련번호:%s"),view_write.Mid(106,10));
			ServerWrite(_T("SAMID:%s"),view_write.Mid(116,16));
			ServerWrite(_T("센터SAM거래일련번호:%s"),view_write.Mid(132,10));
			ServerWrite(_T("SIGN3:%s"),view_write.Mid(142,8));
			ServerWrite(_T("카드소지자구분:%s"),view_write.Mid(180,2));
			//=====================================================================
			//보낼 데이타 디버그 출력
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("충전 LOAD 보낼 데이타 출력"));
				debug->Write(_T("전체데이타크기:%d"), view_write.GetLength());
				debug->Write(_T("1,전화번호:%s"),view_write.Mid(0,12));
				debug->Write(_T("2,거래요청일시:%s"),view_write.Mid(12,14));
				debug->Write(_T("3,요청금액:%s"),view_write.Mid(26,10));
				debug->Write(_T("4,수수료금액:%s"),view_write.Mid(36,10));
				debug->Write(_T("5,PG결제금액:%s"),view_write.Mid(46,10));
				debug->Write(_T("6,거래전카드잔액:%s"),view_write.Mid(56,10));
				debug->Write(_T("7,거래후카드잔액:%s"),view_write.Mid(66,10));
				debug->Write(_T("8,전자화폐ID:%s"),view_write.Mid(76,16));
				debug->Write(_T("9,카드거래일련번호:%s"),view_write.Mid(92,10));
				debug->Write(_T("10,전자화폐사ID:%s"),view_write.Mid(102,2));
				debug->Write(_T("11,알고리즘ID:%s"),view_write.Mid(104,2));
				debug->Write(_T("12,SAM 거래일련번호:%s"),view_write.Mid(106,10));
				debug->Write(_T("13,SAMID:%s"),view_write.Mid(116,16));
				debug->Write(_T("14,센터SAM거래일련번호:%s"),view_write.Mid(132,10));
				debug->Write(_T("15,SIGN3:%s"),view_write.Mid(142,8));
				debug->Write(_T("16,서버승인번호:%s"),view_write.Mid(150,16));
				debug->Write(_T("17,거래요청일자:%s"),view_write.Mid(166,8));
				debug->Write(_T("18,거래요청시간:%s"),view_write.Mid(174,6));
				debug->Write(_T("19,카드소지자구분:%s"),view_write.Mid(180,2));
				debug->Write(_T("20,결제사코드:%s"),view_write.Mid(182,10));
				debug->Write(_T("21,결제사거래번호:%s"),view_write.Mid(192,30));
				debug->Write(_T("22,결제수단:%s"),view_write.Mid(222,2));
				debug->Write(_T("23,Padding:%s"),view_write.Mid(224,12));
			}
			//==============================================================
			//LOAD 서버로 데이터를 보냄
			client->SendDatafn(sizeof(clientloadRq)-1, &clientloadRq, timeout); //사이즈에서 -1은 end(CHAR)은 전송에서 제외
			//==============================================================
			//서버로부터 데이터를 받음
			ChargeResultResponse clientloadRp;
			client->RecvDatafn(sizeof(clientloadRp)-1, &clientloadRp, timeout); //사이즈에서 -1은 end(CHAR)은 수신에서 제외
			clientloadRp.end=0x00;  //NULL 종료문자 삽입
			//===================================================================
			// LOAD 받은 데이터 출력
			//=================================================================
			view_write="";
			view_write=(CHAR*)&clientloadRp;
			//=====================================================================
			//받은 데이터 디버그 모두 출력
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("충전 LOAD 받은 데이타 출력"));
				debug->Write(_T("전체데이타크기:%d"), view_write.GetLength());
				debug->Write(_T("응답코드:%s"),view_write.Mid(0,10));
				debug->Write(_T("응답메시지:%s"),view_write.Mid(10,50));
				debug->Write(_T("Padding:%s"),view_write.Mid(60,4));
			}
			debug->Write(_T("============충전 완료============="));

		}break;
	}
	debug->Write(_T("============출력 종료 ==============="));
	ServerWrite(MAINGDISTRING_1);
}