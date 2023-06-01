
#include "stdAfx.h"

//--------------------------------------------------------------
//|
//|��ư ��� �Լ�
//|
//---------------------------------------------------------------

//--------------------------------------------------------------
//|
//|������
//|
//---------------------------------------------------------------
CAtlmapStorageBitmap::CAtlmapStorageBitmap(void):
drawname(_T(""))
{
}
//--------------------------------------------------------------
//|
//|�Ҹ���
//|
//---------------------------------------------------------------
CAtlmapStorageBitmap::~CAtlmapStorageBitmap(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::AddImageBitmap(CString name, CBitmapHandle bit, BOOL bViewChange)
{
	BOOL b=FALSE;
	if(storage.Lookup(name)==NULL)
	{
		storage[name]=bit;
		if(bViewChange)
			drawname = name;
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::DeleteImageBitmap(CString name)
{
	if(storage.IsEmpty()) return FALSE;
	BOOL b=FALSE;
	BitmapPair bitpair = NULL;
	if(bitpair = storage.Lookup(name))
	{
		//CString strkey = bitpair->m_key;
		CBitmap bithandle = bitpair->m_value;
		//�޸� ����
		bithandle.DeleteObject();
		storage.RemoveKey(name);
		b=TRUE;
	}
	return b;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::DeleteImageBitmapAll()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	while (pos)
	{
		BitmapPair bitpair = NULL;
		bitpair = storage.GetNext(pos);
		CBitmap bithandle = bitpair->m_value;
		//��Ʈ�� �ڵ� �޸� ����
		bithandle.DeleteObject();
	}
	//����Ʈ���� ��� ����
	storage.RemoveAll();
	return TRUE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CBitmap CAtlmapStorageBitmap::GetImageBitmap(CString name)
{
	if(storage.IsEmpty()&&name.IsEmpty()) return NULL;
	CBitmapHandle bitvalue;
	drawname=name;
	if(storage.Lookup(drawname, bitvalue))
	{
		return (HBITMAP)CopyImage((HANDLE)bitvalue, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
CBitmap CAtlmapStorageBitmap::GetImageBitmap()
{
	if(storage.IsEmpty()&&drawname.IsEmpty()) return NULL;
	CBitmapHandle bitvalue;
	if(storage.Lookup(drawname, bitvalue))
	{
		return (HBITMAP)CopyImage((HANDLE)bitvalue, IMAGE_BITMAP, 0,0, LR_COPYRETURNORG);
	}
	return NULL;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::GetNextImageBitmap()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	while (pos)
	{
		BitmapPair bitpair = NULL;
		bitpair = storage.GetNext(pos);
		CString strname = bitpair->m_key;
		if(pos&&strname.Compare(drawname)==0)
		{
			bitpair = NULL;
			bitpair = storage.GetAt(pos);
			drawname = bitpair->m_key;
			return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
BOOL CAtlmapStorageBitmap::GetFirstImageBitmap()
{
	if(storage.IsEmpty()) return FALSE;
	POSITION pos = storage.GetStartPosition();
	if(pos)
	{
		BitmapPair bitpair = NULL;
		bitpair = storage.GetAt(pos);
		drawname = bitpair->m_key;
		return TRUE;
	}
	return FALSE;
}
//--------------------------------------------------------------
//|
//|��Ʈ�� ���� �̸� ���� ����
//|
//---------------------------------------------------------------
CString CAtlmapStorageBitmap::GetDrawBitmapName()
{
	return drawname;
}