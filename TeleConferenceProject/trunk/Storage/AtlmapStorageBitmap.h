
#ifndef __ATLMAPSTORAGEBITMAP_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __ATLMAPSTORAGEBITMAP_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//버튼 이미지 개수


class CAtlmapStorageBitmap
{
	//스트링 저장 CAtlMap
	typedef CAtlMap<CString,CBitmapHandle,CStringElementTraits<CString> > BitmapAtlMap;
	typedef CAtlMap<CString,CBitmapHandle,CStringElementTraits<CString> >::CPair* BitmapPair;
	BitmapAtlMap storage;
	CString drawname;
public:
	//생성자
	CAtlmapStorageBitmap();
	//소멸자
	~CAtlmapStorageBitmap();

	//추가
	BOOL AddImageBitmap(CString name, CBitmapHandle bit, BOOL bViewChange=TRUE);
	//삭제
	BOOL DeleteImageBitmap(CString name);
	//아이템 모두 삭제
	BOOL DeleteImageBitmapAll();
	//이미지를 가져 온다(Key 이름으로)
	CBitmap GetImageBitmap(CString name);
	//
	CBitmap GetImageBitmap();
	//다음 비트맵
	BOOL GetNextImageBitmap();
	//시작 비트맵
	BOOL GetFirstImageBitmap();
	//비트맵 저장 이름 가져 오기
	CString GetDrawBitmapName();
};
#endif// __ATLMAPSTORAGEBITMAP_B7A15A9D01DC403996FC45F7F92F52D3_H__
