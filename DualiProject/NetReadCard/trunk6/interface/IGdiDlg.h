
#ifndef __IGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IGdiDlg
{
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release()=0;
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg �׸��� �Լ�
	virtual void DrawPaint()=0;
	//�� ä���
	virtual void SetDrawItem(int index, COLORREF _color=RGB(0xff,0x00,0x00))=0;
	//�ؽ�Ʈ �߰�(����Ʈ�ڽ�)
	virtual void SetListString(CString _string)=0;
	//VIEW �ؽ�Ʈ ũ�� ����
	virtual void StaticText(CDCHandle dc)=0;
	//VIEW �ؽ�Ʈ ����
	virtual void SetStaticString(CString _string)=0;
	//
	virtual void SetTerminalStaticString(CString _string)=0;
};

#endif //__IGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__