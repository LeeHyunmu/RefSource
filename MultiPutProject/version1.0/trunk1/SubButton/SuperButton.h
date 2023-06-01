
#ifndef __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


#define BUT_MAXNUM	4
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
	//비트맵 버튼 이미지 저장
	CBitmap butoriginimage;
	//버튼 이미지 만들어 놓는다
	VECBITHANDLE butimage;
	//폰트 시작 포인트
	CPoint fontpoint;
	//배경 투명 설정
	//지정색 빼기
	BOOL btransperent;
	COLORREF transperentcolor;
#ifndef WINCE
	//버튼의 후버를 위한 함수
	BOOL m_bTracking;
	BOOL m_bHover;
#endif //WINCE
public:
	DECLARE_WND_SUPERCLASS(_T("SuperButton"), _T("BUTTON"))
	BEGIN_MSG_MAP_EX(CSuperButton)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
#ifndef WINCE
		MSG_WM_MOUSELEAVE(OnMouseLeave)
		MSG_WM_MOUSEHOVER(OnMouseHover)
		MSG_WM_MOUSEMOVE(OnMouseMove)
#endif //WINCE
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
	//버튼 초기화 함수
	void initSuperButton(UINT ImageId, BOOL bOwnerDraw=TRUE);
#ifdef WINCE
	void initSuperButton(UINT ImageId, LPCWSTR type, BOOL bOwnerDraw=TRUE);
#endif //WINCE
	void initSuperButton(CBitmap bit, BOOL bOwnerDraw=TRUE);
	//Owner Draw 호출 함수
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//버튼 그리기;
	void DrawButton(CDCHandle * pDC, UINT state);
	//텍스트를 쓰는 함수
	void DrawWhiteText(CDCHandle* pDC, CSize winSize, CString text);
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//폰트 설정
	void SetSuperButFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperButFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL);
	//이미지 삭제
	void butClear();
	//
	BOOL DivideHbitmap(HDC hdc, CBitmap hbit);
	//특정 색 빼기
	void SetTransperent(COLORREF _transperentcolor, BOOL _btransperent=TRUE);
#ifndef WINCE
	//버튼의 후버를 위한 함수
	void OnMouseHover(WPARAM wParam, CPoint ptPos);
	void OnMouseLeave();
	void OnMouseMove(UINT nFlags, CPoint point);
#endif //WINCE
#ifdef WINCE
	CBitmap MakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
};
#endif// __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__
