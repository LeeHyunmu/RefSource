
#ifndef __AQDEVICE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __AQDEVICE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#define MAX_COMMANDDATA 1026
#define THREAD_LOOPTIME 200
#define EVENTTHREAD_LOOPTIME 2000
#define POLLINGSTARTLEN 3

#include "AQCmdformat.h"
#include "../BaseClass/BaseClass.h"
#include "../Serial/Serial.h"
#include "DeviceCmd.h"
#define AQDC CDeviceCmd::getInstance()
#endif //__AQDEVICE_74F736E25AB94b5dA789E4730F58F2A1_H__