#ifndef __IADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __IADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
enum cadoCommandType
{
	typeCmdText = adCmdText,
	typeCmdTable = adCmdTable,
	typeCmdTableDirect = adCmdTableDirect,
	typeCmdStoredProc = adCmdStoredProc,
	typeCmdUnknown = adCmdUnknown,
	typeCmdFile = adCmdFile
};
//--------------------------------------------------------------
//|
//|IADOCommand �������̽�
//|
//---------------------------------------------------------------
interface IADOCommand
{
	virtual void Release()=0;
	//���ڵ� ������ ����
	//�����޴��� �ʱ�ȭ
	virtual BOOL initAdoCommand(IMyObject* _obj)=0;
	//
	virtual CString GetText()=0;
	//
	virtual	_CommandPtr GetCommand()=0;
	//
	virtual int GetType()=0;
};

#endif //__IADOCOMMAND_74F736E25AB94b5dA789E4730F58F2A1_H__