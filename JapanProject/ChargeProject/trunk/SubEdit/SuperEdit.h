
#ifndef __SUPEREDIT_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPEREDIT_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSuperEdit : public CWindowImpl<CSuperEdit, CEdit>
	,public CEditCommands<CSuperEdit>
	,public CMessageFilter
{
	//Ŀ�� ������ ���̵� ����
	UINT uCursorID;
public:
	//������
	CSuperEdit(void);
	//�Ҹ���
	~CSuperEdit(void);

	DECLARE_WND_SUPERCLASS(_T("SuperEdit"), CEdit::GetWndClassName())
	BEGIN_MSG_MAP_EX(CSuperEdit)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_SETCURSOR(OnSetCursor)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnSetCursor(HWND hwndCtrl, UINT uHitTest, UINT uMouseMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void init();
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void PreSubclassWindow(HWND hWnd);
};
#endif// __SUPEREDIT_B7A15A9D01DC403996FC45F7F92F52D3_H__
