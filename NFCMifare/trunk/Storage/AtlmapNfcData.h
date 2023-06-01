
#ifndef __ATLMAPNFCDATA_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __ATLMAPNFCDATA_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAtlmapNfcData
{
	//Apud header
	static const byte NfcApduCmdHeader[4];
	static const byte NfcMifareHeader[1];
	struct Data 
	{
		int datalen;
		byte* data;
	};
	// CAtlMap
	typedef CAtlMap<int,Data> NfcCommand;
	typedef CAtlMap<int,Data>::CPair* NfcCommandPair;
	NfcCommand storage;
public:
	//생성자
	CAtlmapNfcData();
	//소멸자
	~CAtlmapNfcData();

	//추가
	BOOL AddData(int name, byte* data, int datalen);
	//삭제
	BOOL DeleteData(int name);
	//아이템 모두 삭제
	BOOL DeleteDataAll();
	//(Key 이름으로)
	int GetApduCmdData(int name, byte* data, int datalen);
	//
	int GetNdefData(int name, byte* data, int datalen);
	//Mifare Data 만들기 함수
	int GetMifareData(int name, byte* data, int datalen);
};
#endif// __ATLMAPNFCDATA_B7A15A9D01DC403996FC45F7F92F52D3_H__
