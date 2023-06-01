
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
	//그려줄 이미지 넘버
	int mgnum;
	//비트맵 버튼 이미지 저장
	VECBITHANDLE staticimage;
	//폰트 시작 포인트
	CPoint fontpoint;
	//배경 투명 설정
	BOOL btransparent;
	//그리기 알파값
	COLORREF drawalpha;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperStatic)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_KEYUP, KeyHandler) 
		MESSAGE_HANDLER(WM_KEYDOWN, KeyHandler) 
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
	void initSuperStatic(UINT ImageId, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
#ifdef WINCE
	void initSuperStatic(UINT ImageId, LPCWSTR type, BOOL bsize=TRUE);
#endif //WINCE
	void initSuperStatic(CBitmap bit, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
	//Owner Draw 호출 함수
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//이미지 그리기;
	void DrawStatic(CDC * pDC, UINT state);
	//텍스트를 쓰는 함수
	void DrawWhiteText(CDCHandle* pDC, CSize winSize, CString text);
	//텍스트 설정
	void SetText(CString text);
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//폰트 설정
	void SetSuperStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperStaticFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL, BOOL* _btransparent=NULL);
	//이미지 삭제
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmap hbit);
#ifdef WINCE
	BOOL MakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
	//
	void DrawTransperImage(CDC dc);
	//이미지 추가
	BOOL AddImage(CBitmap bit, COLORREF* _drawalpha=NULL);
	//이미지를 카피해서 넘긴다
	CBitmap GetSuperStaticBitmap(int imgnum);
	LRESULT KeyHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )    
	{   
		return 0;  
	} 
};
#endif// __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
