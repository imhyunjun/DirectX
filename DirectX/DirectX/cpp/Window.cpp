#include "../header/Window.h"
#include <sstream>
#include <Windows.h>
#include "../resource.h"

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
	wc.hIcon = LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1));
	//wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCEA(IDB_PNG1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	//wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCEA(IDB_PNG1), IMAGE_ICON, 16, 16, 0));
	wc.hIconSm = LoadIconA(hInst, MAKEINTRESOURCE(IDI_ICON1));
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
Window::Window(int width, int height, const char* name) : width(width), height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	if(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw WND_LAST_EXCEPT();
	}

	// create window & get hWnd
	hWnd = CreateWindowExA(0,
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	if (hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT(hr);
	}

	// show window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& _title)
{
	if (SetWindowText(hWnd, _title.c_str()) == 0)
	{
		throw WND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessage()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
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
	case WM_KILLFOCUS:	// 윈도우가 키 소유권을 잃었을때
		kbd.ClearState();
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		//lParam의 첫번째 비트가 1이면 true(lParam이 1이면 눌린상태, 0이면 키 뗀 상태)
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		kbd.OnChar(static_cast<char>(wParam));
		break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		//클라이언트 안
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);

			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		//클라이언트 바깥
		else
		{
			//버튼이 눌려져 있으면
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}

		
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEHWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);

		/*if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			mouse.OnWheelUp(pt.x, pt.y);
		}
		else
		{
			mouse.OnWheelDown(pt.x, pt.y);
		}*/
		break;
	}
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

Window::Exception::Exception(int _line, const char* _file, HRESULT _hr) noexcept
{
	PException(_line, _file);
	hr = _hr;
}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << "Error Code : " << GetErrorCode() << std::endl
		<< "Description : " << GetErrorString() << std:: endl << GetOriginString() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "Winodw Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT _hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, _hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified Error Code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);

	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
