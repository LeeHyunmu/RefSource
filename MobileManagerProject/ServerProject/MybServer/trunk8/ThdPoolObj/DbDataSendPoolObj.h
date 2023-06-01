
#ifndef __DBDATASENDPOOLOBJ_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __DBDATASENDPOOLOBJ_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			���� ��ü ����
//======================================================//

//=======================================================//

class CDbDataSendPoolObj : public  IThreadObjectBase
{
public:
	//������
	CDbDataSendPoolObj();
	//�Ҹ���
	~CDbDataSendPoolObj();

//======================================================//
//			�Ϲ��Լ� ����
//======================================================//

//=================������ �Լ�===========================//

//=======================================================//
//			��ü ������ ����
//======================================================//
	IMyObject* obj;
	IOutput* debug;
	IADORecordset* record;
	IServer* iocp;
//=================inner class===========================//
//=======================================================//
//			inner class
//======================================================//

//=======================================================//
//=======================================================//
//		 ���	���� ����
//======================================================//
public:
	static LONG s_lNext;
	static LONG s_lCount;
	ULONG	m_n;
	DWORD	thdid;
	HANDLE		hSendEvent;
	SOCKET		sock;
//======================================================//
//			�������̽� ���� �� ���� �Լ�
//======================================================//
	virtual void Release();
	//�ʱ�ȭ
	virtual BOOL initThreadObject(IMyObject* _obj);
	virtual void Run(IThreadPoolThreadCallback &pool);
	virtual void Done();
	//DB ����Ÿ ���� �Լ�
	virtual HANDLE SetDbDataInfo(IServerMg* servermg, SOCKET _sock, CString _where="");
//=======================================================//

};
#endif// __DBDATASENDPOOLOBJ_74F736E25AB94b5dA789E4730F58F2A1_H__
