#ifndef __CONTENTSMALLRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __CONTENTSMALLRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ContentsMallRecord {
public:
	//持失切
	ContentsMallRecord();
	//社瑚切
	~ContentsMallRecord();

	//Header 92
	static const byte NfcContentsMall_Header;
	//Type Length
	static const byte NfcContentsMall_TypeLength;
	//Payload Type aqcoupon
	static const byte NfcContentsMall_PayloadType[23];
	//
	static byte* makePayloadData(CString payloaddata, byte* NfcParserData, int& NfcParserDatalen);
};
#endif //__CONTENTSMALLRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__