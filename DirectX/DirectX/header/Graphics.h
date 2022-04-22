#pragma once
#include "Win.h"
#include <d3d11.h>	

class Graphics
{
public:
	Graphics(HWND _hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float _red, float _green, float _blue)
	{
		const float color[] = { _red, _green, _blue, 1.0f };
		pContext->ClearRenderTargetView(pTarget, color);
	}
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};