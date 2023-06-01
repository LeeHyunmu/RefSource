
#ifndef __THREADPOOL_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __THREADPOOL_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//======================================================//
//			전역 객체 선언
//======================================================//

//======================================================//
//			스드래 풀 종료을 위한 클래스
//======================================================//
class CThreadPoolThreadCallback : public IThreadPoolThreadCallback
{
protected:
	HANDLE		m_hEventShutdown;
	CThreadPoolThreadCallback(void);
public:
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual BOOL CanContinue();
};
//=======================================================//

class CThreadPool : public IThreadPool, CThreadPoolThreadCallback
{
public:
	//생성자
	CThreadPool();
	//소멸자
	~CThreadPool();

//======================================================//
//			일반함수 선언
//======================================================//
	inline UINT GetActiveThreadCount()
	{return m_uThreadsActive;}
	inline UINT GetThreadCount()
	{return m_uThreadsCreated;}
	inline UINT GetMaxThreadCount()
	{return m_uThreadCount;}
	DWORD GetThreadId(UINT n);
	BOOL GetThreadStatus(UINT n);

	void EmptyQueue();
	void Close();
//=================스래드 함수===========================//

//=======================================================//
//			객체 포인터 변수
//======================================================//
	IMyObject* obj;
	IOutput* debug;
//=================inner class===========================//
private:
//=======================================================//
//			thread class
//======================================================//
	class CThread
	{
	private:
		CThreadPool&	m_Pool;
		HANDLE				m_hThread;
		DWORD					m_dwThreadID;
		DWORD					m_dwThreadResult;
		BOOL					m_bHasObject;
	public:
		CThread(CThreadPool& pool);
		~CThread(void);
		HRESULT Start(SIZE_T dwStackSize, LPSECURITY_ATTRIBUTES pSecurityAttributes);
		inline HANDLE GetHandle()
		{return m_hThread;}
		inline DWORD GetThreadId()
		{return m_dwThreadID;}
		inline BOOL GetStatus()
		{return m_bHasObject;}
		static DWORD WINAPI ThreadProc(LPVOID p);
	};
	typedef CThread*	LPTHREAD;
	friend CThread;

	class CThreadObjectListAccessor;
//=======================================================//
//			the worker object queue
//======================================================//
	class CThreadObjectList
	{
	private:
		CRITICAL_SECTION		m_Crit;
		struct Node
		{
			Node*	m_pNext;
			LPTHREADOBJECT	m_pThreadObject;
			Node(LPTHREADOBJECT pThreadObject);
		};
		Node*	m_pFirst;
		Node*	m_pLast;
		friend CThreadObjectListAccessor;
	public:
		CThreadObjectList(void);
		~CThreadObjectList(void);
	};
//=======================================================//
//			accessor class to the queue
//======================================================//
	class CThreadObjectListAccessor
	{
	private:
		CThreadObjectList& m_list;
	public:
		CThreadObjectListAccessor(CThreadObjectList& list);
		~CThreadObjectListAccessor(void);
		void RemoveAll();
		LPTHREADOBJECT Get();
		HRESULT Add(LPTHREADOBJECT pThreadObject);
		BOOL IsEmpty();
	};
//=======================================================//
//=======================================================//
//		 멤버	변수 선언
//======================================================//
private:
	//////////////////////////////////
	// members for CThreadPool
	UINT				m_uThreadCount;
	UINT				m_uThreadsCreated;
	UINT				m_uThreadsActive;
	LPTHREAD*		m_pThreads;
	HANDLE			m_hEventObjectAvailable;
	SIZE_T								m_dwStackSize;
	LPSECURITY_ATTRIBUTES	m_pSecurityAttributes;
	CThreadObjectList	m_ThreadObjectList;

	void Cleanup();
	BOOL GetNextObject(LPTHREADOBJECT& pThreadObject);
	HRESULT AddThread();

public:
//======================================================//
//			인터페이스 선언 재 정의 함수
//======================================================//
	virtual void Release();
	virtual BOOL initThreadPool(IMyObject* _obj);
	virtual HRESULT Init(UINT uInitalNumberOfThreads, UINT uMaxNumberOfThreads = 0,
		SIZE_T dwStackSize = 0, LPSECURITY_ATTRIBUTES pSecurityAttributes = NULL);
	virtual HRESULT Add(LPTHREADOBJECT pThreadObject);

//=======================================================//

};
#endif// __THREADPOOL_74F736E25AB94b5dA789E4730F58F2A1_H__
