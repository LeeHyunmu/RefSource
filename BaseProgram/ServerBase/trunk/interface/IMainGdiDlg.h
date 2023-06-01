
#ifndef __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IMainGdiDlg
{
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ControlDestroy()=0;
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg �׸��� �Լ�
	virtual void DrawPaint()=0;
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID)=0;
};

#endif //__MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__