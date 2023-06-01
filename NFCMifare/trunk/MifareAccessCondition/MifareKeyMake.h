// MifareKeyMake.h

#ifndef __MIFAREKEYMAKE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __MIFAREKEYMAKE_B7A15A9D01DC403996FC45F7F92F52D3_H__

class CMifareKeyMake
{

public:
	//블록 값
	enum BlockNum{
		Mifare_datablock0=0,
		Mifare_datablock1=1,
		Mifare_datablock2=2,
		Mifarekeyblock=3
	};
	CMifareKeyMake();
	~CMifareKeyMake();
	//Access Condition 가져오기
	byte GetAccessCondition(byte* AccessBits, BlockNum blocknum);
	//Access Condition 변경
	BOOL SetAccessCondition(byte* AccessBits, BlockNum blocknum, byte key);
};

#endif//__MIFAREKEYMAKE_B7A15A9D01DC403996FC45F7F92F52D3_H__
