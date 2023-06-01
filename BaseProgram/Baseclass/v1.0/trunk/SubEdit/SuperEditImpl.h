
#ifndef __SUPEREDITIMPL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPEREDITIMPL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSuperEditImpl : public CWindowImpl<CSuperEditImpl, CEdit>
{
	typedef CWindowImpl<CSuperEditImpl, CEdit> baseClass;
	BEGIN_MSG_MAP(CSuperEditImpl)
	DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	// data member
private:
	static CSuperEditImpl * pThis;
	HHOOK m_hHook;
public:
	// c'tor
	CSuperEditImpl();
	// overrides
public:
	virtual void PreSubclassWindow(HWND hWnd);
	HWND Create(HWND hWndParent, RECT& rcPos, LPCTSTR szWindowName = NULL,
		DWORD dwStyle = 0, DWORD dwExStyle = 0,	UINT nID = 0, LPVOID lpCreateParam = NULL);
	BOOL SubclassWindow(HWND hWnd);
	static LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam);
};
#endif// __SUPEREDITIMPL_B7A15A9D01DC403996FC45F7F92F52D3_H__
