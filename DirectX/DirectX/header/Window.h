#pragma once
#include "Win.h"
#include "PException.h"
#include <sstream>
#include "./Keyboard.h"
#include "./Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

class Window
{
public:
	class Exception : public PException
	{
		using PException::PException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
	public:
		HrException(int _line, const char* _file, HRESULT _hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	//ΩÃ±€≈Ê
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		WindowClass(const WindowClass&) = delete;
		~WindowClass();
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "HyunJun Direct3D Engine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
	Window(int _width, int _heihgt, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& _title);
	static std::optional<int> ProcessMessage();
	Graphics& Gfx();

private:
	static LRESULT WINAPI HandleMsgSetup(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept;
	LRESULT HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept;

private:
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;

public:
	Keyboard kbd;
	Mouse mouse;
};

//ø°∑Ø µµøÚ ∏≈≈©∑Œ
#define WND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define HWND_NOFGX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )
