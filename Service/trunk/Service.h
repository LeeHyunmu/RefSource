// Update.h : main header file for the UPDATE application
//

#if !defined(AFX_UPDATE_H__0098F7CE_9B60_4A50_99B1_218F34AC6883__INCLUDED_)
#define AFX_UPDATE_H__0098F7CE_9B60_4A50_99B1_218F34AC6883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CServiceApp:
// See Update.cpp for the implementation of this class
//

class CServiceApp : public CWinApp
{
public:
	CServiceApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServiceApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CServiceApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATE_H__0098F7CE_9B60_4A50_99B1_218F34AC6883__INCLUDED_)
