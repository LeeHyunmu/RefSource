
#ifndef __SCARDGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SCARDGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
#pragma pack(push,1)
//==================================================
// 카드 통신 구조체
//================================================
struct ChargeinitCardDataSend
{
	CHAR CARDCHARGEINIT[10];	//충전시작 카드 코드
	CHAR CHARGE_MONEY[8];		//충전 금액
	CHAR end;
};
struct ChargeinitCardDataRecv
{
	CHAR ALGORI_ID[2];			//알고리즘
	CHAR BEF_AMT[8];			//잔액
	CHAR KSET_VER[2];			//전자화폐회사ID
	CHAR CARD_ID[16];			//전자화폐ID
	CHAR CARD_TR_SEQ[8];		//거래일련번호
	CHAR RANDM_NUM[16];			//생성난수
	CHAR SIGN1[8];				//S1
	CHAR end;
};
struct ChargeLoadCardDataSend
{
	CHAR CARDCHARGELOAD[8];		//카드 거래(충전초기전문)
	CHAR SAM_ID[16];;			//충전SAMID
	CHAR SAM_SEQ[8];			//충전SAM거래일련번호
	CHAR SIGN2[8];				//S2
	CHAR end;
};
struct ChargeLoadCardDataRecv
{
	CHAR BEF_AMT[8];			//잔액
	CHAR SIGN3[8];              //S3
	CHAR end;
};
#pragma pack(pop)
#endif //__SCARDGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__