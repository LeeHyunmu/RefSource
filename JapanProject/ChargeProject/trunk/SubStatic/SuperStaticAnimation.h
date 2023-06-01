
#ifndef __SUPERSTATICANIMATION_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERSTATICANIMATION_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����


class CSuperStaticAnimation : public CWindowImpl<CSuperStaticAnimation, CStatic>, public COwnerDraw<CSuperStaticAnimation>,public CMessageFilter
{
	LONG xpos;
	LONG ypos;
	LONG width;
	LONG Height;
	//
	typedef vector<CBitmapHandle> VECBITHANDLE;
	typedef vector<CRgnHandle> VECRGNHANDLE;
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	BOOL bNeedImage;
	int mgnum;
	//��Ʈ�� ��ư �̹��� ����
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
	//������
	CSuperStaticAnimation(void);
	//�Ҹ���
	~CSuperStaticAnimation(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//��ư �ʱ�ȭ �Լ�
	void initSuperStatic(UINT ImageId, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
#ifdef WINCE
	void initSuperStatic(UINT ImageId, LPCTSTR type, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
#endif //WINCE
	void initSuperStatic(CBitmapHandle bit, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
	//Rgn  �ʱ�ȭ �Լ�
	void initRgnSuperStatic(UINT ImageId, UINT MaskId, UINT imgcount, BOOL _bXCount=FALSE, BOOL bOwnerDraw=TRUE);
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
	//�̹��� ����
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmapHandle hbit, UINT imgcount, BOOL _bXCount);
	//
	BOOL SetAnimation();
};
#endif// __SUPERSTATICANIMATION_B7A15A9D01DC403996FC45F7F92F52D3_H__
