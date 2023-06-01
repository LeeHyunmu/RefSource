
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
GATypeChange::GATypeChange(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
GATypeChange::~GATypeChange(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|
//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
//|
//---------------------------------------------------------------
int  GATypeChange::GAChangeHexCordInt(CString data, DWORD dwLen)
{
	int mum;
	char* buffer;
	buffer =(char*)&mum;
	memset(buffer,0x00,sizeof(buffer));
	int dwtransLen= dwLen/2;
	for(int i = 0 ; i < dwtransLen ; i++ )
	{
		buffer[i] = (((BYTE)data.GetAt(i*2)) & 0x0F) << 4;

		if( (BYTE)data.GetAt(i*2) > 0x39 )
			buffer[i] += 0x90;

		buffer[i] |= ((BYTE)data.GetAt(i*2+1)) & 0x0F;

		if( (BYTE)data.GetAt(i*2+1) > 0x39 )
			buffer[i] += 0x09;
	}
	return mum;
}
//--------------------------------------------------------------
//|
//|스트링 숫자를 핵사 스트링으로 변환한다
//|
//---------------------------------------------------------------
BOOL GATypeChange::ChangStringbyhexString(CString pass, CString& repass)
{
	CString temp;
	int dwtransLen=pass.GetLength();
	for( int i = 0; i < (int)dwtransLen; i++ )
	{
		temp.Format(_T("%02X"), pass[i]);
		repass += temp;
	}
	return FALSE;
}