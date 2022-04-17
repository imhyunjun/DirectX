#include "../header/PException.h"
#include <sstream>

PException::PException()
{
}

PException::PException(int _line, const char* _file) noexcept
{
	line = _line;
	file = _file;
}

const char* PException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* PException::GetType() const noexcept
{
	return "Exception";
}

int PException::GetLine() const noexcept
{
	return line;
}

const std::string& PException::GetFile() const noexcept
{
	return file;
}

std::string PException::GetOriginString() const noexcept
{
	return std::string();
}
