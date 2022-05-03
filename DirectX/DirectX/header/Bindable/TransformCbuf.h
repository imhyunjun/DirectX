#pragma once
#include "ConstantBuffers.h"
#include "../Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	//VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcBuf;
	const Drawable& parent;
};