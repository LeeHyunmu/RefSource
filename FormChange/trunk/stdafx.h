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

//atl 베이스 클래스
#include <atlbase.h>
//CString 클래스 사용
#include <atlstr.h>
//#define _WTL_NO_WTYPES
#include <atltypes.h>

#include <atlapp.h>
extern CAppModule _Module;
//윈도우 기반의 기본 클래스
#include <atlwin.h>
// MSG_WM_ 메시지를 사용하기 위한 클래스
#include <atlcrack.h>

//다이얼로그 기반 기본 클래스
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlcoll.h>
//COleDateTime 클래스
#include <ATLComTime.h>

//STL 관련 클래스
#include <vector>
#include <algorithm>

using namespace std;

//Help Class
#include "HelpClass/MyHelpClass.h"
//전역 API
#include "GlobalApi/GlobalApi.h"
//리소스 해더파일
#include "resource.h"