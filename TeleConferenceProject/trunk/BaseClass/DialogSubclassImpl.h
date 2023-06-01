/** @file DialogSubclassImpl.h
 *
 *	@brief  Definition of CDialogSubclassImpl
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

#if !defined(__DIALOGSUBCLASSIMPL_H__INCLUDED__)
#define __DIALOGSUBCLASSIMPL_H__INCLUDED__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __cplusplus
	#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLBASE_H__
	#error DialogSubclassImpl.h requires atlbase.h to be included first
#endif

#ifndef __ATLWIN_H__
	#error DialogSubclassImpl.h requires atlwin.h to be included first
#endif

namespace ATL
{

template<class T, class TBase = CWindow>
class ATL_NO_VTABLE CDialogSubclassImpl : public CDialogImpl<T, TBase>
{
  typedef CDialogSubclassImpl     thisClass;
  typedef CDialogImpl<T, TBase>   baseClass;

  // data member
private:

  HHOOK m_hHook;

  LPTSTR m_szDialogClassName;

  static thisClass * pThis;

protected:
  
  // c'tor
  CDialogSubclassImpl() : m_hHook(NULL), m_szDialogClassName(NULL)
  {
    _FindDialogClassName();
  }

  // d'tor
  virtual ~CDialogSubclassImpl()
  {
	  SAFEDELETE(m_szDialogClassName);
  }


  // overrides
public:

  virtual void PreSubclassWindow(HWND hWnd)
  {
    ATLASSERT(::IsWindow(hWnd) );

    // default : NOP
  }

  // overriddens
public:

  int DoModal(HWND hWndParent = ::GetActiveWindow(), LPARAM dwInitParam = NULL)
  {
    int bRet = 0;
    
    // to set up CBT hook for subclassing (in order to be able to receive WM_NCCREATE and WM_CREATE messages)
    _HookCBT();
    
    bRet = baseClass::DoModal(hWndParent, dwInitParam);
    
    _UnhookCBT(); // to remove CBT hook (just in case)

    return bRet;
  }

	HWND Create(HWND hWndParent, LPARAM dwInitParam = NULL)
  {
    HWND hWnd = NULL;

    // to set up CBT hook for subclassing (in order to be able to receive WM_NCCREATE and WM_CREATE messages)
    _HookCBT();
    
    hWnd = baseClass::Create(hWndParent, dwInitParam);
    
    _UnhookCBT(); // to remove CBT hook (just in case)
    
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
      //	LPCBT_CREATEWND lpccw = (LPCBT_CREATEWND)lParam;

      if(thisClass::pThis && NULL == thisClass::pThis->m_hWnd)
      {
        int nBuf = ::lstrlen(thisClass::pThis->m_szDialogClassName) + 1;
        LPTSTR szBuf = (LPTSTR)::_alloca( nBuf * sizeof(TCHAR) );
        ::GetClassName(hwndCreated, szBuf, nBuf);

        if( !lstrcmpi( szBuf, thisClass::pThis->m_szDialogClassName ) )
        {
          T * pT = static_cast<T *>(thisClass::pThis);
          pT;

          pT->PreSubclassWindow(hwndCreated);

          LRESULT lRes = ::CallNextHookEx(thisClass::pThis->_GetHHOOK(), nCode, wParam, lParam);

          // to remove CBT hook immediately
          thisClass::pThis->_UnhookCBT();
          ATLASSERT(NULL == thisClass::pThis);

          return lRes;
        }
      }
    }
		
    return ::CallNextHookEx(thisClass::pThis->_GetHHOOK(), nCode, wParam, lParam);
  }
  
  void _FindDialogClassName()
  {
    /////////////////////////////////////////////////////////////////////////////
    // Extended dialog templates (new in Win95)
#pragma pack(push, 1)
    
    typedef struct
    {
      WORD dlgVer;
      WORD signature;
      DWORD helpID;
      DWORD exStyle;
      DWORD style;
      WORD cDlgItems;
      short x;
      short y;
      short cx;
      short cy;
    } DLGTEMPLATEEX;
    
#pragma pack(pop)
    
    HRSRC hResource = ::FindResource(NULL, MAKEINTRESOURCE(T::IDD), RT_DIALOG);
    ATLASSERT(hResource);

    HGLOBAL hDialogTemplate = ::LoadResource(NULL, hResource);
    ATLASSERT(hDialogTemplate);

    LPCDLGTEMPLATE lpDialogTemplate = (LPCDLGTEMPLATE)::LockResource(hDialogTemplate);
    ATLASSERT(lpDialogTemplate);
    
    WORD * pw = NULL;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // to skip DLGTEMPLATE or DLGTEMPLATEEX
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if (((DLGTEMPLATEEX*)lpDialogTemplate)->signature == 0xFFFF)
    {
      // it's a DIALOGEX resource
      pw = (WORD *)( (DLGTEMPLATEEX *)lpDialogTemplate + 1 );
    }
    else
    {
      // it's a DIALOG resource
      pw = (WORD *)(lpDialogTemplate + 1);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // to skip menu name string or ordinal
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    if(*pw == (WORD)-1)
    {
      pw += 2; // WORDs
    }
    else
    {
      while(*pw++) ;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // ClassName
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    LPWSTR lpszClassName = NULL;
    if(*pw == (WORD)-1)
       lpszClassName = L"#32770";
    else
    {
      if(NULL != lpszClassName)
        lpszClassName = (LPWSTR)pw;
      else
        lpszClassName = L"#32770";
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // to save dialog class name
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    USES_CONVERSION;
	SAFEDELETE(m_szDialogClassName);
    m_szDialogClassName = new TCHAR[::lstrlen(W2CT(lpszClassName)) + 1];
    ::lstrcpy(m_szDialogClassName, W2CT(lpszClassName));
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
  
};  // class ATL_NO_VTABLE CDialogSubclassImpl : public CDialogImpl<T, TBase>

template<class T, class TBase>
CDialogSubclassImpl<T, TBase> * CDialogSubclassImpl<T, TBase>::pThis = NULL;

class CBase : public CMessageFilter
{
};
} // namespace ATL

#endif  // #define __DIALOGSUBCLASSIMPL_H__INCLUDED__
