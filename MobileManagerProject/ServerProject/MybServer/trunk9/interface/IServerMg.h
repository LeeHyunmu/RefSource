
#ifndef __ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|������ ����ü ����
//|
//---------------------------------------------------------------


//--------------------------------------------------------------
//|
//|IServerMg �������̽�
//|
//---------------------------------------------------------------
interface IServerMg
{
	virtual void Release()=0;
	//�����޴��� �ʱ�ȭ
	virtual BOOL initServerMg(IMyObject* _obj)=0;
	//���ø��� ��Ʈ �ڵ��� ���� �´�
	virtual HANDLE GetCompletionPort()=0;
	virtual HANDLE GetClntCompletionPort()=0;
	//������ �����Ѵ�
	virtual BOOL ServerStart(u_short nPort)=0;
	//������ �����Ѵ�(���� �Ⱦ��� ����)
	virtual void ServerStop()=0;
	//�񵿱� ������ recv�� Ŭ���̾�Ʈ ���� ���� ���
	virtual SOCKET MakeCompetionPortClient(SOCKET hSock)=0;
	//�񵿱� ���� ��� �Լ�
	virtual SOCKET MakeCompetionPortServer()=0;
	//Iocp Server ���� ������ ���� ó�� �Լ�
	virtual void IocpServerSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)=0;
	//Iocp Server ���� ������ ó�� �Լ�
	virtual void IocpServerRecvHandling(SOCKETINFO* _info, DWORD flag)=0;
	//Iocp Client ���� ������ ���� ó�� �Լ�
	virtual void IocpClientSendHandling(SOCKETINFO* _info, DWORD dwTransfer,  DWORD flag)=0;
	//Iocp Client ���� ������ ó�� �Լ�
	virtual void IocpClientRecvHandling(SOCKETINFO* _info, DWORD flag)=0;
	//���� ����
	virtual BOOL GetEndServer()=0;
	//���� ������ ������ �� ó��	
	virtual BOOL DisConnectHandling(SOCKETINFO* _info, DWORD flag)=0;
	//IOCP ����� �ѱ��
	virtual IServer* GetIocpServer()=0;
	//������ ���̽� �޴����� �ѱ��
	virtual IDatabasebMg* GetDatabasebMg()=0;
	//Db ������ ���� �Լ�
	virtual BOOL DbDataSend()=0;
};

#endif //__ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__