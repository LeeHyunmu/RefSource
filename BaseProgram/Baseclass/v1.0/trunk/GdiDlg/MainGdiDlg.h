
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainGdiDlg : public IMainGdiDlg
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
	//��� ���� ��ư
	CSuperStatic TestStatic;
	//
	CSuperEditImpl TestEdit;

	CSuperCombo TestCombo;

	CSuperListBox TestListBox;
	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	//�׸��� �ٽ� �׸��� ���ؼ� ȣ��
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
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
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__