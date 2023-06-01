// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change this value to use different versions
#define WINVER 0x0420

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

#include <ATLComTime.h>

#include <vector>
#include <deque>
#include <queue>
#include <algorithm>
#include <atlimage.h> //CImage Class 사용
using namespace std;

#include <fstream>

//전역 API
#include "GlobalApi/GlobalApi.h"
//윈도우 베이스 클래스
#include "BaseClass/BaseClass.h"
#include "resource.h"
//인터페이스 선언 클래스
#include "interface/interface.h"
//로고 디버그 & 텍스트 출력
#include "ObjectCreate/ObjectCreate.h"
