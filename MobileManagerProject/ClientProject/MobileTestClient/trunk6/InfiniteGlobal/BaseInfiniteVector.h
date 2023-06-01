
#ifndef __BASEINFINITEVECTOR_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __BASEINFINITEVECTOR_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif 
//
typedef vector<int> vecInt;
typedef vector<CString> vecString;
typedef vector<short> vecShort;
typedef vector<char> vecChar;
//
typedef vector<CPoint> vecPoint;

//--------------------------------------------------------------
//|
//|Recv Code
//|
//---------------------------------------------------------------
//VM 요청 종류
//카드 사용 승인
#define RECVCODE_CARDENABLE            0x00000002
//권종 변경
#define RECVCODE_USERCHANGE            0x00000004
//소득 등록
#define RECVCODE_DEDUCTION             0x00000008
//사용 내역 조회
#define RECVCODE_USECHECK              0x00000010

//에러
//CODE가 에러가 났다
#define RECVCODE_WHERECORDERROR	   	   0x01000000
//(어떤 데이터 처리할 지 알 수 없을 때)DATA처리 코드 에러 났다
#define RECVCODE_WHATDATAERROR     	   0x02000000
//DATA가 에러가 났다
#define RECVCODE_DATAERROR      	   0x04000000
//에러(에러 종류가 불 분명하다)
#define RECVCODE_BASEERROR  		   0x08000000

//통신 개체
//VM => 관리서버
#define RECVCODE_VMCLIENTRECV		   0x10000000
//MYBI => 관리서버
#define RECVCODE_MYBISERVERRECV		   0x20000000
//관리서버 => VM
#define RECVCODE_VMCLIENTSEND	       0x40000000
//관리서버 => MYBI
#define RECVCODE_MYBISERVERSEND		   0x80000000
//--------------------------------------------------------------
//|
//|카드Enable, 권종변경, 소득공제등록, 사용내역조회
//|
//---------------------------------------------------------------


#endif //__BASEINFINITEVECTOR_B7A15A9D01DC403996FC45F7F92F52D3_H__