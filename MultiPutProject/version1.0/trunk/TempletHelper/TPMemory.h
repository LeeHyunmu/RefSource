
#ifndef __TPMEMORY_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __TPMEMORY_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//안전 메모리 제거
#define  SAFEDELETE(x)        CTPMeory<void>::SafeDelete((void **) &(x))

template < typename T >
class CTPMeory
{
public:
	//생성자
	CTPMeory(void){}
	//소멸자
	~CTPMeory(void){}
	static BOOL SafeDelete(T **pDT);
};
#endif// __TPMEMORY_B7A15A9D01DC403996FC45F7F92F52D3_H__
template < typename T >
inline
BOOL CTPMeory<T>::SafeDelete(T **pDT)
{
	if (*pDT == NULL)
		return FALSE;

	__try
	{
		delete *pDT;
	}
	__except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION)
	{
		*pDT = NULL;
		return FALSE;
	}
	*pDT = NULL;
	return TRUE;
}