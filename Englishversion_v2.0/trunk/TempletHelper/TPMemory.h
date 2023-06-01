
#ifndef __TPMEMORY_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __TPMEMORY_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//���� �޸� ����
#define  SAFEDELETE(x)        CSafeDeleteTemplet<void>::SafeDelete((void **) &(x))

template < typename T >
class CSafeDeleteTemplet
{
public:
	//������
	CSafeDeleteTemplet(void);
	//�Ҹ���
	~CSafeDeleteTemplet(void);
	static BOOL CSafeDeleteTemplet<T>::SafeDelete(T **pDT)
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
#endif// __TPMEMORY_B7A15A9D01DC403996FC45F7F92F52D3_H__
