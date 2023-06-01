
#ifndef __SUPERSTATICANIMATION_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERSTATICANIMATION_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


class CSuperStaticAnimation : public CWindowImpl<CSuperStaticAnimation, CStatic>, public COwnerDraw<CSuperStaticAnimation>,public CMessageFilter
{
	LONG xpos;
	LONG ypos;
	LONG width;
	LONG Height;
	//
	typedef vector<CBitmapHandle> VECBITHANDLE;
	typedef vector<CRgnHandle> VECRGNHANDLE;
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
	DECLARE_WND_SUPERCLASS(_T("SuperStaticAnimation"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperStaticAnimation)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSuperStaticAnimation>, 1)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//생성자
	CSuperStaticAnimation(void);
	//소멸자
	~CSuperStaticAnimation(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//버튼 초기화 함수
	void initSuperStatic(UINT ImageId, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
#ifdef WINCE
	void initSuperStatic(UINT ImageId, LPCTSTR type, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
#endif //WINCE
	void initSuperStatic(CBitmapHandle bit, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
	//Rgn  초기화 함수
	void initRgnSuperStatic(UINT ImageId, UINT MaskId, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
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
	//이미지 삭제
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmapHandle hbit, UINT imgcount, BOOL _bXCount);
	//
	BOOL SetAnimation();
};
#endif// __SUPERSTATICANIMATION_B7A15A9D01DC403996FC45F7F92F52D3_H__
