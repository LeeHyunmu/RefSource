
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|버튼 멤버 함수
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|생성자
//|
//---------------------------------------------------------------
CAtlListAqCmd::CAtlListAqCmd(void)
{
}
//--------------------------------------------------------------
//|
//|소멸자
//|
//---------------------------------------------------------------
CAtlListAqCmd::~CAtlListAqCmd(void)
{
	DeleteDataAll();
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlListAqCmd::AddData(int storagekey, int _cmd, CString _cmdName)
{
	BOOL b=FALSE;
	mItem add_teim;
	add_teim.cmd=_cmd&0xFF;
	add_teim.cmdName=_cmdName;
	storage[storagekey].AddHead(add_teim);
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlListAqCmd::DeleteData(int storagekey, CString _cmdName)
{
	BOOL b=FALSE;
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlListAqCmd::DeleteDataAll()
{
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlListAqCmd::GetCmd(int storagekey, CString _cmdName, byte& _cmd)
{
	BOOL b=FALSE;
	mItem search_item;
	search_item.cmd=_cmd;
	search_item.cmdName=_cmdName;
	POSITION pos =  storage[storagekey].Find(search_item);
	if(pos!=NULL)
	{
		_cmd = storage[storagekey].GetAt(pos).cmd;
		b=TRUE;
	}	
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlListAqCmd::GetCmdName(int storagekey, CAtlList<CString>& listitem)
{
	int count = storage[storagekey].GetCount();
	for(int i=0; i<count; i++)
	{
		listitem.AddHead(storage[storagekey].GetAt(storage[storagekey].FindIndex(i)).cmdName);
	}
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlListAqCmd::GetCmdNameCombo(int storagekey, CSuperCombo* combo)
{
	int count = storage[storagekey].GetCount();
	for(int i=0; i<count; i++)
	{
		combo->AddString(storage[storagekey].GetAt(storage[storagekey].FindIndex(i)).cmdName);
	}
	return TRUE;
}
