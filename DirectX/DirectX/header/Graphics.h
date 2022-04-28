#pragma once
#include "Win.h"
#include "PException.h"
#include <d3d11.h>	
#include <vector>
#include "DxgiInfoManager.h"

class Graphics
{
public:
	class Exception : public PException
	{
		using PException::PException;
	};

	class HrException : public Exception
	{
	public:
		HrException(int _line, const char* _file, HRESULT _hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
		std::string GetErrorDescription() const;
		std::string GetErrorInfo() const;
	private:
		HRESULT hr;
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};

public:
	Graphics(HWND _hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void EndFrame();
	void ClearBuffer(float _red, float _green, float _blue);

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};