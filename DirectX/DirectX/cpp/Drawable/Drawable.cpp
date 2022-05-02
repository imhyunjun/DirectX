#include "../../header/Drawable/Drawable.h"
#include "../../header/Macros/GraphicThrowMacros.h"
#include "../../header/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& _gfx) const noexcept(!IS_DEBUG)
{
	for (auto& b : binds)
	{
		b->Bind(_gfx);
	}
	_gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> _bind) noexcept(!IS_DEBUG)
{
	assert("*Must* use AddIndexBuffer to bind index buffer" && typeid(*_bind) != typeid(IndexBuffer));
	binds.push_back(std::move(_bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> _ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && pIndexBuffer == nullptr);
	pIndexBuffer = _ibuf.get();
	binds.push_back(std::move(_ibuf));
}