
#ifndef __SCARDGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SCARDGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
#pragma pack(push,1)
//==================================================
// ī�� ��� ����ü
//================================================
struct ChargeinitCardDataSend
{
	CHAR CARDCHARGEINIT[10];	//�������� ī�� �ڵ�
	CHAR CHARGE_MONEY[8];		//���� �ݾ�
	CHAR end;
};
struct ChargeinitCardDataRecv
{
	CHAR ALGORI_ID[2];			//�˰���
	CHAR BEF_AMT[8];			//�ܾ�
	CHAR KSET_VER[2];			//����ȭ��ȸ��ID
	CHAR CARD_ID[16];			//����ȭ��ID
	CHAR CARD_TR_SEQ[8];		//�ŷ��Ϸù�ȣ
	CHAR RANDM_NUM[16];			//��������
	CHAR SIGN1[8];				//S1
	CHAR end;
};
struct ChargeLoadCardDataSend
{
	CHAR CARDCHARGELOAD[8];		//ī�� �ŷ�(�����ʱ�����)
	CHAR SAM_ID[16];;			//����SAMID
	CHAR SAM_SEQ[8];			//����SAM�ŷ��Ϸù�ȣ
	CHAR SIGN2[8];				//S2
	CHAR end;
};
struct ChargeLoadCardDataRecv
{
	CHAR BEF_AMT[8];			//�ܾ�
	CHAR SIGN3[8];              //S3
	CHAR end;
};
#pragma pack(pop)
#endif //__SCARDGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__