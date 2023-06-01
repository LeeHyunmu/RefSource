
#ifndef __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수
#define BUT_MAXNUM	3
class CSuperButton : public CWindowImpl<CSuperButton, CButton>, public COwnerDraw<CSuperButton>,public CMessageFilter
{
	//
	typedef vector<CBitmapHandle> VECBITHANDLE;
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	BOOL bNeedImage;
	int mgnum;
	CRect rgnbox;
	//캡션 string
	CString caption;
	//비트맵 버튼 이미지 저장
	VECBITHANDLE butimage;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperButton"), _T("BUTTON"))
	BEGIN_MSG_MAP_EX(CSuperButton)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSuperButton>, 1)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//생성자
	CSuperButton(void);
	//소멸자
	~CSuperButton(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void init(CString _caption, UINT ResourceId);
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//버튼 그리기;
	void DrawButton(CDC * pDC, UINT state);
	//텍스트를 쓰는 함수
	void DrawWhiteText(CDC* pDC, CRect* pRect, CString& text );
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//폰트 설정
	void SetSuperButFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	//이미지 삭제
	void butClear();
	//
	BOOL DivideHbitmap(CBitmap hbit);
};
#endif// __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__
