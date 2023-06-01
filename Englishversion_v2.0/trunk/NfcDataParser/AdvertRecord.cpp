#include "stdAfx.h"

#define ADVERTMAXLEN_MAKEPAYLOAD 256
//--------------------------------------------------------------
//|
//|//생성자
//|
//---------------------------------------------------------------
AdvertRecord::AdvertRecord(void)
{

}

//--------------------------------------------------------------
//|
//|//소멸자
//|
//---------------------------------------------------------------
AdvertRecord::~AdvertRecord(void)
{

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Header D2
const byte AdvertRecord::NfcAdvert_Header=0xD2;
//Type Length
const byte AdvertRecord::NfcAdvert_TypeLength=0x10;
//Payload Type text/aq-ActiveAD
const byte AdvertRecord::NfcAdvert_PayloadType[16]=
{0x74, 0x65, 0x78, 0x74, 0x2F, 0x61, 0x71, 0x2D, 0x41, 0x63, 0x74, 0x69, 0x76, 0x65, 0x41, 0x44};

//--------------------------------------------------------------
//|
//| Payload 만들기
//|
//---------------------------------------------------------------
byte* AdvertRecord::makePayloadData(CString adorg, CString link, CString delivery, byte* NfcParserData, int& NfcParserDatalen)
{
	byte payload[ADVERTMAXLEN_MAKEPAYLOAD];
	int payloadlen = PayloadString(adorg, link, delivery, &payload[0], ADVERTMAXLEN_MAKEPAYLOAD);
	//배열 초기화
	memset(NfcParserData, NULL, NfcParserDatalen);
	NfcParserDatalen=0;
	NfcParserData[NfcParserDatalen++]=NfcAdvert_Header;
	NfcParserData[NfcParserDatalen++]=NfcAdvert_TypeLength;
	NfcParserData[NfcParserDatalen++]=payloadlen&0xFF;
	memcpy(&NfcParserData[NfcParserDatalen], NfcAdvert_PayloadType, NfcAdvert_TypeLength);
	NfcParserDatalen+=NfcAdvert_TypeLength;
	memcpy(&NfcParserData[NfcParserDatalen], payload, payloadlen);
	NfcParserDatalen += payloadlen;
	return NfcParserData;
}
//--------------------------------------------------------------
//|
//| Payload 만들기
//|
//---------------------------------------------------------------
int AdvertRecord::PayloadString(CString adorg, CString link, CString delivery, byte* payload, int len)
{
	//배열 초기화
	memset(payload, NULL, len);
	CString str="";
	str+="BEGIN:AQAD\r\n";
	str+="ADORG:"+adorg+"\r\n";
	str+="LINK:"+link+"\r\n";
	str+="DELIVERY:"+delivery+"\r\n";
	str+="END:AQAD\r\n";
	memcpy(&payload[0], str.GetBuffer(0), str.GetLength());
	return str.GetLength();
}