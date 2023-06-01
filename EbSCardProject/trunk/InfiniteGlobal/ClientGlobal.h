
#ifndef __CLIENTGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __CLIENTGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//=======================================================//
#define  MAX_SOCKBUF	512
//--------------------------------------------------------------
//|
//| 프로토콜 데이터 구조체
//|
//---------------------------------------------------------------
#pragma pack(push,1)
//==================================================
// 서버 통신 구조체
//================================================
struct ChargeInitRequest
{
	CHAR CTN[12];			//전화번호
	CHAR REQ_DATE[14];		//거래요청 일시
	CHAR REQ_AMT[10];		//요청금액
	CHAR FEE_AMT[10];		//수수료 금액
	CHAR PGPAV_AMT[10];		//PG 결제 금액
	CHAR BEF_AMT[10];		//거래 전 카드 잔액
	CHAR AFT_AMT[10];		//거래 후 카드 잔액
	CHAR ALGORI_ID[2];		//사용 알고리즘	
	CHAR KSET_VER[2];		//전자화폐사용ID
	CHAR CARD_ID[16];		//전자화폐ID
	CHAR CARD_TR_SEQ[10];	//전자화폐거래일련번호
	CHAR RANDM_NUM[16];		//전자화폐가 생성한 난수
	CHAR SIGN1[8];			//전자화폐가 생성한 서명
	CHAR CARD_CLASS_CD[2];	//카드소지자 구분
	CHAR AFF_ID[10];		//결제사 코드
	CHAR AFF_TRANS_ID[30];	//결제사 거래번호
	CHAR PAY_METHOD_CD[2];	//결제 수단
	CHAR PADDING[10];		//padding
	CHAR end;
};
struct ChargeInitResponse
{
	CHAR REPL_CD[11];			//응답코드
	CHAR ERR_MSG[50];			//응답메시지
	CHAR REQ_AMT[10];			//충전요청금액
	CHAR FEE_AMT[10];			//수수료 금액
	CHAR PGPAV_AMT[10];			//PG 결제 금액
	CHAR CHG_AFT_AMT[10];		//거래 후 카드 잔액
	CHAR BEF_AMT[10];			//거래 전 카드 잔액
	CHAR ALGORI_ID[2];			//사용 알고리즘
	CHAR CARD_ID[16];			//전자화폐ID
	CHAR CARD_TR_SEQ[10];		//전자화폐 거래 일련번호
	CHAR CARD_RANDOMNUM[16];	//전자화폐가 생성한 난수
	CHAR CARD_CLASS_CD[2];		//카드 소지자 구분
	CHAR SAM_ID[16];			//센터 SAM ID
	CHAR SAM_SEQ[10];			//센터 LSAM 일련번호
	CHAR SIGN2[8];				//센터 서명
	CHAR SAM_NT[10];			//센터 거래일련번호
	CHAR AFF_ID[10];			//결제사 코드
	CHAR AFF_TRANS_ID[30];		//결제사 거래번호
	CHAR PAY_METHOD_CD[2];		//결제 수단
	CHAR CUST_INFO[20];			//결제사 고유정보
	CHAR RES_DATA[20];			//결제사 거래일시
	CHAR PADDING[6];			//padding
	CHAR end;
};
struct ChargeResultRequest
{
	CHAR CTN[12];				//전화번호
	CHAR REQ_DATE[14];			//거래요청 일시
	CHAR REQ_AMT[10];			//요청금액
	CHAR FEE_AMT[10];			//수수료 금액
	CHAR PGPAV_AMT[10];			//PG 결제 금액
	CHAR BEF_AMT[10];			//거래 전 카드 잔액
	CHAR AFT_AMT[10];			//거래 후 카드 잔액
	CHAR CARD_ID[16];			//전자화폐ID
	CHAR CARD_TR_SEQ[10];		//카드거래 일련번호
	CHAR ID_CENTER[2];			//전자화폐사ID
	CHAR ALGORI_ID[2];			//알고리즘ID
	CHAR SAM_SEQ[10];			//SAM 거래 일련번호
	CHAR SAM_ID[16];			//SAM ID
	CHAR SAM_NT[10];			//센터 SAM 거래일련번호
	CHAR SIGN3[8];				//S3
	CHAR APPROVALNUM[16];		//서버승인번호
	CHAR REQ_DATA[8];			//거래요청 일자
	CHAR REQ_TIME[6];			//거래요청 시간
	CHAR CARD_CLASS_CD[2];		//카드소지자 구분
	CHAR AFF_ID[10];			//결제사 코드
	CHAR AFF_TRANS_ID[30];		//결제사 거래번호
	CHAR PAY_METHOD_CD[2];		//결제 수단
	CHAR PADDING[12];			//padding
	CHAR end;
};
struct ChargeResultResponse
{
	CHAR REPL_CD[10];			//응답코드
	CHAR ERR_MSG[50];			//응답메시지
	CHAR PADDING[4];			//padding
	CHAR end;
};
#pragma pack(pop)

#endif //__CLIENTGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__