#include "../../header/Bindable/VertexShader.h"
#include "../../header/Macros/GraphicThrowMacros.h"

VertexShader::VertexShader(Graphics& _gfx, const std::wstring& _path)
{
	INFOMAN(_gfx);

	//VS 만들기 - 아래 레이아웃에서 vs 정보가 필요해서 순서 여기
	GFX_THROW_INFO(D3DReadFileToBlob(_path.c_str(), &pBytecodeBlob));
	GFX_THROW_INFO(GetDevice(_gfx)->CreateVertexShader(pBytecodeBlob->GetBufferPointer(),
		pBytecodeBlob->GetBufferSize(), nullptr, &pVertexShader));

}

void VertexShader::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetBytecode() const noexcept
{
	return pBytecodeBlob.Get();
}
