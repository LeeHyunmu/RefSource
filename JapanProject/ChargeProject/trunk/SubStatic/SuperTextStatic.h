
#ifndef __SUPERTEXTSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERTEXTSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����


class CSuperTextStatic : public CWindowImpl<CSuperTextStatic, CStatic>, public CMessageFilter
{
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	BOOL bNeedImage;
	int mgnum;
	//��Ʈ�� ��ư �̹��� ����
	CBitmapHandle staticTextimage;
	//  The invisible DC to draw on.
	CMemDC* m_memDC;
public:
	DECLARE_WND_SUPERCLASS(_T("SuperTextStatic"), _T("STATIC"))
	BEGIN_MSG_MAP_EX(CSuperTextStatic)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MSG_WM_PAINT( OnPaint )
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()
	//������
	CSuperTextStatic(void);
	//�Ҹ���
	~CSuperTextStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//��ư �ʱ�ȭ �Լ�
	void initSuperTextStatic(UINT ImageId);
#ifdef WINCE
	void initSuperTextStatic(UINT ImageId, LPCWSTR type);
#endif //WINCE
	void initSuperTextStatic(CBitmap bit);
	//��ư �׸���;
	void DrawTextStatic(CDC * pDC);
	//�ؽ�Ʈ�� ���� �Լ�
	void DrawWhiteText(CDC* pDC, CRect textrc, CString text);
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//��Ʈ ����
	void SetSuperTextStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	//�̹��� ����
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmap hbit);
	//
	void OnPaint(CDCHandle dc);
#ifdef WINCE
	BOOL MakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
};
#endif// __SUPERTEXTSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
