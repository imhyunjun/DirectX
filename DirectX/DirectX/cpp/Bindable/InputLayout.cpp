#include "../../header/Bindable/InputLayout.h"
#include "../../header/Macros/GraphicThrowMacros.h"

InputLayout::InputLayout(Graphics& _gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& _layout, ID3DBlob* _pVertexShaderBytecode)
{
	INFOMAN(_gfx);

	GFX_THROW_INFO(GetDevice(_gfx)->CreateInputLayout(
		_layout.data(), (UINT)_layout.size(),
		_pVertexShaderBytecode->GetBufferPointer(),
		_pVertexShaderBytecode->GetBufferSize(),
		&pInputLayout
	));

	
}

void InputLayout::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->IASetInputLayout(pInputLayout.Get());
}
