#pragma once
#include "Win.h"
#include "PException.h"
#include <d3d11.h>	
#include <vector>
#include <wrl.h>		//Com ptr
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

	class InfoException : public Exception
	{
	public:
		InfoException(int _line, const char* _file, std::vector<std::string> infoMsgs = {});
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const;
	private:
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
	~Graphics() = default;

	void EndFrame();
	void ClearBuffer(float _red, float _green, float _blue);

	void DrawTestTriangle(float _angle);

private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
};