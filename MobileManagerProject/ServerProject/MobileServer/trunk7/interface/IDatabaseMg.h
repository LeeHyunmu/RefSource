#ifndef __IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__

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
//|IDatabasebMg �������̽�
//|
//---------------------------------------------------------------
interface IDatabasebMg
{
	virtual void Release()=0;
	//�����޴��� �ʱ�ȭ
	virtual BOOL initMobileDbMg(IMyObject* _obj)=0;
	//insert
	virtual void Trans(TCHAR* strQuery, TCHAR* format, ...)=0;
	//DB ����
	virtual BOOL DbConnect(CString strConnection)=0;
	//���ڵ� �¿� ���� ������ ��Ƽ� �ѱ��
	virtual IADORecordset* GetDbRecordset(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...)=0;
};

#endif //__IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__