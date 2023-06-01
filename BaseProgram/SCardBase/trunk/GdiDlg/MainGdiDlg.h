
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainGdiDlg : public IMainGdiDlg, public ISCardMsg
{
public:
	//������
	CMainGdiDlg();
	//�Ҹ���
	~CMainGdiDlg();
	//=================������ �Լ�===========================//

	//=======================================================//
	//			���� ����
	//======================================================//
	//������ �ڵ�
	HWND hWnd;
	//VIDPID
	CComboBox comvpid;
	//Base APDU
	CComboBox combaseapdu;
	//����Ʈī�� �̸�
	CListBox  listscardname;
	//����Ʈī�� USB  ID
	CListBox  listUsbid;
	//���ؼ� ī��
	CStatic    staticconnect;
	//APDU ��
	CEdit    editapdu;
	//����Ʈī�� ���� ��ư
	CButton  butconnect;
	//����Ʈī�� ������ ���´�
	CButton  butdisconnect;
	//APDU ����
	CButton  butapud;
	//APDU RECV ������ ���
	CListBox  listapdurecv;
	//���ؼ� ����
	BOOL bSCardConnect;
	//����Ʈī�� �ڵ�
	SCARDHANDLE hCard;
	//����Ʈī�� ��� ��������
	DWORD ActiveProtocol;
	//���� ����Ʈī�� �̸�
	CString ConnectSCardName;
	//======================================================//b
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//����Ʈī�� ��ſ� ��ü
	ISCardTm* scardtm;
	//======================================================//
	//			�������̽� ������ 
	//======================================================//

	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	//����Ʈī���� ��ȭ�� ������ ��
	void NewSCardListMsg();
	//�����ϰ� �ִ� ����Ʈī�尡 ���� �Ǿ��� ��
	void SCardDiConnectMsg(CString SCardName);
	//����üũ ��� �Լ�
	void ViewSmartCardConnectLastError();
	//APDU �� ���� ��� 
	void CMainGdiDlg::Write(TCHAR* format,...);
	//======================================================//
	//			�⺻ APDU
	//======================================================//
	//���� �̸� ��������
	void LoadBaseApduName();
	//APDU���� �ѱ��
	BOOL FindBaseApdu();
	//=======================================================//
	//======================================================//
	//			����
	//======================================================//
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//����Ʈī�� ���� ����
	vecString vecVidPid;
	vecString vecUsbid;
	vecString vecSCardName;
	//���Ͽ��� ����  APDU ���ð� ����
	vecString vecDefalutAPDU;
	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd);
	//Dlg �׸��� �Լ�
	virtual void DrawPaint();
	//�޺��ڽ� IDC_VIDPID �޽���
	virtual void ComboboxVidPidMessage(UINT uNotifyCode, int nID);
	//�޺��ڽ� IDC_APDUSHORT �޽���
	virtual void ComboboxApduShortMessage(UINT uNotifyCode, int nID);
	//����Ʈ�ڽ� IDC_PSCARDNMAE �޽���
	virtual void ListboxSCardNameMessage(UINT uNotifyCode, int nID);
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual void ButtonClickedMessage(UINT uNotifyCode, int nID);
	//Static Control CtlColor ����
	virtual HBRUSH StaticCtlColorMessage(CDCHandle dc, CStatic wndStatic);
	//======================================================//
	//			����Ʈī�� ��ȭ ���� �Լ�
	//======================================================//
	//��� ���� Ŭ�������� ����Ʈī�� ��ġ�� ��ȭ�� �˰� ���� ��
	//flag���� ���� ó�� ����� �ٸ��� ����
	virtual void DeviceListChangeMsg(int flag, CString SCardName=_T(""));
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
