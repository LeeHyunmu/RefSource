#pragma once

//#pragma comment (lib, "shlwapi.lib")
//#pragma comment (lib, "gdiplus.lib")

#include <string>
#include <map>

#include "simpleiunknown.h"

class EncoderInfo;

class 
	__declspec( uuid("274FD788-4F2C-4a52-81BB-6CC8A9567D8E") ) 
	ImageFormat : public SimpleIUnknown
{
private:
	ImageFormat(EncoderInfo* encoder_info);
	virtual ~ImageFormat();

private:
	HGLOBAL m_hmem;
	EncoderInfo*	m_encoder_info;

private:
	void ensurePathExist(const wchar_t* path);
	void mem_alloc(unsigned int length);
	void mem_free();

public:
	// factory method
	static ImageFormat* create();

public:
	//데이타로 이미지 로드
	virtual void loadImage(LPSTR file, unsigned int length);
	//스트림으로 이미지 로드
	virtual void loadImage(std::istream& is, unsigned int length);
	//이미지 파일로 만든다
	virtual void saveAs(const wchar_t* type, const wchar_t* savepath);
	virtual void loadAndSaveAs(HBITMAP hbm, HPALETTE hpal, const wchar_t* type, const wchar_t* savepath);
	virtual void loadAndSaveAs(IStream* stm, const wchar_t* type, const wchar_t* savepath);
	virtual void loadWMFAndSaveAs(IStream* stm, int width, int height,
		const wchar_t* type, const wchar_t* savepath);

};

_COM_SMARTPTR_TYPEDEF(ImageFormat, __uuidof(ImageFormat));

