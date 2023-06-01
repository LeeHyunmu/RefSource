
#include "stdAfx.h"
#include "../Client/Client.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Crc/Crc.h"

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
client(NULL),
debug(NULL),
crc32(NULL)
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
	if(crc32)
	{
		crc32->Release();
		delete ((CCrc32*)crc32);	crc32=NULL;
	}
	if(client)
	{
		client->ReleaseClient();
		delete ((CMobileClient*)client);	client=NULL;
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
BOOL CClassObject::CreateObject()
{
#ifdef _DEBUG
	debug = new CTraceOutput(CTraceOutput::CONSOLE,_T("MobileDebug"));
#else
	debug = new CTraceOutput(CTraceOutput::FILE,_T("MobileDebug"));
#endif
	//Crc32 생성
	crc32=new CCrc32;
	if(crc32)
		crc32->initCrc(this);
	client = new CMobileClient;
	if(client)
		client->initClient(this);

	return FALSE;
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
IOutput* CClassObject::GetOutput()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|데이터 검증 클래스
//|
//---------------------------------------------------------------
ICrc* CClassObject::GetCrc32()
{
	return crc32;
}
//--------------------------------------------------------------
//|
//|통신 클라언트 객체
//|
//---------------------------------------------------------------
IClient* CClassObject::GetClient()
{
	return client;
}