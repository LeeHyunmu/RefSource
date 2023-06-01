
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../GdiDlg/GdiDlg.h"
#include "../Server/Server.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
debug(NULL),
maingdi(NULL),
socketserver(NULL),
udpsocketserver(NULL),
mKillthread(NULL),
mhKillEvent(NULL)
{}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CClassObject::~CClassObject()
{
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CClassObject::KillObject()
{
	////////////////////////////////////////////////////////////////
	//���� ������ ����
	mhKillEvent.Create();
	mKillthread.Create(KillProcess, (LPVOID)this);
	if(mhKillEvent.WaitForEvent())
	{
		if(debug)
			debug->Write(_T("��� ��ü �޸� ����"));
	}
	//����� ��� â�� ���� ���߿� �����Ѵ�
	if(debug)
	{
#ifdef _DEBUG
		delete ((CTraceWinDebug*)debug);	debug=NULL;
#else
		delete ((CTraceFileDebug*)debug);	debug=NULL;
#endif
	}
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//| �޸𸮸� �����ϱ� ���� ������
//|
//---------------------------------------------------------------
DWORD WINAPI CClassObject::KillProcess(LPVOID lpArg)
{
	IMyObject *obj = (IMyObject *)lpArg;
	if(obj)
		obj->Release();
	return 0;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CClassObject::Release()
{
	//|=============================================
	//|MainDlg Gdi Ŭ����
	//|==================================================
	if(maingdi)
	{
		maingdi->Release();
		delete ((CMainGdiDlg*)maingdi);	maingdi=NULL;
	}
	//|=============================================
	//|WinSocketServer Ŭ����
	//|==================================================
	if(socketserver)
	{
		socketserver->Release();
		delete ((CWinSocketServer*)socketserver);	socketserver=NULL;
	}
	//|=============================================
	//|UdpWinSocketServer Ŭ����
	//|==================================================
	if(udpsocketserver)
	{
		udpsocketserver->Release();
		delete ((CWinSocketServer*)udpsocketserver);	udpsocketserver=NULL;
	}
	//|=============================================
	//|������� ��ü�� �޸𸮸� ��� ���� ������
	//|==================================================
	mhKillEvent.SetEvent();
}
//--------------------------------------------------------------
//|
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
BOOL CClassObject::CreateObject()
{
#ifdef _DEBUG
	debug = new CTraceWinDebug(_T("Baseclass_Debug"));
#else
	debug = new CTraceFileDebug(_T("Baseclass_Debug"));
#endif
	//======================================
	// Main Dlg Gdi Ŭ����
	//======================================
	maingdi = new CMainGdiDlg;
	//======================================
	// Main Dlg Gdi Ŭ����
	//======================================
	socketserver = new CWinSocketServer;
	return TRUE;
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ
//|
//---------------------------------------------------------------
BOOL CClassObject::initObject()
{
	//======================================
	// Ŭ���� �ʱ�ȭ
	//======================================
	//======================================
	// Main Dlg Gdi Ŭ���� �ʱ�ȭ
	//======================================
	if(maingdi)
	{
		maingdi->initInterfaceObject(this);
	}
	//======================================
	// SocketServer Ŭ���� �ʱ�ȭ
	//======================================
	if(socketserver)
	{
		socketserver->initInterfaceObject(this);
	}
	//======================================
	// SocketServer Ŭ���� �ʱ�ȭ
	//======================================
	if(udpsocketserver)
	{
		udpsocketserver->initInterfaceObject(this);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|������ Ŭ���� �������̽� ��ü ������ �ѱ�� �Լ�
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|����� ��� Ŭ����
//|
//---------------------------------------------------------------
IOutput* CClassObject::GetOutputObject()
{
	return debug;
}
//--------------------------------------------------------------
//|
//| MainDlg Ŭ����
//|
//---------------------------------------------------------------
IMainGdiDlg* CClassObject::GetMainGdiObject()
{
	return maingdi;
}
//--------------------------------------------------------------
//|
//| WinSocketServer Ŭ����
//|
//---------------------------------------------------------------
IWinSocket* CClassObject::GetWinSocketObject()
{
	return socketserver;
}
//--------------------------------------------------------------
//|
//| UdpWinSocketServer Ŭ����
//|
//---------------------------------------------------------------
IUdpWinSocket* CClassObject::GetUdpWinSocketObject()
{
	return udpsocketserver;
}