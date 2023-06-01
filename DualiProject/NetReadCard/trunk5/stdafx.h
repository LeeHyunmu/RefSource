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
#include <atlimage.h> //CImage Class ���
using namespace std;

#include <fstream>

//���� API
#include "GlobalApi/GlobalApi.h"
//������ ���̽� Ŭ����
#include "BaseClass/BaseClass.h"
#include "resource.h"
//�������̽� ���� Ŭ����
#include "interface/interface.h"
//�ΰ� ����� & �ؽ�Ʈ ���
#include "ObjectCreate/ObjectCreate.h"
