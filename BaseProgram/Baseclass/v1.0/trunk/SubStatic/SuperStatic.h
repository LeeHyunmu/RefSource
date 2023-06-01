
#ifndef __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


class CSuperStatic : public CWindowImpl<CSuperStatic, CStatic>, public COwnerDraw<CSuperStatic>,public CMessageFilter
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
	VECBITHANDLE staticimage;
	CRgnHandle m_hRgn;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperStatic)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSuperStatic>, 1)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//생성자
	CSuperStatic(void);
	//소멸자
	~CSuperStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//버튼 초기화 함수
	void initSuperStatic(UINT ImageId, BOOL bOwnerDraw=TRUE);
#ifdef WINCE
	void initSuperStatic(UINT ImageId, LPCWSTR type, BOOL bOwnerDraw=TRUE);
#endif //WINCE
	void initSuperStatic(CBitmapHandle bit, BOOL bOwnerDraw=TRUE);
	//Rgn  초기화 함수
	void initRgnSuperStatic(UINT ImageId, UINT MaskId, BOOL bOwnerDraw=TRUE);
	//Owner Draw 호출 함수
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//버튼 그리기;
	void DrawStatic(CDC * pDC, UINT state);
	//텍스트를 쓰는 함수
	void DrawWhiteText(CDC* pDC, SIZE bmSize);
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//폰트 설정
	void SetSuperStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	//이미지 삭제
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmapHandle hbit);
	//Rgn 
	BOOL RgnDivideHbitmap(CBitmapHandle hbit);
	//이미지 추가 함수
	void AddImagedata(CBitmapHandle bit);
	//
	void AddImagedata(UINT ImageId);
	//Rgn 이미지 추가
	void AddImagedataRgn(UINT ImageId, UINT MaskId);
#ifdef WINCE
	BOOL MakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
};
#endif// __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
