
#ifndef __IDUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IDualiP2pMg
{
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	virtual void Release()=0;
	//�����忡�� ȣ��Ǵ� �Լ�
	virtual void P2pFunc()=0;
	//������ ����
	virtual BOOL SendDatafn(DWORD _dwSendflage, CString data, DWORD dwTimeout = INFINITE)=0;
};

#endif //__IDUALIP2PMG_74F736E25AB94b5dA789E4730F58F2A1_H__