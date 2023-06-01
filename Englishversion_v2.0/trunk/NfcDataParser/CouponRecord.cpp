#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|//생성자
//|
//---------------------------------------------------------------
CouponRecord::CouponRecord(void)
{

}

//--------------------------------------------------------------
//|
//|//소멸자
//|
//---------------------------------------------------------------
CouponRecord::~CouponRecord(void)
{

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Header D2
const byte CouponRecord::NfcCoupon_Header=0x92;
//Type Length
const byte CouponRecord::NfcCoupon_TypeLength=0x15;
//Payload Type aqcoupon
const byte CouponRecord::NfcCoupon_PayloadType[21]={0x61, 0x70, 0x70, 0x6C, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6F, 0x6E
,0x2F, 0x76, 0x6E, 0x64, 0x2E, 0x6D, 0x64, 0x61, 0x70, 0x70};

//--------------------------------------------------------------
//|
//| Payload 만들기
//|
//---------------------------------------------------------------
byte* CouponRecord::makePayloadData(CString payloaddata, byte* NfcParserData, int& NfcParserDatalen)
{
	//배열 초기화
	memset(NfcParserData, NULL, NfcParserDatalen);
	NfcParserDatalen=0;
	NfcParserData[NfcParserDatalen++]=NfcCoupon_Header;
	NfcParserData[NfcParserDatalen++]=NfcCoupon_TypeLength;
	int payloadlen = payloaddata.GetLength();
	NfcParserData[NfcParserDatalen++]=payloadlen&0xFF;
	memcpy(&NfcParserData[NfcParserDatalen], &NfcCoupon_PayloadType[0], NfcCoupon_TypeLength);
	NfcParserDatalen+=NfcCoupon_TypeLength;
	memcpy(&NfcParserData[NfcParserDatalen], payloaddata.GetBuffer(), payloadlen);
	NfcParserDatalen += payloadlen;
	return NfcParserData;
}
