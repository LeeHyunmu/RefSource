#include "stdAfx.h"
#include "MainGdiDlg.h"

// ��� �ؽ�Ʈ ���� ����
#define  SCARDDATAFILE_MAXLEN 74
//====================================================
// ��Ʈ�� ����
//=====================================================
#define MAINGDISTRING_1 _T("================================================================")
#define MAINGDISTRING_2 _T("GLOBAL_SCARD")

//--------------------------------------------------------------
//|
//|�⺻ APDU ����
//|
//---------------------------------------------------------------
#define DEFAULTPORT _T("9000")
#define DEFAULTIP _T("127.0.0.1")
#define DEFAULTMONEY _T("20000")
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

	//���� ��Ʈ�� ����
	Serverip.Attach(GetDlgItem(hWnd, IDC_SERVERIPNUM));
	Serverport.Attach(GetDlgItem(hWnd, IDC_SERVERPORTNUM));
	Serverconnect.Attach(GetDlgItem(hWnd, IDC_SERVERCONNECTION));
	listserverview.Attach(GetDlgItem(hWnd, IDC_SERVERCOMMECTDATA));
	Servertimeout.Attach(GetDlgItem(hWnd, IDC_SERVERTIMEOUT));

	//���� ��Ʈ��
	chargemoney.Attach(GetDlgItem(hWnd, IDC_CHARGEMONEY));
	chargebut.Attach(GetDlgItem(hWnd, IDC_CLCHARGE));
	//������ư ��Ȱ��ȭ
	chargebut.EnableWindow(FALSE);

	Serverip.SetWindowText(DEFAULTIP);
	Serverport.SetWindowText(DEFAULTPORT);
	chargemoney.SetWindowText(DEFAULTMONEY);
	CHAR timeout[11];
	memset(timeout,0x00,sizeof(timeout));
	sprintf_s(timeout,_T("%u"),INFINITE);
	Servertimeout.SetWindowText(timeout);
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
//|�ؽ�Ʈ ���(����Ʈī�� ������ ���)
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
		//���
		listapdurecv.AddString(strData);
		int selcount = listapdurecv.GetCount();
		listapdurecv.SetCurSel(selcount-1);
	}
	m_cs.Unlock();
}
//--------------------------------------------------------------
//|
//|�ؽ�Ʈ ���(���� ������ ���)
//|
//---------------------------------------------------------------
void CMainGdiDlg::ServerWrite(TCHAR* format,...)
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
		//���
		listserverview.AddString(strData);
		int selcount = listserverview.GetCount();
		listserverview.SetCurSel(selcount-1);
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
//|�����̸����� �⺻ APDU �ѱ��
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
//|����Ʈī�� ��ư ��� �Լ�
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
						//��ư�� Ȱ��ȭ ����
						//===================================================
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
				scardtm->ScardDisConnect(hCard);
				//=================================================
				//��ư�� Ȱ��ȭ ������ �ٽ� �Ѵ�
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//������ư  �� Ȱ��ȭ
				chargebut.EnableWindow(FALSE);
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
				//������ư Ȱ��ȭ
				chargebut.EnableWindow(TRUE);
			}
			else
			{
				Write(_T("����"));
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					Write(_T("���������ڵ�:%s"),ErrorCode);
				}
				//=================================================
				//��ư�� Ȱ��ȭ ������ �ٽ� �Ѵ�
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//������ư  �� Ȱ��ȭ
				chargebut.EnableWindow(FALSE);
			}
		}break;
	}
	Write(MAINGDISTRING_1);
}
//--------------------------------------------------------------
//|
//|����Ʈī�� ��ư ��� �Լ�
//|
//---------------------------------------------------------------
//USERCODE(01: ����, 02: û�ҳ�, 03: �л�, 04:14���̸� ���)
#define USERCODE_1 _T("01")
#define USERCODE_2 _T("02")
#define USERCODE_3 _T("03")
#define USERCODE_4 _T("04")
//���� 
//init
#define CHARGEAPDU_INIT _T("9040000004")
//Load
#define CHARGEAPDU_LOAD _T("9042000010")
//���� 
//init
#define PAYMONEYAPDU_INIT _T("9040030004")
//Load
#define PAYMONEYAPDU_PURCHASE _T("9046000012")
void CMainGdiDlg::ClientButtonMessage(UINT uNotifyCode, int nID)
{
	ServerWrite(MAINGDISTRING_1);
	debug->Write(_T("============��� ���� ==============="));
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
				ServerWrite(_T("���� ���� ����"));
			}
			else
			{
				ServerWrite(_T("���� ���� ����"));
				debug->Write(_T("���� ���� ����"));
				Serverconnect.EnableWindow(FALSE);
			}
		}break;
	case IDC_CLCHARGE:
		{
			debug->Write(_T("============���� ����============="));
			//==============================================
			//�ݾ� + ī�� init
			//==============================================
			//����init ����Ÿ�� ���� ����ϱ� ���� ����ü
			ChargeinitCardDataRecv chrc;
			//==============================================
			//�Է� �ݾ� ��ȯ
			CString strmoney;
			chargemoney.GetWindowText(strmoney);
			CString MoneyHex;
			MoneyHex.Format("%08X",atoi(strmoney));
			//============================================================================================
			CString RecvData="";
			//ī�� ������ ��������
 			BOOL b = scardtm->CommandAll(hCard, ActiveProtocol, CHARGEAPDU_INIT+MoneyHex, RecvData, 0,TRUE);
			//===============================================================================================
			//���� ���� �ڵ�
 			if(b)
			{
 				ServerWrite(_T("����"));
				if(RecvData.GetLength())
				{
					ServerWrite(_T("����������:%s"),RecvData);
					//=================================================
					//ī�忡�� ���� ����Ÿ�� ����ü�� �����Ѵ�
					//===================================================
					memcpy(&chrc,RecvData.GetBuffer(RecvData.GetLength()),sizeof(ChargeinitCardDataRecv));
					chrc.end=0x00;
				}
				//=================================================
				//���������ڵ� �������� �Լ� ȣ���Ͽ� ���� �����´�
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("���������ڵ�:%s"),ErrorCode);
				}
			}
 			else
 			{
 				ServerWrite(_T("����"));
				//=================================================
				//���������ڵ� �������� �Լ� ȣ���Ͽ� ���� �����´�
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("���������ڵ�:%s"),ErrorCode);
				}
				//=================================================
				//��ư�� Ȱ��ȭ ������ �ٽ� �Ѵ�
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//������ư  �� Ȱ��ȭ
				chargebut.EnableWindow(FALSE);
				break;
			}
			//=================================================
			//���� ������(init ����Ÿ)
			//===================================================
			//������ ���� �����͸� ����ü�� �̿��Ͽ� ���� �����Ѵ�
			ChargeInitRequest clientinitRq;
			int totalinitRqsize = sizeof(clientinitRq);   //����ü ������ ���Ѵ�
			memset(&clientinitRq, 0x30, totalinitRqsize); //���� ���� 0(�ƽ�Ű�ڵ尪 0x30)���� �ʱ�ȭ
			clientinitRq.end=0x00; //end�� ���ڿ� NULL(�ƽ�Ű�ڵ� 0x00)���θ����
			//==============================================================
			//�ŷ��� �ܾ� ��ȯ(10���� ��Ʈ������ ��ȯ) , ī�� ����Ÿ 8����Ʈ�� 10����Ʈ�� �����
			CHAR befatm16[9];
			memset(befatm16,0x00,sizeof(befatm16));
			memcpy(befatm16, chrc.BEF_AMT, sizeof(chrc.BEF_AMT)); //chrc.BEF_AMT ����ü ũ�� ��ŭ ���� , 8�ڸ�
			CHAR befatm10[11];
			memset(befatm10,0x00,sizeof(befatm10));
			sprintf_s(befatm10,_T("%010u"),strtoul(befatm16, NULL, 16)); //16���� ��Ʈ���� 10���� unsinge int ��Ʈ������ , 10�ڸ�
			memcpy(clientinitRq.BEF_AMT,befatm10,sizeof(clientinitRq.BEF_AMT));
			//===============================================================
			//��� �˰���
			memcpy(clientinitRq.ALGORI_ID,chrc.ALGORI_ID,sizeof(clientinitRq.ALGORI_ID));
			//����ȭ��� ID
			memcpy(clientinitRq.KSET_VER,chrc.KSET_VER,sizeof(clientinitRq.KSET_VER));
			//����ȭ�� ID
			memcpy(clientinitRq.CARD_ID,chrc.CARD_ID,sizeof(clientinitRq.CARD_ID));
			//==============================================================
			//����ȭ�� �Ϸù�ȣ ��ȯ(10���� ��Ʈ������ ��ȯ)
			CHAR cardtrseq16[9];
			memset(cardtrseq16,0x00,sizeof(cardtrseq16));
			memcpy(cardtrseq16, chrc.CARD_TR_SEQ, sizeof(chrc.CARD_TR_SEQ)); //chrc.CARD_TR_SEQ ����ü ũ�� ��ŭ ���� , 8�ڸ�
			CHAR cardtrseq10[11];
			memset(cardtrseq10,0x00,sizeof(cardtrseq10));
			sprintf_s(cardtrseq10,_T("%010u"),strtoul(cardtrseq16, NULL, 16)); //16���� ��Ʈ���� 10���� unsinge int ��Ʈ������, 10�ڸ�
			memcpy(clientinitRq.CARD_TR_SEQ,cardtrseq10,sizeof(clientinitRq.CARD_TR_SEQ));
			//===============================================================
			//����ȭ�� ���� ����
			memcpy(clientinitRq.RANDM_NUM,chrc.RANDM_NUM,sizeof(clientinitRq.RANDM_NUM));
			//����ȭ�� ����
			memcpy(clientinitRq.SIGN1,chrc.SIGN1,sizeof(clientinitRq.SIGN1));
			//ī�� ������ ����
			memcpy(clientinitRq.CARD_CLASS_CD,USERCODE_1,sizeof(clientinitRq.CARD_CLASS_CD));
			//=============================================================
			// ���� ����Ÿ ���(ListBox�� ���)
			//==========================================================
			//CString ������ ���� ����Ѵ�(CAtlString)
			CString view_write=(CHAR*)&clientinitRq;
			ServerWrite(MAINGDISTRING_1);
			ServerWrite(_T("���� INIT ���� ����Ÿ ���"));
			ServerWrite(_T("�ŷ���ī���ܾ�:%s"),view_write.Mid(56,10));
			ServerWrite(_T("�˰���:%s"),view_write.Mid(76,2));
			ServerWrite(_T("����ȭ���ID:%s"),view_write.Mid(78,2));
			ServerWrite(_T("����ȭ��ID:%s"),view_write.Mid(80,16));
			ServerWrite(_T("����ȭ���Ϸù�ȣ:%s"),view_write.Mid(96,10));
			ServerWrite(_T("����ȭ�󳭼�:%s"),view_write.Mid(106,16));
			ServerWrite(_T("����ȭ�󼭸�:%s"),view_write.Mid(122,8));
			ServerWrite(_T("ī������ڱ���:%s"),view_write.Mid(130,2));
			//=====================================================================
			//���� ����Ÿ ����� ���(��� ����Ÿ ���)
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("���� INIT ���� ����Ÿ ���"));
				debug->Write(_T("��ü����Ÿũ��:%d"), view_write.GetLength());
				debug->Write(_T("1,��ȭ��ȣ:%s"),view_write.Mid(0,12));
				debug->Write(_T("2,�ŷ���û�Ͻ�:%s"),view_write.Mid(12,14));
				debug->Write(_T("3,��û�ݾ�:%s"),view_write.Mid(26,10));
				debug->Write(_T("4,������ݾ�:%s"),view_write.Mid(36,10));
				debug->Write(_T("5,PG�����ݾ�:%s"),view_write.Mid(46,10));
				debug->Write(_T("6,�ŷ���ī���ܾ�:%s"),view_write.Mid(56,10));
				debug->Write(_T("7,�ŷ���ī���ܾ�:%s"),view_write.Mid(66,10));
				debug->Write(_T("8,���˰���:%s"),view_write.Mid(76,2));
				debug->Write(_T("9,����ȭ���ID:%s"),view_write.Mid(78,2));
				debug->Write(_T("10,����ȭ��ID:%s"),view_write.Mid(80,16));
				debug->Write(_T("11,����ȭ��ŷ��Ϸù�ȣ:%s"),view_write.Mid(96,10));
				debug->Write(_T("12,����ȭ�󰡻����ѳ���:%s"),view_write.Mid(106,16));
				debug->Write(_T("13,����ȭ�󰡻����Ѽ���:%s"),view_write.Mid(122,8));
				debug->Write(_T("14,ī������ڱ���:%s"),view_write.Mid(130,2));
				debug->Write(_T("15,�������ڵ�:%s"),view_write.Mid(132,10));
				debug->Write(_T("16,������ŷ���ȣ:%s"),view_write.Mid(142,30));
				debug->Write(_T("17,��������:%s"),view_write.Mid(172,2));
				debug->Write(_T("18,Padding:%s"),view_write.Mid(174,10));
			}
			//=============================================================
			//Ŭ���̾�Ʈ ��ü
			IClient* client = obj->GetClientObject();
			//=================================================================
			//Ŭ���̾�Ʈ�� ������ ���ӵǾ� �ִ°��� Ȯ���Ѵ�
			if(client->ActiveClient()==FALSE)
			{
				ServerWrite(_T("������ ������ �������ų� �������� ����"));
				break;
			}
			//==================================================================
			//Ÿ�Ӿƿ� �б�
			DWORD timeout;
			CString timestr;
			Servertimeout.GetWindowText(timestr);
			timeout= strtoul(timestr.GetBuffer(timestr.GetLength()), NULL, 10);
			//==============================================================
			//INIT ������ �����͸� ����
			client->SendDatafn(sizeof(clientinitRq)-1, &clientinitRq, timeout); //������� -1�� end(CHAR)�� ���ۿ��� ����
			//==============================================================
			//INIT �����κ��� �����͸� ����
			ChargeInitResponse clientinitRp;
			client->RecvDatafn(sizeof(clientinitRp)-1, &clientinitRp,timeout); //������� -1�� end(CHAR)�� ���ſ��� ����
			clientinitRp.end=0x00; //���� ������ ��
			//=====================================================================
			//INIT ���� ����Ÿ ���
			//=======================================================================
			view_write="";
			view_write=(CHAR*)&clientinitRp;
			ServerWrite(MAINGDISTRING_1);
			ServerWrite(_T("���� INIT ���� ����Ÿ ���"));
			ServerWrite(_T("�ŷ���ī���ܾ�:%s"),view_write.Mid(100,10));
			ServerWrite(_T("�˰���:%s"),view_write.Mid(110,2));
			ServerWrite(_T("����ȭ��ID:%s"),view_write.Mid(112,16));
			ServerWrite(_T("����ȭ���Ϸù�ȣ:%s"),view_write.Mid(128,10));
			ServerWrite(_T("����ȭ�󳭼�:%s"),view_write.Mid(138,16));
			ServerWrite(_T("����ȭ�󼭸�:%s"),view_write.Mid(122,8));
			ServerWrite(_T("ī������ڱ���:%s"),view_write.Mid(154,2));
			ServerWrite(_T("���� SAM ID:%s"),view_write.Mid(156,16));
			ServerWrite(_T("���� LSAM �Ϸù�ȣ:%s"),view_write.Mid(172,10));
			ServerWrite(_T("���� ����:%s"),view_write.Mid(182,8));
			ServerWrite(_T("���� �ŷ��Ϸù�ȣ:%s"),view_write.Mid(190,10));
			//=====================================================================
			//���� ������ ����� ��� ���
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("���� INIT ���� ����Ÿ ���"));
				debug->Write(_T("��ü����Ÿũ��:%d"), view_write.GetLength());
				debug->Write(_T("1,�����ڵ�:%s"),view_write.Mid(0,10));
				debug->Write(_T("2,����޽���:%s"),view_write.Mid(10,50));
				debug->Write(_T("3,������û�ݾ�:%s"),view_write.Mid(60,10));
				debug->Write(_T("4,������ݾ�:%s"),view_write.Mid(70,10));
				debug->Write(_T("5,PG�����ݾ�:%s"),view_write.Mid(80,10));
				debug->Write(_T("6,�ŷ���ī���ܾ�:%s"),view_write.Mid(90,10));
				debug->Write(_T("7,�ŷ���ī���ܾ�:%s"),view_write.Mid(100,10));
				debug->Write(_T("8,���˰���:%s"),view_write.Mid(110,2));
				debug->Write(_T("9,����ȭ��ID:%s"),view_write.Mid(112,16));
				debug->Write(_T("10,����ȭ��ŷ��Ϸù�ȣ:%s"),view_write.Mid(128,10));
				debug->Write(_T("11,����ȭ�󰡻����ѳ���:%s"),view_write.Mid(138,16));
				debug->Write(_T("12,ī������ڱ���%s"),view_write.Mid(154,2));
				debug->Write(_T("13,����SMID:%s"),view_write.Mid(156,16));
				debug->Write(_T("14,����LSAM�Ϸù�ȣ:%s"),view_write.Mid(172,10));
				debug->Write(_T("15,���ͼ���:%s"),view_write.Mid(182,8));
				debug->Write(_T("16,���Ͱŷ��Ϸù�ȣ:%s"),view_write.Mid(190,10));
				debug->Write(_T("17,�������ڵ�:%s"),view_write.Mid(200,10));
				debug->Write(_T("18,������ŷ���ȣ:%s"),view_write.Mid(210,30));
				debug->Write(_T("19,��������:%s"),view_write.Mid(240,2));
				debug->Write(_T("20,�������������:%s"),view_write.Mid(242,20));
				debug->Write(_T("21,������ŷ��Ͻ�:%s"),view_write.Mid(262,20));
				debug->Write(_T("22,Padding:%s"),view_write.Mid(282,6));
			}
			//=====================================================================
			//���� ������ ī�� �Է�
			//=======================================================================
			//==============================================
			//ī�� ���� ����Ÿ LOAD
			//==============================================
			//���� ����Ÿ�� ī�忡 ����ϱ� ���� ����ü
			ChargeLoadCardDataSend scardlosd;
			int losdsize = sizeof(scardlosd);
			memset(&scardlosd, 0x30, losdsize);
			scardlosd.end=0x00;
			//=================================================================
			//ī�� �ŷ� (���� LOAD APDU) ���� 
			//==================================================================
			memcpy(scardlosd.CARDCHARGELOAD,CHARGEAPDU_LOAD,sizeof(scardlosd.CARDCHARGELOAD));

			//========================================================================
			//SAM ������ ����
			//���� SAMID
			memcpy(scardlosd.SAM_ID,clientinitRp.SAM_ID,sizeof(scardlosd.SAM_ID));
			//==============================================================
			//����SAM�ŷ��Ϸù�ȣ ��ȯ(16���� ��Ʈ������ ��ȯ)
			CHAR samseq10[11]; 
			memset(samseq10,0x00,sizeof(samseq10));
			memcpy(samseq10, clientinitRp.SAM_SEQ, sizeof(clientinitRp.SAM_SEQ)); //clientinitRp.SAM_SEQ ����ü ũ�� ��ŭ ���� ,10�ڸ�
			CHAR samseq16[9];
			memset(samseq16,0x00,sizeof(samseq16));
			sprintf_s(samseq16,_T("%08X"),strtoul(befatm10, NULL, 10)); //10���� ��Ʈ���� 16���� unsinge int ��Ʈ������, 8�ڸ�
			memcpy(scardlosd.SAM_SEQ,samseq16,sizeof(scardlosd.SAM_SEQ));

			//===============================================================
			//ī�� ������ LOAD �� ����ü
			ChargeLoadCardDataRecv lorc;
			RecvData="";
			//================================================================================================
			//ī�� ������ ��������
			b = scardtm->CommandAll(hCard, ActiveProtocol, (CHAR*)&scardlosd, RecvData, 0,TRUE);
			//===================================================================================================
			//���� ���� �ڵ�
			if(b)
			{
				ServerWrite(_T("����"));
				if(RecvData.GetLength())
				{
					ServerWrite(_T("����������:%s"),RecvData);
					//=================================================
					//ī�忡�� ���� ����Ÿ�� ����ü�� �����Ѵ�
					//===================================================
					memcpy(&lorc,RecvData.GetBuffer(RecvData.GetLength()),sizeof(ChargeLoadCardDataRecv));
					lorc.end=0x00; //NULL ���� ���� �߰�
				}
				//=================================================
				//���������ڵ� �������� �Լ� ȣ���Ͽ� ���� �����´�
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("���������ڵ�:%s"),ErrorCode);
				}
			}
			else
			{
				ServerWrite(_T("����"));
				//=================================================
				//���������ڵ� �������� �Լ� ȣ���Ͽ� ���� �����´�
				//===================================================
				CString ErrorCode = scardtm->GetLastErrorCode();
				if(ErrorCode.GetLength())
				{
					ServerWrite(_T("���������ڵ�:%s"),ErrorCode);
				}
				//=================================================
				//��ư�� Ȱ��ȭ ������ �ٽ� �Ѵ�
				//===================================================
				bSCardConnect=FALSE;
				butconnect.EnableWindow(TRUE);
				butdisconnect.EnableWindow(FALSE);
				butapud.EnableWindow(FALSE);
				//������ư  �� Ȱ��ȭ
				chargebut.EnableWindow(FALSE);
				break;
			}
			//=================================================
			//���� ������(Load ����Ÿ ���� ���� ����ü�� ����)
			//===================================================
			ChargeResultRequest clientloadRq;
			int totalloadRqsize = sizeof(clientloadRq); //����ü ũ��
			memset(&clientloadRq, 0x30, totalloadRqsize); //0x30 (char 0)���� �ʱ�ȭ
			clientloadRq.end=0x00; //NULL  ���� ���� ����
			//==============================================================
			//�ŷ��� �ܾ� ��ȯ(10���� ��Ʈ������ ��ȯ) //ī�忡�� ���� ������
			CHAR loadbefatm16[9];
			memset(loadbefatm16,0x00,sizeof(loadbefatm16));
			memcpy(loadbefatm16, lorc.BEF_AMT, sizeof(lorc.BEF_AMT)); //lorc.BEF_AMT ����ü ũ�� ��ŭ ���� , 8�ڸ�
			CHAR loadbefatm10[11];
			memset(loadbefatm10,0x00,sizeof(loadbefatm10));
			sprintf_s(loadbefatm10,_T("%010u"),strtoul(loadbefatm16, NULL, 16)); //16���� ��Ʈ���� 10���� unsinge int ��Ʈ������, 10�ڸ�
			memcpy(clientloadRq.BEF_AMT,loadbefatm10,sizeof(clientloadRq.BEF_AMT));

			//===============================================================
			//����ȭ�� ID         //�������� ���� ������ �׷��� ����
			memcpy(clientloadRq.CARD_ID,clientinitRp.CARD_ID,sizeof(clientloadRq.CARD_ID));
			//����ȭ�� �Ϸù�ȣ ��ȯ(10���� ��Ʈ������ ��ȯ)  //�������� ���� ������ �״�� ����
			memcpy(clientloadRq.CARD_TR_SEQ,clientinitRp.CARD_TR_SEQ,sizeof(clientloadRq.CARD_TR_SEQ));

			//=================chrc==========================================
			//����ȭ��� ID  //init ī�� �����Ϳ��� ���� ��
			memcpy(clientloadRq.ID_CENTER,chrc.KSET_VER,sizeof(clientloadRq.ID_CENTER));

			//��� �˰���  //�������� ���� ������ �״�� ����
			memcpy(clientloadRq.ALGORI_ID,clientinitRp.ALGORI_ID,sizeof(clientloadRq.ALGORI_ID));
			//==========================================================================
			//��ī�� �ֱ�
			//SAM �ŷ��Ϸù�ȣ //�������� ���� ������ �״�� ����
			memcpy(clientloadRq.SAM_SEQ,clientinitRp.SAM_SEQ,sizeof(clientloadRq.SAM_SEQ));
			//SAM ID  //�������� ���� ������ �׷��� ����
			memcpy(clientloadRq.SAM_ID,clientinitRp.SAM_ID,sizeof(clientloadRq.SAM_ID));
			//���� SAM �ŷ��Ϸù�ȣ  //�������� ���� ������ �״�� ����
			memcpy(clientloadRq.SAM_NT,clientinitRp.SAM_NT,sizeof(clientloadRq.SAM_NT));

			//S3             //ī�忡�� ���� ����Ÿ
			memcpy(clientloadRq.SIGN3,lorc.SIGN3,sizeof(clientloadRq.SIGN3));

			//ī�� ������ ����  //�������� ���� ������ �״�� ����
			memcpy(clientloadRq.CARD_CLASS_CD,clientinitRp.CARD_CLASS_CD,sizeof(clientloadRq.CARD_CLASS_CD));
			//=============================================================
			// ���� ����Ÿ ���
			//==========================================================
			view_write="";
			view_write=(CHAR*)&clientloadRq;
			ServerWrite(MAINGDISTRING_1);
			ServerWrite(_T("���� LOAD ���� ����Ÿ ���"));
			ServerWrite(_T("�ŷ���ī���ܾ�:%s"),view_write.Mid(56,10));
			ServerWrite(_T("����ȭ��ID:%s"),view_write.Mid(76,16));
			ServerWrite(_T("ī���Ϸù�ȣ:%s"),view_write.Mid(92,10));
			ServerWrite(_T("����ȭ���ID:%s"),view_write.Mid(102,2));
			ServerWrite(_T("�˰���ID:%s"),view_write.Mid(104,2));
			ServerWrite(_T("SAM�ŷ��Ϸù�ȣ:%s"),view_write.Mid(106,10));
			ServerWrite(_T("SAMID:%s"),view_write.Mid(116,16));
			ServerWrite(_T("����SAM�ŷ��Ϸù�ȣ:%s"),view_write.Mid(132,10));
			ServerWrite(_T("SIGN3:%s"),view_write.Mid(142,8));
			ServerWrite(_T("ī������ڱ���:%s"),view_write.Mid(180,2));
			//=====================================================================
			//���� ����Ÿ ����� ���
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("���� LOAD ���� ����Ÿ ���"));
				debug->Write(_T("��ü����Ÿũ��:%d"), view_write.GetLength());
				debug->Write(_T("1,��ȭ��ȣ:%s"),view_write.Mid(0,12));
				debug->Write(_T("2,�ŷ���û�Ͻ�:%s"),view_write.Mid(12,14));
				debug->Write(_T("3,��û�ݾ�:%s"),view_write.Mid(26,10));
				debug->Write(_T("4,������ݾ�:%s"),view_write.Mid(36,10));
				debug->Write(_T("5,PG�����ݾ�:%s"),view_write.Mid(46,10));
				debug->Write(_T("6,�ŷ���ī���ܾ�:%s"),view_write.Mid(56,10));
				debug->Write(_T("7,�ŷ���ī���ܾ�:%s"),view_write.Mid(66,10));
				debug->Write(_T("8,����ȭ��ID:%s"),view_write.Mid(76,16));
				debug->Write(_T("9,ī��ŷ��Ϸù�ȣ:%s"),view_write.Mid(92,10));
				debug->Write(_T("10,����ȭ���ID:%s"),view_write.Mid(102,2));
				debug->Write(_T("11,�˰���ID:%s"),view_write.Mid(104,2));
				debug->Write(_T("12,SAM �ŷ��Ϸù�ȣ:%s"),view_write.Mid(106,10));
				debug->Write(_T("13,SAMID:%s"),view_write.Mid(116,16));
				debug->Write(_T("14,����SAM�ŷ��Ϸù�ȣ:%s"),view_write.Mid(132,10));
				debug->Write(_T("15,SIGN3:%s"),view_write.Mid(142,8));
				debug->Write(_T("16,�������ι�ȣ:%s"),view_write.Mid(150,16));
				debug->Write(_T("17,�ŷ���û����:%s"),view_write.Mid(166,8));
				debug->Write(_T("18,�ŷ���û�ð�:%s"),view_write.Mid(174,6));
				debug->Write(_T("19,ī������ڱ���:%s"),view_write.Mid(180,2));
				debug->Write(_T("20,�������ڵ�:%s"),view_write.Mid(182,10));
				debug->Write(_T("21,������ŷ���ȣ:%s"),view_write.Mid(192,30));
				debug->Write(_T("22,��������:%s"),view_write.Mid(222,2));
				debug->Write(_T("23,Padding:%s"),view_write.Mid(224,12));
			}
			//==============================================================
			//LOAD ������ �����͸� ����
			client->SendDatafn(sizeof(clientloadRq)-1, &clientloadRq, timeout); //������� -1�� end(CHAR)�� ���ۿ��� ����
			//==============================================================
			//�����κ��� �����͸� ����
			ChargeResultResponse clientloadRp;
			client->RecvDatafn(sizeof(clientloadRp)-1, &clientloadRp, timeout); //������� -1�� end(CHAR)�� ���ſ��� ����
			clientloadRp.end=0x00;  //NULL ���Ṯ�� ����
			//===================================================================
			// LOAD ���� ������ ���
			//=================================================================
			view_write="";
			view_write=(CHAR*)&clientloadRp;
			//=====================================================================
			//���� ������ ����� ��� ���
			//=======================================================================
			if(debug)
			{
				debug->Write(_T("���� LOAD ���� ����Ÿ ���"));
				debug->Write(_T("��ü����Ÿũ��:%d"), view_write.GetLength());
				debug->Write(_T("�����ڵ�:%s"),view_write.Mid(0,10));
				debug->Write(_T("����޽���:%s"),view_write.Mid(10,50));
				debug->Write(_T("Padding:%s"),view_write.Mid(60,4));
			}
			debug->Write(_T("============���� �Ϸ�============="));

		}break;
	}
	debug->Write(_T("============��� ���� ==============="));
	ServerWrite(MAINGDISTRING_1);
}