
#ifndef __GARGN_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GARGN_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GARgn  GARgn
class GARgn
{
public:
	//持失切
	GARgn(void);
	//社瑚切
	~GARgn(void);
	static HRGN CreateRoundRectRgn (int left, int top, int right, int bottom, int ellipse_width, int ellipse_height );
	static BOOL ImageLoadRgn(HMODULE hModule,UINT ResourceName, LPCTSTR ResourceType, CRgnHandle& rgn);
	static BOOL SetShapeBitmap(CBitmapHandle hBitmap, COLORREF rgbstart,COLORREF rgbend, CRgnHandle& rgn);
};
#endif// __GARGN_B7A15A9D01DC403996FC45F7F92F52D3_H__
