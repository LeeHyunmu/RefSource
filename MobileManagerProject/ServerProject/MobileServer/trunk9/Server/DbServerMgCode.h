
#ifndef __DBSERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DBSERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			코드
//======================================================//
//--------------------------------------------------------------
//|
//|Recv Send Code
//|
//---------------------------------------------------------------
//MYBI가 클라이언트로 붙을 때
//데이터 수신
#define MYBIDATABASECOMM_DATARECV              0x00000001
//데이터 요청
#define MYBIDATABASECOMM_DATASEND              0x00000002
//Recv 스타트
#define MYBIDATABASECOMM_STARTDATARECV         0x00000010
//확인 요청
#define MYBIDATABASECOMM_IDENTIFYRECV          0x00000004
//확인 요청 처리
#define MYBIDATABASECOMM_IDENTIFYSEND          0x00000008

//관리서버에서 마이비로 요청이 필요할 때
//확인 요청
#define MOBILEDATABASECOMM_IDENTIFYRECV          0x00000040
//확인 요청 처리
#define MOBILEDATABASECOMM_IDENTIFYSEND          0x00000080

//에러
//CODE가 에러가 났다
#define DATABASECOMM_ERROR    	   	      0x10000000
//=======================================================//
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//=====================================================
// 스래드 생성 개수
//======================================================
#define  IOCP_DbSERVERCOUNT 1 // IOCP_DbSERVERCOUNT 개수
#define  IOCP_DbCLIENTCOUNT 1 // IOCP_DbCLIENTCOUNT 개수
//=====================================================
// Mybi 서버 IP, Port
//======================================================
#define  DBSERVER_IP "127.0.0.1"
#define  DBSERVER_PORT 9102
//=====================================================
// MS SQL ADO 연동 스트링(Database=Mobile에 접속)
//======================================================
#define DBCONNECT_STR  _T("Driver={SQL Server};Server=localhost;Trusted_Connection=Yes;Database=Mobile;Initial Catalog=ADO;Max Pool Size=75;Min Pool Size=5;uid=sa;pdw=gusan5763")

#endif //__DBSERVERMGCODE_74F736E25AB94b5dA789E4730F58F2A1_H__