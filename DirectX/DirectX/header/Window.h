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
	public:
		Exception(int _line, const char* _file, HRESULT _hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT _hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
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
#define WND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, GetLastError())
