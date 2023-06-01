// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#pragma once

// Change this value to use different versions
#define WINVER 0x0420
#define _WIN32_WCE_AYGSHELL 1

#include <atlbase.h>
#include <atlstr.h>
//#define _WTL_NO_WTYPES
#include <atltypes.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#include <atlcrack.h>
#include <ATLComTime.h> //debug 타임 함수 사용
#include <aygshell.h>

#include <ATLComTime.h>

#include <vector>
#include <deque>
#include <queue>
#include <atlcoll.h>
#include <algorithm>
#include <atlimage.h> //CImage Class 사용
using std::vector;
using std::queue;
using std::deque;
using std::allocator;


//전역
#include "InfiniteGlobal/InfiniteGlobal.h"
//리소스 ID
#include "resource.h"

//전역 API
#include "GlobalApi/GlobalApi.h"
//Helper Class
#include "HelperClass/HelperClass.h"
//저장 클래스
#include "Storage/Storage.h"
//윈도우 베이스 클래스
#include "BaseClass/BaseClass.h"
//디버그 출력 클래스
#include "DebugDlg/DebugDlg.h"
//버튼 Sub 슈퍼클래싱 클래스
#include "SubButton/SubButton.h"
//
#include "SubStatic/SubStatic.h"
//Edit 슈퍼 클래싱 클래스
#include "SubEdit/SubEdit.h"
#include "Serial/Serial.h"
#include "Water/Water.h"
#include "Nfcdevice/Nfcdevice.h"
#include "GdiDlg/GdiDlg.h"