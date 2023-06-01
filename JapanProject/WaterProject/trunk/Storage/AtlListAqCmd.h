
#ifndef __ATLLISTAQCMD_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __ATLLISTAQCMD_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수

class CSuperCombo;
class CAtlListAqCmd
{
	struct mItem
	{
		byte cmd;
		CString cmdName;
	};
	class MyTraits : public CElementTraits<mItem>
	{
	public:
		static bool CompareElements(const mItem& element1, const mItem& element2)
		{
			if(element1.cmdName.Compare(element2.cmdName)==0)
				return true;
			else
				return false;
		}
	};
	// CAtlMap
	typedef CAtlList<mItem,MyTraits> AQDevice;
	AQDevice storage[4];
public:
	//생성자
	CAtlListAqCmd();
	//소멸자
	~CAtlListAqCmd();
public:
	//추가
	BOOL AddData(int storagekey, int _cmd, CString _cmdName);
	//삭제
	BOOL DeleteData(int storagekey, CString _cmdName);
	//아이템 모두 삭제
	BOOL DeleteDataAll();
	//
	BOOL GetCmd(int storagekey, CString _cmdName, byte& _cmd);
	//
	BOOL GetCmdName(int storagekey, CAtlList<CString>& listitem);
};
#endif// __ATLLISTAQCMD_B7A15A9D01DC403996FC45F7F92F52D3_H__
