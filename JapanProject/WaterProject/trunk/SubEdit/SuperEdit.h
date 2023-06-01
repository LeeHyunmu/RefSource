
#ifndef __SUPEREDIT_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPEREDIT_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSuperEdit : public CWindowImpl<CSuperEdit, CEdit>
	,public CEditCommands<CSuperEdit>
	,public CMessageFilter
{
	//커서 아이콘 아이디 저장
	UINT uCursorID;
public:
	//생성자
	CSuperEdit(void);
	//소멸자
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
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void PreSubclassWindow(HWND hWnd);
};
#endif// __SUPEREDIT_B7A15A9D01DC403996FC45F7F92F52D3_H__
