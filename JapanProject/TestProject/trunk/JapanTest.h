// JapanTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif

// CJapanTestApp:
// �� Ŭ������ ������ ���ؼ��� JapanTest.cpp�� �����Ͻʽÿ�.
//

class CJapanTestApp : public CWinApp
{
public:
	CJapanTestApp();
	
// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CJapanTestApp theApp;
