
#ifndef __SFAEDELETETEMPLET_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __SFAEDELETETEMPLET_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//안전 메모리 제거
#define  SAFEDELETE(x)        CSafeDeleteTemplet<void>::SafeDelete((void **) &(x))

template < typename T >
class CSafeDeleteTemplet
{
public:
	//생성자
	CSafeDeleteTemplet(void);
	//소멸자
	~CSafeDeleteTemplet(void);
	static BOOL SafeDelete(T **pDT)
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
};
#endif// __SFAEDELETETEMPLET_B7A15A9D01DC403996FC45F7F92F52D3_H__
