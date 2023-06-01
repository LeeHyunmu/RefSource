#ifndef __ADVERTRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ADVERTRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class AdvertRecord {
public:
	//持失切
	AdvertRecord();
	//社瑚切
	~AdvertRecord();

	//Header D2
	static const byte NfcAdvert_Header;
	//Type Length
	static const byte NfcAdvert_TypeLength;

	//Payload Type 
	static const byte NfcAdvert_PayloadType[16];
	//
	static byte* makePayloadData(CString adorg, CString link, CString delivery, byte* NfcParserData, int& NfcParserDatalen);
	//
	static int PayloadString(CString adorg, CString link, CString delivery, byte* payload, int len);
};
#endif //__ADVERTRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__