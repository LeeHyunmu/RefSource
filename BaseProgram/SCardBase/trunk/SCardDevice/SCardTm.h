
#ifndef __SCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSCardTm : public ISCardTm
{
	struct SCARDINFOSTRUCT 
	{
		ISCardMsg* object;
		CString VP_id;
		CString USB_id;
	};
	//스트링을 탐색하기 위한 선언
	typedef vector<ISCardMsg*> SCARDOBJECTMSG;
	//스트링을 탐색하기 위한 선언
	typedef CStringElementTraits<CString> ATLELEMENTSCARD;
	//스마트카드 정보
	typedef CAtlMap<CString, SCARDINFOSTRUCT, ATLELEMENTSCARD> ATLMAPSCARD;
	//검색에서 쓰일 객체
	typedef CAtlMap<CString, SCARDINFOSTRUCT, ATLELEMENTSCARD>::CPair ATLPAIRSCARD;
	//스마트카드판독기 contex
	SCARDCONTEXT	 hContextHandle;
	//VID_PID로 구분하여 저장
	ATLMAPSCARD SCardList;
	//장치 변화에 따른 메시지 받기 위한 객체 저장
	SCARDOBJECTMSG ObjectMsg;

	//Last Recv Code
	CString ErrorCode;
public:
	//생성자
	CSCardTm();
	//복사 생성자
	CSCardTm(CSCardTm &v);
	//소멸자
	~CSCardTm();
	//=================스래드 함수===========================//

	//=======================================================//
	//			변수 선언
	//======================================================//

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
	//======================================================//
	//			데이터 변환 기본 함수
	//======================================================//
	//함수 스트링 문자를 HEX 값으로 변환한다
	int ChangeHexCord(CString data, BYTE hex[]);
	//char를 hex로
	BOOL Changcharbyhex(CString pass, CString& repass);
	//BYTE를 hex로
	BOOL ChangBytebyhex(BYTE* pass, DWORD len,  CString& repass);
	//
	void ChangStringtoChar(CString pass,  char repass[]);
	//======================================================//
	//			스마트카드 데이타 오류분석
	//======================================================//
	BOOL Analysis(SCARDHANDLE scard, DWORD protocol,CString cord, CString& recv, int nCount, BOOL brecv);
	//======================================================//
	//			스마트카드 초기화 컨텍스트 핸들을 얻는다
	//======================================================//
	bool SCardinit();
	//======================================================//
	//			컨텍스트 핸들을 닫는다
	//======================================================//
	void ReleaseinitHandle(void);
	//======================================================//
	//			스마트카드 리더 리스트 읽기 함수
	//======================================================//
	//이미 접속된 스마트장치를 로드하기 위해서 PidVid값을 _T("")로 한다
	size_t ConnectSCardList(CString PidVid, CString Usbid);
	//======================================================//
	//		실질적인 통신 함수(BYTE형으로 데이터 통신)
	//======================================================//
	//T0 프로토콜 통신
	int  CommandT0(SCARDHANDLE scard,LPBYTE sendbuffer, unsigned long dwtransLen, BYTE recvbuffer[],int len);
	//T1프로토콜 통신
	int  CommandT1(SCARDHANDLE scard,LPBYTE sendbuffer, unsigned long dwtransLen, BYTE recvbuffer[],int len);

	//======================================================//
	//	윈도우로부터 스마트카드 접속 메시지를 받았을 때
	//======================================================//
	BOOL AddSCardList(CString PidVid, CString Usbid);
	BOOL RemoveSCardList(CString PidVid, CString Usbid);
	//======================================================//
	//	프로그램 시작 전에 접속된 스마트카드
	//======================================================//
	//스마트카드 네임으로 Vid Pid 찾기
	CString GetDevicePidVid(CString DeviceName);

	//======================================================//
	//	Usbid로 삭제하지 못했을 때
	//======================================================//
	//삭제하기위해서 접속된 리스트목록을 만든다
	size_t GetSCardList(vecString& vecSCard);
	//Usbid로 찾지 못했을 경우
	BOOL RemoveSCardList();
	//======================================================//
	//	등록된 클래스로 변화에 대한 통지를 보낸다
	//======================================================//
	void SendSCardChangMsg();
	void SendSCardDisconnectMsg(POSITION pos);
	//=======================================================//
	//======================================================//
	//			변수
	//======================================================//

	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void Release();
	//======================================================//
	//	 운영체제로부터 스마트카드 접속 메시지 처리함수
	//======================================================//
	//======================================================//
	//			디바이스 컨넥션(Smart Card 축출)
	//======================================================//
	virtual BOOL DeviceConnect(LPARAM lParam, BOOL bConnect);
	//=======================================================//
	//======================================================//
	//		스마트카드 리스트 목록을 외부로 준다
	//======================================================//
	virtual BOOL GetSCardListinfo(CString VidPid, vecString& vecSCardName);
	virtual BOOL GetSCardListinfo(vecString& vecVidPid, vecString& vecUsbid, vecString& vecSCardName);
	//======================================================//
	//			스마트카드 접속 처리 함수
	//======================================================//
	//접속 정보 얻기(이름으로 해서 핸들과 프로토클을 얻음)
	virtual BOOL ScardConnect(SCARDHANDLE& hCard, DWORD& ActiveProtocol, CString SCardName, ISCardMsg* _connectobj);
	//접속 정보 얻기(핸들을 반납함)
	virtual BOOL ScardDisConnect(SCARDHANDLE hCard);
	//======================================================//
	//			안쓰는 함수
	//======================================================//
	//T0 통신용 인터페이스 함수
	virtual BOOL CommandT0(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//T1 통신용 인터페이스 함수
	virtual BOOL CommandT1(SCARDHANDLE scard,CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//======================================================//
	//			통신용 함수
	//======================================================//
	//프로토콜에 따라서  통신이 가능한 인터페이스 함수
	virtual BOOL CommandAll(SCARDHANDLE scard, DWORD protocol, CString apdu, CString& recv, int nCount=0, BOOL brecv=0);
	//======================================================//
	//			Last Error Code
	//======================================================//
	//제일 마지막 통신한 에러 검출코드를 넘긴다
	virtual CString GetLastErrorCode();
	//======================================================//
	//			장치변화에 대한 통지 객체 등록
	//======================================================//
	virtual void SetSCardMsg(ISCardMsg* obj);
};
#endif// __SCARDTM_74F736E25AB94b5dA789E4730F58F2A1_H__
