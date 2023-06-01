// CMifareKeyMake.cpp

#include "stdafx.h"
#include "MifareKeyMake.h"


//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CMifareKeyMake::CMifareKeyMake()
{

}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CMifareKeyMake::~CMifareKeyMake()
{

}
//--------------------------------------------------------------
//|
//|Access Condition 가져오기
//|
//---------------------------------------------------------------
byte CMifareKeyMake::GetAccessCondition(byte* AccessBits, BlockNum blocknum)
{
	byte accesscondition=0;
	switch (blocknum)
	{
	case Mifare_datablock0:
		{
			accesscondition += (AccessBits[1]>>4)&0x1;
			accesscondition += (AccessBits[2]<<1)&0x2;
			accesscondition += (AccessBits[2]>>2)&0x4;
		}
		break;
	case Mifare_datablock1:
		{
			accesscondition += (AccessBits[1]>>5)&0x1;
			accesscondition += (AccessBits[2])&0x2;
			accesscondition += (AccessBits[2]>>3)&0x4;
		}
		break;
	case Mifare_datablock2:
		{
			accesscondition += (AccessBits[1]>>6)&0x1;
			accesscondition += (AccessBits[2]>>1)&0x2;
			accesscondition += (AccessBits[2]>>4)&0x4;
		}
		break;
	case Mifarekeyblock:
		{
			accesscondition += (AccessBits[1]>>7)&0x1;
			accesscondition += (AccessBits[2]>>2)&0x2;
			accesscondition += (AccessBits[2]>>5)&0x4;
		}
		break;
	}
	return accesscondition;
}
//--------------------------------------------------------------
//|
//|Access Condition 변경
//|
//---------------------------------------------------------------
BOOL CMifareKeyMake::SetAccessCondition(byte* AccessBits, BlockNum blocknum, byte key)
{
	byte C1 = (AccessBits[1]>>4)&0xFF;
	byte C2 = (AccessBits[2])&0xFF;
	byte C3 = (AccessBits[2]>>4)&0xFF;
	//
	byte C3Key = key&0x01;
	byte C2Key = (key>>1)&0x01;
	byte C1Key = (key>>2)&0x01;

	byte accesscondition=0;
	switch (blocknum)
	{
	case Mifare_datablock0:
		{
			C1&=0x0E; C1+=C1Key;
			C2&=0x0E; C2+=C2Key;
			C3&=0x0E; C3+=C3Key;

		}
		break;
	case Mifare_datablock1:
		{
			C1&=0x0D; C1+=(C1Key<<1);
			C2&=0x0D; C2+=(C2Key<<1);
			C3&=0x0D; C3+=(C3Key<<1);

		}
		break;
	case Mifare_datablock2:
		{
			C1&=0x0B; C1+=(C1Key<<2);
			C2&=0x0B; C2+=(C2Key<<2);
			C3&=0x0B; C3+=(C3Key<<2);

		}
		break;
	case Mifarekeyblock:
		{
			C1&=0x07; C1+=(C1Key<<3);
			C2&=0x07; C2+=(C2Key<<3);
			C3&=0x07; C3+=(C3Key<<3);

		}
		break;
	}
	byte Access1 = ((0x0F^C2)<<4)+(0x0F^C1);
	byte Access2 = (C1<<4)+(0x0F^C3);
	byte Access3 = (C3<<4)+C2;

	AccessBits[1]=Access1;
	AccessBits[2]=Access2;
	AccessBits[3]=Access3;
	return TRUE;
}