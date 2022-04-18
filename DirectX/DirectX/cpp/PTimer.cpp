#include "../header/PTimer.h"

using namespace std::chrono;

PTimer::PTimer()
{
	last = steady_clock::now();
}

float PTimer::Mark()
{
	const auto  old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float PTimer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
