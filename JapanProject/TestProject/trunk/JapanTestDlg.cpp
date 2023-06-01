// JapanTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "JapanTest.h"
#include "JapanTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CJapanTestDlg 대화 상자

CJapanTestDlg::CJapanTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJapanTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CJapanTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DebugView);
}

BEGIN_MESSAGE_MAP(CJapanTestDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CJapanTestDlg 메시지 처리기

BOOL CJapanTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	//
	AQDevice.initAQNFC(this, ReadEventFunc);

	BOOL b = test.initAQdeviceCmd(m_hWnd, _T("DualDlleMbedded.dll"));
	test.NFCAtypeConnect(this, AtypEventMessageCall, MtypEventMessageCall, DisConnectEventMessageCall);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CJapanTestDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_JAPANTEST_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_JAPANTEST_DIALOG));
	}
}
#endif


void CJapanTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	test.NFCDisConnect();
}
DWORD WINAPI CJapanTestDlg::AtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen)
{
	CJapanTestDlg* mdlg = (CJapanTestDlg*)lpThreadParameter;
	return mdlg->AtypEventMessage(eventdata, eventdatalen);
}
DWORD CJapanTestDlg::AtypEventMessage(byte* eventdata, int eventdatalen)
{
	DebugWrite(_T("a type"));
	return 0;
}
DWORD WINAPI CJapanTestDlg::MtypEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen)
{
	CJapanTestDlg* mdlg = (CJapanTestDlg*)lpThreadParameter;
	return mdlg->MtypEventMessage(eventdata, eventdatalen);
}
DWORD CJapanTestDlg::MtypEventMessage(byte* eventdata, int eventdatalen)
{
	int senddatalen=0;
	BYTE senddata[BUFFSIZE_1024];
	DebugWrite(_T("Mifare type"));
	senddata[0]=0x31;
	senddata[1]=0x32;
	senddata[2]=0x33;
	senddata[3]=0x34;
	senddata[4]=0x00;
	senddata[5]=0x00;
	NFCErrorCode err = test.DEA_Authkey(senddata, 4);
	if(err == DE_OK)
	{
		DebugWrite(_T("Mifare type key AUTH"));
		int outdatalen;
		BYTE outdata[BUFFSIZE_1024];
		DebugWrite(_T("print Card data"));
		for(int i = 0; i < 3; i++)
		{
			outdatalen=0;
			err = test.DEA_Read(4+i, &outdatalen, outdata);
			if(err == DE_OK)
			{
				CString str;;
				str.Format(_T("blockno:%d, data:%s"), 4+i, byteToHexbyteValue(outdata, outdatalen));
				DebugWrite(str);
			}
		}
	}
	return 0;
}
DWORD WINAPI CJapanTestDlg::DisConnectEventMessageCall(LPVOID lpThreadParameter, byte* eventdata, int eventdatalen)
{
	CJapanTestDlg* mdlg = (CJapanTestDlg*)lpThreadParameter;
	return mdlg->DisConnectEventMessage(eventdata, eventdatalen);
}
DWORD CJapanTestDlg::DisConnectEventMessage(byte* eventdata, int eventdatalen)
{
	DebugWrite(_T("Card DisConnect"));
	return 0;
}


DWORD WINAPI CJapanTestDlg::ReadEventFunc(LPVOID lpThreadParameter, CWaterCmdFormat::WaterCheck check)
{
	CJapanTestDlg* mdlg = (CJapanTestDlg*)lpThreadParameter;
	return mdlg->ReadEventMessage(check);
}
DWORD  CJapanTestDlg::ReadEventMessage(CWaterCmdFormat::WaterCheck check)
{
	switch(check)
	{
	case CWaterCmdFormat::water_ready:
		{
			DebugWrite(_T("water ready"));
		}break;
	case CWaterCmdFormat::water_readycomplete:
		{
			DebugWrite(_T("water ready complete"));
		}break;
	case CWaterCmdFormat::water_run:
		{
			DebugWrite(_T("water run"));
		}break;
	case CWaterCmdFormat::water_error:
		{
			DebugWrite(_T("water error"));
		}break;
	}
	return 0;
}
static char hexChar[] = {
	0x30 , 0x31 , 0x32 , 0x33 ,
	0x34 , 0x35 , 0x36 , 0x37 ,
	0x38 , 0x39 , 0x41 , 0x42 ,
	0x43 , 0x44 , 0x45 , 0x46 
};
CString CJapanTestDlg::byteToHexbyteValue(byte* data, int len) 
{
	CString hex=_T("");
	for (int i = 0, h = 0; i < len; i++, h+=2) {
		hex += hexChar[(data[i]&0xF0)>>4];
		hex += hexChar[(data[i]&0x0F)];
	}
	return hex;
}
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
int  CJapanTestDlg::ChangeHexCord(CString data, BYTE* hex, int maxlen)
{
	memset(hex, 0, maxlen);
	int dwtransLen=data.GetLength()/2;
	for(int i = 0 ; i < dwtransLen ; i++ )
	{
		hex[i] = (((BYTE)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			hex[i] += 0x90;

		hex[i] |= ((BYTE)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			hex[i] += 0x09;
	}
	return dwtransLen;
}
//--------------------------------------------------------------
//|
//|디버그 출력
//|
//---------------------------------------------------------------
void  CJapanTestDlg::DebugWrite(CString data)
{
	m_DebugView.AddString(data);
	int count = m_DebugView.GetCount();
	m_DebugView.SetCurSel(m_DebugView.GetCount()-1);
}