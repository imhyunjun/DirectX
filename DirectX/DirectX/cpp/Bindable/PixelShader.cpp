#include "../../header/Bindable/PixelShader.h"
#include "../../header/Macros/GraphicThrowMacros.h"

PixelShader::PixelShader(Graphics& _gfx, const std::wstring& _path)
{
	INFOMAN(_gfx);

	//PS ∏∏µÈ±‚(«»ºø ºŒ¿Ã¥ı)
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_INFO(D3DReadFileToBlob(_path.c_str(), &pBlob));
	GFX_THROW_INFO(GetDevice(_gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

}

void PixelShader::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
}
