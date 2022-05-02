#pragma once

//에러 도움 매크로
#define WND_EXCEPT(hr) Window::HrException(__LINE__, __FILE__, hr)
#define WND_LAST_EXCEPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define HWND_NOFGX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )
