
#ifndef __ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//--------------------------------------------------------------
//|
//|IClient �������̽�
//|
//---------------------------------------------------------------
interface IClient
{
	//��ü �ʱ�ȭ
	virtual BOOL initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void ReleaseClient()=0;
	//���� ����(ip , port)
	virtual	BOOL ServerConnect(u_short _port, CString _ip)=0;
	//������ ����
	virtual BOOL SendDatafn(DWORD _dwSendSize, void* data, DWORD dwTimeout = INFINITE)=0;
	//������ �ޱ�
	virtual BOOL RecvDatafn(DWORD _dwRecvSize, void* data, DWORD dwTimeout = INFINITE)=0;
	//Ŭ���̾�Ʈ�� ���ӻ����ΰ�?
	virtual BOOL ActiveClient()=0;
};

#endif //__ICLIENT_74F736E25AB94b5dA789E4730F58F2A1_H__