
#ifndef __ISCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|ISCardMsg 인터페이스
//|
//---------------------------------------------------------------
//--------------------------------------------------------------
//|Flag 값
//|---------------------------------------------------------------
//
#define ISCARDMSG_NEW 1
//
#define ISCARDMSG_DISCONNECT 2

interface ISCardMsg
{
	//======================================================//
	//SCard	Msg  받을 재정의 함수
	//======================================================//
	//상속 받은 클래스에서 스마트카드 장치의 변화를 알고 싶을 때
	//flag값에 따라서 처리 기능이 다르게 쓴다
	virtual void DeviceListChangeMsg(int flag, CString SCardName=_T(""))=0;
};

//--------------------------------------------------------------
//|
//|ISCardTm 인터페이스
//|
//---------------------------------------------------------------
interface ISCardTm
{
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//클래스에 있는 객체 메모리 해제
	virtual void Release()=0;
	//======================================================//
	//	 운영체제로부터 스마트카드 접속 메시지 처리함수
	//======================================================//
	//======================================================//
	//			디바이스 컨넥션(Smart Card 축출)
	//======================================================//
	virtual BOOL DeviceConnect(LPARAM lParam, BOOL bConnect)=0;
	//=======================================================//
	//======================================================//
	//		스마트카드 리스트 목록을 외부로 준다
	//======================================================//
	virtual BOOL GetSCardListinfo(CString VidPid, vecString& vecSCardName)=0;
	virtual BOOL GetSCardListinfo(vecString& vecVidPid, vecString& vecUsbid, vecString& vecSCardName)=0;
	//======================================================//
	//			스마트카드 접속 처리 함수
	//======================================================//
	//접속 정보 얻기(이름으로 해서 핸들과 프로토클을 얻음)
	virtual BOOL ScardConnect(SCARDHANDLE& hCard, DWORD& ActiveProtocol, CString SCardName, ISCardMsg* _connectobj)=0;
	//접속 정보 얻기(핸들을 반납함)
	virtual BOOL ScardDisConnect(SCARDHANDLE hCard)=0;
	//======================================================//
	//			안쓰는 함수
	//======================================================//
	//T0 통신용 인터페이스 함수
	virtual BOOL CommandT0(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//T1 통신용 인터페이스 함수
	virtual BOOL CommandT1(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//======================================================//
	//			통신용 함수
	//======================================================//
	//프로토콜에 따라서  통신이 가능한 인터페이스 함수
	virtual BOOL CommandAll(SCARDHANDLE scard, DWORD protocol, CString apdu, CString& recv, int nCount=0, BOOL brecv=0)=0;
	//======================================================//
	//			Last Error Code
	//======================================================//
	//제일 마지막 통신한 에러 검출코드를 넘긴다
	virtual CString GetLastErrorCode()=0;
	//======================================================//
	//			장치변화에 대한 통지 객체 등록
	//======================================================//
	virtual void SetSCardMsg(ISCardMsg* obj)=0;
};

#endif //__ISCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__