
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
GASCard::GASCard(void)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
GASCard::~GASCard(void)
{
}
//--------------------------------------------------------------
//|
//|�����Լ�
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|����Ʈī�� OnDeviceChange �޽��� �ޱ� ���� ����ϴ� �κ�
//|
//---------------------------------------------------------------
void GASCard::RegisterForDeviceNotifications(HWND hWnd)
{
	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	HDEVNOTIFY DeviceNotificationHandle;
	GUID UDEVICE_GUID = {0x50DD5230, 0xBA8A, 0x11D1, 0xBF, 0x5D, 0x00, 0x00, 0xF8, 0x05, 0xF5, 0x30};

	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = UDEVICE_GUID;
	DeviceNotificationHandle = RegisterDeviceNotification(hWnd, &DevBroadcastDeviceInterface, DEVICE_NOTIFY_WINDOW_HANDLE);
}