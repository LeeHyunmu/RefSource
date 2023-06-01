
#ifndef __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����
#define BUT_MAXNUM	3
class CSuperButton : public CWindowImpl<CSuperButton, CButton>, public COwnerDraw<CSuperButton>,public CMessageFilter
{
	//
	typedef vector<CBitmapHandle> VECBITHANDLE;
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	BOOL bNeedImage;
	int mgnum;
	CRect rgnbox;
	//ĸ�� string
	CString caption;
	//��Ʈ�� ��ư �̹��� ����
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
	//������
	CSuperButton(void);
	//�Ҹ���
	~CSuperButton(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	void init(CString _caption, UINT ResourceId);
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//��ư �׸���;
	void DrawButton(CDC * pDC, UINT state);
	//�ؽ�Ʈ�� ���� �Լ�
	void DrawWhiteText(CDC* pDC, CRect* pRect, CString& text );
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//��Ʈ ����
	void SetSuperButFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	//�̹��� ����
	void butClear();
	//
	BOOL DivideHbitmap(CBitmap hbit);
};
#endif// __SUPERBUTTON_B7A15A9D01DC403996FC45F7F92F52D3_H__
