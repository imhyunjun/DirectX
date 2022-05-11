#pragma once

#include <vector>
#include <array>
#include "IndexedTriangleList.h"
#include "../LHJMath.h"

class Plane
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int _divisons_x, int _divisons_y)
	{
		namespace dx = DirectX;
		assert(_divisons_x >= 1);
		assert(_divisons_y >= 1);

		constexpr float width = 2.0f;
		constexpr float height = 2.0f;
		const int vertices_x = _divisons_x + 1;
		const int vertices_y = _divisons_y + 1;
		std::vector<V> vertices(vertices_x * vertices_y);

		{
			const float side_x = width / 2.0f;
			const float side_y = height / 2.0f;
			const float divisionSize_x = width / float(_divisons_x);
			const float divisionSize_y = height / float(_divisons_y);
			const auto bottomLeft = dx::XMVectorSet(-side_x, -side_y, 0.0f, 0.0f);
		}
	}
};