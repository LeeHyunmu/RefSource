#pragma once

#include <Unknwn.h>
#include <crtdbg.h>
#include <comdef.h>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
#include <limits>


class SimpleIUnknown : public IUnknown
{
protected:
	SimpleIUnknown()
		: m_ref(0)
	{
	}
	virtual ~SimpleIUnknown()
	{
	}

private:
	long m_ref;

public:
	virtual HRESULT __stdcall QueryInterface(REFIID riid, void ** ppv)
	{
		if (riid == __uuidof(IUnknown)) {
			*ppv = static_cast<IUnknown*>(this);
		} else {
			*ppv = 0;
			return E_NOINTERFACE;
		}
		AddRef();
		return S_OK;
	}
	virtual ULONG __stdcall AddRef()
	{
		_ASSERTE( m_ref < std::numeric_limits<long>::max() );
		return ::InterlockedIncrement(&m_ref);
	}
	virtual ULONG __stdcall Release()
	{
		_ASSERTE( m_ref > 0 );
		if (InterlockedDecrement(&m_ref) <= 0) {
			delete this;
			return 0;
		}
		return m_ref;
	}
};	