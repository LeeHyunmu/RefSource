
#ifndef __SERVERBASE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __SERVERBASE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CServerBase : public IServer
{
public:
	//������
	CServerBase();
	//�Ҹ���
	virtual ~CServerBase();
//======================================================//
//			�Ϲ��Լ� ����
//======================================================//
	//���� ����
	void fnCloseSocket(SOCKET& Socket);
	//bind �Լ�
	BOOL fnBind(SOCKET Socket, u_short nPort);
	//Listen �Լ�
	BOOL fnListen();
	//accept �Լ�
	SOCKET fnAccept();
	//���� IP �˾Ƴ���
	BOOL ServerLocalip();
	//�ͺ��ŷ���� ���� ���� ��ȯ(TCP)
	BOOL ChangeTcpNonblocking(SOCKET Socket);
	//�ͺ��ŷ���� ���� ���� ��ȯ(UDP)
	BOOL ChangeUdpNonblocking();
	//Ŭ���̾�Ʈ ����
	//Ŭ���̾�Ʈ ����
	BOOL fnconnect(SOCKET sock, u_short _port, CString _ip);
	//����� ��� �Լ�
	void err_display(char* msg);
//======================================================//
//			���� �ʱ�ȭ �� ����
//======================================================//
	// ���� �ʱ�ȭ ī��Ʈ
	static BOOL bwsacount;
	//���� �ʱ�ȭ
	static void Startup();
	//���� ����
	static void Cleanup();
//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=======================================================//
//			���� ����
//======================================================//
private:
	//���� ���� ��ü(TCP)
	SOCKET      m_hTcpSocket;
	//���� ���� ��ü(UDP)
	SOCKET      m_hUdpSocket;
	//Ŭ���̾�Ʈ ���� ��ü(TCP)
	SOCKET      m_hTcpClientSocket;
	//Ŭ���̾�Ʈ ���� ��ü(UDP)
	SOCKET      m_hUdpClientSocket;
	//��ε��ɽ�Ʈ ���� ��ü(UDP)
	SOCKET		m_hUdpBroadcastSocket;
	//���� IP
	CString		Local_IP;
	//TCP ���� IP
	CString		TCPClient_IP;
	//UDP ���� IP
	CString		UDPClient_IP;
	//TCP ���� ���� ��Ʈ ��ȣ
	u_short     m_nTcpPort;
	//Udp ���� ���� ��Ʈ ��ȣ
	u_short     m_nUdpPort;
	//TCP Ŭ���̾�Ʈ ���� ��Ʈ ��ȣ
	u_short     m_nTcpClientPort;
	//Udp Ŭ���̾�Ʈ ���� ��Ʈ ��ȣ
	u_short     m_nUdpClientPort;
	//Udp ��ε��ɽ�Ʈ ��Ʈ ��ȣ
	u_short     m_nUdpBroadcastPort;

public:
	//TCP ���� 
	SOCKET GetTCPSocket();
	//UDP ����
	SOCKET GetUDPSocket();
	//TCP Ŭ���̾�Ʈ ���� 
	SOCKET GetTCPClientSocket();
	//UDP Ŭ���̾�Ʈ ����
	SOCKET GetUDPClientSocket();
	//���� IP �ּ�
	CString GetServerIp();
	//���� ��Ʈ ��ȣ(TCP)
	u_short GetTcpServerPort();
	//���� ��Ʈ ��ȣ(UDP)
	u_short GetUdpServerPort();
	//Ŭ���̾�Ʈ ���� ��Ʈ ��ȣ(TCP)
	u_short GetTcpClientPort();
	//Ŭ���̾�Ʈ ���� ��Ʈ ��ȣ(UDP)
	u_short GetUdpClientPort();
	//TCP Ŭ���̾�Ʈ IP �ּ�
	CString GetTcpClientIp();
	//UDP Ŭ���̾�Ʈ IP �ּ�
	CString GetUdpClientIp();
//======================================================//
//			���� �Լ�
//======================================================//
	//����
	virtual void ReleaseBase();
	//�ʱ�ȭ
	virtual BOOL initServerBase(IMyObject* _obj);
	//���� ����(TCP)
	virtual BOOL CreateServerBaseTcp(u_short _nport);
	//���� ����(TCP) ����
	virtual void CloseServerBaseTcp();
	//���� ����(UDP)
	virtual BOOL CreateServerBaseUdp(u_short _nport);
	//���� ����(UDP) ����
	virtual void CloseServerBaseUdp();
	//Ŭ���̾�Ʈ ����(TCP)
	virtual BOOL CreateServerBaseTcpClient(u_short _port, CString _ip);
	//Ŭ���̾�Ʈ ����(TCP) ����
	virtual void CloseServerBaseClientTcp();
	//Ŭ���̾�Ʈ ����(UDP)
	virtual BOOL CreateServerBaseUdpClient(u_short _port, CString _ip);
	//Ŭ���̾�Ʈ ����(UDP) ����
	virtual void CloseServerBaseClientUdp();
	//��ε��ɽ�Ʈ (UDP)
	virtual BOOL CreateServerBaseUdpbroadcast(SOCKET& sock, u_short _port);
	//��ε��ɽ�Ʈ (UDP) ����
	virtual void CloseServerBaseUdpbroadcast();
//=======================================================//

};
#endif// __SERVERBASE_74F736E25AB94b5dA789E4730F58F2A1_H__
