
#ifndef __IWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IWinSocket
{
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release()=0;
	//Accept  �Լ�
	virtual void Acceptfunc()=0;
	//������ �����
	virtual BOOL MakeServer(u_short _port, CString _ip)=0;
	//������ ���ú� ���
	virtual void Recvfunc(SOCKET client_sock)=0;
};

#endif //__IWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__