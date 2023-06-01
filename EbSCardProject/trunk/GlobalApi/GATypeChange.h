
#ifndef __GATYPECHANGE_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GATYPECHANGE_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GATYPECH  GATypeChange
class GATypeChange
{
public:
	//생성자
	GATypeChange(void);
	//소멸자
	~GATypeChange(void);
	//핵사를 바이트로 변환(포인터를 넘기고 할당된 메리는 쓰고 나서 해제한다)
	static int GAChangeHexCordInt(CString data, DWORD dwLen);
	static BOOL ChangStringbyhexString(CString pass, CString& repass);
};
#endif// __GATYPECHANGE_B7A15A9D01DC403996FC45F7F92F52D3_H__
