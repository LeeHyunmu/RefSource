#include "stdAfx.h"
#include "MainGdiDlg.h"

// 출력 텍스트 길이 제한
#define  SCARDDATAFILE_MAXLEN 80
//====================================================
// 스트링 정의
//=====================================================
#define MAINGDISTRING_1 _T("================================================================================")
#define MAINGDISTRING_2 _T("GLOBAL_SCARD")

//--------------------------------------------------------------
//|
//|기본 APDU 정의
//|
//---------------------------------------------------------------
typedef struct defultApdu
{
	CString ShortName;
	CString Apdu;
}DEFULTAPDU;
DEFULTAPDU _defultapdu[]=
{
	{_T("MAIN"),_T("00A4040007A0000000030000")},
	{_T(""),_T("")}
};
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
				scardtm->ScardDisConnect(hCard);bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
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
			}
			else
			{
				Write(_T("실패"));
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					Write(_T("에러검출코드:%s"),ErrorCode);
				}
			}
		}break;
	}
	Write(MAINGDISTRING_1);
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
//|텍스트 출력
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
	DWORD textpoint = 0;
	while(textlen != textpoint)
	{
		TCHAR WriteData[SCARDDATAFILE_MAXLEN+1];
		memset(WriteData, 0, sizeof(WriteData));
		int offset = textlen-textpoint;
		if(offset > SCARDDATAFILE_MAXLEN)
		{
			memcpy(WriteData,buffer+textpoint,sizeof(TCHAR)*SCARDDATAFILE_MAXLEN); textpoint+=SCARDDATAFILE_MAXLEN;
		}
		else
		{
			memcpy(WriteData,buffer+textpoint,sizeof(TCHAR)*offset); textpoint+=offset;
		}
		//출력
		listapdurecv.AddString(WriteData);
		int selcount = listapdurecv.GetCount();
		listapdurecv.SetCurSel(selcount-1);
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
	GAFileStream::CreateLineReadFile(_T("BaseApdu.txt"),vecFile);
	for (int i=0; i<(int)vecFile.size(); i++)
	{
		vecString str;
		_GASRING::GetTokenize(vecFile[i].GetBuffer(vecFile[i].GetLength()),str);
		combaseapdu.AddString(str[0]);
		vecDefalutAPDU.push_back(str[1]);
	}
// 	int loop=0;
// 	while(TRUE)
// 	{
// 		if(_defultapdu[loop].ShortName.Compare(_T(""))==0)
// 			break;
// 		combaseapdu.AddString(_defultapdu[loop].ShortName);
// 		loop++;
// 	}
	combaseapdu.SetCurSel(0);
	FindBaseApdu();
}
//--------------------------------------------------------------
//|
//|단축이름으로 기본 APDU 넘긴다
//|
//---------------------------------------------------------------
BOOL CMainGdiDlg::FindBaseApdu()
{
	int intsel;
	//CString strShortName;
	intsel = combaseapdu.GetCurSel();
	//combaseapdu.GetLBText(intsel,strShortName);
// 	int loop=0;
// 	while(TRUE)
// 	{
// 		if(_defultapdu[loop].ShortName.Compare(strShortName)==0)
// 		{
// 			editapdu.SetWindowText(_defultapdu[loop].Apdu);
// 			return TRUE;
// 		}
// 		loop++;
// 	}
	if((int)vecDefalutAPDU.size()>intsel)
	{
		editapdu.SetWindowText(vecDefalutAPDU[intsel]);
		return TRUE;
	}
	return FALSE;
}