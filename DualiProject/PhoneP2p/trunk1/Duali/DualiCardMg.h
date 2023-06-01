
#ifndef __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDualiCardMg : public IDualiCardMg
{
public:
	//생성자
	CDualiCardMg();
	//소멸자
	~CDualiCardMg();
//=================스래드 함수===========================//
	static DWORD WINAPI DetectProcess(LPVOID lpArg);
//=======================================================//
//			변수 선언
//======================================================//
public:
	CThread  Detectthread;
	CEvent  DetectEvent;
//======================================================//
//			인터페이스 객체 선언
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//======================================================//
//			인터페이스 포인터 
//======================================================//

//======================================================//
//			일반 함수
//======================================================//
	//display 문자 구성 함수
	CString ShowString(int len,BYTE* data,BYTE flag);
	//display 문자 구성 함수
	CString ShowString(CString data,BYTE flag);
	//카드 충돌 방지와 카드 UI를 가져온다
	CString Anticol();
	//카드 선택
	void Select(BYTE* Recv, int len=4);
	//스트링2바이트의값을 BYTE핵사 값으로 변환한다(길이를 넘긴다)
	int  ChangeHexCord(CString data, BYTE hex[]);
	//장비 컨넥트
	void DetectObject();
	//캐릭터를 헥사로
	CString Changcharbyhex(CString pass);
	//헥사 스트링으로 변환
	BOOL ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass);
//=======================================================//
//======================================================//
//			변수
//======================================================//
	BOOL m_bConnect;
	int  m_nPort;
	BYTE  nSlotno;
	//카드ID값
	CString cardID;
	BOOL bSAMOn;
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void initInterfaceObject(IMyObject* _obj);
	virtual void Release();
	//카드 컨넥션
	virtual void CardConnent(CString nPort);
	//아이들 감지
	virtual BOOL IdleReqa();
	//폴링 감지
	virtual BOOL PollingDetect();
	//마이페어 읽기
	virtual BOOL DeviceInfoRead();
	//======================================================//
	//	apud 커멘트
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//카드 찾기
	virtual BOOL FindCard();
	//======================================================//
	//	SAM 카드
	//======================================================//
	//off
	virtual void SAMPwoff();
	//on
	virtual void SAMPwon(int _nSlotno=0);
	//샘 Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//카드 감지 및 처리 함수
	virtual void CardDetectFunc();
	//폴링 Send 함수
	virtual BOOL PollingSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//파일 데이타 전송
	virtual BOOL  FileDataSend(CString FileName);
	//체크 데이타(데이타 전송 함수)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
//=======================================================//
};
#endif// __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
