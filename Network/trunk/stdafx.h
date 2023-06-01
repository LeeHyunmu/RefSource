// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
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
//#include <atltypes.h>
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

//COleDateTime 클래스
#include <ATLComTime.h>
//CAtlMap 클래스
#include <atlcoll.h>

//STL 관련 클래스
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>
//이미지 관련 클래스
#include <atlimage.h> //CImage Class 사용
using namespace std;

//무한 전역 선언
#include "InfiniteGlobal/InfiniteGlobal.h"
//전역 API
#include "GlobalApi/GlobalApi.h"
//윈도우 베이스 클래스
#include "BaseClass/BaseClass.h"
//리소스 해더파일
#include "resource.h"
//인터페이스 선언 클래스
#include "interface/interface.h"
//로고 디버그 & 텍스트 출력
#include "ObjectCreate/ObjectCreate.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

