
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainGdiDlg : public IGdiDlg
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
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//======================================================//
	//			�������̽� ������ 
	//======================================================//

	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//

	//=======================================================//
	//======================================================//
	//			����
	//======================================================//
	HRGN hrgn[3];
	COLORREF color[3];
	CListBox List;
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//
	CStatic stc;
	//
	CStatic terminalstc;
	//
	CBitmap Aqlogo;
	CPoint  AqImgsize;
	CBitmap AqMainlogo;
	CPoint  AqMainImgsize;

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
	//�� ä���
	virtual void SetDrawItem(int index, COLORREF _color=RGB(0xff,0x00,0x00));
	//�ؽ�Ʈ �߰�(����Ʈ�ڽ�)
	virtual void SetListString(CString _string);
	//
	virtual void StaticText(CDCHandle dc);
	//
	virtual void SetStaticString(CString _string);
	//
	virtual void SetTerminalStaticString(CString _string);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
