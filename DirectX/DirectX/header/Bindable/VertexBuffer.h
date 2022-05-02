#pragma once
#include "Bindable.h"
#include "../Macros/GraphicThrowMacros.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& _gfx, const std::vector<V>& _vertices)
		:
		stride(sizeof(V))
	{
		INFOMAN(_gfx);

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = UINT(sizeof(V) * _vertices.size());
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = _vertices.data();
		GFX_THROW_INFO(GetDevice(_gfx)->CreateBuffer(&bd, &sd, &pVertexBuffer));
	}

		
	void Bind(Graphics& _gfx) noexcept override;
protected:
	UINT stride;
	UINT offset;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};