// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0200

//atl ���̽� Ŭ����
#include <atlbase.h>
//CString Ŭ���� ���
#include <atlstr.h>
//#define _WTL_NO_WTYPES
#include <atltypes.h>

#include <atlapp.h>
extern CAppModule _Module;
//������ ����� �⺻ Ŭ����
#include <atlwin.h>
// MSG_WM_ �޽����� ����ϱ� ���� Ŭ����
#include <atlcrack.h>

//���̾�α� ��� �⺻ Ŭ����
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcoll.h>
//COleDateTime Ŭ����
#include <ATLComTime.h>

//STL ���� Ŭ����
#include <vector>
#include <algorithm>

using namespace std;

//Help Class
#include "HelpClass/MyHelpClass.h"
//���� API
#include "GlobalApi/GlobalApi.h"
//���ҽ� �ش�����
#include "resource.h"