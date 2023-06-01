
#ifndef __GTSFAEDELETE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GTSFAEDELETE_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//���� �޸� ����
#define  SAFEDELETE(x)        CGTSafeDelete<void>::SafeDelete((void **) &(x))

template < typename T >
class CGTSafeDelete
{
public:
	//������
	CGTSafeDelete(void);
	//�Ҹ���
	~CGTSafeDelete(void);
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
#endif// __GTSFAEDELETE_B7A15A9D01DC403996FC45F7F92F52D3_H__
