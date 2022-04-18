#include "../header/App.h"
#include "../header/WindowMessageMap.h"

/// <param name="hinstance"></param> 인스턴스 : 아이디
/// <param name="hPrevInstance"></param> 항상 0
/// <param name="lpCmdLine"></param> 프로그램 외부에서 내부러 값 전달
/// <param name="nCmdShow"></param> 윈도우 출력 형태에 관한 값
/// <returns></returns>
int CALLBACK WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try {
		return App{}.Go();
	}
	catch (const PException& _e)
	{
		MessageBoxA(nullptr, _e.what(), _e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception _e)
	{
		MessageBoxA(nullptr, _e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "세부정보 없음", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}



