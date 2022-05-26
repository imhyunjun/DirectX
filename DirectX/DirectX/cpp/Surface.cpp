#define FULL_WINTARD
#include "../header/Surface.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment(lib, "gdiplus.lib")

Surface::Surface(unsigned int _width, unsigned int _height, unsigned int _pitch) noexcept
	: pBuffer(std::make_unique<Color[]>(_pitch * _height)),
	width(_width),
	height(_height)
{
}

Surface::Surface(unsigned int _width, unsigned int _height) noexcept
	: Surface(_width, _height, _width)
{	
}

Surface::Surface(Surface&& _source) noexcept
	:
	pBuffer(std::move(_source.pBuffer)),
	width(_source.width),
	height(_source.height)
{
}

Surface& Surface::operator=(Surface&& _donor) noexcept
{
	width = _donor.width;
	height = _donor.height;
	pBuffer = std::move(_donor.pBuffer);
	_donor.pBuffer = nullptr;
	return *this;
}

Surface::~Surface()
{
}

void Surface::Clear(Color _fillValue)
{
	memset(pBuffer.get(), _fillValue.dword, width * height * sizeof(Color));
}

void Surface::PutPixel(unsigned int _x, unsigned int _y, Color _c)
{
	assert(_x >= 0);
	assert(_y >= 0);
	assert(_x < width);
	assert(_y < height);
	pBuffer[_y * width + _x] = _c;
}

Surface::Color Surface::GetPixel(unsigned int _x, unsigned int _y) const
{
	assert(_x >= 0);
	assert(_y >= 0);
	assert(_x < width);
	assert(_y < height);
	return pBuffer[_y * width + _x];
}

unsigned int Surface::GetWidth() const
{
	return width;
}

unsigned int Surface::GetHeight() const
{
	return height;
}

Surface::Color* Surface::GetBufferPtr()
{
	return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPTR() const
{
	return pBuffer.get();
}

const Surface::Color* Surface::GetBufferPtrConst() const
{
	return pBuffer.get();
}

Surface Surface::FromFile(const std::string& _name)
{
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int pitch = 0;
	std::unique_ptr<Color[]> pBuffer = nullptr;

	{
		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, _name.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(wideName);
		if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Loading image [" << _name << "]: failed to load.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		height = bitmap.GetHeight();
		pBuffer = std::make_unique<Color[]>(width * height);

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				Gdiplus::Color c;
				bitmap.GetPixel(x, y, &c);
				pBuffer[y * pitch + x] = c.GetValue();
			}
		}
	}

	return Surface(width, height, std::move(pBuffer));
}

void Surface::Save(const std::string& _filename) const
{
	auto GetEncoderClsid = [&_filename](const WCHAR* format, CLSID* pClsid) -> void
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

		Gdiplus::GetImageEncodersSize(&num, &size);
		if (size == 0)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << _filename << "]: failed to get encoder; size == 0.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
		if (pImageCodecInfo == nullptr)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << _filename << "]: failed to get encoder; failed to allocate memory.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j)
		{
			if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return;
			}
		}

		free(pImageCodecInfo);
		std::stringstream ss;
		ss << "Saving surface to [" << _filename <<
			"]: failed to get encoder; failed to find matching encoder.";
		throw Exception(__LINE__, __FILE__, ss.str());
	};

	CLSID bmpID;
	GetEncoderClsid(L"image/bmp", &bmpID);


	// convert filenam to wide string (for Gdiplus)
	wchar_t wideName[512];
	mbstowcs_s(nullptr, wideName, _filename.c_str(), _TRUNCATE);

	Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer.get());
	if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok)
	{
		std::stringstream ss;
		ss << "Saving surface to [" << _filename << "]: failed to save.";
		throw Exception(__LINE__, __FILE__, ss.str());
	}
}

void Surface::Copy(const Surface& _src)
{
	assert(width == _src.width);
	assert(height == _src.height);
	memcpy(pBuffer.get(), _src.pBuffer.get(), width * height * sizeof(Color));
}

Surface::Surface(unsigned int _width, unsigned int _height, std::unique_ptr<Color[]> _pBufferParam)
	:
	width(_width),
	height(_height),
	pBuffer(std::move(_pBufferParam))
{}

Surface::Exception::Exception(int _line, const char* _file, std::string _note)
	:
	PException(_line, _file),
	note(std::move(_note))
{
}

const char* Surface::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << PException::what() << std::endl
		<< "[Note] " << Getnote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Surface::Exception::GetType() const noexcept
{
	return "P Graphics Exception";
}

const std::string& Surface::Exception::Getnote() const
{
	// // O: 여기에 return 문을 삽입합니다.
	return note;
}
