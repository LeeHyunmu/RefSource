
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CTimeApi::CTimeApi(void)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CTimeApi::~CTimeApi(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CTimeApi::SetWait(HANDLE hevent, DWORD wait_t)
{
	BOOL b=FALSE;
	DWORD waitobject=0;
	if(hevent)
	{
		waitobject = WaitForSingleObject(hevent,wait_t);
		if(waitobject==WAIT_TIMEOUT)
		{
			b=FALSE;
		}
		else if(waitobject==WAIT_OBJECT_0)
		{
			b=TRUE;
		}
	}
	return b;
}