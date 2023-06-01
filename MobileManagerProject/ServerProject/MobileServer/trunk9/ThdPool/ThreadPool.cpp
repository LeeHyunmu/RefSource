
#include "stdAfx.h"
#include "ThreadPool.h"

//--------------------------------------------------------------
//|
//|예외 처리 , 스래드 생성 함수 define
//|
//---------------------------------------------------------------
#ifdef ATLASSERT
// for ATL
#define THREADASSERT ATLASSERT
#elif defined ASSERT
// for MfC
#define THREADASSERT ASSERT
#endif	// defined ATLASSERT

#ifndef THREADASSERT
#define THREADASSERT
#endif

#ifndef _ATL_MIN_CRT
#define THREADPOOL_USE_CRT
#endif

//--------------------------------------------------------------
//|
//|CThreadPoolThreadCallback
//|
//---------------------------------------------------------------
// CThreadPoolThreadCallback
CThreadPoolThreadCallback::CThreadPoolThreadCallback(void)
: m_hEventShutdown(NULL)
{
}
//--------------------------------------------------------------
//|
//|종료 이벤트 감지 함수
//|
//---------------------------------------------------------------
BOOL CThreadPoolThreadCallback::CanContinue()
{
	if (NULL == m_hEventShutdown)
		return FALSE;
	return (WAIT_OBJECT_0 != WaitForSingleObject(m_hEventShutdown, 0));
}
//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CThreadPool::CThreadPool():
obj(NULL),
debug(NULL),
m_uThreadCount(0),
m_uThreadsCreated(0),
m_pThreads(NULL),
m_uThreadsActive(0),
m_hEventObjectAvailable(NULL),
m_dwStackSize(0),
m_pSecurityAttributes(NULL)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CThreadPool::~CThreadPool()
{
	Cleanup();
}
//--------------------------------------------------------------
//|
//|객체 초기화 함수
//|
//---------------------------------------------------------------
BOOL CThreadPool::initThreadPool(IMyObject* _obj)
{
	//=====================================================
	// 전 객체 생성 클래스 오브젝트
	//======================================================
	obj=_obj;
	//=====================================================
	// 디버그 출력 클래스
	//======================================================
	debug=obj->GetOutput();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|사용중인 객체를 메모리에서 제거한다
//|
//---------------------------------------------------------------
void CThreadPool::Release()
{
}
//--------------------------------------------------------------
//|
//|CThread inner 생성자
//|
//---------------------------------------------------------------
CThreadPool::CThread::CThread(CThreadPool& pool)
: m_Pool(pool), m_hThread(NULL), m_dwThreadID(0), m_dwThreadResult(0), m_bHasObject(FALSE)
{
}
//--------------------------------------------------------------
//|
//|CThread inner 소멸자
//|
//---------------------------------------------------------------
CThreadPool::CThread::~CThread(void)
{
	// wait for the thread to end
	if (NULL != m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
	m_hThread = NULL;
	m_dwThreadID = 0;
}
//--------------------------------------------------------------
//|
//|스래드 생성
//|
//---------------------------------------------------------------
HRESULT CThreadPool::CThread::Start(SIZE_T dwStackSize, LPSECURITY_ATTRIBUTES pSecurityAttributes)
{
	// start the thread 
#ifdef THREADPOOL_USE_CRT
	m_hThread = (HANDLE) _beginthreadex(pSecurityAttributes, (unsigned int)dwStackSize, 
		(unsigned (__stdcall*)(void*)) CThreadPool::CThread::ThreadProc, this, 0, (unsigned int*)&m_dwThreadID);
#else
	m_hThread = ::CreateThread(pSecurityAttributes, dwStackSize, CThreadPool::CThread::ThreadProc, 
		this, 0, &m_dwThreadID);
#endif
	HRESULT hr = HRESULT_FROM_WIN32(GetLastError());
	return (NULL == m_hThread) ? hr : S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
DWORD WINAPI CThreadPool::CThread::ThreadProc(LPVOID p)
{
	LPTHREAD pThis = (LPTHREAD)p;
	IThreadPoolThreadCallback& pool = (IThreadPoolThreadCallback&)(pThis->m_Pool);

	HANDLE phWaitHandles[] = {pThis->m_Pool.m_hEventShutdown, pThis->m_Pool.m_hEventObjectAvailable};

	BOOL bRun = TRUE;
	DWORD dw = 0;
	LPTHREADOBJECT pThreadObject = NULL;

	// the main thread loop
	while(bRun)
	{
		dw = WaitForMultipleObjects(2, phWaitHandles, FALSE, INFINITE);
		switch(dw)
		{
		case WAIT_OBJECT_0+1:	// object available
			pThis->m_bHasObject = pThis->m_Pool.GetNextObject(pThreadObject);
			if (pThis->m_bHasObject)
			{
				InterlockedIncrement((LONG*)&pThis->m_Pool.m_uThreadsActive);
				pThreadObject->Run(pool);
				pThreadObject->Done();
				pThis->m_bHasObject = FALSE;
				InterlockedDecrement((LONG*)&pThis->m_Pool.m_uThreadsActive);
			}
			// fallthrough
		case WAIT_TIMEOUT:	// timedout
			break;
		case WAIT_OBJECT_0:	// shutdown
		default:	// some error
			bRun = FALSE;
			break;
		}
	}
	pThis->m_dwThreadResult = dw;
#ifdef THREADPOOL_USE_CRT
	_endthreadex(dw);
#else
	ExitThread(dw);
#endif
	return dw;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//////////////////////////////////
// Node
CThreadPool::CThreadObjectList::Node::Node(LPTHREADOBJECT pThreadObject)
: m_pNext(NULL), m_pThreadObject(pThreadObject)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//////////////////////////////////
// CThreadObjectList
CThreadPool::CThreadObjectList::CThreadObjectList(void)
: m_pFirst(NULL), m_pLast(NULL)
{
	InitializeCriticalSection(&m_Crit);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CThreadPool::CThreadObjectList::~CThreadObjectList(void)
{
	EnterCriticalSection(&m_Crit);
	while(NULL != m_pFirst)
	{
		CThreadObjectList::Node* pNext = m_pFirst->m_pNext;
		m_pFirst->m_pThreadObject->Done();
		delete m_pFirst;
		m_pFirst = pNext;
	}
	m_pFirst = m_pLast = NULL;
	LeaveCriticalSection(&m_Crit);
	DeleteCriticalSection(&m_Crit);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
//////////////////////////////////
// CThreadObjectListAccessor
CThreadPool::CThreadObjectListAccessor::CThreadObjectListAccessor(CThreadObjectList& list)
: m_list(list)
{
	EnterCriticalSection(&m_list.m_Crit);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CThreadPool::CThreadObjectListAccessor::~CThreadObjectListAccessor(void)
{
	LeaveCriticalSection(&m_list.m_Crit);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CThreadPool::CThreadObjectListAccessor::RemoveAll()
{
	while(NULL != m_list.m_pFirst)
	{
		CThreadObjectList::Node* pNext = m_list.m_pFirst->m_pNext;
		m_list.m_pFirst->m_pThreadObject->Done();
		delete m_list.m_pFirst;
		m_list.m_pFirst = pNext;
	}
	m_list.m_pFirst = m_list.m_pLast = NULL;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
LPTHREADOBJECT CThreadPool::CThreadObjectListAccessor::Get()
{
	if (NULL == m_list.m_pFirst)
		return NULL;
	CThreadObjectList::Node* pNode = m_list.m_pFirst;
	LPTHREADOBJECT pThreadObject = pNode->m_pThreadObject;
	m_list.m_pFirst = pNode->m_pNext;
	if (NULL == m_list.m_pFirst)
		m_list.m_pLast = NULL;
	delete pNode;
	return pThreadObject;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HRESULT CThreadPool::CThreadObjectListAccessor::Add(LPTHREADOBJECT pThreadObject)
{
	if (NULL != m_list.m_pLast)
	{
		m_list.m_pLast->m_pNext = new CThreadObjectList::Node(pThreadObject);
		if (NULL == m_list.m_pLast->m_pNext)
			return E_OUTOFMEMORY;
		m_list.m_pLast = m_list.m_pLast->m_pNext;
	}
	else
	{
		m_list.m_pFirst = m_list.m_pLast = new CThreadObjectList::Node(pThreadObject);
		if (NULL == m_list.m_pFirst)
			return E_OUTOFMEMORY;
	}
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CThreadPool::CThreadObjectListAccessor::IsEmpty()
{
	return (NULL == m_list.m_pFirst);
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CThreadPool::Cleanup()
{
	// reset object-available-event
	if (NULL != m_hEventObjectAvailable)
	{
		ResetEvent(m_hEventObjectAvailable);
	}

	// set shutdown-event
	if (NULL != m_hEventShutdown)
	{
		SetEvent(m_hEventShutdown);
	}

	// remove all threads
	if (NULL != m_pThreads)
	{
		for (UINT n = 0; n < m_uThreadCount; n++)
		{
			if (NULL != m_pThreads[n])
			{
				delete m_pThreads[n];
				m_pThreads[n] = NULL;
			}
		}
		// delete thread pool
		delete [] m_pThreads;
		m_pThreads = NULL;
	}
	m_uThreadCount = 0;
	m_uThreadsCreated = 0;

	// empty wait-queue
	EmptyQueue();

	// close shutdown-event
	if (NULL != m_hEventShutdown)
	{
		CloseHandle(m_hEventShutdown);
		m_hEventShutdown = NULL;
	}

	// close object-available-event
	if (NULL != m_hEventObjectAvailable)
	{
		CloseHandle(m_hEventObjectAvailable);
		m_hEventObjectAvailable = NULL;
	}

	m_uThreadsActive = 0;
	m_dwStackSize = 0;
	m_pSecurityAttributes = NULL;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HRESULT CThreadPool::Init(UINT uInitalNumberOfThreads, UINT uMaxNumberOfThreads,
						  SIZE_T dwStackSize, LPSECURITY_ATTRIBUTES pSecurityAttributes)
{
	if (NULL != m_pThreads)
		return E_FAIL;

	if (uMaxNumberOfThreads < uInitalNumberOfThreads)
		uMaxNumberOfThreads = uInitalNumberOfThreads;
	if (uInitalNumberOfThreads < 1)
		uInitalNumberOfThreads = 1;	// create at least one thread

	// copy security attributes if any
	m_dwStackSize = dwStackSize;
	m_pSecurityAttributes = pSecurityAttributes;

	// accessor just to prevent early access to the queue from the created threads
	CThreadObjectListAccessor acc(m_ThreadObjectList);
	m_uThreadsActive = 0;

	// create shutdown-event (manual reset)
	m_hEventShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hEventShutdown)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// create object-available-event (manual reset)
	m_hEventObjectAvailable = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == m_hEventObjectAvailable)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// create thread pool
	m_uThreadCount = uMaxNumberOfThreads;
	m_pThreads = new LPTHREAD[m_uThreadCount];
	if (NULL == m_pThreads)
	{
		m_uThreadCount = 0;
		Cleanup();
		return E_OUTOFMEMORY;
	}
	memset(m_pThreads, 0, sizeof(LPTHREAD) * m_uThreadCount);

	// create and start threads
	HRESULT hr = S_OK;
	HRESULT hrRet = S_OK;
	for (UINT n = 0; n < uInitalNumberOfThreads; n++)
	{
		hr = AddThread();
		if (FAILED(hr))
		{
			hrRet = S_FALSE;
			break;
		}
	}
	return hrRet;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CThreadPool::GetNextObject(LPTHREADOBJECT& pThreadObject)
{
	CThreadObjectListAccessor acc(m_ThreadObjectList);
	if (acc.IsEmpty())
		return FALSE;
	pThreadObject = acc.Get();
	if (acc.IsEmpty())
		ResetEvent(m_hEventObjectAvailable);
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HRESULT CThreadPool::AddThread()
{
	// create thread
	m_pThreads[m_uThreadsCreated] = new CThread(*this);
	if (NULL == m_pThreads[m_uThreadsCreated])
	{
		return E_OUTOFMEMORY;
	}

	// start thread
	HRESULT hr = m_pThreads[m_uThreadsCreated]->Start(m_dwStackSize, m_pSecurityAttributes);
	if (FAILED(hr))
	{
		delete m_pThreads[m_uThreadsCreated];
		m_pThreads[m_uThreadsCreated] = NULL;
		return hr;
	}
	m_uThreadsCreated++;
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HRESULT CThreadPool::Add(LPTHREADOBJECT pThreadObject)
{
	THREADASSERT(NULL != pThreadObject);
	CThreadObjectListAccessor acc(m_ThreadObjectList);
	HRESULT hr = acc.Add(pThreadObject);
	if (FAILED(hr))
		return hr;
	if ((m_uThreadsActive == m_uThreadsCreated) && (m_uThreadsCreated < m_uThreadCount))
	{
		AddThread();
	}
	SetEvent(m_hEventObjectAvailable);
	return S_OK;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CThreadPool::Close()
{
	Cleanup();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
void CThreadPool::EmptyQueue()
{
	CThreadObjectListAccessor acc(m_ThreadObjectList);
	acc.RemoveAll();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
DWORD CThreadPool::GetThreadId(UINT n)
{
	if (n >= m_uThreadCount)
		return 0;
	if (NULL == m_pThreads[n])
		return 0;
	return m_pThreads[n]->GetThreadId();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CThreadPool::GetThreadStatus(UINT n)
{
	if (n >= m_uThreadCount)
		return FALSE;
	if (NULL == m_pThreads[n])
		return FALSE;
	return m_pThreads[n]->GetStatus();
}