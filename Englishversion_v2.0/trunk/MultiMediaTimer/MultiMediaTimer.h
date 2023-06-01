// CMultiMediaTimer 클래스 헤더 파일 
// 

#pragma once 

#include <mmsystem.h> 

template<class TWindow> 
class CMultiMediaTimer 
{ 
private: 
	UINT m_nTimerID; 
	UINT m_nInterval; 
	int m_nCount; 
	TWindow *m_pOwner; 

	TIMECAPS m_TimerCaps; 

public: 
	CMultiMediaTimer() 
	{ 
		m_nTimerID = NULL; 
		m_nCount = -1; 
		m_nInterval = 1000; 
		m_pOwner = NULL; 

		::timeGetDevCaps(&m_TimerCaps, sizeof(TIMECAPS)); 
	} 
	virtual ~CMultiMediaTimer() 
	{ 
		if(m_nTimerID) StopTimer(); 
	} 

	TWindow* GetTimerOwner() 
	{ 
		return m_pOwner; 
	} 
	BOOL SetTimerOwner(TWindow *pOwner) 
	{ 
		if(m_nTimerID) return FALSE; 

		m_pOwner = pOwner; 

		return TRUE; 
	} 
	UINT GetTimerID() 
	{ 
		return m_nTimerID; 
	} 
	BOOL SetInterval(UINT nInterval) 
	{ 
		if(m_nTimerID) return FALSE; 
		if(nInterval < m_TimerCaps.wPeriodMin) return FALSE; 
		if(nInterval > m_TimerCaps.wPeriodMax) return FALSE; 

		m_nInterval = nInterval; 

		return TRUE; 
	} 
	UINT GetInterval() 
	{ 
		return m_nInterval; 
	} 
	BOOL SetCount(UINT nInterval) 
	{ 
		m_nCount = nInterval; 
		return TRUE; 
	} 
	int GetCount() 
	{ 
		return m_nCount; 
	} 
	BOOL StartTimer(int nCount = -1) 
	{ 
		if(m_nTimerID) return FALSE; 
		if(nCount > 0) m_nCount = nCount; 

		m_nTimerID = ::timeSetEvent(m_nInterval, 0, MMTimerProc, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION); 

		return TRUE; 
	} 
	BOOL StopTimer() 
	{ 
		if(m_nTimerID == NULL) return FALSE; 

		::timeKillEvent(m_nTimerID); 

		m_nTimerID = NULL; 

		return TRUE; 
	} 
	BOOL IsRunning() 
	{ 
		return m_nTimerID != NULL; 
	} 

protected: 
	static void CALLBACK MMTimerProc(UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2) 
	{ 
		CMultiMediaTimer<TWindow> *pTimer = (CMultiMediaTimer<TWindow>*)dwUser; 
		TWindow *pOwner = pTimer->GetTimerOwner(); 

		if(pOwner) pOwner->OnMMTimer(uID); 

		if(pTimer->m_nCount > 0) 
		{ 
			pTimer->m_nCount--; 
			if(pTimer->m_nCount == 0) 
			{ 
				::timeKillEvent(pTimer->m_nTimerID); 

				pTimer->m_nTimerID = NULL; 
			} 
		} 
	} 
}; 
