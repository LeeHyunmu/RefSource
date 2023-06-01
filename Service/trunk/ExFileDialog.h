////////////////////////////////////////////////////////////////
// MSDN Magazine -- March 2004
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio .NET on Windows XP. Tab size=3.
//
// My open dialog -- customized to set default view mode.
//

#ifndef _EXFILEDIALOG_
#define  _EXFILEDIALOG_
#include <dlgs.h>				 // for common dialog control IDs


class CExFileDialog : public CFileDialog {

public:

	// reverse-engineered command codes for SHELLDLL_DefView
	typedef enum
	{	ODM_VIEW_ICONS = 0x7029,
		ODM_VIEW_LIST  = 0x702b,
		ODM_VIEW_DETAIL= 0x702c,
		ODM_VIEW_THUMBS= 0x702d,
		ODM_VIEW_TILES = 0x702e,
	} LISTVIEWCMD;

	CExFileDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt = NULL, LPCTSTR lpszFileName = NULL, DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, LPCTSTR lpszFilter = NULL, CWnd* pParentWnd = NULL, LISTVIEWCMD listViewType = ODM_VIEW_DETAIL);

protected:
	enum { ID_LISTVIEW = lst2 };

	LISTVIEWCMD m_listViewType;			// list view type

	virtual BOOL OnInitDialog();		// handle init dialog
	afx_msg LRESULT OnPostInit(WPARAM wp, LPARAM lp); // the REAL initialization
	BOOL SetListView(LISTVIEWCMD cmd);

	DECLARE_DYNAMIC(CExFileDialog)
	DECLARE_MESSAGE_MAP()
};
#endif //_EXFILEDIALOG_
