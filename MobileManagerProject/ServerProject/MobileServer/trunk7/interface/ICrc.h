
#ifndef __ICRC_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ICRC_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------
//|
//|ICrc �������̽�
//|
//---------------------------------------------------------------
interface ICrc
{
	//�ʱ� ����
	virtual BOOL initCrc(IMyObject* _obj)=0;
	//����
	virtual void Release()=0;
	//üũ �� ����
	virtual unsigned long GetResult(char *dataptr,int datasize)=0;
	//�� üũ�ϱ�
	virtual BOOL GetCheck(char *dataptr,int datasize)=0;
};

#endif //__ICRC_74F736E25AB94b5dA789E4730F58F2A1_H__