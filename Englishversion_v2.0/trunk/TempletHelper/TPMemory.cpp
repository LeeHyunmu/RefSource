
#include "stdafx.h"

template < typename T >
BOOL CSafeDeleteTemplet<T>::SafeDelete(T **pDT)
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