
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
typedef vector<HANDLE> vechThread;

//--------------------------------------------------------------
//|
//|Recv Code
//|
//---------------------------------------------------------------
//VM ��û ����
//ī�� ��� ����
#define RECVCODE_CARDENABLE            0x00000002
//���� ����
#define RECVCODE_USERCHANGE            0x00000004
//�ҵ� ���
#define RECVCODE_DEDUCTION             0x00000008
//��� ���� ��ȸ
#define RECVCODE_USECHECK              0x00000010

//����
//CODE�� ������ ����
#define RECVCODE_WHERECORDERROR	   	   0x01000000
//(� ������ ó���� �� �� �� ���� ��)DATAó�� �ڵ� ���� ����
#define RECVCODE_WHATDATAERROR     	   0x02000000
//DATA�� ������ ����
#define RECVCODE_DATAERROR      	   0x04000000
//����(���� ������ �� �и��ϴ�)
#define RECVCODE_BASEERROR  		   0x08000000

//��� ��ü
//VM => ��������
#define RECVCODE_VMCLIENTRECV		   0x10000000
//MYBI => ��������
#define RECVCODE_MYBISERVERRECV		   0x20000000
//�������� => VM
#define RECVCODE_VMCLIENTSEND	       0x40000000
//�������� => MYBI
#define RECVCODE_MYBISERVERSEND		   0x80000000
//--------------------------------------------------------------
//|
//|IServer �������̽�
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
};

#endif //__ISERVERMG_74F736E25AB94b5dA789E4730F58F2A1_H__