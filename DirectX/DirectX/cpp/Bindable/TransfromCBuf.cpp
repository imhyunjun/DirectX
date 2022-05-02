#include "../../header/Bindable/TransformCbuf.h"

TransformCbuf::TransformCbuf(Graphics& _gfx, const Drawable& _parent)
	:
	vcbuf(_gfx),
	parent(_parent)
{}

void TransformCbuf::Bind(Graphics& _gfx) noexcept
{
	vcbuf.Update(_gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * _gfx.GetProjection()
		)
	);
	vcbuf.Bind(_gfx);
}