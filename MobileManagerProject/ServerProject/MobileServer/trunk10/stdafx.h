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

//|============================================================
//| MS C++ 최신계열에서만 나오는 것
//| MS C++에서 기존 C Library 계열에 BOF와 같은 보안문제를 해결하기
//| 위해 시큐어 버전을 제공하는데 그걸 안썼다고 Deprecated 판정을
//| 내렸다고 경고하는 것
//|============================================================
//#define _CRT_SECURE_NO_DEPRECATE  // 위 경고를 죽이기 위한 define 값
//#define _CRT_SECURE_NO_WARNINGS   // 위 경고를 죽이기 위한 define 값


#include <atlbase.h>
#include <atlstr.h>
//#define _WTL_NO_WTYPES
#include <atltypes.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlcrack.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include <vector>
#include <algorithm>
#include <atlimage.h> //CImage Class 사용
using namespace std;

#include <fstream>
//com
#include <comdef.h>
#include <icrsint.h>
#include <atlcomtime.h>
#include <math.h>
//--------------------------------------------------------------
//|
//| ADO DB 연결
//|
//---------------------------------------------------------------
#pragma warning (disable: 4146)
// CG : In order to use this code against a different version of ADO, the appropriate
// ADO library needs to be used in the #import statement
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" rename("EOF", "EndOfFile")
#import "C:\Program Files\Common Files\System\ado\MSJRO.DLL" no_namespace
using namespace ADODB;
#pragma warning (default: 4146)

#include "resource.h"
#include "interface/interface.h"
#include "BaseClass/BaseClass.h"
#include "InfiniteGlobal/InfiniteGlobal.h"
#include "StaticAPI/staticapi.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
