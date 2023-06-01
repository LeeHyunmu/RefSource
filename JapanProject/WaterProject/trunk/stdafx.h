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
#include <ATLComTime.h> //debug Ÿ�� �Լ� ���
#include <aygshell.h>

#include <ATLComTime.h>

#include <vector>
#include <deque>
#include <queue>
#include <atlcoll.h>
#include <algorithm>
#include <atlimage.h> //CImage Class ���
using std::vector;
using std::queue;
using std::deque;
using std::allocator;


//����
#include "InfiniteGlobal/InfiniteGlobal.h"
//���ҽ� ID
#include "resource.h"

//���� API
#include "GlobalApi/GlobalApi.h"
//Helper Class
#include "HelperClass/HelperClass.h"
//���� Ŭ����
#include "Storage/Storage.h"
//������ ���̽� Ŭ����
#include "BaseClass/BaseClass.h"
//����� ��� Ŭ����
#include "DebugDlg/DebugDlg.h"
//��ư Sub ����Ŭ���� Ŭ����
#include "SubButton/SubButton.h"
//
#include "SubStatic/SubStatic.h"
//Edit ���� Ŭ���� Ŭ����
#include "SubEdit/SubEdit.h"
#include "Serial/Serial.h"
#include "Water/Water.h"
#include "Nfcdevice/Nfcdevice.h"
#include "GdiDlg/GdiDlg.h"