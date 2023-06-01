#include "stdafx.h"

#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")
//#include <gdiplus.h>
//#pragma comment (lib, "gdiplus.lib")

#include <comdef.h>
#include <crtdbg.h>
#include <wchar.h>

#include <string>
#include <map>
#include <strstream>
#include <sstream>
#include <memory>
#include "imageformat.h"

using namespace std;


class EncoderInfo {
public:
	EncoderInfo()
	{
		unsigned int num = 0;
		unsigned int size = 0;
		Gdiplus::GetImageEncodersSize(&num, &size);
		_ASSERTE(size != 0);

		Gdiplus::ImageCodecInfo* codec_info = (Gdiplus::ImageCodecInfo*) malloc(size);
		Gdiplus::GetImageEncoders(num, size, codec_info);

		for (unsigned int j=0; j<num; ++j) {
			wstring _type = codec_info[j].FormatDescription;
			m_clsid_map.insert(
				map<wstring, CLSID>::value_type( _type, codec_info[j].Clsid )
			);
			// special patch for 'jpg'
			if (_wcsicmp(_type.c_str(), L"jpeg") == 0) {
				m_clsid_map.insert(
					map<wstring, CLSID>::value_type( wstring(L"jpg"), codec_info[j].Clsid )
				);
			}
		}
		free(codec_info);
	}
	CLSID getEncoderClsid(const wchar_t* format)
	{
		return m_clsid_map[format];
	}
	struct nocase_comparator : public std::binary_function<const std::wstring, const std::wstring, bool>
	{
		bool operator()(const std::wstring& x, const std::wstring& y) const
		{
			return (_wcsicmp(x.c_str(), y.c_str()) < 0);
		}
	};
protected:
	std::map<std::wstring, CLSID, nocase_comparator>	m_clsid_map;
};


ImageFormat* ImageFormat::create()
{
	return new ImageFormat(new EncoderInfo);
}

ImageFormat::ImageFormat(EncoderInfo* encoder_info)
:	m_hmem(0),
	m_encoder_info(encoder_info)
{
}

ImageFormat::~ImageFormat()
{
	delete m_encoder_info;
	mem_free();
}

void ImageFormat::ensurePathExist(const wchar_t* path)
{
	auto_ptr<wchar_t> _dir(new wchar_t[ lstrlenW(path)+1 ]);
	wchar_t* dir = _dir.get();

	lstrcpyW(dir, path);
	PathRemoveFileSpecW(dir);
	if (wcslen(dir))
		if (FALSE == PathFileExistsW(dir))
			CreateDirectoryW(dir, 0);
}

void ImageFormat::mem_alloc(unsigned int length)
{
	mem_free();
	m_hmem = GlobalAlloc(GMEM_DISCARDABLE, length);
	_ASSERTE(m_hmem != 0);
}

void ImageFormat::mem_free()
{
	if (m_hmem) {
		GlobalFree(m_hmem);
		m_hmem = 0;
	}
}

void ImageFormat::loadImage(LPSTR file, unsigned int length)
{
	try {
		mem_alloc(length);
		void* buf = GlobalLock(m_hmem);
		memcpy(buf, file, length);
		GlobalUnlock(m_hmem);
	}
	catch (...) {
		mem_free();
		throw;
	}
}

void ImageFormat::loadImage(std::istream& is, unsigned int length)
{
	try {
		mem_alloc(length);
		void* buf = GlobalLock(m_hmem);
		is.read((char*)buf, length);
		GlobalUnlock(m_hmem);
		_ASSERTE(is.good());
	}
	catch (...) {
		mem_free();
		throw;
	}
}

void ImageFormat::saveAs(const wchar_t* type, const wchar_t* savepath)
{
	try {
		IStreamPtr pstm;
		HRESULT hr = CreateStreamOnHGlobal(m_hmem, false, &pstm);
		_ASSERTE(SUCCEEDED(hr));
		loadAndSaveAs(pstm, type, savepath);
	}
	catch (...) {
		mem_free();
		throw;
	}
	mem_free();
}

void ImageFormat::loadAndSaveAs(IStream* stm, const wchar_t* type, const wchar_t* savepath)
{
	const CLSID& encoderClsid = m_encoder_info->getEncoderClsid(type);
	ensurePathExist(savepath);
	auto_ptr<Gdiplus::Image> img( new Gdiplus::Image(stm) );
	Gdiplus::Status stat = img->Save(savepath, &encoderClsid);
	if (stat != Gdiplus::Ok) {
		if (stat == Gdiplus::Win32Error) {
			DWORD ec = GetLastError();
			_RPT1(_CRT_ASSERT, "ImageFormat::saveAs, Win32Error: %d", ec);
		}
	}
}

void ImageFormat::loadAndSaveAs(HBITMAP hbm, HPALETTE hpal, const wchar_t* type, const wchar_t* savepath)
{
	const CLSID& encoderClsid = m_encoder_info->getEncoderClsid(type);
	ensurePathExist(savepath);
	auto_ptr<Gdiplus::Bitmap> img( new Gdiplus::Bitmap(hbm, hpal) );
	Gdiplus::Status stat = img->Save(savepath, &encoderClsid);
	if (stat != Gdiplus::Ok) {
		if (stat == Gdiplus::Win32Error) {
			DWORD ec = GetLastError();
			_RPT1(_CRT_ASSERT, "ImageFormat::saveAs, Win32Error: %d", ec);
		}
	}
}

void ImageFormat::loadWMFAndSaveAs(IStream* stm, int width, int height, 
									const wchar_t* type, const wchar_t* savepath)
{
	const CLSID& encoderClsid = m_encoder_info->getEncoderClsid(type);
	ensurePathExist(savepath);

	auto_ptr<Gdiplus::Image> img( new Gdiplus::Image(stm, TRUE) );
	auto_ptr<Gdiplus::Bitmap> bitmap( new Gdiplus::Bitmap(width, height,
										PixelFormat24bppRGB ) );
	auto_ptr<Gdiplus::Graphics> graphics( new Gdiplus::Graphics( bitmap.get() ) );
	graphics->Clear( Gdiplus::Color(255,255,255) );
	graphics->DrawImage(img.get(), 0, 0, width+1, height+1);
	Gdiplus::Status stat = bitmap->Save(savepath, &encoderClsid);
	if (stat != Gdiplus::Ok) {
		if (stat == Gdiplus::Win32Error) {
			DWORD ec = GetLastError();
			_RPT1(_CRT_ASSERT, "ImageFormat::saveAs, Win32Error: %d", ec);
		}
	}
}

