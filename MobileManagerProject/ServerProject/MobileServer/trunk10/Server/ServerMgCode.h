
#ifndef __SERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__

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
//VM ��û ����
//ī�� ��� ����
#define RECVCODE_CARDENABLE            0x00000002
//���� ����
#define RECVCODE_USERCHANGE            0x00000004
//�ҵ� ���
#define RECVCODE_DEDUCTION             0x00000008
//��� ���� ��ȸ
#define RECVCODE_USECHECK              0x00000010

//����
//CODE�� ������ ����
#define RECVCODE_WHERECORDERROR	   	   0x01000000
//(� ������ ó���� �� �� �� ���� ��)DATAó�� �ڵ� ���� ����
#define RECVCODE_WHATDATAERROR     	   0x02000000
//DATA�� ������ ����
#define RECVCODE_DATAERROR      	   0x04000000
//����(���� ������ �� �и��ϴ�)
#define RECVCODE_BASEERROR  		   0x08000000

//��� ��ü
//VM => ��������
#define RECVCODE_VMCLIENTRECV		   0x10000000
//MYBI => ��������
#define RECVCODE_MYBISERVERRECV		   0x20000000
//�������� => VM
#define RECVCODE_VMCLIENTSEND	       0x40000000
//�������� => MYBI
#define RECVCODE_MYBISERVERSEND		   0x80000000
//=======================================================//
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//=====================================================
// ������ ���� ����
//======================================================
#define  IOCP_SERVERCOUNT 2 //CPU*IOCP_SERVERCOUNT
#define  IOCP_CLIENTCOUNT 2 //CPU*IOCP_CLIENTCOUNT
//=====================================================
// Mybi ���� IP, Port
//======================================================
#define  MB_IP "127.0.0.1"
#define  MB_PORT 9101
//=====================================================
// MS SQL ADO ���� ��Ʈ��(Database=Mobile�� ����)
//======================================================
#define DBCONNECT_STR  _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763")

#endif //__SERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__