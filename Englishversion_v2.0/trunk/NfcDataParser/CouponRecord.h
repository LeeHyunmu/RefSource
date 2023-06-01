#ifndef __COUPONRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __COUPONRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CouponRecord {
public:
	//持失切
	CouponRecord();
	//社瑚切
	~CouponRecord();

	//Header 92
	static const byte NfcCoupon_Header;
	//Type Length
	static const byte NfcCoupon_TypeLength;
	//Payload Type aqcoupon
	static const byte NfcCoupon_PayloadType[21];
	//
	static byte* makePayloadData(CString payloaddata, byte* NfcParserData, int& NfcParserDatalen);
};
#endif //__COUPONRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__