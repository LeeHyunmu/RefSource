#include "stdAfx.h"
#include "MainGdiDlg.h"

// ��� �ؽ�Ʈ ���� ����
#define  SCARDDATAFILE_MAXLEN 80
//====================================================
// ��Ʈ�� ����
//=====================================================
#define MAINGDISTRING_1 _T("================================================================================")
#define MAINGDISTRING_2 _T("GLOBAL_SCARD")

//--------------------------------------------------------------
//|
//|�⺻ APDU ����
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
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CMainGdiDlg::~CMainGdiDlg()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
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
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::initInterfaceObject(IMyObject* _obj)
{
	//========================================
	// ������Ʈ ���� ���� ���� Ŭ����
	//========================================
	obj=_obj;
	//========================================
	// ����� ��� Ŭ����
	//========================================
	debug=obj->GetOutputObject();
	//=============================================
	// scardtm
	//=============================================
	scardtm = obj->GetSCardTmObject();
	//========================================
	// ����Ʈī�� ��ȭ ���� ���
	//========================================
	scardtm->SetSCardMsg(this);
}
//--------------------------------------------------------------
//|
//|dlg �ʱ�ȭ 
//|
//---------------------------------------------------------------
void CMainGdiDlg::initDlg(HWND _hWnd)
{
	//========================================
	// HWND ����
	//========================================
	hWnd = _hWnd;
	//========================================
	// ������ �̸�
	//========================================
	SetWindowText(hWnd, MAINGDISTRING_2);
	//========================================
	// ��Ʈ��
	//========================================
	comvpid.Attach(GetDlgItem(hWnd, IDC_VIDPID));
	comvpid.AddString(_T("��ü"));
	comvpid.SetCurSel(0);
	listscardname.Attach(GetDlgItem(hWnd, IDC_SCARDNAME));
	listUsbid.Attach(GetDlgItem(hWnd, IDC_SCARDUSBID));
	//���ؼ� ī��
	staticconnect.Attach(GetDlgItem(hWnd, IDC_CONNECTCARD));
	//APDU ��
	editapdu.Attach(GetDlgItem(hWnd, IDC_APDUSEND));
	//����Ʈī�� ���� ��ư
	butconnect.Attach(GetDlgItem(hWnd, IDC_CONNECTION));
	//����Ʈī�� ������ ���´�
	butdisconnect.Attach(GetDlgItem(hWnd, IDC_DISCONNECTION));
	//APDU ����
	butapud.Attach(GetDlgItem(hWnd, IDC_SEND));
	//APDU RECV ������ ���
	listapdurecv.Attach(GetDlgItem(hWnd, IDC_RECVDATA));
	//���̽� APDU �޺��ڽ�
	combaseapdu.Attach(GetDlgItem(hWnd, IDC_APDUSHORT));
	LoadBaseApduName();

	//��ư Ȱ��ȭ ��Ȱ��ȭ ����
	butconnect.EnableWindow(TRUE);
	butdisconnect.EnableWindow(FALSE);
	butapud.EnableWindow(FALSE);
	//========================================
	// ��Ʈ ����
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
	_tcscpy_s(fontinfo.lfFaceName,sizeof(fontinfo.lfFaceName),_T("����ü"));
	fontcolor=RGB(255,0,0);
}
//--------------------------------------------------------------
//|
//|Dlg �׸��� �Լ�
//|
//---------------------------------------------------------------
void CMainGdiDlg::DrawPaint()
{
	CPaintDC dc(hWnd);
}
//--------------------------------------------------------------
//|
//|�޺��ڽ� IDC_VIDPID �޽���
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
		if(strvpid.Compare(_T("��ü"))==0)
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
	//���ؼ� ����Ʈī�� �̸� �ʱ�ȭ
	if(bSCardConnect==FALSE)
	{
		staticconnect.SetWindowText(_T("None"));
		ConnectSCardName = _T("None");
	}
}
//--------------------------------------------------------------
//|
//|�޺��ڽ� IDC_APDUSHORT �޽���
//|
//---------------------------------------------------------------
void CMainGdiDlg::ComboboxApduShortMessage(UINT uNotifyCode, int nID)
{
	FindBaseApdu();
}
//--------------------------------------------------------------
//|
//|List �ڽ� IDC_SCARDNAME �޽���
//|
//---------------------------------------------------------------
void CMainGdiDlg::ListboxSCardNameMessage(UINT uNotifyCode, int nID)
{
	//���ؼ� ����Ʈī�� �̸� �ʱ�ȭ
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
//| ��ư Ŭ�� �̺�Ʈ �޽���
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
				Write(_T("����Ʈī�� ���� ����=>ī���̸�:%s"),ConnectSCardName);
			}
			else
			{
				Write(_T("����Ʈī�� ���� ����=>ī���̸�:%s"),ConnectSCardName);
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
			Write(_T("%s�ǽ���Ʈī�� ������ �����մϴ�"),ConnectSCardName);
		}break;
	case IDC_SEND:
		{
			CString SendApdu;
			editapdu.GetWindowText(SendApdu);
			CString RecvData="";
			BOOL b = scardtm->CommandAll(hCard, ActiveProtocol, SendApdu, RecvData, 0,TRUE);
			if(b)
			{
				Write(_T("����"));
				if(RecvData.GetLength())
				{
					Write(_T("����������:%s"),RecvData);
				}
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					Write(_T("���������ڵ�:%s"),ErrorCode);
				}
			}
			else
			{
				Write(_T("����"));
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					Write(_T("���������ڵ�:%s"),ErrorCode);
				}
			}
		}break;
	}
	Write(MAINGDISTRING_1);
}
//--------------------------------------------------------------
//|
//|Static Control CtlColor ����
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
//|����Ʈī�� ��ȭ ���� �Լ�
//|
//---------------------------------------------------------------
//��� ���� Ŭ�������� ����Ʈī�� ��ġ�� ��ȭ�� �˰� ���� ��
//flag���� ���� ó�� ����� �ٸ��� ����
void CMainGdiDlg::DeviceListChangeMsg(int flag, CString SCardName)
{
	switch (flag)
	{
	case ISCARDMSG_NEW: NewSCardListMsg();	break;
	case ISCARDMSG_DISCONNECT: SCardDiConnectMsg(SCardName); break;
	}
	//���ؼ� ����Ʈī�� �̸� �ʱ�ȭ
	if(bSCardConnect==FALSE)
	{
		staticconnect.SetWindowText(_T("None"));
		ConnectSCardName = _T("None");
	}
}
//--------------------------------------------------------------
//|
//|�Ϲ��Լ�
//|
//---------------------------------------------------------------
//=======================================================
//����Ʈī�� ������ ��ȭ�� �߻��ߴ�
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
		comvpid.AddString(_T("��ü"));
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
//�����ϰ� �ִ� ����Ʈī�尡 ������
//===========================================================
void CMainGdiDlg::SCardDiConnectMsg(CString SCardName)
{
	if(bSCardConnect)
	{
		bSCardConnect=scardtm->ScardDisConnect(hCard);
		butconnect.EnableWindow(TRUE);
		butdisconnect.EnableWindow(FALSE);
		butapud.EnableWindow(FALSE);
		Write(_T("%s �̸��� ����Ʈī�带 ����ڰ� ����"),SCardName);
		ViewSmartCardConnectLastError();
	}
}
//--------------------------------------------------------------
//|
//|�Ϲ��Լ�
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
//|�ؽ�Ʈ ���
//|
//---------------------------------------------------------------
void CMainGdiDlg::Write(TCHAR* format,...)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	//============================================
	// �������ڷκ��� ���ڿ� ���
	//=============================================
	va_list args;
	TCHAR buffer[1024]={0,};
	va_start(args, format);
	//int len = vswprintf(NULL, format, args) + 1; 
	//buffer = new TCHAR[len];
	_vstprintf_s(buffer, 1024, format, args);
	va_end(args);
	//============================================
	// ���
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
		//���
		listapdurecv.AddString(WriteData);
		int selcount = listapdurecv.GetCount();
		listapdurecv.SetCurSel(selcount-1);
	}
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//|�⺻ APDU ���� ����
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
//|�����̸����� �⺻ APDU �ѱ��
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