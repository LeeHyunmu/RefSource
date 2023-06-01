
#ifndef __INFINITEGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __INFINITEGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//AQ Device에서 보내는 메시지를 메인 UI에서 받아 처리할 수 있도록 정의된 상수
#define AQDCMSG_COMPLET 0x1001
#define AQDCMSG_VIDEOCHANGE 0x1002
#define AQDCMSG_COMPLETDLG 0x1003

#define  MAINCLOSE_DELAYTIME  11
#define WM_USERTIMER WM_USER + 100
#define WM_DEVICECMDCALLMESSAGE WM_USER + 112
#define WM_NFCAQDEVICEMESSAGE WM_USER + 120
//디버그 메시지 출력 에디터 호출
#define WM_COMMANDDEBUGSTRING WM_USER+121
#define WM_COUPONCLICKMSG WM_USER + 130
//buffer size 크기
#define BUFFSIZE_1024 1024
#define BUFFSIZE_2048 2048
#define BUFFSIZE_4096 4096

#endif //__INFINITEGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__