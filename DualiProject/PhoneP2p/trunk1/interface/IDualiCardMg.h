
#ifndef __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput 인터페이스
//|
//---------------------------------------------------------------
interface IDualiCardMg
{
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	virtual void Release()=0;
	//카드 컨넥션
	virtual void CardConnent(CString nPort)=0;
	//
	virtual BOOL IdleReqa()=0;
	//폴링 감지
	virtual BOOL PollingDetect()=0;
	//마이페어 읽기
	virtual BOOL DeviceInfoRead()=0;
	//======================================================//
	//	apud 커멘트
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//카드 찾기
	virtual BOOL FindCard()=0;
	//======================================================//
	//	SAM 카드
	//======================================================//
	//off
	virtual void SAMPwoff()=0;
	//on
	virtual void SAMPwon(int _nSlotno=0)=0;
	//샘 Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//카드 감지 및 처리 함수
	virtual void CardDetectFunc()=0;
	//폴링 Send 함수
	virtual BOOL PollingSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//파일 데이타 전송
	virtual BOOL  FileDataSend(CString FileName)=0;
	//체크 데이타(데이타 전송 함수)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
};

#endif //__IDUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__