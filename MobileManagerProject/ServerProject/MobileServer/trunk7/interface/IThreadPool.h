
#ifndef __ITHREADPOOL_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __ITHREADPOOL_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
#define INITNUMOFTHREAD 2
#define MAXNUMOFTHREAD  10
//--------------------------------------------------------------
//|
//|IThreadPool �������̽�
//|
//---------------------------------------------------------------
interface IThreadPoolThreadCallback
{
	virtual BOOL CanContinue()=0;
};
//--------------------------------------------------------------
//|
//|IThreadPool �������̽�
//|
//---------------------------------------------------------------
interface IThreadObjectBase
{
	virtual void Release()=0;
	//�ʱ�ȭ
	virtual BOOL initThreadObject(IMyObject* _obj)=0;
	virtual void Run(IThreadPoolThreadCallback &pool) = 0;
	virtual void Done() = 0;
	//DB ����Ÿ ���� �Լ�
	virtual HANDLE SetDbDataInfo(IServerMg* servermg, SOCKET _sock, CString _where){return NULL;};
};
typedef IThreadObjectBase*	LPTHREADOBJECT;
//--------------------------------------------------------------
//|
//|IThreadPool �������̽�
//|
//---------------------------------------------------------------
interface IThreadPool
{
	virtual void Release()=0;
	//�ʱ�ȭ
	virtual BOOL initThreadPool(IMyObject* _obj)=0;
	virtual HRESULT Init(UINT uInitalNumberOfThreads, UINT uMaxNumberOfThreads = 0,
		SIZE_T dwStackSize = 0, LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL)=0;
	virtual HRESULT Add(LPTHREADOBJECT pThreadObject)=0;
};

#endif //__ITHREADPOOL_74F736E25AB94b5dA789E4730F58F2A1_H__