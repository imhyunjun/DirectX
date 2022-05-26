#pragma once
#include "Win.h"
#include "PException.h"
#include <string>
#include <assert.h>
#include <memory>

class Surface
{
public:
	class Color
	{
	public:
		unsigned int dword;
	public:
		constexpr Color() : dword() {}
		constexpr Color(const Color& _col) : dword(_col.dword) {}
		constexpr Color(unsigned int _dw) : dword(_dw) {}
		constexpr Color(unsigned char _x, unsigned char _r, unsigned char _g, unsigned char _b)
			: dword((_x << 24u) | (_r << 16u) | (_g << 8u) | _b) {}
		constexpr Color(unsigned char _r, unsigned char _g, unsigned char _b)
			: dword((_r << 16u | _g << 8u) | _b) {}
		constexpr Color(Color _col, unsigned char _x)
			: Color((_x << 24u) | _col.dword) {}
		Color& operator =(Color _color)
		{
			dword = _color.dword;
			return *this;
		}
		constexpr unsigned char GetX() const
		{
			return dword >> 24u;
		}
		constexpr unsigned char GetA() const { return GetX(); }
		constexpr unsigned char GerR() const { return (dword >> 16u) & 0xFFu; }
		constexpr unsigned char GetG() const { return (dword >> 8u) & 0xFFu; }
		constexpr unsigned char GetB() const { return (dword & 0xFFu); }
		void SetX(unsigned char _x) { dword = (dword & 0xFFFFFFu) | (_x << 24u); }
		void SetA(unsigned char _a) { SetX(_a); }
		void SetR(unsigned char _r) { dword = (dword & 0xFFFFFFu) | (_r << 16u); }
		void SetG(unsigned char _g) { dword = (dword & 0xFFFFFFu) | (_g << 8u); }
		void SetB(unsigned char _b) { dword = (dword & 0xFFFFFFu) | (_b); }
	};
public:
	class Exception : public PException
	{
	public:
		Exception(int _line, const char* _file, std::string _note);
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		const std::string& Getnote() const;
	private:
		std::string note;
	};
public:
	Surface(unsigned int _width, unsigned int _height, unsigned int _pitch) noexcept;
	Surface(unsigned int _width, unsigned int _height) noexcept;
	Surface(Surface&& _source) noexcept;
	Surface(Surface&) = delete;
	Surface& operator=(Surface&& _donor) noexcept;
	Surface& operator=(const Surface&) = delete;
	~Surface();
	void Clear(Color _fillValue);
	void PutPixel(unsigned int _x, unsigned int _y, Color _c);
	Color GetPixel(unsigned int _x, unsigned int _y) const;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	Color* GetBufferPtr();
	const Color* GetBufferPTR() const;
	const Color* GetBufferPtrConst() const;
	static Surface FromFile(const std::string& _name);
	void Save(const std::string& _filename) const;
	void Copy(const Surface& _src);
private:
	Surface(unsigned int _width, unsigned int _height, std::unique_ptr<Color[]> _pBufferParam);
private:
	std::unique_ptr<Color[]> pBuffer;
	unsigned int width;
	unsigned int height;

};