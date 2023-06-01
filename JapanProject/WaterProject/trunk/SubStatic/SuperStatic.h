
#ifndef __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����


class CSuperStatic : public CWindowImpl<CSuperStatic, CButton>, public COwnerDraw<CSuperStatic>,public CMessageFilter
{
	LONG xpos;
	LONG ypos;
	LONG width;
	LONG Height;
	//
	typedef vector<CBitmapHandle> VECBITHANDLE;
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	BOOL bNeedImage;
	int mgnum;
	//��Ʈ�� ��ư �̹��� ����
	VECBITHANDLE butimage;
	//Rgn
	CRgnHandle m_hRgn;
#ifndef WINCE
	//��ư�� �Ĺ��� ���� �Լ�
	BOOL m_bTracking;
	BOOL m_bHover;
#endif //WINCE
public:
	DECLARE_WND_SUPERCLASS(_T("SuperStatic"), _T("BUTTON"))
	BEGIN_MSG_MAP_EX(CSuperStatic)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		CHAIN_MSG_MAP_ALT(COwnerDraw<CSuperStatic>, 1)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//������
	CSuperStatic(void);
	//�Ҹ���
	~CSuperStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//��ư �ʱ�ȭ �Լ�
	void initSuperStatic(UINT ImageId, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw=TRUE);
#ifdef WINCE
	void initSuperStatic(UINT ImageId, LPCWSTR type, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw=TRUE);
#endif //WINCE
	void initSuperStatic(CBitmap bit, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw=TRUE);
	//Rgn ��ư �ʱ�ȭ �Լ�
	void initRgnSuperStatic(UINT ImageId, UINT MaskId, UINT imgcount, BOOL _bXCount, BOOL bOwnerDraw=TRUE);
	//Owner Draw ȣ�� �Լ�
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//��ư �׸���;
	void DrawStatic(CDC * pDC, UINT state);
	//�ؽ�Ʈ�� ���� �Լ�
	void DrawWhiteText(CDC* pDC, SIZE bmSize);
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//��Ʈ ����
	void SetSuperStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	//�̹��� ����
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmap hbit, UINT imgcount, BOOL _bXCount);
#ifdef WINCE
	BOOL MakeResource(HMODULE hMod, UINT resId, LPCWSTR type, UINT imgcount, BOOL _bXCount);
#endif //WINCE
};
#endif// __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
