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
#pragma warning (default: 4146)
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
#include "ObjectCreate/ObjectCreate.h"
#include "InfiniteGlobal/InfiniteGlobal.h"
#include "StaticAPI/staticapi.h"
#include "BaseClass/BaseClass.h"

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
