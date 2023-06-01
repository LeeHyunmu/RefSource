
#ifndef __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__
#define __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define  _GAFILESTREAM  GAFileStream
typedef vector<CString> vecFileStreamString;
class GAFileStream
{
public:
	//持失切
	GAFileStream(void);
	//社瑚切
	~GAFileStream(void);
	static BOOL CreateLineReadFile(CString filename, vecFileStreamString& vecfile);
	static BOOL CreateLineWriteFile(CString filename, vecFileStreamString& vecfile);
	static BOOL CreateFEFFWriteFile(CString filename, LPTSTR file);
	static BOOL CreateReadFile(CString filename, LPBYTE& bytefile, DWORD& dwFileSize);
};
#endif// __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__
