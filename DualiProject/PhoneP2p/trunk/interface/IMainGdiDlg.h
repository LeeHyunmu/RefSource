
#ifndef __IMAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

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
	virtual void Release()=0;
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg �׸��� �Լ�
	virtual void DrawPaint()=0;
	//VIEW �ؽ�Ʈ ũ�� ����
	virtual void StaticText(CDCHandle dc)=0;
	// ��Ʈ�� �ı�
	virtual void  ControlDestroy()=0;
};

#endif //__IMAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__