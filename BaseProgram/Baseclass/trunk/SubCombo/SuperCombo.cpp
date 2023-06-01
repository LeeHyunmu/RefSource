
#include "stdAfx.h"
#include <atldef.h>

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CSuperCombo::CSuperCombo(void)
{

}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CSuperCombo::~CSuperCombo(void)
{
}
//--------------------------------------------------------------
//|
//|서브 클래싱 함수
//|
//---------------------------------------------------------------
BOOL CSuperCombo::SubclassWindow(HWND hWnd)
{
	ATLASSERT(::IsWindow(hWnd));
	BOOL bRet = CWindowImpl<CSuperCombo, CComboBox>::SubclassWindow(hWnd);
	return bRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperCombo::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
//--------------------------------------------------------------
//|
//|서브 클래싱 해제
//|
//---------------------------------------------------------------
HWND CSuperCombo::UnsubclassWindow(BOOL bForce /*= FALSE*/)
{
	return CWindowImpl<CSuperCombo, CComboBox>::UnsubclassWindow(bForce);
}
//--------------------------------------------------------------
//|
//|버튼 Create 함수 
//|
//---------------------------------------------------------------
int CSuperCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	LRESULT lRes = DefWindowProc(); 
	//프리트랜스 메시지
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	_Init();
	return lRes; 
}

//--------------------------------------------------------------
//|
//|Create 했으면 Desroy 아니면 이상 발생(종료시)
//|
//---------------------------------------------------------------
LRESULT CSuperCombo::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	return 0;
}

//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
int CSuperCombo::AddColor(int iIndex, COLORREF clr) 
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
	int iItem = InsertString(iIndex, _T("")); 
	if( iItem == -1 ) return -1; 
	SetItemData(iItem, (DWORD) clr); 
	return iItem; 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperCombo::RemoveColor(COLORREF clr) 
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
	int iItem = FindColor(clr); 
	if( iItem == -1 ) return FALSE; 
	return DeleteString(iItem); 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperCombo::ChangeColor(int iIndex, COLORREF clr) 
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
	int iItem = FindColor(clr); 
	if( iItem == -1 ) return FALSE; 
	SetItemData(iItem, (DWORD) clr); 
	Invalidate(); 
	return TRUE; 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CSuperCombo::SelectColor(COLORREF clr) 
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
	int iItem = FindColor(clr); 
	if( iItem == -1 ) return FALSE; 
	return SetCurSel(iItem); 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
COLORREF CSuperCombo::GetSelectedColor() const 
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
	int iItem = GetCurSel(); 
	if( iItem == -1 ) return (COLORREF) -1; // Returns -1 if none if selected 
	return (COLORREF) GetItemData(iItem) & 0xFFFFFF; 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
int CSuperCombo::FindColor(COLORREF clr) const
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
 	int nCount = GetCount(); 
	for( int i = 0; i < nCount; i++ )
	{ 
		if( (GetItemData(i) & 0xFFFFFF) == (DWORD) clr ) 
			return i; 
	} 
	return -1; 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CSuperCombo::_Init() 
{ 
	ATLASSERT(::IsWindow(m_hWnd)); 
	// Need to set these in resource editor 
	// If it's a ListBox, use LBS_OWNERDRAWFIXED. 
	ATLASSERT(GetStyle() & CBS_OWNERDRAWFIXED); 
} 
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CSuperCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{ 
	if( lpDrawItemStruct->itemID == -1 ) 
		return; 

	CDCHandle dc = lpDrawItemStruct->hDC; 
	RECT rc = lpDrawItemStruct->rcItem; 
	LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData; 
	bool bSelected = lpDrawItemStruct->itemState & ODS_SELECTED; 

	// Never paint selected color for combobox itself (edit part) 
	if( lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT ) bSelected = false; 

	// Fill background and color item 
	CBrushHandle brBack = ::GetSysColorBrush(bSelected ? COLOR_HIGHLIGHT : COLOR_WINDOW); 
	dc.FillRect(&rc, brBack); 
	::InflateRect(&rc, -s_cxIndent, -s_cyIndent); 
	dc.FillSolidRect(&rc, RGB(0,0,0)); // Hmm, should have been a call to ::Rectangle() 
	::InflateRect(&rc, -1, -1); 
	dc.FillSolidRect(&rc, RGB(0,0,255)); 

#if /*defined(IDS_CUSTOM) && */(_ATL_VER >= 0x0700) 
	// NOTE: A little hack to display the "Custom" text on 
	//       a color item. Simply add 0xFF000000 to the value of 
	//       the color and remember to include the "resource.h" header 
	//       before this file... 
	//if( (clr & 0xFF000000) == 0xFF000000 ) { 
		//TCHAR szCustom[32] = _T("테스트스트링"); 
		dc.SetBkMode(TRANSPARENT); 
		dc.SetTextColor(RGB(255,255,255)); 
		dc.DrawText(lpszText, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER); 
	//} 
#endif //
} 

// void CSuperCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
// {
// 	ASSERT(lpDrawItemStruct->CtlType == ODT_COMBOBOX);
// 	LPCTSTR lpszText = (LPCTSTR) lpDrawItemStruct->itemData;
// 	ASSERT(lpszText != NULL);
// 	CDC dc;
// 
// 	dc.Attach(lpDrawItemStruct->hDC);
// 
// 	// Save these value to restore them when done drawing.
// 	COLORREF crOldTextColor = dc.GetTextColor();
// 	COLORREF crOldBkColor = dc.GetBkColor();
// 
// 	// If this item is selected, set the background color 
// 	// and the text color to appropriate values. Erase
// 	// the rect by filling it with the background color.
// 	if ((lpDrawItemStruct->itemAction & ODA_SELECT) &&
// 		(lpDrawItemStruct->itemState  & ODS_SELECTED))
// 	{
// 		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
// 		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
// 		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
// 	}
// 	else
// 	{
// 		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
// 	}
// 
// 	// Draw the text.
// 	dc.DrawText(
// 		lpszText,
// 		(int)_tcslen(lpszText),
// 		&lpDrawItemStruct->rcItem,
// 		DT_CENTER|DT_SINGLELINE|DT_VCENTER);
// 
// 	// Reset the background color and the text color back to their
// 	// original values.
// 	dc.SetTextColor(crOldTextColor);
// 	dc.SetBkColor(crOldBkColor);
// 
// 	dc.Detach();
// }