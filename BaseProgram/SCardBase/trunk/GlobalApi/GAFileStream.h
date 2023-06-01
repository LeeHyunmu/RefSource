
#ifndef __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GAFILESTREAM  GAFileStream
class GAFileStream
{
public:
	//持失切
	GAFileStream(void);
	//社瑚切
	~GAFileStream(void);
	static BOOL CreateLineReadFile(CString filename, vecString& vecfile);
	static BOOL CreateLineWriteFile(CString filename, vecString& vecfile);
	static BOOL CreateFEFFWriteFile(CString filename, LPTSTR file);
	static BOOL CreateFEFFReadFile(CString filename, LPBYTE& bytefile);
};
#endif// __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__
