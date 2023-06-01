
#ifndef __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|스래드 상태 설정 플래그
//|
//---------------------------------------------------------------
// Mode 플래그
#define DEVICEMODE_INITIATOR	0x00000001 //initiator Mode
#define DEVICEMODE_TARGET		0x00000002 //Target Mode
#define DEVICEMODE_NONE			0x00000004 //None

// Thread 플래그
#define THREADLOOP_IDLING		0x00000001 //idling
#define THREADLOOP_DETECTING	0x00000002 //detecting
#define THREADLOOP_NONE			0x00000004 //None
//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IDualiCardMg
{
	virtual ~IDualiCardMg()=0;
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	virtual void Release()=0;
	//카드 컨넥션
	virtual void CardConnent(CString nPort)=0;
	//
	virtual BOOL IdleReqa(BOOL bDebug=TRUE)=0;
	//폴링 감지
	//initiator
	virtual BOOL PollingDetectInitiator(BOOL bDebug=TRUE)=0;
	//Target
	virtual BOOL PollingDetectTarget(BOOL bDebug=TRUE)=0;
	//Release
	virtual BOOL PollingDetectRelease(BOOL bDebug=TRUE)=0;
	//마이페어 읽기
	virtual BOOL DeviceInfoRead(BOOL bDebug=TRUE)=0;
	//======================================================//
	//	apud 커멘트
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE)=0;
	//카드 찾기
	virtual BOOL FindCard(BOOL bDebug=TRUE)=0;
	//======================================================//
	//	SAM 카드
	//======================================================//
	//off
	virtual void SAMPwoff(BOOL bDebug=TRUE)=0;
	//on
	virtual void SAMPwon(int _nSlotno=0, BOOL bDebug=TRUE)=0;
	//샘 Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE)=0;
	//폴링 Send 함수
	virtual BOOL PollingSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE)=0;
	//파일 데이타 전송
	virtual BOOL  FileDataSend()=0;
	//체크 데이타(데이타 전송 함수)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	// card findding loop
	virtual BOOL  CardFinddingLoop()=0;
	// 접속된 카드가 아직 존재하는가 물음
	virtual BOOL  isCardid()=0;
};

#endif //__IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__