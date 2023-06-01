
#ifndef __GDIPLUSBITMAP_74F736E25AB94b5dA789E4730F58F2A1_H__
#define __GDIPLUSBITMAP_74F736E25AB94b5dA789E4730F58F2A1_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//파일 로드를 기본으로 하는 비트맵 클래스
class CGdiPlusBitmap
{
	 static ULONG_PTR       m_gdiplusToken;
public:
	//======================================================//
	//			Gdi 비트맵
	//======================================================//
	Gdiplus::Bitmap* m_pBitmap;
public:
	//생성자
	CGdiPlusBitmap();
	CGdiPlusBitmap(LPCWSTR pFile);
	//소멸자
	virtual ~CGdiPlusBitmap();
	//객체를 비운다
	void Empty();
	//파일을 이용하여 비트맵을 만들 때
	bool Load(LPCWSTR pFile);
	//비트맵 객체를 넘긴다
	operator Gdiplus::Bitmap*() const;
	//Gdiplus 사용 등록
	static void GdiStart();
	//Gdiplus 사용 해제
	static void GdiShut();
};
//리소스 이미지를 로드해서 쓰기 위한 클래스
class CGdiPlusBitmapResource : public CGdiPlusBitmap
{
protected:
	//글로벌 항당 메모리 복사 객체
	HGLOBAL m_hBuffer;
public:
	// 생성자
	CGdiPlusBitmapResource();
	CGdiPlusBitmapResource(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	CGdiPlusBitmapResource(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	CGdiPlusBitmapResource(UINT id, UINT type, HMODULE hInst = NULL);
	//소멸자
	virtual ~CGdiPlusBitmapResource();
	//객체를 비운다
	void Empty();
	//리소스 로드함수
	bool Load(LPCTSTR pName, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	bool Load(UINT id, LPCTSTR pType = RT_RCDATA, HMODULE hInst = NULL);
	bool Load(UINT id, UINT type, HMODULE hInst = NULL);
	//비트맵 파일을 넘긴다
	CBitmap GetWinBitmap();
};
#endif //__GDIPLUSBITMAP_74F736E25AB94b5dA789E4730F58F2A1_H__