#ifndef __IADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
enum cadoConnectModeEnum
{	
	connectModeUnknown = adModeUnknown,
	connectModeRead = adModeRead,
	connectModeWrite = adModeWrite,
	connectModeReadWrite = adModeReadWrite,
	connectModeShareDenyRead = adModeShareDenyRead,
	connectModeShareDenyWrite = adModeShareDenyWrite,
	connectModeShareExclusive = adModeShareExclusive,
	connectModeShareDenyNone = adModeShareDenyNone
};
//--------------------------------------------------------------
//|
//|IADODatabase �������̽�
//|
//---------------------------------------------------------------
interface IADODatabase
{
	virtual void Release()=0;
	//�����޴��� �ʱ�ȭ
	virtual BOOL initAdoDatabase(IMyObject* _obj)=0;
	//���ؼ��� �ξ��� ���� �ľ�
	virtual BOOL IsOpen()=0;
	//
	virtual _ConnectionPtr GetActiveConnection()=0; 
	//DB ����
	virtual BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""))=0;
	//Ʈ������
	//����
	virtual HRESULT BeginTransaction()=0; 
	//���� �Է�
	virtual BOOL Execute(LPCTSTR lpstrExec, long Option=adExecuteNoRecords)=0;
	// Ŀ��
	virtual HRESULT CommitTransaction()=0; 
	// �ѹ�
	virtual HRESULT RollbackTransaction()=0; 
};

#endif //__IADODATABASE_74F736E25AB94b5dA789E4730F58F2A1_H__