#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|//생성자
//|
//---------------------------------------------------------------
CNfcUriRecord::CNfcUriRecord(void)
{
	AddData((byte) 0x01, "http://www.");
	AddData((byte) 0x02, "https://www.");
	AddData((byte) 0x03, "http://");
	AddData((byte) 0x04, "https://");
	AddData((byte) 0x05, "tel:");
	AddData((byte) 0x06, "mailto:");
	AddData((byte) 0x07, "ftp://anonymous:anonymous@");
	AddData((byte) 0x08, "ftp://ftp.");
	AddData((byte) 0x09, "ftps://");
	AddData((byte) 0x0A, "sftp://");
	AddData((byte) 0x0B, "smb://");
	AddData((byte) 0x0C, "nfs://");
	AddData((byte) 0x0D, "ftp://");
	AddData((byte) 0x0E, "dav://");
	AddData((byte) 0x0F, "news:");
	AddData((byte) 0x10, "telnet://");
	AddData((byte) 0x11, "imap:");
	AddData((byte) 0x12, "rtsp://");
	AddData((byte) 0x13, "urn:");
	AddData((byte) 0x14, "pop:");
	AddData((byte) 0x15, "sip:");
	AddData((byte) 0x16, "sips:");
	AddData((byte) 0x17, "tftp:");
	AddData((byte) 0x18, "btspp://");
	AddData((byte) 0x19, "btl2cap://");
	AddData((byte) 0x1A, "btgoep://");
	AddData((byte) 0x1B, "tcpobex://");
	AddData((byte) 0x1C, "irdaobex://");
	AddData((byte) 0x1D, "file://");
	AddData((byte) 0x1E, "urn:epc:id:");
	AddData((byte) 0x1F, "urn:epc:tag:");
	AddData((byte) 0x20, "urn:epc:pat:");
	AddData((byte) 0x21, "urn:epc:raw:");
	AddData((byte) 0x22, "urn:epc:");
	AddData((byte) 0x23, "urn:nfc:");
}

//--------------------------------------------------------------
//|
//|//소멸자
//|
//---------------------------------------------------------------
CNfcUriRecord::~CNfcUriRecord(void)
{

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//Uri
CAtlList<CNfcUriRecord::mItem,CNfcUriRecord::MyTraits> CNfcUriRecord::storage;
//Header D2
const byte CNfcUriRecord::Uri_Well_Known_Header=0x51;
//Type Length
const byte CNfcUriRecord::Uri_Well_Known_TypeLength=0x01;
//Payload Type "U"
const byte CNfcUriRecord::Rtd_Uri_PayloadType[1]={0x55};

//--------------------------------------------------------------
//|
//| Payload 만들기
//|
//---------------------------------------------------------------
byte* CNfcUriRecord::makePayloadData(CString key, CString uridata, byte* NfcParserData, int& NfcParserDatalen)
{
	//배열 초기화
	memset(NfcParserData, NULL, NfcParserDatalen);
	NfcParserDatalen=0;
	//header 51 TNF_WELL_KNOWN 마지막 데이터
	NfcParserData[NfcParserDatalen++]=Uri_Well_Known_Header;
	//type 0x55 "U" 길이
	NfcParserData[NfcParserDatalen++]=Uri_Well_Known_TypeLength;
	//http://www. 키 변환Uri
	int payloadlen = uridata.GetLength()-key.GetLength();
	//http://www.(0x01) 1바이트 
	NfcParserData[NfcParserDatalen++]=(1+payloadlen)&0xFF;
	//"U" RTD_URI type
	memcpy(&NfcParserData[NfcParserDatalen], &Rtd_Uri_PayloadType[0], Uri_Well_Known_TypeLength);
	NfcParserDatalen+=Uri_Well_Known_TypeLength;
	//http://www.을 byte 값으로 대체
	byte _cmd;
	GetCmd(key, _cmd);
	NfcParserData[NfcParserDatalen++]=_cmd;
	//http://www. 뒤의 데이터 추가
	memcpy(&NfcParserData[NfcParserDatalen], uridata.GetBuffer()+key.GetLength(), payloadlen);
	NfcParserDatalen += payloadlen;
	return NfcParserData;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CNfcUriRecord::AddData(int _cmd, CString _cmdName)
{
	BOOL b=FALSE;
	mItem add_teim;
	add_teim.cmd=_cmd&0xFF;
	add_teim.cmdName=_cmdName;
	storage.AddTail(add_teim);
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CNfcUriRecord::GetCmd(CString _cmdName, byte& _cmd)
{
	BOOL b=FALSE;
	mItem search_item;
	search_item.cmd=_cmd;
	search_item.cmdName=_cmdName;
	POSITION pos =  storage.Find(search_item);
	if(pos!=NULL)
	{
		_cmd = storage.GetAt(pos).cmd;
		b=TRUE;
	}	
	return b;
}
