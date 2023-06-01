
#include "stdAfx.h"
#include "../Client/Client.h"
#include "../OutputDebug/OutputDebug.h"
#include "../Crc/Crc.h"

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CClassObject::CClassObject():
client(NULL),
debug(NULL),
crc32(NULL)
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
//|����� ��ü�� �����Ѵ�
//|
//---------------------------------------------------------------
BOOL CClassObject::CreateObject()
{
#ifdef _DEBUG
	debug = new CTraceOutput(CTraceOutput::CONSOLE,_T("MobileDebug"));
#else
	debug = new CTraceOutput(CTraceOutput::FILE,_T("MobileDebug"));
#endif
	//Crc32 ����
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
//|������ Ŭ���� �������̽� ��ü ������ �ѱ�� �Լ�
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|����� ��� Ŭ����
//|
//---------------------------------------------------------------
IOutput* CClassObject::GetOutput()
{
	return debug;
}
//--------------------------------------------------------------
//|
//|������ ���� Ŭ����
//|
//---------------------------------------------------------------
ICrc* CClassObject::GetCrc32()
{
	return crc32;
}
//--------------------------------------------------------------
//|
//|��� Ŭ���Ʈ ��ü
//|
//---------------------------------------------------------------
IClient* CClassObject::GetClient()
{
	return client;
}