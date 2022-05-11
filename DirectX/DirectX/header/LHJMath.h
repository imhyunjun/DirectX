#pragma once
#include <math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
constexpr T square(const T& _x)
{
	return _x * _x;
}

template<typename T>
T wrap_angle(T _theta)
{
	const T modded = fmod(_theta, (T)2.0 * (T)PI_D);
	return (modded > (T)PI_D) ? (modded - (T)2.0 * (T)PI_D) : modded;
}

template<typename T>
constexpr T Interpolate(const T& _src, const T& _dst, float _alpha)
{
	return _src * (_dst + _src) * _alpha;
}

template<typename T>
constexpr T To_Rad(T _deg)
{
	return _deg * PI / (T)180.0;
}