
#include "stdAfx.h"
#include "IocpServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>

//--------------------------------------------------------------
//|
//|������
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
//|�Ҹ���
//|
//---------------------------------------------------------------
CIocpServer::~CIocpServer()
{
}
//--------------------------------------------------------------
//|
//|��ü �ʱ�ȭ �Լ�
//|
//---------------------------------------------------------------
BOOL CIocpServer::initServer(IMyObject* _obj)
{
	obj=_obj;
	debug=obj->GetOutput();

	return TRUE;
}
//--------------------------------------------------------------
//|
//|������� ��ü�� �޸𸮿��� �����Ѵ�
//|
//---------------------------------------------------------------
void CIocpServer::Release()
{
}
//--------------------------------------------------------------
//|
//| ���� �ʱ�ȭ �� Bind/Listen
//|
//---------------------------------------------------------------
BOOL CIocpServer::Initialize(u_short _nport)
{
	m_nPort=_nport;
	WSADATA m_wsaData;
	if(WSAStartup(MAKEWORD(2,2), &m_wsaData) != 0) 
	{
		debug->err_display("WSAStartup()");
		return FALSE;
	}

	m_hSocket = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	//m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(!fnBind())		return FALSE;
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
	//hClientSocket = WSAAccept(m_hSocket, (LPSOCKADDR)&sockAddr, &iAddrLen, NULL, NULL);
	hClientSocket = accept(m_hSocket, (LPSOCKADDR)&sockAddr, &iAddrLen);
	if(hClientSocket == INVALID_SOCKET) 
	{
		debug->err_display("WSAAccept()");
		return NULL;
	}
	strIP="";
	strIP = inet_ntoa(sockAddr.sin_addr);
	port = ntohs(sockAddr.sin_port);
	debug->Write("IP:[%s] PORT:[%d]����", strIP, port);
	return hClientSocket;
}
//--------------------------------------------------------------
//|
//| ���� ����
//|
//---------------------------------------------------------------
void CIocpServer::Cleanup()
{
	WSACleanup();
}
//--------------------------------------------------------------
//|
//| ���� close
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
	if(!_info->hSock)
		return FALSE;
	DWORD dwRead;
	ZeroMemory(&_info->wsaOv, sizeof(_info->wsaOv));
	ZeroMemory(&_info->data, sizeof(_info->data));
	_info->wsaBuf[0].buf = (char *)&_info->data;
	_info->wsaBuf[0].len = sizeof(DATA);
	_info->wType[0] = IO_TYPE_RECV;
	// �������� �ޱ� �õ��Ѵ�. 
	iRetVal = WSARecv(_info->hSock, &_info->wsaBuf[0], 1, &dwRead, &dwFlags, (LPOVERLAPPED)_info, NULL);
	if(iRetVal == 0 )
	{
		//�ѹ��� �ޱ� ����
		debug->Write("�ѹ��� �ޱ� ����");
	}
	else if(iRetVal == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING )
	{
		// ���� �� �ޱ� ����
		debug->Write("���� �� �ޱ� ����");
	}
	else
	{
		// ����, ���� �ݱ�
		::MessageBox(NULL, " recv() failed !!! ", "Error", MB_ICONERROR);
		::shutdown(_info->hSock, SD_BOTH);
		fnRemoveSocketInfo(_info->hSock);
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
	if(!_info->hSock)
		return FALSE;
	ZeroMemory(&_info->wsaOv, sizeof(_info->wsaOv));
	_info->wsaBuf[1].buf = (char *)lpBuf;
	_info->wsaBuf[1].len = sizeof(DATA);
	_info->wType[1] = IO_TYPE_SEND;
	iRetVal = WSASend(_info->hSock, &_info->wsaBuf[1], 1, &dwSend, 0, (LPOVERLAPPED)_info, NULL);
	if(iRetVal == 0 )
	{
		// ���������� �ѹ��� ������ ����
		debug->Write("���������� �ѹ��� ������ ����");
	}
	else if(iRetVal == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
	{
		// ���������� �������� �����Ⱑ ������.
		debug->Write("���������� �������� �����Ⱑ ������.");
	}
	else // else if(iRetVal == SOCKET_ERROR)
	{
		// ������ ����.
		::MessageBox(NULL, " Send failed !!! ", "Error", MB_ICONERROR);
		fnCloseSocket();
		fnRemoveSocketInfo(_info->hSock);
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
		if(hSock!=SockInfo[i]->hSock)
		{
			ZeroMemory(&SockInfo[i]->wsaOv, sizeof(SockInfo[i]->wsaOv));
			SockInfo[i]->wsaBuf[1].buf = (char *)lpBuf;
			SockInfo[i]->wsaBuf[1].len = sizeof(DATA);
			SockInfo[i]->wType[1] = IO_TYPE_SEND;
			iRetVal = WSASend(SockInfo[i]->hSock, &SockInfo[i]->wsaBuf[1], 1, &dwSend, 0, (LPOVERLAPPED)&SockInfo[i], NULL);
			if(iRetVal == 0 )
			{
				// ���������� �ѹ��� ������ ����
				debug->Write("���������� �ѹ��� ������ ����");
			}
			else if(iRetVal == SOCKET_ERROR && WSAGetLastError() == WSA_IO_PENDING)
			{
				// ���������� �������� �����Ⱑ ������.
				debug->Write("���������� �������� �����Ⱑ ������.");
			}
			else
			{
				// ������ ����.
				::MessageBox(NULL, " Send failed !!! ", "Error", MB_ICONERROR);
				fnCloseSocket();
				fnRemoveSocketInfo(SockInfo[i]->hSock);
			}

		}
	}
	m_cs.Unlock();
	return iRetVal;
}
//--------------------------------------------------------------
//|
//| ���� ���� �߰�
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnAddSocketInfo(SOCKET hSock)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	SOCKETINFO* info=NULL;
	info=new SOCKETINFO;
	ZeroMemory(&(info->wsaOv), sizeof(info->wsaOv));
	ZeroMemory(&info->szCardNum, sizeof(info->szCardNum));
	ZeroMemory(&info->data, sizeof(info->data));
	info->hSock=hSock;
	SockInfo.push_back(info);
	m_cs.Unlock();
	return TRUE;
}
//--------------------------------------------------------------
//|
//| ���� ���� ���(��������)
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock)
{
	for(int i=0; i <(int) SockInfo.size(); i++)
	{
		if(hSock == SockInfo[i]->hSock)
		{
			*_info= SockInfo[i];
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| ���� ���� ���(���ϰ� �ٸ��� ī���̸��� ���� ����)
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnGetSockInfo(SOCKETINFO** _info, SOCKET hSock, TCHAR *szCardNum)
{
	for(int i=0; i <(int) SockInfo.size(); i++)
	{
		if(_tccmp(szCardNum,SockInfo[i]->szCardNum)==0)
		{
			if(hSock != SockInfo[i]->hSock)
			{
				*_info= SockInfo[i];
				return TRUE;
			}
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//| ���� ���� ����
//|
//---------------------------------------------------------------
BOOL CIocpServer::fnRemoveSocketInfo(SOCKET hSock)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	for(int i=0; i < (int)SockInfo.size(); i++)
	{
		if(hSock == SockInfo[i]->hSock)
		{
			ZeroMemory(&(SockInfo[i]->wsaOv), sizeof(SockInfo[i]->wsaOv));
			ZeroMemory(&SockInfo[i]->szCardNum, sizeof(SockInfo[i]->szCardNum));
			ZeroMemory(&SockInfo[i]->data, sizeof(SockInfo[i]->data));
			::shutdown(SockInfo[i]->hSock, SD_BOTH);
			closesocket(SockInfo[i]->hSock);
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
//| Client �� ��Ĺ ����(�񵿱� Ŭ���̾�Ʈ)
//|
//---------------------------------------------------------------
BOOL CIocpServer::CompletionPortClientSocket(CString _ip, u_short _port, SOCKET& hClientSock)
{
	CStaticDataInitCriticalSectionLock  m_cs;
	m_cs.Lock();
	hClientSock=0;
	SOCKADDR_IN cIntArrd;
	hClientSock = WSASocket( AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED );
	if(hClientSock==INVALID_SOCKET)
	{
		debug->err_display("WSASocket()");
		return FALSE;
	}
	memset(&cIntArrd,0,sizeof(cIntArrd));
	cIntArrd.sin_family=AF_INET;
	cIntArrd.sin_addr.s_addr = inet_addr(_ip);
	cIntArrd.sin_port = htons(_port);
	//connect
	if(connect(hClientSock, (SOCKADDR*)&cIntArrd, sizeof(cIntArrd))==SOCKET_ERROR)
	{
		debug->err_display("connect()");
		return FALSE;
	}
	m_cs.Unlock();
	return TRUE;
}