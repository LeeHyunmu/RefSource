
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../DualiCard/DualiCard.h"
#include "../GdiDlg/GdiDlg.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
debug(NULL),
dualimg(NULL),
maingdi(NULL),
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
		debug->Write(_T("모든 객체 메모리 해제"));
	}
	//디버그 출력 창을 제일 나중에 종료한다
	if(debug)
	{
		delete ((CTraceWinDebug*)debug);	debug=NULL;
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
	if(dualimg)
	{
		dualimg->Release();
		delete ((CDualiCardMg*)dualimg);	dualimg=NULL;
	}
	if(maingdi)
	{
		maingdi->Release();
		delete ((CDualiCardMg*)maingdi);	maingdi=NULL;
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
	debug = new CTraceWinDebug(_T("NetCard_Debug"));
#ifndef _DEBUG
	debug->SetVisible(FALSE);
#endif
	//======================================
	// Duali 단말기 DLL 메니저 클래스
	//======================================
	dualimg = new CDualiCardMg;
	if(dualimg)
	{
		dualimg->initInterfaceObject(this);
	}
	//======================================
	// Main Dlg Gdi 클래스
	//======================================
	maingdi = new CMainGdiDlg;
	if(maingdi)
	{
		maingdi->initInterfaceObject(this);
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
//|듀얼아이 메니저 클래스
//|
//---------------------------------------------------------------
IDualiCardMg* CClassObject::GetDualiCardMgObject()
{
	return dualimg;
}
//--------------------------------------------------------------
//|
//| 클래스
//|
//---------------------------------------------------------------
IGdiDlg* CClassObject::GetMainGdiObject()
{
	return maingdi;
}