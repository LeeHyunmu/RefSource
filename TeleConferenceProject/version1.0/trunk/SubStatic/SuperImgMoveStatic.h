
#ifndef __SUPERIMGMOVESTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SUPERIMGMOVESTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����


class CSuperImgMoveStatic : public CWindowImpl<CSuperImgMoveStatic, CStatic>, public COwnerDraw<CSuperImgMoveStatic>,public CMessageFilter
{
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//��Ʈ ���� ����Ʈ
	CPoint fontpoint;
	//��Ʈ�� �̹��� ����
	CAtlmapStorageBitmap imagestorage;
	//�ڽ��� ����
	CString ownname;
	//�̹��� ī��Ʈ
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
	//������
	CSuperImgMoveStatic(void);
	//�Ҹ���
	~CSuperImgMoveStatic(void);
	//
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//Ŀ�� �޽���
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	//��ư �ʱ�ȭ �Լ�
	void initSuperImgMoveStatic(CString _ownname, UINT ImageId, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
#ifdef WINCE
	void initSuperImgMoveStatic(CString _ownname, UINT ImageId, LPCWSTR type, BOOL bsize=TRUE);
#endif //WINCE
	void initSuperImgMoveStatic(CString _ownname, CBitmap bit, BOOL bOwnerDraw=TRUE, BOOL bsize=TRUE);
	//Owner Draw ȣ�� �Լ�
	void DrawItem(LPDRAWITEMSTRUCT lpdis);
	//�ؽ�Ʈ�� ���� �Լ�
	void DrawImgMoveStaticWhiteText(CDCHandle* pDC, CSize winSize, CString text);
	//�ؽ�Ʈ ����
	void SetText(CString text);
	//����Ŭ���� �Լ�
	BOOL SubclassWindow(HWND hWnd);
	HWND UnsubclassWindow(BOOL bForce = FALSE);
	//��ư ����
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDCHandle dc);
	//��Ʈ ����
	void SetSuperImgMoveStaticFont(LOGFONT* fontinfo=NULL,COLORREF* fontcolor=NULL);
	void SetSuperImgMoveStaticFont(UINT lfHeight, CString lfFaceName=_T(""), COLORREF* _fontcolor=NULL, CPoint* _fontpoint=NULL);
	//�̹��� ����
	void ImgMoveStaticClear();
	//
	BOOL ImgMoveStaticDivideHbitmap(CString Imagename, CBitmap hbit, BOOL bViewChange=TRUE);
#ifdef WINCE
	BOOL ImgMoveStaticMakeResource(HMODULE hMod, UINT resId, LPCWSTR type);
#endif //WINCE
	//�̹��� �߰�
	BOOL AddImageImgMoveStatic(CBitmap bit);
	//������ �̹��� �������� ������ ����
	BOOL AddImageNoChangeBitmapp(CBitmap bit);
	//�������� ���� �̹��� �߰�
	BOOL AddImageServerRecvImgMoveStatic(CString Imagename, CBitmap bit);
	//������ �ִ� �̹��� �ٲ㰡�鼭 �����ֱ�
	//�̹��� �̸� �߿��� ù��° �����ֱ�
	BOOL ShowFirstImageBitmap();
	//�̹��� �߿��� ���� �˻��Ǵ� �̹��� �����ֱ�
	BOOL ShowNextImageBitmap();
	//���� �������� �̹��� ���Ϸ� �����
	BOOL MakeSendFileJpg(CString& filename);
	//�������� �ϴ� ������ �ִ°�
	BOOL RecvImageFileFind(CString filename);
	//���콺 ��ġ�� ���� �׷��� ������ �ִ°�
	BOOL MouseCursorArea(CPoint _pt);
};
#endif// __SUPERIMGMOVESTATIC_B7A15A9D01DC403996FC45F7F92F52D3_H__
