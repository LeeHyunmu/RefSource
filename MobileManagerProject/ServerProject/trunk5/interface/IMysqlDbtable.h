
#ifndef __IMYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IMYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------


//--------------------------------------------------------------
//|
//|IMysqlDbtable �������̽�
//|
//---------------------------------------------------------------
interface IMysqlDbtable
{
	virtual void Release()=0;
	//���ڵ� ������ ����
	virtual void SetRecordPtr(RecPtr _rec)=0;
	//�����޴��� �ʱ�ȭ
	virtual BOOL initMysqlDbtable(IMyObject* _obj)=0;
};

#endif //__IMYSQLDBTABLE_74F736E25AB94b5dA789E4730F58F2A1_H__