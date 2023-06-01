
#ifndef __SUPERIMGMOVESTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERIMGMOVESTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


class CSuperImgMoveStatic : public CWindowImpl<CSuperImgMoveStatic, CStatic>, public COwnerDraw<CSuperImgMoveStatic>,public CMessageFilter
{
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	//폰트 시작 포인트
	CPoint fontpoint;
	//비트맵 이미지 저장
	CAtlmapStorageBitmap imagestorage;
	//자신의 네임
	CString ownname;
	//이미지 카운트
	UINT ownnamecount;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperImgMoveStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperImgMoveStatic)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_SETCURSOR(OnSetCursor)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSuperImgMoveStatic>, 1)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//생성자
	CSuperImgMoveStatic(void);
	//소멸자
	~CSuperImgMoveStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//커서 메시지
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	//버튼 초기화 함수
	void initSuperImgMoveStatic(CString _ownname, UINT ImageId, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
#ifdef WINCE
	void initSuperImgMoveStatic(CString _ownname, UINT ImageId, LPCWSTR type, BOOL bsize=TRUE);
#endif //WINCE
	void initSuperImgMoveStatic(CString _ownname, CBitmap bit, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
	//Owner Draw 호출 함수
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//텍스트를 쓰는 함수
	void DrawImgMoveStaticWhiteText(CDCHandle* pDC, CSize winSize, CString text);
	//텍스트 설정
	void SetText(CString text);
	//서브클래싱 함수
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//버튼 생성
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//폰트 설정
	void SetSuperImgMoveStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperImgMoveStaticFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL);
	//이미지 삭제
	void ImgMoveStaticClear();
	//
	BOOL ImgMoveStaticDivideHbitmap(CString Imagename, CBitmap hbit, BOOL bViewChange=TRUE);
#ifdef WINCE
	BOOL ImgMoveStaticMakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
	//이미지 추가
	BOOL AddImageImgMoveStatic(CBitmap bit);
	//보여질 이미지 선택하지 않으며 저장
	BOOL AddImageNoChangeBitmapp(CBitmap bit);
	//서버에서 받은 이미지 추가
	BOOL AddImageServerRecvImgMoveStatic(CString Imagename, CBitmap bit);
	//가지고 있는 이미지 바꿔가면서 보여주기
	//이미지 이름 중에서 첫번째 보여주기
	BOOL ShowFirstImageBitmap();
	//이미지 중에서 다음 검색되는 이미지 보여주기
	BOOL ShowNextImageBitmap();
	//현재 보여지는 이미지 파일로 만들기
	BOOL MakeSendFileJpg(CString& filename);
	//받으려고 하는 파일이 있는가
	BOOL RecvImageFileFind(CString filename);
	//마우스 위치가 내가 그려진 영역에 있는가
	BOOL MouseCursorArea(CPoint _pt);
};
#endif// __SUPERIMGMOVESTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
