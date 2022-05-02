#include "../../header/Bindable/Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& _gfx) 
{
	return _gfx.pContext.Get();
}

ID3D11Device* Bindable::GetDevice(Graphics& _gfx) 
{
	return _gfx.pDevice.Get();
}

DxgiInfoManager& Bindable::GetInfoManager(Graphics& _gfx) noexcept(!IS_DEBUG)
{
#ifndef NDEBUG
	return _gfx.infoManager;
#else
	throw std::logic_error("YouFuckedUp! (tried to access gfx.infoManager in Release config)");
#endif
}