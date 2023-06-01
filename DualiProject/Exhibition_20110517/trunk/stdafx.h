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
#include <algorithm>
#include <atlimage.h> //CImage Class ���
using std::vector;
using std::queue;
using std::deque;
using std::allocator;


#include <fstream>
//���ҽ� ID
#include "resource.h"

//���� API
#include "GlobalApi/GlobalApi.h"
//������ ���̽� Ŭ����
#include "BaseClass/BaseClass.h"
//��ư Sub ����Ŭ���� Ŭ����
#include "SubButton/SubButton.h"
//Static ���� Ŭ���� Ŭ����
#include "SubStatic/SubStatic.h"
//�������̽� ���� Ŭ����
#include "interface/interface.h"
//�ΰ� ����� & �ؽ�Ʈ ���
#include "ObjectCreate/ObjectCreate.h"
