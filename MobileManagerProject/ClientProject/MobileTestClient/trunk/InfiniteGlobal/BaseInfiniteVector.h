
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
//| Code
//|
//---------------------------------------------------------------
#define RECVCODE_CARDENABLE            0x00000002
#define RECVCODE_USERCHANGE            0x00000004
#define RECVCODE_DEDUCTION             0x00000008
#define RECVCODE_USECHECK              0x00000010
#define RECVCODE_VMCLIENTRECV		   0x10000000
#define RECVCODE_MYBISERVERRECV		   0x20000000
#define RECVCODE_VMCLIENTSEND	       0x40000000
#define RECVCODE_MYBISERVERSEND		   0x80000000
//--------------------------------------------------------------
//|
//|카드Enable, 권종변경, 소득공제등록, 사용내역조회
//|
//---------------------------------------------------------------


#endif //__BASEINFINITEVECTOR_B7A15A9D01DC403996FC45F7F92F52D3_H__