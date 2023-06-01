
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
	//������
	CAtlmapNfcData();
	//�Ҹ���
	~CAtlmapNfcData();

	//�߰�
	BOOL AddData(int name, byte* data, int datalen);
	//����
	BOOL DeleteData(int name);
	//������ ��� ����
	BOOL DeleteDataAll();
	//(Key �̸�����)
	int GetApduCmdData(int name, byte* data, int datalen);
	//
	int GetNdefData(int name, byte* data, int datalen);
	//Mifare Data ����� �Լ�
	int GetMifareData(int name, byte* data, int datalen);
};
#endif// __ATLMAPNFCDATA_B7A15A9D01DC403996FC45F7F92F52D3_H__
