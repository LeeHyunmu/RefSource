
#include "stdAfx.h"
//--------------------------------------------------------------
//|
//|持失切
//|
//---------------------------------------------------------------
GARgn::GARgn(void)
{
}
//--------------------------------------------------------------
//|
//|社瑚切
//|
//---------------------------------------------------------------
GARgn::~GARgn(void)
{
}
//--------------------------------------------------------------
//|
//|
//|
//---------------------------------------------------------------
HRGN GARgn::CreateRoundRectRgn (int left, int top, int right, int bottom, int ellipse_width, int ellipse_height )
{
	HRGN              hrgn = 0;
	int               asq, bsq, d, xd, yd;
	RECT              rect;
	std::vector<RECT> rects;
	if (ellipse_width == 0 || ellipse_height == 0)
		return CreateRectRgn( left, top, right, bottom );
	/* Make the dimensions sensible */
	if (left > right ) { int tmp = left; left = right;  right  = tmp; }
	if (top  > bottom) { int tmp = top;  top  = bottom; bottom = tmp; }
	ellipse_width  = abs(ellipse_width);
	ellipse_height = abs(ellipse_height);
	/* Check parameters */
	if (ellipse_width  > right-left) ellipse_width  = right-left;
	if (ellipse_height > bottom-top) ellipse_height = bottom-top;
	/* Ellipse algorithm, based on an article by K. Porter */
	/* in DDJ Graphics Programming Column, 8/89 */
	asq = ellipse_width  * ellipse_width  / 4; /* a^2 */
	bsq = ellipse_height * ellipse_height / 4; /* b^2 */
	if (asq == 0) asq = 1;
	if (bsq == 0) bsq = 1;
	d  = bsq - asq * ellipse_height / 2 + asq / 4; /* b^2 - a^2b + a^2/4 */
	xd = 0;
	yd = asq * ellipse_height;                     /* 2a^2b */
	rect.left   = left + ellipse_width  / 2;
	rect.right  = right - ellipse_width / 2;
	rects.reserve (ellipse_height);
	/* Loop to draw first half of quadrant */
	while (xd < yd)
	{
		/* if nearest pixel is toward the center */
		if (d > 0)  
		{
			/* move toward center */
			rect.top    = top++;
			rect.bottom = rect.top + 1;
			rects.push_back (rect);
			rect.top    = --bottom;
			rect.bottom = rect.top + 1;
			rects.push_back (rect);
			yd -= 2*asq;
			d  -= yd;
		}
		/* next horiz point */
		rect.left --;        
		rect.right++;
		xd += 2*bsq;
		d  += bsq + xd;
	}
	/* Loop to draw second half of quadrant */
	d += (3 * (asq-bsq) / 2 - (xd+yd)) / 2;
	while (yd >= 0)
	{
		/* next vertical point */
		rect.top    = top++;
		rect.bottom = rect.top + 1;
		rects.push_back (rect);
		rect.top    = --bottom;
		rect.bottom = rect.top + 1;
		rects.push_back (rect);
		/* if nearest pixel is outside ellipse */
		if (d < 0)   
		{
			/* move away from center */
			rect.left --;     
			rect.right++;
			xd += 2*bsq;
			d  += xd;
		}
		yd -= 2*asq;
		d  += asq - yd;
	}
	/* Add the inside rectangle */
	if (top <= bottom)
	{
		rect.top    = top;
		rect.bottom = bottom;
		//rects.push_back (rect);
		rects.insert (rects.begin(), rect);
	}

	static int count=0;
	std::vector<RECT>::iterator i;
	for (i=rects.begin(); i!=rects.end(); i++)
	{
		ATLTRACE (_T("[%d]%3d,%3d:%3d,%3d \r\n"), ++count,
			(*i).top,
			(*i).bottom,
			(*i).left,
			(*i).right
			);
	}
	if (rects.empty())
		return NULL;
	RGNDATAHEADER* pRgnData;
	UINT           RgnDataSize;
	UINT           RgnRectCount;
	LPRECT         pRect;
	RgnRectCount = rects.size();
	RgnDataSize  = sizeof(RGNDATAHEADER) + sizeof(RECT)*RgnRectCount;
	pRgnData     = (RGNDATAHEADER*)new BYTE[ RgnDataSize ];
	if (pRgnData==NULL)
		return NULL;
	pRgnData->dwSize         = sizeof(RGNDATAHEADER);
	pRgnData->iType          = RDH_RECTANGLES;
	pRgnData->nCount         = RgnRectCount;
	pRgnData->rcBound.left   = 0;
	pRgnData->rcBound.top    = 0;
	pRgnData->rcBound.right  = 0;
	pRgnData->rcBound.bottom = 0;
	pRect = (LPRECT) ( (LPBYTE) pRgnData + sizeof(RGNDATAHEADER) );
	memcpy (pRect, &*rects.begin(), RgnRectCount*sizeof(RECT));
	hrgn = ExtCreateRegion(NULL, RgnDataSize, (LPRGNDATA)pRgnData);
	delete pRgnData;
	return hrgn;
}
