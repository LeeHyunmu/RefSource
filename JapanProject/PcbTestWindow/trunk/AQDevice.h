
// AQDevice.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CAQDeviceApp:
// �� Ŭ������ ������ ���ؼ��� AQDevice.cpp�� �����Ͻʽÿ�.
//

class CAQDeviceApp : public CWinAppEx
{
public:
	CAQDeviceApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CAQDeviceApp theApp;