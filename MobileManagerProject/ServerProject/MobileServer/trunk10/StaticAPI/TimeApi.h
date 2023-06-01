
#ifndef __TIMEAPI_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __TIMEAPI_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _TIMEC  CTimeApi
class CTimeApi
{
public:
	//持失切
	CTimeApi(void);
	//社瑚切
	~CTimeApi(void);
	static BOOL SetWait(HANDLE hevent, DWORD wait_t);
};
#endif// __TIMEAPI_H__
