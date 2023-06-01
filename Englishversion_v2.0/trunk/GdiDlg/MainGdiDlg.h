
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CWTLIExplorer;
class CCouponPopDlg;
class CCouponDlg;
class CContentsMallDlg;
class CContentsMallPopDlg;
class CAdvertsDlg;
class CCompleteDlg;
class CMainGdiDlg : public IMainGdiDlg
{
	CSuperButton videochange;
public:
	//������
	CMainGdiDlg();
	//�Ҹ���
	~CMainGdiDlg();
	//=================������ �Լ�===========================//

	//=======================================================//
	//			���� ����
	//======================================================//
	HWND hWnd;
	ULONG_PTR m_gdiplusToken;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//======================================================//
	//			��ư ��ü
	//======================================================//
	CComPtr<WMPLib::IWMPPlayer4> pWMP;
	CComPtr<WMPLib::IWMPControls> pCtrl;
	CComPtr<WMPLib::IWMPSettings> pSetting;
	//
	CSuperStatic mpw;

	//CWTLIExplorer* pBrowser;
	//���� dlg
	CCouponDlg* CouponDlg;
	//contents dlg
	CContentsMallDlg* contensmall;

	//AQ ���� dlg
	CAdvertsDlg* AQadvertsDlg;

	//��� ����
	CString FilePath;
	int filenum;

	//
	CCouponPopDlg* coupondlg;
	//
	CContentsMallPopDlg* mallPop;
	//�Ϸ� â
	CCompleteDlg* comdlg;
	//aq ����̽�
	CDeviceCmd AqDevice;
	//|=============================================
	//|ī�� �޽��� ���� ����
	//|==================================================
	int NFCCommandNum;
	BOOL bCouponClick;
	BOOL bContentsMallClick;
	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	//�׸��� �ٽ� �׸��� ���ؼ� ȣ��
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);

	CString   ChangeExt(CString sPathName, CString sNewExt, char find=_T('\\'));

	//=======================================================//
	//======================================================//
	//			����
	//======================================================//
	CListBox List;
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;

	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ControlDestroy();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd);
	//Dlg �׸��� �Լ�
	virtual void DrawPaint();
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);

	//
	virtual BOOL MediaPlayTimer(UINT_PTR nIDEvent);
	//NFC Call
	virtual BOOL NfcEventCall(WPARAM wParam);
	//
	//NDEFData Change Message
	virtual BOOL DeviceCmdCallMessage(WPARAM wParam);
	//UI ���� �̺�Ʈ �ޱ� ���� �Լ�
	virtual BOOL NfcAqDeviceMessage(WPARAM wParam);
	//UI CouponClick Msg
	virtual BOOL CouponClickMessage(WPARAM wParam);
	//
	//virtual BOOL CardFindConnectLoop(DWORD* conectflag);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
