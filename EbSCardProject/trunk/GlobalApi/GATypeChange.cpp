
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
GATypeChange::GATypeChange(void)
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
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
//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
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
//|��Ʈ�� ���ڸ� �ٻ� ��Ʈ������ ��ȯ�Ѵ�
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