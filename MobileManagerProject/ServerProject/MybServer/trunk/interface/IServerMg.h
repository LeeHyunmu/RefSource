
#ifndef __ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|서버용 구조체 선언
//|
//---------------------------------------------------------------
typedef vector<HANDLE> vechThread;

//--------------------------------------------------------------
//|
//|Recv Code
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
//|IServer 인터페이스
//|
//---------------------------------------------------------------
interface IServerMg
{
	virtual BOOL initServerMg(IMyObject* _obj)=0;
	virtual HANDLE GetCompletionPort()=0;
	virtual BOOL ServerStart(u_short nPort)=0;
	virtual void ServerStop()=0;
	virtual void Release()=0;
};

#endif //__ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__