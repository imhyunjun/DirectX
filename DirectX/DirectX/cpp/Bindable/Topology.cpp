#include "../../header/Bindable/Topology.h"
#include "../../header/Macros/GraphicThrowMacros.h"

Topology::Topology(Graphics& _gfx, D3D11_PRIMITIVE_TOPOLOGY _type)
	:type(_type)
{
}

void Topology::Bind(Graphics& _gfx) noexcept
{
	//�⺻ ������ �ﰢ������
	GetContext(_gfx)->IASetPrimitiveTopology(type);
	//pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	//pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
}
