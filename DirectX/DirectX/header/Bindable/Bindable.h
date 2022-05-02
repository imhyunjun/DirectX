#pragma once
#include "../Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& _gfx) = 0;
	virtual ~Bindable() = default;

protected:
	static ID3D11DeviceContext* GetContext(Graphics& _gfx);
	static ID3D11Device* GetDevice(Graphics& _gfx);
	static DxgiInfoManager& GetInfoManager(Graphics& _gfx) noexcept(!IS_DEBUG);

};
