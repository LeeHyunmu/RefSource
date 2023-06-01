
#ifndef __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����


class CSuperStatic : public CWindowImpl<CSuperStatic, CStatic>, public COwnerDraw<CSuperStatic>,public CMessageFilter
{
	//
	typedef vector<CBitmapHandle> VECBITHANDLE;
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//�׷��� �̹��� �ѹ�
	int mgnum;
	//��Ʈ�� ��ư �̹��� ����
	VECBITHANDLE staticimage;
	//��Ʈ ���� ����Ʈ
	CPoint fontpoint;
	//��� ���� ����
	BOOL btransparent;
	//�׸��� ���İ�
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
	//������
	CSuperStatic(void);
	//�Ҹ���
	~CSuperStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//��ư �ʱ�ȭ �Լ�
	void initSuperStatic(UINT ImageId, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
#ifdef WINCE
	void initSuperStatic(UINT ImageId, LPCWSTR type, BOOL bsize=TRUE);
#endif //WINCE
	void initSuperStatic(CBitmap bit, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
	//Owner Draw ȣ�� �Լ�
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//�̹��� �׸���;
	void DrawStatic(CDC * pDC, UINT state);
	//�ؽ�Ʈ�� ���� �Լ�
	void DrawWhiteText(CDCHandle* pDC, CSize winSize, CString text);
	//�ؽ�Ʈ ����
	void SetText(CString text);
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//��Ʈ ����
	void SetSuperStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperStaticFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL, BOOL* _btransparent=NULL);
	//�̹��� ����
	void StaticClear();
	//
	BOOL DivideHbitmap(CBitmap hbit);
#ifdef WINCE
	BOOL MakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
	//
	void DrawTransperImage(CDC dc);
	//�̹��� �߰�
	BOOL AddImage(CBitmap bit, COLORREF* _drawalpha=NULL);
	//�̹����� ī���ؼ� �ѱ��
	CBitmap GetSuperStaticBitmap(int imgnum);
	LRESULT KeyHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled )    
	{   
		return 0;  
	} 
};
#endif// __SUPERSTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
