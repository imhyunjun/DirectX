#include "../../header/Bindable/IndexBuffer.h"
#include "../../header/Macros/GraphicThrowMacros.h"

IndexBuffer::IndexBuffer(Graphics& _gfx, const std::vector<unsigned short>& _indices)
	:
	count((UINT)_indices.size())
{
	INFOMAN(_gfx);

	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = UINT(count * sizeof(unsigned short));
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = _indices.data();
	GFX_THROW_INFO(GetDevice(_gfx)->CreateBuffer(&ibd, &isd, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& _gfx) noexcept
{
	//ÀÎµ¦½º ¹öÆÛ ¹­±â
	GetContext(_gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
