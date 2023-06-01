
#include "stdAfx.h"
#include "IocpServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>
//--------------------------------------------------------------
//|
//|윈속 초기화를 1번만 하기 위한 변수
//|
//---------------------------------------------------------------
BOOL CIocpServer::bWinsockinitialize=FALSE;
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CIocpServer::CIocpServer():
obj(NULL),
debug(NULL)
{
	m_hSocket = NULL;
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CIocpServer::~CIocpServer()
{
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CIocpServer::initServer(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutput();
	//=====================================================
	// 윈속 초기화(1번만 할 수 있도록)
	//======================================================
	if(bWinsockinitialize==FALSE)
		bWinsockinitialize = WinSockInitialize();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CIocpServer::Release()
{
	//=====================================================
	// 서버 소켓을 닫는다
	//======================================================
	fnCloseSocket();
	//=====================================================
	// 저장하고 있는 모든 소켓 정보를 삭제하고 연결을 끊는다
	//======================================================
	for(int i=0; i < (int)SockInfo.size(); i++)
	{
		SOCKLIST* socklist=&SockInfo[i]->socklist;
		::shutdown(socklist->hSock, SD_BOTH);
		closesocket(socklist->hSock);
		delete SockInfo[i];SockInfo[i]=NULL;
	}
	SockInfo.clear();
}
//--------------------------------------------------------------
//|
//| 윈속 초기화
//|
//---------------------------------------------------------------
BOOL CIocpServer::WinSockInitialize()
{
	WSADATA m_wsaData;
	if(WSAStartup(MAKEWORD(2,2), &m_wsaData) != 0) 
	{
		debug->err_display("WSAStartup()");
		return FALSE;
	}
	//=====================================================
	// 로컬 IP를 구한다
	//======================================================
	char Host[500];
	if (gethostname(Host, sizeof(Host)) == SOCKET_ERROR)
		return WSAGetLastError();
	struct hostent* pk_Host = gethostbyname(Host);
	if (!pk_Host)
		return WSAGetLastError();
	BYTE* pu8_Addr = (BYTE*)((DWORD*)pk_Host->h_addr_list[0]);
	Local_IP.Format(_T("%d.%d.%d.%d"), pu8_Addr[0],pu8_Addr[1],pu8_Addr[2],pu8_Addr[3]);
	debug->Write("로컬IP:%s",Local_IP);
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Bind/Listen
//|
//---------------------------------------------------------------
BOOL CIocpServer::ServerListen(u_short _nport)
{
	//=====================================================
	// 서버 생성 포트
	//======================================================
	m_nPort=_nport;
	//=====================================================
	// OVERLAPPED 기능으르로 소켓을 생성한다(IOCP 기능을 사용하기 위한 설정)
	//======================================================
	m_hSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	//=====================================================
	// Bind
	//======================================================
	if(!fnBind())		return FALSE;
	//=====================================================
	// Listen
	//======================================================
	if(!fnListen())		return FALSE;
	return TRUE;
}
//--------------------------------------------------------------
//|
//| Bind()
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnBind()
{
	//=====================================================
	// IP, PORT를 서버 소켓에 bind
	//======================================================
	SOCKADDR_IN  sockAddr;  // Server Addr
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(m_nPort);
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(m_hSocket, (LPSOCKADDR)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
		debug->err_display("bind()");
		fnCloseSocket();
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------
//|
//| Listen()
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnListen()
{
	//=====================================================
	// 
	//======================================================
	if (listen(m_hSocket, SOMAXCONN) == SOCKET_ERROR) 
	{
		debug->err_display("listen()");
		fnCloseSocket();
		return FALSE;
	}

	return TRUE;
}
//--------------------------------------------------------------
//|
//| Accept()
//|
//---------------------------------------------------------------
SOCKET CIocpServer::fnAccept()
{
	CString     strIP="";
	u_short		port;
	SOCKADDR_IN sockAddr;
	SOCKET hClientSocket;
	int iAddrLen = sizeof(SOCKADDR_IN);
	hClientSocket = WSAAccept(m_hSocket, (LPSOCKADDR)&sockAddr, &iAddrLen, NULL, NULL);
	//hClientSocket = accept(m_hSocket, (LPSOCKADDR)&sockAddr, &iAddrLen);
	if(hClientSocket == INVALID_SOCKET) 
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			debug->err_display("accept()");
		return NULL;
	}
	strIP="";
	strIP = inet_ntoa(sockAddr.sin_addr);
	port = ntohs(sockAddr.sin_port);
	debug->Write("IP:[%s] PORT:[%d]접속", strIP, port);
	return hClientSocket;
}
//--------------------------------------------------------------
//|
//| Client Accept()
//|
//---------------------------------------------------------------
HANDLE CIocpServer::CompletionPortServerSocket(HANDLE hIocp)
{
	CString     strIP="";
	SOCKET hClientSocket;
	HANDLE  hResult;
	int iAddrLen = sizeof(SOCKADDR_IN);
	hClientSocket = fnAccept();
	if(hClientSocket == INVALID_SOCKET) 
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			debug->err_display("accept()");
		return NULL;
	}
	// 소켓과 입출력 완료 포트 연결
	hResult = CreateIoCompletionPort((HANDLE)hClientSocket, hIocp, (DWORD)hClientSocket , 0);
	if(hResult == NULL)
		return hResult;
	// 비동기 입출력 시작
	fnAddSocketInfo(hClientSocket);
	fnWSARecv(hClientSocket);
	return hResult;
}
//--------------------------------------------------------------
//|
//| 윈속 종료
//|
//---------------------------------------------------------------
void CIocpServer::Cleanup()
{
	WSACleanup();
}
//--------------------------------------------------------------
//|
//| 소켓 close
//|
//---------------------------------------------------------------
void CIocpServer::fnCloseSocket()
{
	if(m_hSocket)
	{
		::shutdown(m_hSocket, SD_BOTH);
		closesocket(m_hSocket);
		m_hSocket = NULL;
	}
}
//--------------------------------------------------------------
//|
//| WSARecv
//|
//---------------------------------------------------------------
/////////////////////////////////////////////////////////////
DWORD CIocpServer::fnWSARecv(SOCKET hSock, DWORD dwFlags)
{
	int iRetVal;
	if(!m_hSocket)
		return FALSE;
	SOCKETINFO* _info=NULL;
	BOOL b = fnGetSockInfo(&_info,hSock);
	if(!b)
		return FALSE;
	if(!_info->socklist.hSock)
		return FALSE;
	DWORD dwRead;
	ZeroMemory(&_info->wsaOv, sizeof(_info->wsaOv));
	ZeroMemory(_info->data, sizeof(_info->data));
	_info->wsaBuf[0].buf = (char *)_info->data;
	_info->wsaBuf[0].len = MAX_SOCKBUF;
	_info->wType[0] = IO_TYPE_RECV;
	// 오버랩드 받기 시도한다. 
	iRetVal = WSARecv(hSock, &_info->wsaBuf[0], 1, &dwRead, &dwFlags, (LPOVERLAPPED)_info, NULL);
	if(iRetVal == 0 )
	{
		//한번에 받기 성공
		debug->Write("한번에 받기 성공");
	}
	else if(iRetVal == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING )
	{
		// 오버 랩 받기 시작
		debug->Write("오버 랩 받기 시작");
		_info->overlappedcode=IO_TYPE_RECV;
	}
	else
	{
		// 실패, 소켓 닫기
		::MessageBox(NULL, " recv() failed !!! ", "Error", MB_ICONERROR);
		::shutdown(hSock, SD_BOTH);
		closesocket(hSock);
		fnRemoveSocketInfo(hSock);
		return -1;
	}
	return iRetVal;
}
//--------------------------------------------------------------
//|
//| WSASend
//|
//---------------------------------------------------------------
DWORD CIocpServer::fnWSASend(SOCKET hSock, const void *lpBuf, DWORD dwSend, DWORD dwFlags)
{
	int iRetVal;
	if(!m_hSocket)
		return FALSE;
	SOCKETINFO* _info;
	BOOL b = fnGetSockInfo(&_info,hSock);
	if(!b)
		return FALSE;
	if(!_info->socklist.hSock)
		return FALSE;
	ZeroMemory(&_info->wsaOv, sizeof(_info->wsaOv));
	_info->wsaBuf[1].buf = (char *)lpBuf;
	_info->wsaBuf[1].len = dwSend;
	_info->wType[1] = IO_TYPE_SEND;
	iRetVal = WSASend(hSock, &_info->wsaBuf[1], 1, &dwSend, 0, (LPOVERLAPPED)_info, NULL);
	if(iRetVal == 0 )
	{
		// 정상적으로 한번에 보내기 성공
		debug->Write("정상적으로 한번에 보내기 성공");
		//if(_info->sendinfo.hSendEvent)
		//{
		//	SetEvent(_info->sendinfo.hSendEvent);
		//	debug->Write("보내기 이벤트");
		//}
	}
	else if(iRetVal == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
	{
		// 정상적으로 오버랩드 보내기가 수행중.
		debug->Write("정상적으로 오버랩드 보내기가 수행중.");
		_info->overlappedcode=IO_TYPE_SEND;
	}
	else // else if(iRetVal == SOCKET_ERROR)
	{
		// 보내기 에러.
		::MessageBox(NULL, " Send failed !!! ", "Error", MB_ICONERROR);
		::shutdown(hSock, SD_BOTH);
		closesocket(hSock);
		fnRemoveSocketInfo(hSock);
		return -1;
	}
	return iRetVal;
}
//--------------------------------------------------------------
//|
//| WSASendAll
//|
//---------------------------------------------------------------
DWORD CIocpServer::fnWSASendAll(SOCKET hSock, const void* lpBuf, DWORD dwSend, DWORD dwFlags)
{
	int iRetVal;
	if(!m_hSocket)
		return -1;
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	for(int i=0; i < (int)SockInfo.size(); i++)
	{
		SOCKLIST* socklist=&SockInfo[i]->socklist;
		if(hSock!=socklist->hSock)
		{
			ZeroMemory(&SockInfo[i]->wsaOv, sizeof(SockInfo[i]->wsaOv));
			memcpy_s(SockInfo[i]->data,dwSend,lpBuf, dwSend);
			SockInfo[i]->wsaBuf[1].buf = (char *)SockInfo[i]->data;
			SockInfo[i]->wsaBuf[1].len = dwSend;
			SockInfo[i]->wType[1] = IO_TYPE_SEND;
			iRetVal = WSASend(socklist->hSock, &SockInfo[i]->wsaBuf[1], 1, &dwSend, 0, (LPOVERLAPPED)&SockInfo[i], NULL);
			if(iRetVal == 0 )
			{
				// 정상적으로 한번에 보내기 성공
				debug->Write("정상적으로 한번에 보내기 성공");
			}
			else if(iRetVal == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
			{
				// 정상적으로 오버랩드 보내기가 수행중.
				debug->Write("정상적으로 오버랩드 보내기가 수행중.");
				SockInfo[i]->overlappedcode=IO_TYPE_SEND;
			}
			else
			{
				// 보내기 에러.
				::MessageBox(NULL, " Send failed !!! ", "Error", MB_ICONERROR);
				::shutdown(socklist->hSock, SD_BOTH);
				closesocket(socklist->hSock);
				fnRemoveSocketInfo(socklist->hSock);
				return -1;
			}

		}
	}
	m_cs.Unlock();
	return iRetVal;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 추가
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnAddSocketInfo(SOCKET hSock, SOCKET hlinkSock)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	SOCKETINFO* info=NULL;
	info=new SOCKETINFO;
	ZeroMemory(info, sizeof(info));
	info->socklist.hSock=hSock;
	info->socklist.hlinkSock=hlinkSock;
	SockInfo.push_back(info);
	m_cs.Unlock();
	return TRUE;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 얻기(소켓으로)
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock)
{
	for(int i=0; i <(int) SockInfo.size(); i++)
	{
		if(hSock == SockInfo[i]->socklist.hSock)
		{
			*_info= SockInfo[i];
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| 소켓 정보 삭제
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnRemoveSocketInfo(SOCKET hSock)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	for(int i=0; i < (int)SockInfo.size(); i++)
	{
		SOCKLIST* socklist=&SockInfo[i]->socklist;
		if(hSock==socklist->hSock)
		{
			::shutdown(hSock, SD_BOTH);
			closesocket(hSock);
			delete SockInfo[i];SockInfo[i]=NULL;
			CTempletApi<vecSocketInfo>::DeleteVector(SockInfo, i);
			return TRUE;
		}
	}
	m_cs.Unlock();
	return FALSE;
}
//--------------------------------------------------------------
//|
//| Client 용 소캣 리턴(비동기 클라이언트)
//|
//---------------------------------------------------------------
SOCKET CIocpServer::CompletionPortClientSocket(HANDLE hIocp, SOCKET hSock, CString _ip, u_short _port)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	SOCKET hClientSock=0;
	SOCKADDR_IN cIntArrd;
	hClientSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if(hClientSock==INVALID_SOCKET)
	{
		debug->err_display("WSASocket()");
		return 0;
	}
	memset(&cIntArrd,0,sizeof(cIntArrd));
	cIntArrd.sin_family=AF_INET;
	cIntArrd.sin_addr.s_addr = inet_addr(_ip);
	cIntArrd.sin_port = htons(_port);
	//connect
	if(connect(hClientSock, (SOCKADDR*)&cIntArrd, sizeof(cIntArrd))==SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
			debug->err_display("connect()");
		return 0;
	}
	// 소켓과 입출력 완료 포트 연결
	HANDLE hResult = CreateIoCompletionPort((HANDLE)hClientSock, hIocp, (DWORD)hClientSock , 0);
	if(hResult == NULL)
		return 0;
	// 비동기 입출력 소켓 저장
	fnAddSocketInfo(hClientSock, hSock);
	return hClientSock;
	m_cs.Unlock();
	return TRUE;
}