
#ifndef __SUPERCOMBO_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERCOMBO_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// if( !m_pAdWin->Create(m_hWnd, rcCombo, NULL,
// 
//    WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
// 
//    | WS_TABSTOP | CBS_NOINTEGRALHEIGHT
// 
//    | CBS_OWNERDRAWFIXED | CCS_NOMOVEY
// 
//    | CBES_EX_NOSIZELIMIT) ){
// 
// 	   return FALSE;
// 
// }


class CSuperCombo : public CWindowImpl<CSuperCombo, CComboBox>
	,public COwnerDraw<CSuperCombo>
	,public CMessageFilter
{
public:
	DECLARE_WND_SUPERCLASS(_T("SuperCombo"), CComboBox::GetWndClassName())
	BEGIN_MSG_MAP_EX(CSuperCombo)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSuperCombo>, 1)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()

	enum  
	{ 
		s_cxIndent = 2, 
		s_cyIndent = 1 
	}; 

	//생성자
	CSuperCombo(void);
	//소멸자
	~CSuperCombo(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//버튼 초기화 함수

	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
 	int AddColor(int iIndex, COLORREF clr);
 	BOOL RemoveColor(COLORREF clr);
	BOOL ChangeColor(int iIndex, COLORREF clr);
	BOOL SelectColor(COLORREF clr);
	COLORREF GetSelectedColor() const;
 	int FindColor(COLORREF clr) const;
 	void _Init();
 	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};
#endif// __SUPERCOMBO_B7A15A9D01DC403996FC45F7F92F52D3_H__
