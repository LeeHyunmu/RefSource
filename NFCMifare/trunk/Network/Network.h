
#ifndef __NETWORK_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __NETWORK_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include <ws2bth.h>
#include <process.h>
#include <Wlanapi.h>

//======================================================//
//			���� ��ü ����
//======================================================//
#define SOCKETCLIENTTHREAD_LOOPTIME 200
//=======================================================//
//--------------------------------------------------------------
//|
//| �������� ������ ����ü
//|
//---------------------------------------------------------------
#pragma pack(push,1)
struct baseDATA
{
	byte dwCode;
	byte typelen;
	byte dwdatalen[2];
	byte data[1];
};
#pragma pack(pop)

#include "SocketClient.h"

#endif //__NETWORK_74F736E25AB94b5dA789E4730F58F2A1_H__