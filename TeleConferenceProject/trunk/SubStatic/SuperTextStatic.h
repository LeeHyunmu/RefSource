
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
	//��Ʈ�� ��ư �̹��� ����
	CBitmapHandle staticTextimage;
	//  The invisible DC to draw on.
	CMemDC* m_memDC;
	//��Ʈ ���� ����Ʈ
	CPoint fontpoint;
	//��� ���� ����
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
	//������
	CSuperTextStatic(void);
	//�Ҹ���
	~CSuperTextStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//�޸𸮿��� ����
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//��ư �ʱ�ȭ �Լ�
	void initSuperTextStaticBitmap(UINT ImageId, BOOL bsize=TRUE);
#ifdef WINCE
	void initSuperTextStatic(UINT ImageId, LPCWSTR type, BOOL bsize=TRUE);
#endif //WINCE
	void initSuperTextStatic(CBitmap bit, BOOL bsize=TRUE);
	//��ư �׸���;
	void DrawTextStatic(CDC * pDC,const CRect rc);
	//�ؽ�Ʈ�� ���� �Լ�
	void DrawWhiteText(CDC* pDC, CSize winSize, CString text);
	//�ؽ�Ʈ ����
	void SetText(CString text);
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//��Ʈ ����
	void SetSuperTextStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperTextStaticFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL,  BOOL* _btransparent=NULL);
	//�̹��� ����
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
