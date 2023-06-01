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
#include <deque>
#include <queue>
#include <string>
#include <algorithm>
//�̹��� ���� Ŭ����
#include <atlimage.h> //CImage Class ���

using namespace std;

//#include <mpconfig.h>
#include "InfiniteGlobal/InfiniteGlobal.h"
#include "TempletHelper/TempletHelper.h"
#include "ImageLoadApi/ImageLoadApi.h"
//Help Class
#include "HelpClass/MyHelpClass.h"
//���� API
#include "GlobalApi/GlobalApi.h"
//MultiTimer
#include "MultiMediaTimer/MultiMediaTimer.h"
//���ҽ� �ش�����
#include "resource.h"
//�����
#include "Storage/Storage.h"
//������ ���̽� Ŭ����
#include "BaseClass/BaseClass.h"
//��ư Sub ���̽� Ŭ����
#include "SubButton/SubButton.h"
//static sub ���̽� Ŭ����
#include "SubStatic/SubStatic.h"
//combo sub ���̽� Ŭ����
#include "SubCombo/SubCombo.h"
//combo sub ���̽� Ŭ����
#include "SubEdit/SubEdit.h"
//�������̽� ���� Ŭ����
#include "interface/interface.h"
//����� Ŭ����
#include "OutputDebug/OutputDebug.h"
//�ΰ� ����� & �ؽ�Ʈ ���
#include "ObjectCreate/ObjectCreate.h"

//������ �ļ� Ŭ����
#include "NfcDataParser/NfcDataParser.h"
//�ø��� ���
#include "Serial/Serial.h"
//AQ ����̽� Command Class
#include "AQDevice/AQDevice.h"
//�̵�� �÷��̾� ����Ʈ
#import <wmp.dll> named_guids raw_interfaces_only
//Gif���� Ŭ����
#include "GifWnd/GifWnd.h"

#define  MAINCLOSE_DELAYTIME  11
#define WM_USERTIMER WM_USER + 100
#define WM_NFCEVENTCALL WM_USER + 110
#define WM_DEVICECMDCALLMESSAGE WM_USER + 112
#define WM_NFCAQDEVICEMESSAGE WM_USER + 120
#define WM_COUPONCLICKMSG WM_USER + 130

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

