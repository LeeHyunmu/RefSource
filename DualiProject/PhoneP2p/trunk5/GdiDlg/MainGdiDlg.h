
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

interface IImagingFactory;
class CMainGdiDlg : public IMainGdiDlg
{
public:
	//������
	CMainGdiDlg();
	//�Ҹ���
	~CMainGdiDlg();
	//=================������ �Լ�===========================//
	//=======================================================//
	//			��Ʈ�� ����
	//======================================================//
	//CSuperButton but1;
	//=======================================================//
	//			���� ����
	//======================================================//
	HWND hWnd;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//IDualiP2pMg*  dualip2p;
	IDualiCardMg* dualicard;
	//======================================================//
	//			�������̽� ������ 
	//======================================================//

	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	void DrawWhiteText(CDC* pDC, CRect textrc, CString text);
	//=======================================================//
	//======================================================//
	//			����
	//======================================================//
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//
	UINT MgNum;
	//
	UINT MgMode;
	//
	IImagingFactory* pImgFactory;
	//��� ���� ��ư
	CSuperButton ModeChangeBut;
	//��ư
	CSuperButton HospitalBut_1;
	//��ư
	CSuperButton HospitalBut_2;
	//��ư
	CSuperButton HospitalBut_3;
	//
	UINT hospitalnum;
	//
	UINT ButShow;
	UINT backButShow;
	//��� 3���� ��ȭ��ȣ�� �ؽ�Ʈ�� ����ϴ� ��
	CString strMode3;
	CRect   rcMode3;
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
	//
	virtual void StaticText(CDCHandle dc);
	//�ı�
	virtual void  ControlDestroy();
	//�׸��� �ٽ� �׸��� ���ؼ� ȣ��
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
	//�̹��� ���� �� ����Ÿ�� ����(�޴�������)
	virtual BOOL ImageChange(UINT _MgNum);
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//Ÿ�̸� �޽���
	virtual void TimerMessage();
	//��� ���� ���� ó�� Ȯ�� �Լ�
	virtual BOOL GetModeException();
	//���3���� �ؽ�Ʈ ����Լ�
	virtual void SetMode3Text(CString text, CRect* rc);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
