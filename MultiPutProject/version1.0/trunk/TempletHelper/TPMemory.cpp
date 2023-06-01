
#include "stdafx.h"

// template < typename T >
// inline
// BOOL CTPMeory<T>::SafeDelete(T **pDT)
// {
// 	if (*pDT == NULL)
// 		return FALSE;
// 
// 	__try
// 	{
// 		delete *pDT;
// 	}
// 	__except(GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION)
// 	{
// 		*pDT = NULL;
// 		return FALSE;
// 	}
// 	*pDT = NULL;
// 	return TRUE;
// }
