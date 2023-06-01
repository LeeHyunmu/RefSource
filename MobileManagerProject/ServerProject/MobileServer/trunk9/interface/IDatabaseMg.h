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
	//���ڵ� �� ����(Ư�� �� ���� ����)
	virtual BOOL RecordsetSetting(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...)=0;
	//���ڵ� �� ����(���̺� ��ü)
	virtual BOOL RecordsetSetting(TCHAR* tablename)=0;
	//���ڵ� �� �����͸� �ѱ��
	virtual IADORecordset* GetRecordsetPoint()=0;
	//Ʈ������ ��� ����
	//����
	virtual BOOL BeginTn()=0;
	//Query
	virtual BOOL ActionQuery(TCHAR* strQuery, TCHAR* format, ...)=0;
	//commit
	virtual BOOL CommitTn()=0;
};

#endif //__IDATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__