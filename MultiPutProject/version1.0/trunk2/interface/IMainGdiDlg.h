
#ifndef __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IMainGdiDlg
{
	//�������� �Ҹ���
	virtual ~IMainGdiDlg()=0;
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ControlDestroy()=0;
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd)=0;
	//Dlg �׸��� �Լ�
	virtual void MainGdiDlgDrawPaint()=0;
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID)=0;

	virtual BOOL CtlColorStatic(CDCHandle dc, CStatic wndStatic, HBRUSH& hbrush)=0;
	//���콺 �̺�Ʈ �ޱ�
	virtual BOOL MouseEventMessage(UINT Msgtype, CPoint cuserpos)=0;
	//Hot Key Message
	virtual BOOL HotKeyMessage(UINT KeyId)=0;
	//cursor Message
	virtual BOOL MouseCursorMessage(CWindow wnd, UINT nHitTest, UINT message)=0;
	//Key Message
	virtual BOOL VirtualKeyMessage(UINT Msgtype, UINT nChar, UINT nRepCnt)=0;
	//==================char type (hex, byte, string) ��ȯ �Լ�===========================//
	//|char�� hex�� ��ȯ
	virtual CString Changcharbyhex(CString pass)=0;
	//|ī��� ���� ���� ����Ÿ�� ��ȯ �Ѵ�(0x00~0xFF���� �̹Ƿ� ��Ʈ�������� ��ȯ�ؼ� �Ǵ��Ѵ�)
	virtual BOOL ChangBytebyhex(char* pass, DWORD len,  CString& repass)=0;
	//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
	virtual int  ChangeHexCord(CString data, char hex[])=0;
	//������ �����Ѵ�
	virtual BOOL ServerStartCall()=0;
};

#endif //__MAINIGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__