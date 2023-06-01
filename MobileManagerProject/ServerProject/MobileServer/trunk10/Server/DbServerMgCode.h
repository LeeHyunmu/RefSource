
#ifndef __DBSERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DBSERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			�ڵ�
//======================================================//
//--------------------------------------------------------------
//|
//|Recv Send Code
//|
//---------------------------------------------------------------
//MYBI�� Ŭ���̾�Ʈ�� ���� ��
//������ ����
#define MYBIDATABASECOMM_DATARECV              0x00000001
//������ ��û
#define MYBIDATABASECOMM_DATASEND              0x00000002
//Recv ��ŸƮ
#define MYBIDATABASECOMM_STARTDATARECV         0x00000010
//Ȯ�� ��û
#define MYBIDATABASECOMM_IDENTIFYRECV          0x00000004
//Ȯ�� ��û ó��
#define MYBIDATABASECOMM_IDENTIFYSEND          0x00000008

//������������ ���̺�� ��û�� �ʿ��� ��
//Ȯ�� ��û
#define MOBILEDATABASECOMM_IDENTIFYRECV          0x00000040
//Ȯ�� ��û ó��
#define MOBILEDATABASECOMM_IDENTIFYSEND          0x00000080

//����
//CODE�� ������ ����
#define DATABASECOMM_ERROR    	   	      0x10000000
//=======================================================//
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//=====================================================
// ������ ���� ����
//======================================================
#define  IOCP_DbSERVERCOUNT 1 // IOCP_DbSERVERCOUNT ����
#define  IOCP_DbCLIENTCOUNT 1 // IOCP_DbCLIENTCOUNT ����
//=====================================================
// Mybi ���� IP, Port
//======================================================
#define  DBSERVER_IP "127.0.0.1"
#define  DBSERVER_PORT 9102
//=====================================================
// MS SQL ADO ���� ��Ʈ��(Database=Mobile�� ����)
//======================================================
#define DBCONNECT_STR  _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763")

#endif //__DBSERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__