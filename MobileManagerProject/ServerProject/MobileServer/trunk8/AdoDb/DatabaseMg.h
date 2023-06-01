
#ifndef __DATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CDatabasebMg : public IDatabasebMg
{
public:
	//������
	CDatabasebMg();
	//�Ҹ���
	~CDatabasebMg();

//======================================================//
//			�Ϲ��Լ� ����
//======================================================//

//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IADODatabase* adodb;
//=======================================================//
//			���� ����
//======================================================//

//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	virtual BOOL initMobileDbMg(IMyObject* _obj);
	virtual void Trans(TCHAR* strQuery, TCHAR* format, ...);
	//DB ����
	virtual BOOL DbConnect(CString strConnection);
	//���ڵ� �¿� ���� ������ ��Ƽ� �ѱ��
	virtual IADORecordset* GetDbRecordset(UINT pagsize, TCHAR* strQuery, TCHAR* format, ...);
	//Ʈ������ ��� ����
	//����
	virtual BOOL BeginTn();
	//Query
	virtual BOOL ActionQuery(TCHAR* strQuery, TCHAR* format, ...);
	//commit
	virtual BOOL CommitTn();

//=======================================================//

};
#endif// __DATABASEMG_74F736E25AB94b5dA789E4730F58F2A1_H__
