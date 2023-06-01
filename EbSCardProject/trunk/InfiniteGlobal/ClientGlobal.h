
#ifndef __CLIENTGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __CLIENTGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//
#define  MAX_SOCKBUF	512
//--------------------------------------------------------------
//|
//| �������� ������ ����ü
//|
//---------------------------------------------------------------
#pragma pack(push,1)
//==================================================
// ���� ��� ����ü
//================================================
struct ChargeInitRequest
{
	CHAR CTN[12];			//��ȭ��ȣ
	CHAR REQ_DATE[14];		//�ŷ���û �Ͻ�
	CHAR REQ_AMT[10];		//��û�ݾ�
	CHAR FEE_AMT[10];		//������ �ݾ�
	CHAR PGPAV_AMT[10];		//PG ���� �ݾ�
	CHAR BEF_AMT[10];		//�ŷ� �� ī�� �ܾ�
	CHAR AFT_AMT[10];		//�ŷ� �� ī�� �ܾ�
	CHAR ALGORI_ID[2];		//��� �˰���	
	CHAR KSET_VER[2];		//����ȭ����ID
	CHAR CARD_ID[16];		//����ȭ��ID
	CHAR CARD_TR_SEQ[10];	//����ȭ��ŷ��Ϸù�ȣ
	CHAR RANDM_NUM[16];		//����ȭ�� ������ ����
	CHAR SIGN1[8];			//����ȭ�� ������ ����
	CHAR CARD_CLASS_CD[2];	//ī������� ����
	CHAR AFF_ID[10];		//������ �ڵ�
	CHAR AFF_TRANS_ID[30];	//������ �ŷ���ȣ
	CHAR PAY_METHOD_CD[2];	//���� ����
	CHAR PADDING[10];		//padding
	CHAR end;
};
struct ChargeInitResponse
{
	CHAR REPL_CD[11];			//�����ڵ�
	CHAR ERR_MSG[50];			//����޽���
	CHAR REQ_AMT[10];			//������û�ݾ�
	CHAR FEE_AMT[10];			//������ �ݾ�
	CHAR PGPAV_AMT[10];			//PG ���� �ݾ�
	CHAR CHG_AFT_AMT[10];		//�ŷ� �� ī�� �ܾ�
	CHAR BEF_AMT[10];			//�ŷ� �� ī�� �ܾ�
	CHAR ALGORI_ID[2];			//��� �˰���
	CHAR CARD_ID[16];			//����ȭ��ID
	CHAR CARD_TR_SEQ[10];		//����ȭ�� �ŷ� �Ϸù�ȣ
	CHAR CARD_RANDOMNUM[16];	//����ȭ�� ������ ����
	CHAR CARD_CLASS_CD[2];		//ī�� ������ ����
	CHAR SAM_ID[16];			//���� SAM ID
	CHAR SAM_SEQ[10];			//���� LSAM �Ϸù�ȣ
	CHAR SIGN2[8];				//���� ����
	CHAR SAM_NT[10];			//���� �ŷ��Ϸù�ȣ
	CHAR AFF_ID[10];			//������ �ڵ�
	CHAR AFF_TRANS_ID[30];		//������ �ŷ���ȣ
	CHAR PAY_METHOD_CD[2];		//���� ����
	CHAR CUST_INFO[20];			//������ ��������
	CHAR RES_DATA[20];			//������ �ŷ��Ͻ�
	CHAR PADDING[6];			//padding
	CHAR end;
};
struct ChargeResultRequest
{
	CHAR CTN[12];				//��ȭ��ȣ
	CHAR REQ_DATE[14];			//�ŷ���û �Ͻ�
	CHAR REQ_AMT[10];			//��û�ݾ�
	CHAR FEE_AMT[10];			//������ �ݾ�
	CHAR PGPAV_AMT[10];			//PG ���� �ݾ�
	CHAR BEF_AMT[10];			//�ŷ� �� ī�� �ܾ�
	CHAR AFT_AMT[10];			//�ŷ� �� ī�� �ܾ�
	CHAR CARD_ID[16];			//����ȭ��ID
	CHAR CARD_TR_SEQ[10];		//ī��ŷ� �Ϸù�ȣ
	CHAR ID_CENTER[2];			//����ȭ���ID
	CHAR ALGORI_ID[2];			//�˰���ID
	CHAR SAM_SEQ[10];			//SAM �ŷ� �Ϸù�ȣ
	CHAR SAM_ID[16];			//SAM ID
	CHAR SAM_NT[10];			//���� SAM �ŷ��Ϸù�ȣ
	CHAR SIGN3[8];				//S3
	CHAR APPROVALNUM[16];		//�������ι�ȣ
	CHAR REQ_DATA[8];			//�ŷ���û ����
	CHAR REQ_TIME[6];			//�ŷ���û �ð�
	CHAR CARD_CLASS_CD[2];		//ī������� ����
	CHAR AFF_ID[10];			//������ �ڵ�
	CHAR AFF_TRANS_ID[30];		//������ �ŷ���ȣ
	CHAR PAY_METHOD_CD[2];		//���� ����
	CHAR PADDING[12];			//padding
	CHAR end;
};
struct ChargeResultResponse
{
	CHAR REPL_CD[10];			//�����ڵ�
	CHAR ERR_MSG[50];			//����޽���
	CHAR PADDING[4];			//padding
	CHAR end;
};
#pragma pack(pop)

#endif //__CLIENTGLOBAL_B7A15A9D01DC403996FC45F7F92F52D3_H__