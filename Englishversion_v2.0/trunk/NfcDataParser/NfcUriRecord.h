#ifndef __NFCURIRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __NFCURIRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNfcUriRecord {

	struct mItem
	{
		byte cmd;
		CString cmdName;
	};
	class MyTraits : public CElementTraits<mItem>
	{
	public:
		static bool CompareElements(const mItem& element1, const mItem& element2)
		{
			if(element1.cmdName.Compare(element2.cmdName)==0)
				return true;
			else
				return false;
		}
	};
	static CAtlList<mItem,MyTraits> storage;
public:
	//持失切
	CNfcUriRecord();
	//社瑚切
	~CNfcUriRecord();

	//
	static BOOL AddData(int _cmd, CString _cmdName);
	//
	static BOOL GetCmd(CString _cmdName, byte& _cmd);

	//Header D2
	static const byte Uri_Well_Known_Header;
	//Type Length
	static const byte Uri_Well_Known_TypeLength;
	//Payload Type aqcoupon
	static const byte Rtd_Uri_PayloadType[1];
	//
	static byte* makePayloadData(CString key, CString uridata, byte* NfcParserData, int& NfcParserDatalen);
};
#endif //__NFCURIRECORD_74F736E25AB94b5dA789E4730F58F2A1_H__