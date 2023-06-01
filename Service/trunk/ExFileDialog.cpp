////////////////////////////////////////////////////////////////
// MSDN Magazine -- March 2004
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
#include "StdAfx.h"
#include "ExFileDialog.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// self-initialization message posted
const MYWM_POSTINIT = WM_USER+1;

IMPLEMENT_DYNAMIC(CExFileDialog, CFileDialog)
BEGIN_MESSAGE_MAP(CExFileDialog, CFileDialog)
	ON_MESSAGE(MYWM_POSTINIT, OnPostInit)
END_MESSAGE_MAP()

////////////////
// ctor
//
CExFileDialog::CExFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName, DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd, LISTVIEWCMD listViewType)
			: CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
	m_listViewType = listViewType;
}

//////////////////
// Handle WM_INITDIALOG: shows that list view still not created yet.
//
BOOL CExFileDialog::OnInitDialog()
{
	TRACE(_T("CExFileDialog::OnInitDialog\n"));
	CFileDialog::OnInitDialog();
	PostMessage(MYWM_POSTINIT,0,0);
	return TRUE;
}

//////////////////
// Handle MYWN_POSTINIT: finally, the list view is created.
//
LRESULT CExFileDialog::OnPostInit(WPARAM wp, LPARAM lp)
{
	TRACE(_T("CExFileDialog::OnPostInit\n"));

	// Note real dialog is my parent, not me!
	SetListView(m_listViewType);
	return 0;
}

//////////////////
// Change the list view to desired mode if the view exists.
// Display TRACE diagnostics either way.
//
BOOL CExFileDialog::SetListView(LISTVIEWCMD cmd)
{
	m_listViewType = cmd;

	// Note real dialog is my parent, not me!
	CWnd* pshell = GetParent()->GetDlgItem(lst2);
	TRACE(_T("CExFileDialog::SetListView: hwnd=%p\n"),pshell->GetSafeHwnd());
	if (pshell) {
		// SHELLDLL_DefView window found: send it the command.
		pshell->SendMessage(WM_COMMAND, m_listViewType);
		return TRUE;
	}
	return FALSE;
}
