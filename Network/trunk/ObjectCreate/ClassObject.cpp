
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../GdiDlg/GdiDlg.h"
#include "../Server/Server.h"

//--------------------------------------------------------------
//|
//|생성자
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
//|소멸자
//|
//---------------------------------------------------------------
CClassObject::~CClassObject()
{
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CClassObject::KillObject()
{
	////////////////////////////////////////////////////////////////
	//종료 스레드 생성
	mhKillEvent.Create();
	mKillthread.Create(KillProcess, (LPVOID)this);
	if(mhKillEvent.WaitForEvent())
	{
		if(debug)
			debug->Write(_T("모든 객체 메모리 해제"));
	}
	//디버그 출력 창을 제일 나중에 종료한다
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
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//| 메모리를 해제하기 위한 스래드
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
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CClassObject::Release()
{
	//|=============================================
	//|MainDlg Gdi 클래스
	//|==================================================
	if(maingdi)
	{
		maingdi->Release();
		delete ((CMainGdiDlg*)maingdi);	maingdi=NULL;
	}
	//|=============================================
	//|WinSocketServer 클래스
	//|==================================================
	if(socketserver)
	{
		socketserver->Release();
		delete ((CWinSocketServer*)socketserver);	socketserver=NULL;
	}
	//|=============================================
	//|UdpWinSocketServer 클래스
	//|==================================================
	if(udpsocketserver)
	{
		udpsocketserver->Release();
		delete ((CWinSocketServer*)udpsocketserver);	udpsocketserver=NULL;
	}
	//|=============================================
	//|사용중인 객체의 메모리를 모두 해제 했으면
	//|==================================================
	mhKillEvent.SetEvent();
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
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
	// Main Dlg Gdi 클래스
	//======================================
	maingdi = new CMainGdiDlg;
	//======================================
	// Main Dlg Gdi 클래스
	//======================================
	socketserver = new CWinSocketServer;
	return TRUE;
}
//--------------------------------------------------------------
//|
//|객체 초기화
//|
//---------------------------------------------------------------
BOOL CClassObject::initObject()
{
	//======================================
	// 클래스 초기화
	//======================================
	//======================================
	// Main Dlg Gdi 클래스 초기화
	//======================================
	if(maingdi)
	{
		maingdi->initInterfaceObject(this);
	}
	//======================================
	// SocketServer 클래스 초기화
	//======================================
	if(socketserver)
	{
		socketserver->initInterfaceObject(this);
	}
	//======================================
	// SocketServer 클래스 초기화
	//======================================
	if(udpsocketserver)
	{
		udpsocketserver->initInterfaceObject(this);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|생성된 클래스 인터페이스 객체 포인터 넘기는 함수
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|디버그 출력 클래스
//|
//---------------------------------------------------------------
IOutput* CClassObject::GetOutputObject()
{
	return debug;
}
//--------------------------------------------------------------
//|
//| MainDlg 클래스
//|
//---------------------------------------------------------------
IMainGdiDlg* CClassObject::GetMainGdiObject()
{
	return maingdi;
}
//--------------------------------------------------------------
//|
//| WinSocketServer 클래스
//|
//---------------------------------------------------------------
IWinSocket* CClassObject::GetWinSocketObject()
{
	return socketserver;
}
//--------------------------------------------------------------
//|
//| UdpWinSocketServer 클래스
//|
//---------------------------------------------------------------
IUdpWinSocket* CClassObject::GetUdpWinSocketObject()
{
	return udpsocketserver;
}