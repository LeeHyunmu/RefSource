
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------
//Apud header(Apdu command 함수 호출 시 연결
const byte CAtlmapNfcData::NfcApduCmdHeader[4]={0x00, 0xDC, 0x04, 0x00};
//Mifare Data 해더
const byte CAtlmapNfcData::NfcMifareHeader[1]={0x03};
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CAtlmapNfcData::CAtlmapNfcData(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CAtlmapNfcData::~CAtlmapNfcData(void)
{
	DeleteDataAll();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapNfcData::AddData(int name, byte* data, int datalen)
{
	BOOL b=FALSE;
	if(storage.Lookup(name)==NULL)
	{
		Data dt;
		dt.datalen=datalen;
		dt.data = new byte[datalen];
		memcpy(dt.data, data, datalen);
		storage[name]=dt;
		b=TRUE;
	}

	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapNfcData::DeleteData(int name)
{
	if(storage.IsEmpty()) return FALSE;
	BOOL b=FALSE;
	NfcCommandPair bitpair = NULL;
	if(bitpair = storage.Lookup(name))
	{
		delete bitpair->m_value.data;
		//메모리 해제
		storage.RemoveKey(name);
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapNfcData::DeleteDataAll()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	while (pos)
	{
		NfcCommandPair bitpair = NULL;
		bitpair = storage.GetNext(pos);
		delete bitpair->m_value.data;
	}
	//리스트에서 모두 삭제
	storage.RemoveAll();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
int CAtlmapNfcData::GetNdefData(int name, byte* data, int datalen)
{
	if(storage.IsEmpty()) return 0;
	Data dt;
	if(storage.Lookup(name, dt))
	{
		datalen = dt.datalen;
		memcpy(data, dt.data, datalen);
		return datalen;
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
int CAtlmapNfcData::GetApduCmdData(int name, byte* data, int datalen)
{
	if(storage.IsEmpty()) return 0;
	Data dt;
	if(storage.Lookup(name, dt))
	{
		datalen=0;
		memcpy(&data[datalen], &NfcApduCmdHeader[0], 4);
		datalen+=4;
		data[datalen++]=dt.datalen&0xFF;
		memcpy(&data[datalen], &dt.data[0], dt.datalen);
		datalen += dt.datalen;
		return datalen;
	}
	return 0;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
int CAtlmapNfcData::GetMifareData(int name, byte* data, int datalen)
{
	if(storage.IsEmpty()) return 0;
	Data dt;
	if(storage.Lookup(name, dt))
	{
		datalen=0;
		memcpy(&data[datalen], &NfcMifareHeader[0], 1);
		datalen+=1;
		data[datalen++]=dt.datalen&0xFF;
		memcpy(&data[datalen], &dt.data[0], dt.datalen);
		datalen += dt.datalen;
		data[datalen++]=0xFE; //Mifare 끝
		return datalen;
	}
	return 0;
}
