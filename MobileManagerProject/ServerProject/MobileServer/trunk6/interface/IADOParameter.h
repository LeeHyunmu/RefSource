#ifndef __IADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
enum cadoParameterDirection
{
	paramUnknown = adParamUnknown,
	paramInput = adParamInput,
	paramOutput = adParamOutput,
	paramInputOutput = adParamInputOutput,
	paramReturnValue = adParamReturnValue 
};
//--------------------------------------------------------------
//|
//|IADOParameter �������̽�
//|
//---------------------------------------------------------------
interface IADOParameter
{
	virtual void Release()=0;
	//�����޴��� �ʱ�ȭ
	virtual BOOL initAdoParameter(IMyObject* _obj)=0;
	//
	virtual _ParameterPtr GetParameter()=0;
};

#endif //__IADOPARAMETER_74F736E25AB94b5dA789E4730F58F2A1_H__