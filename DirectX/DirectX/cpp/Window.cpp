#include "../header/Window.h"
#include "sstream"
#include <Windows.h>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassExA(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClassA(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


// Window Stuff
Window::Window(int width, int height, const char* name) noexcept
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// create window & get hWnd
	hWnd = CreateWindowExA(0,
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// show window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//LRESULT WINAPI Window::handleMsg(HWND _hWnd, UINT _msg, WPARAM _wParam, LPARAM _lParam) noexcept
//{
//	//static WindowMessageMap mm;
//	//OutputDebugStringA(mm(message, lParam, wParam).c_str());
//
//	PAINTSTRUCT ps;
//	HDC hdc;
//
//	switch (_msg)
//	{
//	case WM_PAINT:
//		hdc = BeginPaint(_hWnd, &ps);	//beginpaint(화면 그리기 준비), endpaint(화면 그리기 종료)는 오직 여기에서만 사용 가능
//		//여기다 그리기
//		EndPaint(_hWnd, &ps);
//		break;
//	
//	case WM_KEYDOWN:
//		if (_wParam == 'F')
//		{
//			SetWindowTextA(_hWnd, "Lim");
//		}
//		break;
//	case WM_KEYUP:
//		if (_wParam == 'F')
//		{
//			SetWindowTextA(_hWnd, "HyunJun");
//		}
//		break;
//	case WM_CHAR:		//대소문자 구분 테스트에 대한 입력
//		{
//			static std::string title;
//			title.push_back((char)_wParam);
//			SetWindowTextA(_hWnd, title.c_str());
//		}
//		break;
//	case WM_LBUTTONDOWN:
//		{
//			const POINTS pt = MAKEPOINTS(_lParam);
//			std::ostringstream oss;
//			oss << "(" << pt.x << "," << pt.y << ")";
//			SetWindowTextA(_hWnd, oss.str().c_str());
//		}
//		break;
//	case WM_DESTROY:
//		//메모리 해제
//
//		PostQuitMessage(0);		//WM_QUIT 발생
//		return 0;
//		break;
//	default:
//		return 0;
//	}
//
//	return DefWindowProc(_hWnd, _msg, _wParam, _lParam); //WndProc가 처리 하지 않은 메시지 처리
//}
