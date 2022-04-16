#include "../header/WindowMessageMap.h"
#include <string>
#include <iomanip>
#include <sstream>

#define REGISTER_MESSEAGE(msg){msg, #msg}

WindowMessageMap::WindowMessageMap() :
	map({REGISTER_MESSEAGE(WM_PAINT), REGISTER_MESSEAGE(WM_DESTROY)})
{

}

std::string WindowMessageMap::operator()(DWORD msg, LPARAM lp, WPARAM wp) const
{
	constexpr int firstColWidth = 25;
	const auto i = map.find(msg);

	std::ostringstream oss;
	if (i != map.end())
	{
		oss << std::left << std::setw(firstColWidth) << i->second << std::right;
	}
	else
	{
		std::ostringstream padss;
		padss << "Unknown Message : 0x" << std::hex << msg << "\n";
		oss << std::left << std::setw(firstColWidth) << padss.str() << std::right;
	}

	return oss.str();
}
