
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

private:
	HWND hWnd;
	//����� ���
	void DebugEditCall(CString str);
public:
	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//

	CString   ChangeExt(CString sPathName, CString sNewExt, char find=_T('\\'));

	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ControlDestroy();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd);
	//Dlg �׸��� �Լ�
	virtual void DrawPaint();
	//�׸��� �ٽ� �׸��� ���ؼ� ȣ��
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
