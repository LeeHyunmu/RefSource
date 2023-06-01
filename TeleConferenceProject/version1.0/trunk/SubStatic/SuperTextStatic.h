
#ifndef __SUPERTEXTSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERTEXTSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


class CSuperTextStatic : public CWindowImpl<CSuperTextStatic, CStatic>, public CMessageFilter
{
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	//비트맵 버튼 이미지 저장
	CBitmapHandle staticTextimage;
	//  The invisible DC to draw on.
	CMemDC* m_memDC;
	//폰트 시작 포인트
	CPoint fontpoint;
	//배경 투명 설정
	BOOL btransparent;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperTextStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperTextStatic)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MSG_WM_PAINT( OnPaint )
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//생성자
	CSuperTextStatic(void);
	//소멸자
	~CSuperTextStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//메모리에서 제거
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//버튼 초기화 함수
	void initSuperTextStaticBitmap(UINT ImageId, BOOL bsize=TRUE);
#ifdef WINCE
	void initSuperTextStatic(UINT ImageId, LPCWSTR type, BOOL bsize=TRUE);
#endif //WINCE
	void initSuperTextStatic(CBitmap bit, BOOL bsize=TRUE);
	//버튼 그리기;
	void DrawTextStatic(CDC * pDC,const CRect rc);
	//텍스트를 쓰는 함수
	void DrawWhiteText(CDC* pDC, CSize winSize, CString text);
	//텍스트 설정
	void SetText(CString text);
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//폰트 설정
	void SetSuperTextStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperTextStaticFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL,  BOOL* _btransparent=NULL);
	//이미지 삭제
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmap hbit);
	//======================================================//
	//			Paint
	//======================================================//
	void OnPaint( HDC );
#ifdef WINCE
	BOOL MakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
};
#endif// __SUPERTEXTSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
