#pragma once
#include <chrono>

class PTimer
{
public:
	PTimer();
	float Mark();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point last;
};