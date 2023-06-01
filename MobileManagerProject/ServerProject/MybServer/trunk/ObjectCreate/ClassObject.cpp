
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Server/Server.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
debug(NULL),
ServerMg(NULL),
IocpServer(NULL)
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
void CClassObject::Release()
{
	if(IocpServer)
	{
		delete ((CIocpServer*)IocpServer);	IocpServer=NULL;
	}
	if(ServerMg)
	{
		delete ((CServerMg*)ServerMg);	ServerMg=NULL;
	}
	if(debug)
	{
		delete ((CTraceOutput*)debug);	debug=NULL;
	}
}
//--------------------------------------------------------------
//|
//|선언된 객체를 생성한다
//|
//---------------------------------------------------------------
BOOL CClassObject::CreateObject(LPVOID _pross)
{
	mPross=_pross;
#ifdef _DEBUG
	debug = new CTraceOutput(CTraceOutput::CONSOLE,_T("MobileDebug"));
#else
	debug = new CTraceOutput(CTraceOutput::FILE,_T("MobileDebug"));
#endif
	IocpServer=new CIocpServer;
	if(IocpServer)
		IocpServer->initServer(this);
	ServerMg=new CServerMg;
	if(ServerMg)
		ServerMg->initServerMg(this);
	return TRUE;
}
IOutput* CClassObject::GetOutput()
{
	return debug;
}
IServer* CClassObject::GetIocpServer()
{
	return IocpServer;
}
IServerMg* CClassObject::GetServerMg()
{
	return ServerMg;
}