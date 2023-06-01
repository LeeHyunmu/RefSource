
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
//=======================================================//
//			변수 선언
//======================================================//
public:
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
	CString Anticol(BOOL bDebug=TRUE);
	//카드 선택
	BOOL CDualiCardMg::Select(CString id) ;
	//스트링2바이트의값을 BYTE핵사 값으로 변환한다(길이를 넘긴다)
	int  ChangeHexCord(CString data, BYTE hex[]);
	//장비 컨넥트
	void DetectObject();
	//캐릭터를 헥사로
	CString Changcharbyhex(CString pass);
	//헥사 스트링으로 변환
	BOOL ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass);
	//
	BOOL SendFile();
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

	//initiator Mode, Target Mode 설정
	UINT uLoopMode;

	//데이타 보내는 순간에는 감지를 정지한다
	//데이타를 모두 보내고 카드가 있는 지를 감지한다
	//스래드 Flage
	UINT uThreadLoop;
	//시연 모드
	UINT uShowMode;
	//
	LPBYTE l_pbuf;
	DWORD read_size;
	CString repass;
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void initInterfaceObject(IMyObject* _obj);
	virtual void Release();
	//카드 컨넥션
	virtual void CardConnent(CString nPort);
	//아이들 감지
	virtual BOOL IdleReqa(BOOL bDebug=TRUE);
	//폴링 감지
	//initiator
	virtual BOOL PollingDetectInitiator(BOOL bDebug=TRUE);
	//Target
	virtual BOOL PollingDetectTarget(BOOL bDebug=TRUE);
	//Release
	virtual BOOL PollingDetectRelease(BOOL bDebug=TRUE);
	//마이페어 읽기
	virtual BOOL DeviceInfoRead(BOOL bDebug=TRUE);
	//======================================================//
	//	apud 커멘트
	//======================================================//
	//ApduCommand
	virtual BOOL ApduCommand(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE);
	//카드 찾기
	virtual BOOL FindCard(BOOL bDebug=TRUE);
	//======================================================//
	//	SAM 카드
	//======================================================//
	//off
	virtual void SAMPwoff(BOOL bDebug=TRUE);
	//on
	virtual void SAMPwon(int _nSlotno=0, BOOL bDebug=TRUE);
	//샘 Apdu
	virtual BOOL SAMApduSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE);
	//폴링 Send 함수
	virtual BOOL PollingSend(CString apdu, CString& recv, BOOL bDebug=TRUE, int nCount=0, BOOL brecv=FALSE);
	//파일 데이타 전송
	virtual BOOL  FileDataSend();
	//체크 데이타(데이타 전송 함수)
	virtual BOOL  OneDataSend(CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	// card findding loop
	virtual BOOL  CardFinddingLoop();
	// 접속된 카드가 아직 존재하는가 물음
	virtual BOOL  isCardid();
//=======================================================//
};
#endif// __DUALICARDMG_74F736E25AB94b5dA789E4730F58F2A1_H__
