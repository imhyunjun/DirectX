#include "../../header/Bindable/TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& _gfx, const Drawable& _parent)
	:
	//vcbuf(_gfx),
	parent(_parent)
{
	if (!pVcBuf)
	{
		pVcBuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(_gfx);
	}
}

void TransformCbuf::Bind(Graphics& _gfx) noexcept
{
	pVcBuf->Update(_gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * _gfx.GetProjection()
		)
	);
	pVcBuf->Bind(_gfx);
}

//스태틱이라 아래에 선언
std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> pVcBuf;