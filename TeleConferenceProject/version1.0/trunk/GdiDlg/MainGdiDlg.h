
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//send flag 
#define  SENDFLAGE_FILERECVINFO				0x1  // 파일 정보를  주고 받을 때
#define  SENDFLAGE_GENDATA					0x2  // 일반 적인 데이타를 보낼 때데이타 보낼 때
#define  SENDFLAGE_MOUSEDATA				0x3  //  마우스에 대한 정보를 주고 받을 때
#define  SENDFLAGE_FILEDATASEND				0x4  //  이미지 파일을 전송 중일 때
#define  SENDFLAGE_FILEDATASENDENDOK		0x5  //  이미지 파일을 전송이 완료 되었고 정상적으로 이미지 로딩했다


#define  SENDFLAGE_IDLESERVER		0x11  // 서버가 노는 상태

#define  SENDFLAGE_FILERECVOK		0x10  // 파일이 이미 있다
#define  SENDFLAGE_FILERECVON		0x20  // 파일이 없다 전송 요청
#define  SENDFLAGE_FILERECVIPSEND	0x30  // ip 주소 요청
#define  SENDFLAGE_FILERECVIPRECV	0x40  // ip 주소 보냄

class CMainGdiDlg : public IMainGdiDlg
{
	//IP 표시
	CIPAddressCtrl m_ip_show;
	//IP 텍스트
	CSuperStatic ipshowText;
	//현재 마우스 포인트 위치
	CPoint Mousept;
	//화면 해상도
	CSize systemXY;
public:
	//생성자
	CMainGdiDlg();
	//소멸자
	~CMainGdiDlg();
	//=================스래드 함수===========================//

	//=======================================================//
	//			변수 선언
	//======================================================//
	HWND hWnd;
	ULONG_PTR m_gdiplusToken;
	//  The invisible DC to draw on.
	CMemDC* m_memDC;
	//이미지 객체
	CBitmap maingdibackgrandimage;
	//TCP 서버 받은 데이터 정리용 스트링값
	CString tcprecvdatastr;
	//======================================================//
	//			인터페이스 객체 선언
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IServer* selectserver;
	//=================Udp서버 Recv 함수===========================//
	static BOOL SelectUdpServerRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL UdpServerRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=================Tcp서버 Recv 함수===========================//
	static BOOL SelectTcpServerRecvFunc(LPVOID lpArg, SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL TcpServerRevcData( SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=================Tcp 클라이언트 Recv 함수===========================//
	static BOOL SelectTcpClientRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL TcpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=================Udp 클라이언트 Recv 함수===========================//
	static BOOL SelectUdpClientRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL UdpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=======================================================//
	//======================================================//
	//			서버 관련 객체
	//======================================================//
	//TCPP  서버 시작 버튼
	CSuperButton TcpServerBut;
	//TCP 서버 텍스트
	CSuperTextStatic TcpServerText;
	//UDP  서버 시작 버튼
	CSuperButton UdpServerBut;
	//UDP 서버 텍스트
	CSuperTextStatic UdpServerText;
	//TCP  클라이언트 시작 버튼
	CSuperButton TcpClientBut;
	//TCP 클라이언트 텍스트
	CSuperTextStatic TcpClientText;
	//UDP  클라이언트 시작 버튼
	CSuperButton UdpClientBut;
	//UDP 클라이언트 텍스트
	CSuperTextStatic UdpClientText;
	//UDP  브로드케스트 버튼
	CSuperButton UdpBroadcastBut;
	//UDP 브로드케스트 텍스트
	CSuperStatic UdpBroadcastText;
	//브로드케스트 못쓰게
	BOOL  bBroadcast;
	//======================================================//
	//			이미지 출력 객체
	//======================================================//
	//자신의 이미지
	CSuperImgMoveStatic myImage;
	//받은 이미지
	CSuperImgMoveStatic revcImage;
	//전환
	BOOL bMyShow;
	//======================================================//
	//			이미지 출력 객체
	//======================================================//
	//뷰어 전환 버튼
	CSuperButton viewchange;
	//뷰어 이미지 이동(처음으로)
	CSuperButton firstImageselect;
	//뷰어 이미지 이동(다음으로)
	CSuperButton nextImageselect;
	//뷰어 이미지 보내기(파일 전송, 우선 같은 이미지가 있는가 UDP로 검사)
	CSuperButton viewImagesend;
	//======================================================//
	//		전송받을	이미지 정보(Tcp 서버 데이터)
	//======================================================//
	//전송할 파일의 저장 bitmap 이름(CBitmap 이름);
	CString recvfilename;
	//전송 파일 포인터
	char* recvfilepointer;
	//전송된 파일 길이
	UINT  recvfilelen;
	//전송할 파일 전체 크기
	UINT  recvfiletotallen;
	//======================================================//
	//		전송할	이미지 정보(Tcp 클라이언트 데이터)
	//======================================================//
	//전송할 파일의 저장 bitmap 이름(CBitmap 이름);
	CString sendfilename;
	//전송 파일 포인터
	char* sendfilepointer;
	//전송된 파일 길이
	UINT  sendfilelen;
	//전송할 파일 전체 크기
	UINT  sendfiletotallen;
	//======================================================//
	//		Tcp , Udp 서버 , 클라이언트 구조체
	//======================================================//
	//Tcp 서버 버퍼 플래그
	UINT TcpServerBufFlag;
	//Udp 클라이언트 버퍼 플래그
	UINT TcpClientBufFlag;
	//Udp 서버 버퍼 플래그
	UINT UdpServerBufFlag;
	//Udp 클라이언트 버퍼 플래그
	UINT UdpClientBufFlag;
	//======================================================//
	//			일반 함수
	//======================================================//
	//그림을 다시 그리기 위해서 호출
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
	//최상위 윈도우 Capture
	BOOL MakeBitmapFocusWindowCapture(HWND hwnd, CBitmapHandle& ScreenImg);
	//풀 스크린 캡쳐
	BOOL MakeBitmapFullScreenCapture(HWND hwnd, CBitmapHandle& captureImg);
	//이미지 그리기 함수
	void DrawTextStatic(CDC * pDC,const CRect rc);
	//이미지 클리어
	void MainGidDlgImageClear();
	//비트맵 파일 만든다
	BOOL MainGdiDlgbitmap(CBitmap hbit);
	//윈도우 스타일 변경
	void ModifyStyle( HWND hwnd, UINT Add, UINT Remove, BOOL bRedraw );
	//전송할 이미지 파일 로딩
	BOOL SendImageFileLoading(CString file);
	//클라이언트를접속한다
	BOOL ClientConnect(CString _ip);
	//=======================================================//
	//======================================================//
	//			변수
	//======================================================//
	CListBox List;
	//폰트 설정
	LOGFONT fontinfo;
	//폰트 칼라
	COLORREF fontcolor;
	//======================================================//
	//			인터페이스 선언 재 정의 함수
	//======================================================//
	//초기 생성 
	virtual void initInterfaceObject(IMyObject* _obj);
	//클래스에 있는 객체 메모리 해제
	virtual void ControlDestroy();
	//Dlg 클래스 초기화
	virtual void initDlg(HWND _hWnd);
	//Dlg 그리기 함수
	virtual void MainGdiDlgDrawPaint();
	//버튼 클릭 메시지(버튼 클릭 전체)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//=======================================================//
	//스태틱 배경
	virtual BOOL CtlColorStatic(CDCHandle dc, CStatic wndStatic, HBRUSH& hbrush);
	//마우스 이벤트 받기
	virtual BOOL MouseEventMessage(UINT Msgtype, CPoint cuserpos);
	//Hot Key Message
	virtual BOOL HotKeyMessage(UINT KeyId);
	//cursor Message
	virtual BOOL MouseCursorMessage(CWindow wnd, UINT nHitTest, UINT message);
	//==================char type (hex, byte, string) 변환 함수===========================//
	//|char를 hex로 변환
	virtual CString Changcharbyhex(CString pass);
	//|카드로 부터 받은 데이타를 변환 한다(0x00~0xFF까지 이므로 스트링형으로 변환해서 판단한다)
	virtual BOOL ChangBytebyhex(char* pass, DWORD len,  CString& repass);
	//|스트링2바이트의값을 BYTE핵사 값으로 변환한다
	virtual int  ChangeHexCord(CString data, char[]);
	//==================서버 스타트 함수===========================//
	//서버를 시작한다
	virtual BOOL ServerStartCall();
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
