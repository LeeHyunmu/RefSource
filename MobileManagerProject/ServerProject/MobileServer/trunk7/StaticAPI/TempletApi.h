
#ifndef __TEMPLETAPI_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __TEMPLETAPI_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template < typename T >
class CTempletApi
{
public:
	//积己磊
	CTempletApi(void);
	//家戈磊
	~CTempletApi(void);
	//--------------------------------------------------------------
	//|
	//|氦磐 昏力
	//|
	//---------------------------------------------------------------
	static BOOL DeleteVector(T& str ,int  index)
	{
		str.erase (str.begin()+index);
		return TRUE;
	}
};
#endif// __TEMPLETAPI_B7A15A9D01DC403996FC45F7F92F52D3_H__
