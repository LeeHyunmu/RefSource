
#include "stdAfx.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Server/Server.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
debug(NULL),
ServerMg(NULL),
IocpServer(NULL)
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
//|����� ��ü�� �����Ѵ�
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