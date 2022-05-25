#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "../LHJMath.h"

class Cone
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int _longDiv)
	{
		namespace dx = DirectX;
		assert(_longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / _longDiv;

		//base
		std::vector<V> vertices;
		for (int iLong = 0; iLong < _longDiv; iLong++)
		{
			vertices.emplace_back();
			auto v = dx::XMVector3Transform(base, dx::XMMatrixRotationZ(longitudeAngle * iLong));
			dx::XMStoreFloat3(&vertices.back().pos, v);
		}

		//center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto iCenter = (unsigned short)(vertices.size() - 1);

		//²ÀÁöÁ¡
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto iTip = (unsigned short)(vertices.size() - 1);

		//base ÀÎµ¦½º
		std::vector<unsigned short> indices;
		for (unsigned short iLong = 0; iLong < _longDiv; iLong++)
		{
			indices.push_back(iCenter);
			indices.push_back((iLong + 1) % _longDiv);
			indices.push_back(iLong);
		}

		//cone ÀÎµ¦½º
		for (unsigned short iLong = 0; iLong < _longDiv; iLong++)
		{
			indices.push_back(iLong);
			indices.push_back((iLong + 1) % _longDiv);
			indices.push_back(iTip);
		}

		return { std::move(vertices),std::move(indices) };
	}

	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};