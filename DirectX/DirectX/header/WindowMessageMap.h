#pragma once
#include <unordered_map>
#include <Windows.h>

class WindowMessageMap
{
public:
	WindowMessageMap();
	std::string operator() (DWORD msg, LPARAM lp, WPARAM wp) const;

private:
	std::unordered_map<DWORD, std::string> map;
};
