
#ifndef __IUDPWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IUDPWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|IOutput �������̽�
//|
//---------------------------------------------------------------
interface IUdpWinSocket
{
	//�ʱ� ���� 
	virtual void initInterfaceObject(IMyObject* _obj)=0;
	//Ŭ������ �ִ� ��ü �޸� ����
	virtual void Release()=0;
	//UDP ��� ������ ȣ��  �Լ�
	virtual void TeleUdpfunc()=0;
};

#endif //__IUDPWINSOCKET_74F736E25AB94b5dA789E4730F58F2A1_H__