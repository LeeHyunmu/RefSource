#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|//생성자
//|
//---------------------------------------------------------------
ContentsMallRecord::ContentsMallRecord(void)
{

}

//--------------------------------------------------------------
//|
//|//소멸자
//|
//---------------------------------------------------------------
ContentsMallRecord::~ContentsMallRecord(void)
{

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Header D2
const byte ContentsMallRecord::NfcContentsMall_Header=0x92;
//Type Length
const byte ContentsMallRecord::NfcContentsMall_TypeLength=0x17;
//Payload Type application/aq-contents
const byte ContentsMallRecord::NfcContentsMall_PayloadType[23]={0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x2F
, 0x61, 0x71, 0x2D, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x73};

//--------------------------------------------------------------
//|
//| Payload 만들기
//|
//---------------------------------------------------------------
byte* ContentsMallRecord::makePayloadData(CString payloaddata, byte* NfcParserData, int& NfcParserDatalen)
{
	//배열 초기화
	memset(NfcParserData, NULL, NfcParserDatalen);
	NfcParserDatalen=0;
	NfcParserData[NfcParserDatalen++]=NfcContentsMall_Header;
	NfcParserData[NfcParserDatalen++]=NfcContentsMall_TypeLength;
	int payloadlen = payloaddata.GetLength();
	NfcParserData[NfcParserDatalen++]=payloadlen&0xFF;
	memcpy(&NfcParserData[NfcParserDatalen], &NfcContentsMall_PayloadType[0], NfcContentsMall_TypeLength);
	NfcParserDatalen+=NfcContentsMall_TypeLength;
	memcpy(&NfcParserData[NfcParserDatalen], payloaddata.GetBuffer(), payloadlen);
	NfcParserDatalen += payloadlen;
	return NfcParserData;
}
