
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
	//생성자
	GAFileStream(void);
	//소멸자
	~GAFileStream(void);
	//파일을 라인단위로 읽어 들인다
	static BOOL CreateLineReadFile(CString filename, vecFileStreamString& vecfile);
	//파일을 라인 단위로 저장한다
	static BOOL CreateLineWriteFile(CString filename, vecFileStreamString& vecfile);
	//유니코드 타입으로 파일 저장한다
	static BOOL CreateFEFFWriteFile(CString filename, LPTSTR file);
	//파일을 바이트 버퍼로 읽어 들인다, 사용후 메모리 해제 필수, 파일 사이즈 넘겨준다
	static BOOL CreateByteReadFile(CString filename, LPBYTE& bytefile, DWORD& dwFileSize);
};
#endif// __GAFILESTREAM_B7A15A9D01DC403996FC45F7F92F52D3_H__
