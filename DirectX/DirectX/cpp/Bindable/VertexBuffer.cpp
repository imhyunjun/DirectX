#include "../../header/Bindable/VertexBuffer.h"

void VertexBuffer::Bind(Graphics& _gfx) noexcept
{
	//VertexShader를 파이프라인에 주입
	GetContext(_gfx)->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);		//IA -> input assembly 렌더링 파이프라인 시작
}

