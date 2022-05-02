#include "../../header/Bindable/VertexBuffer.h"

void VertexBuffer::Bind(Graphics& _gfx) noexcept
{
	//VertexShader�� ���������ο� ����
	GetContext(_gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);		//IA -> input assembly ������ ���������� ����
}

