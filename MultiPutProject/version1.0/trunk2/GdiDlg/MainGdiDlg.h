
#ifndef __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//IP ��û �÷���(UDP)
#define  SERVERFLAGE_IpRequest				0x1  // IP ��û �÷���
#define  SERVERFLAGE_IpResponse				0x2  // IP ���� �÷���

//�޽��� ��û ���� �÷���(TCP)
#define  SERVERFLAGE_KEYEVENTHEAD			0x81  //  Ű���� �̺�Ʈ �ش�
#define  SERVERFLAGE_SENSOREVENTHEAD		0x82  //  Sensor �̺�Ʈ �ش�

#define  SENSORFLAGE_ACCELEROMETOR			0x00  //  
#define  SENSORFLAGE_GYROSCOPE				0x01  //  
#define  SENSORFLAGE_MAGNETIC				0x02  //  
#define  SENSORFLAGE_ORIENTATION			0x03  //  
#define  SENSORFLAGE_TEMPERATURE			0x04  //  
#define  SENSORFLAGE_LIGHT					0x05  //  
#define  SENSORFLAGE_PROXIMITY				0x06  //  
#define  SENSORFLAGE_PRESSURE				0x07  // 

#define  SERVERFLAGE_IDLESERVER		0x11  // ������ ��� ����

class CMainGdiDlg : public IMainGdiDlg
{
	//���� ���콺 ����Ʈ ��ġ
	CPoint Mousept;
	//ȭ�� �ػ�
	CSize systemXY;
	//����
	CWindow sampleGame;
public:
	//������
	CMainGdiDlg();
	//�Ҹ���
	~CMainGdiDlg();
	//=================������ �Լ�===========================//

	//=======================================================//
	//			���� ����
	//======================================================//
	HWND hWnd;
	ULONG_PTR m_gdiplusToken;
	//  The invisible DC to draw on.
	CMemDC* m_memDC;
	//�̹��� ��ü
	CBitmap maingdibackgrandimage;
	//TCP ���� ���� ������ ������ ��Ʈ����
	CString tcprecvdatastr;
	//======================================================//
	//			�������̽� ��ü ����
	//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IServer* selectserver;
	//=================Udp���� Recv �Լ�===========================//
	static BOOL SelectUdpServerRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL UdpServerRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=================Tcp���� Recv �Լ�===========================//
	static BOOL SelectTcpServerRecvFunc(LPVOID lpArg, SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL TcpServerRevcData( SOCKET& sock, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=================Tcp Ŭ���̾�Ʈ Recv �Լ�===========================//
	static BOOL SelectTcpClientRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL TcpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=================Udp Ŭ���̾�Ʈ Recv �Լ�===========================//
	static BOOL SelectUdpClientRecvFunc(LPVOID lpArg, char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	BOOL UdpClientRevcData(char buf[], int& recvbytes, int& maxrecv,	int& sendbytes,	int& totalsend);
	//=======================================================//
	//======================================================//
	//			���� ���� ��ü
	//======================================================//
	//TCPP  ���� ���� ��ư
	CSuperButton TcpServerBut;
	//TCP ���� �ؽ�Ʈ
	CSuperTextStatic TcpServerText;
	//UDP  ���� ���� ��ư
	CSuperButton UdpServerBut;
	//UDP ���� �ؽ�Ʈ
	CSuperTextStatic UdpServerText;
	//======================================================//
	//			�̹��� ��� ��ü
	//======================================================//
	BOOL bMyShow;
	//======================================================//
	//		���۹���	�̹��� ����(Tcp ���� ������)
	//======================================================//
	//������ ������ ���� bitmap �̸�(CBitmap �̸�);
	CString recvfilename;
	//���� ���� ������
	char* recvfilepointer;
	//���۵� ���� ����
	UINT  recvfilelen;
	//������ ���� ��ü ũ��
	UINT  recvfiletotallen;
	//======================================================//
	//		������	�̹��� ����(Tcp Ŭ���̾�Ʈ ������)
	//======================================================//
	//������ ������ ���� bitmap �̸�(CBitmap �̸�);
	CString sendfilename;
	//���� ���� ������
	char* sendfilepointer;
	//���۵� ���� ����
	UINT  sendfilelen;
	//������ ���� ��ü ũ��
	UINT  sendfiletotallen;
	//======================================================//
	//		Tcp , Udp ���� , Ŭ���̾�Ʈ ����ü
	//======================================================//
	//Tcp ���� ���� �÷���
	UINT TcpServerBufFlag;
	//Udp Ŭ���̾�Ʈ ���� �÷���
	UINT TcpClientBufFlag;
	//Udp ���� ���� �÷���
	UINT UdpServerBufFlag;
	//Udp Ŭ���̾�Ʈ ���� �÷���
	UINT UdpClientBufFlag;
	//======================================================//
	//			�Ϲ� �Լ�
	//======================================================//
	//�׸��� �ٽ� �׸��� ���ؼ� ȣ��
	virtual void MainGdiinvalidateRect(CRect* Rc, BOOL bErase);
	//�ֻ��� ������ Capture
	BOOL MakeBitmapFocusWindowCapture(HWND hwnd, CBitmapHandle& ScreenImg);
	//Ǯ ��ũ�� ĸ��
	BOOL MakeBitmapFullScreenCapture(HWND hwnd, CBitmapHandle& captureImg);
	//�̹��� �׸��� �Լ�
	void DrawTextStatic(CDC * pDC,const CRect rc);
	//�̹��� Ŭ����
	void MainGidDlgImageClear();
	//��Ʈ�� ���� �����
	BOOL MainGdiDlgbitmap(CBitmap hbit);
	//������ ��Ÿ�� ����
	void ModifyStyle( HWND hwnd, UINT Add, UINT Remove, BOOL bRedraw );
	//������ �̹��� ���� �ε�
	BOOL SendImageFileLoading(CString file);
	//=======================================================//
	//======================================================//
	//			OpenGL ����
	//======================================================//
	//���� ��ü
	CSuperOpenGLStatic sensoreventopengl;
	//Ű���� ��ü
	CSuperOpenGLStatic keyeventopengl;
	//======================================================//
	//			����
	//======================================================//
	//��Ʈ ����
	LOGFONT fontinfo;
	//��Ʈ Į��
	COLORREF fontcolor;
	//======================================================//
	//			�������̽� ���� �� ���� �Լ�
	//======================================================//
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj);
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ControlDestroy();
	//Dlg Ŭ���� �ʱ�ȭ
	virtual void initDlg(HWND _hWnd);
	//Dlg �׸��� �Լ�
	virtual void MainGdiDlgDrawPaint();
	//��ư Ŭ�� �޽���(��ư Ŭ�� ��ü)
	virtual BOOL ButtonClickedMessage(UINT uNotifyCode, int nID);
	//=======================================================//
	//����ƽ ���
	virtual BOOL CtlColorStatic(CDCHandle dc, CStatic wndStatic, HBRUSH& hbrush);
	//���콺 �̺�Ʈ �ޱ�
	virtual BOOL MouseEventMessage(UINT Msgtype, CPoint cuserpos);
	//Hot Key Message
	virtual BOOL HotKeyMessage(UINT KeyId);
	//cursor Message
	virtual BOOL MouseCursorMessage(CWindow wnd, UINT nHitTest, UINT message);
	//Key Message
	virtual BOOL VirtualKeyMessage(UINT Msgtype, UINT nChar, UINT nRepCnt);
	//==================char type (hex, byte, string) ��ȯ �Լ�===========================//
	//|char�� hex�� ��ȯ
	virtual CString Changcharbyhex(CString pass);
	//|ī��� ���� ���� ����Ÿ�� ��ȯ �Ѵ�(0x00~0xFF���� �̹Ƿ� ��Ʈ�������� ��ȯ�ؼ� �Ǵ��Ѵ�)
	virtual BOOL ChangBytebyhex(char* pass, DWORD len,  CString& repass);
	//|��Ʈ��2����Ʈ�ǰ��� BYTE�ٻ� ������ ��ȯ�Ѵ�
	virtual int  ChangeHexCord(CString data, char[]);
	//==================���� ��ŸƮ �Լ�===========================//
	//������ �����Ѵ�
	virtual BOOL ServerStartCall();
	//������ �ڵ� ���ؿ��� �Լ�
	CWindow GetWindowHandle(CString WinCapName);
	//������ ����
	void SendMessageGame(UINT nChar);
};
#endif// __MAINGDIDLG_74F736E25AB94b5dA789E4730F58F2A1_H__
