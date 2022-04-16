#pragma once
#include "Win.h"

class Window
{
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
	Window(int _width, int _heihgt, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

private:
	static LRESULT WINAPI HandleMsgSetup(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND _hwnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept;
	LRESULT HandleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept;

	HWND hWnd;

};
