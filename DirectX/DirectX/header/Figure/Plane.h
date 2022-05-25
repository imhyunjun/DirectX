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

			for (int y = 0, i = 0; y < vertices_y; y++)
			{
				const float y_pos = float(y) * divisionSize_y;
				for (int x = 0; x < vertices_x; x++, i++)
				{
					const auto v = dx::XMVectorAdd(
						bottomLeft,
						dx::XMVectorSet(float(x) * divisionSize_x, y_pos, 0.0f, 0.0f)
					);
					dx::XMStoreFloat3(&vertices[i].pos, v);
				}
			}
		}

		std::vector<unsigned short> inidices;
		inidices.reserve(square(_divisons_x * _divisons_y) * 6);
		{
			const auto vxy2i = [vertices_x](size_t x, size_t y)
			{
				return (unsigned short)(y * vertices_x + x);
			};
			for (size_t y = 0; y < _divisons_y; y++)
			{
				for (size_t x = 0; x < _divisons_x; x++)
				{
					const std::array<unsigned short, 4> indexArray =
					{ vxy2i(x, y), vxy2i(x + 1, y), vxy21(x, y + 1), vxy2i(x + 1, y + 1) };

					indices.push_back(indexArray[0]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[1]);
					indices.push_back(indexArray[2]);
					indices.push_back(indexArray[3]);
				}
			}
		}
		return{ std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(1, 1);
	}
};