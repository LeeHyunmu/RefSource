
#ifndef __ATLMAPSTORAGEBITMAP_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __ATLMAPSTORAGEBITMAP_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//��ư �̹��� ����


class CAtlmapStorageBitmap
{
	//��Ʈ�� ���� CAtlMap
	typedef CAtlMap<CString,CBitmapHandle,CStringElementTraits<CString> > BitmapAtlMap;
	typedef CAtlMap<CString,CBitmapHandle,CStringElementTraits<CString> >::CPair* BitmapPair;
	BitmapAtlMap storage;
	CString drawname;
public:
	//������
	CAtlmapStorageBitmap();
	//�Ҹ���
	~CAtlmapStorageBitmap();

	//�߰�
	BOOL AddImageBitmap(CString name, CBitmapHandle bit, BOOL bViewChange=TRUE);
	//����
	BOOL DeleteImageBitmap(CString name);
	//������ ��� ����
	BOOL DeleteImageBitmapAll();
	//�̹����� ���� �´�(Key �̸�����)
	CBitmap GetImageBitmap(CString name);
	//
	CBitmap GetImageBitmap();
	//���� ��Ʈ��
	BOOL GetNextImageBitmap();
	//���� ��Ʈ��
	BOOL GetFirstImageBitmap();
	//��Ʈ�� ���� �̸� ���� ����
	CString GetDrawBitmapName();
};
#endif// __ATLMAPSTORAGEBITMAP_B7A15A9D01DC403996FC45F7F92F52D3_H__
