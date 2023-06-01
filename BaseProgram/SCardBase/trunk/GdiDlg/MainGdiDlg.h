
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainGdiDlg : public IMainGdiDlg, public ISCardMsg
{
public:
	//생성자
	CMainGdiDlg();
	//소멸자
	~CMainGdiDlg();
	//=================스래드 함수===========================//

	//=======================================================//
	//			변수 선언
	//======================================================//
	//윈도우 핸들
	HWND hWnd;
	//VIDPID
	CComboBox comvpid;
	//Base APDU
	CComboBox combaseapdu;
	//스마트카드 이름
	CListBox  listscardname;
	//스마트카드 USB  ID
	CListBox  listUsbid;
	//컨넥션 카드
	CStatic    staticconnect;
	//APDU 값
	CEdit    editapdu;
	//스마트카드 접속 버튼
	CButton  butconnect;
	//스마트카드 접속을 끊는다
	CButton  butdisconnect;
	//APDU 전송
	CButton  butapud;
	//APDU RECV 데이터 뷰어
	CListBox  listapdurecv;
	//컨넥션 상태
	BOOL bSCardConnect;
	//스마트카드 핸들
	SCARDHANDLE hCard;
	//스마트카드 통신 프로토콜
	DWORD ActiveProtocol;
	//접속 스마트카드 이름
	CString ConnectSCardName;
	//======================================================//b
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	//스마트카드 통신용 객체
	ISCardTm* scardtm;
	//======================================================//
	//			인터페이스 포인터 
	//======================================================//

	//======================================================//
	//			일반 함수
	//======================================================//
	//스마트카드의 변화가 생겼을 때
	void NewSCardListMsg();
	//접속하고 있는 스마트카드가 제거 되었을 때
	void SCardDiConnectMsg(CString SCardName);
	//에러체크 출력 함수
	void ViewSmartCardConnectLastError();
	//APDU 및 정보 출력 
	void CMainGdiDlg::Write(TCHAR* format,...);
	//======================================================//
	//			기본 APDU
	//======================================================//
	//단축 이름 가져가기
	void LoadBaseApduName();
	//APDU값을 넘긴다
	BOOL FindBaseApdu();
	//=======================================================//
	//======================================================//
	//			변수
	//======================================================//
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	//스마트카드 정보 벡터
	vecString vecVidPid;
	vecString vecUsbid;
	vecString vecSCardName;
	//파일에서 읽은  APDU 세팅값 저장
	vecString vecDefalutAPDU;
	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void Release();
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd);
	//Dlg 그리기 함수
	virtual void DrawPaint();
	//콤보박스 IDC_VIDPID 메시지
	virtual void ComboboxVidPidMessage(UINT uNotifyCode, int nID);
	//콤보박스 IDC_APDUSHORT 메시지
	virtual void ComboboxApduShortMessage(UINT uNotifyCode, int nID);
	//리스트박스 IDC_PSCARDNMAE 메시지
	virtual void ListboxSCardNameMessage(UINT uNotifyCode, int nID);
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual void ButtonClickedMessage(UINT uNotifyCode, int nID);
	//Static Control CtlColor 변경
	virtual HBRUSH StaticCtlColorMessage(CDCHandle dc, CStatic wndStatic);
	//======================================================//
	//			스마트카드 변화 통지 함수
	//======================================================//
	//상속 받은 클래스에서 스마트카드 장치의 변화를 알고 싶을 때
	//flag값에 따라서 처리 기능이 다르게 쓴다
	virtual void DeviceListChangeMsg(int flag, CString SCardName=_T(""));
	//=======================================================//
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
