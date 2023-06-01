/** @file WindowSubclassImpl.h
 *
 *	@brief  Definition of CWindowSubclassImpl
 *
 *	@author JaeWook Choi
 *	@version 0.90
 *
 *  @history
 *    0.90 (2005.06.24) - Initial release
 *
 *	<b><i>This software is provided "as is" without express or implied warranty. Use it at your own risk!</i></b>
 *
 */

#if !defined(__WINDOWSUBCLASSIMPL_H__INCLUDED__)
#define __WINDOWSUBCLASSIMPL_H__INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error WindowSubclassImpl.h requires atlbase.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error WindowSubclassImpl.h requires atlwin.h to be included first
#endif

namespace ATL
{

template<class T, class TBase = CWindow, class TWinTraits = CControlWinTraits>
class ATL_NO_VTABLE CWindowSubclassImpl : public CWindowImpl<T, TBase, TWinTraits>
{
  typedef CWindowSubclassImpl               thisClass;
  typedef CWindowImpl<T, TBase, TWinTraits> baseClass;
  // data member
private:

  HHOOK m_hHook;

  static thisClass * pThis;

public:
  
  // c'tor
  CWindowSubclassImpl() : m_hHook(NULL)
  {
  }

  // overrides
public:

  virtual void PreSubclassWindow(HWND hWnd)
  {
    ATLASSERT(::IsWindow(hWnd));

    // default : NOP
  }

  // overriddens
public:

	HWND Create(HWND hWndParent, RECT& rcPos, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			UINT nID = 0, LPVOID lpCreateParam = NULL)
	{
    // to set up CBT hook for subclassing (in order to be able to receive WM_NCCREATE and WM_CREATE messages)
    _HookCBT();
    
    TBase wndTemp;
    // to create a TBase window to subclass
    wndTemp.Create(hWndParent, rcPos, szWindowName, dwStyle, dwExStyle, nID, lpCreateParam);

    _UnhookCBT(); // to remove CBT hook (just in case)

    ATLASSERT(wndTemp.IsWindow());
    HWND hWnd = wndTemp.m_hWnd;

		wndTemp.Detach();

		return hWnd;
	}
  
  BOOL SubclassWindow(HWND hWnd)
  {
    T *pT = static_cast<T *>(this);
    pT;
    ATLASSERT(pT);

    pT->PreSubclassWindow(hWnd);

    return baseClass::SubclassWindow(hWnd);
  }

  // implementations
private:

  BOOL _HookCBT()
  {
    XCriticalSection::Enter();
    ATLASSERT(NULL == thisClass::pThis);
    thisClass::pThis = this;

    // to set up CBT hook
    m_hHook = ::SetWindowsHookEx(WH_CBT, &thisClass::CBTHookProc, NULL, ::GetCurrentThreadId());

    if(NULL == m_hHook)
    {
      ATLASSERT(FALSE);
      return FALSE;
    }
    
    return TRUE;
  }

  void _UnhookCBT()
  {
    if(NULL == m_hHook)
      return; // it has been already removed

    // to remove CBT hook
    ::UnhookWindowsHookEx(m_hHook);
    m_hHook = NULL;
  
    ATLASSERT(NULL != thisClass::pThis);
    thisClass::pThis = NULL;
    XCriticalSection::Leave();
  }

  inline HHOOK _GetHHOOK() const
  {
    return m_hHook;
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // CBTHookProc
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  static LRESULT CALLBACK CBTHookProc(int nCode, WPARAM wParam, LPARAM lParam)
  {
    ATLASSERT(NULL != thisClass::pThis);

		if(HCBT_CREATEWND == nCode)
    {
      HWND hwndCreated = (HWND)wParam;
      LPCBT_CREATEWND lpccw = (LPCBT_CREATEWND)lParam;

      if( thisClass::pThis && NULL == thisClass::pThis->m_hWnd && !::lstrcmpi( lpccw->lpcs->lpszClass, TBase::GetWndClassName() ) )
      {

#ifndef _DEBUG
        
        thisClass::pThis->SubclassWindow(hwndCreated);

#else // #ifndef _DEBUG

        BOOL bSubclass = thisClass::pThis->SubclassWindow(hwndCreated);
        bSubclass;
        ATLASSERT(bSubclass);

#endif  // #ifndef _DEBUG

        LRESULT lRes = ::CallNextHookEx(thisClass::pThis->_GetHHOOK(), nCode, wParam, lParam);

        // to remove CBT hook immediately
        thisClass::pThis->_UnhookCBT();
        ATLASSERT(NULL == thisClass::pThis);

        return lRes;
      }
		}
		
    return ::CallNextHookEx(thisClass::pThis->_GetHHOOK(), nCode, wParam, lParam);
  }
  
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // Singleton CriticalSection to guard thisClass::pThis
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  class XCriticalSection
  {
    // operations
  public:
    static void Enter() { ::EnterCriticalSection( &XCriticalSection::Instance().m_cs ); }
    static void Leave() { ::LeaveCriticalSection( &XCriticalSection::Instance().m_cs ); }
    
    // data member
  private:
    CRITICAL_SECTION m_cs;
    
    // c'tor
  private:
    XCriticalSection() { ::InitializeCriticalSection(&m_cs); }
    
    // d'tor
  public:
    ~XCriticalSection() { ::DeleteCriticalSection(&m_cs); }
    
    // singleton accessor
  private:
    static XCriticalSection & Instance()
    {
      static XCriticalSection instance;
      return instance;
    }
  };  // class XCriticalSection
  ////////////////////////////////////////////////////////////////////////////////////////////////////
 
};  // class ATL_NO_VTABLE CWindowSubclassImpl : public CWindowImpl<T, TBase, TWinTraits>

template<class T, class TBase, class TWinTraits>
CWindowSubclassImpl<T, TBase, TWinTraits> * CWindowSubclassImpl<T, TBase, TWinTraits>::pThis = NULL;

} // namespace ATL

#endif  // #define __WINDOWSUBCLASSIMPL_H__INCLUDED__
