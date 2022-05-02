#include "../header/App.h"
#include <sstream>
#include <iomanip>
#include "../header/Drawable/Box.h"

App::App() : wnd(800, 600, "HyunJun First Window")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Go()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessage())
		{
			//optional 이 값을 가지면 quit이란 뜻, 없으면 false
			return *ecode;
		}
		DoFrame();
	}

#pragma region OldThing
	////메세지 루프
	//MSG msg;
	//BOOL gResult;
	//getmessage -> 메세지를 가져오는 역할, WM_QUIT가 발생할 경우에만 FALSE를 리턴
	//getmessage 인풋이 없으면 작동 안함 -> Peekmessage 사용
	//while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0)
	//{
	//	//문자 키 또는 입력에 대해 메세지 반환
	//	//WM_KEYDOWN -> WM_CHAR
	//	TranslateMessage(&msg);

	//	//WndProc 함수 호출
	//	//WndProc가 종료될때까지 대기
	//	//WndProc에 msg 전달
	//	DispatchMessageW(&msg);

	//	/*if (wnd.kbd.KeyIsPressed(VK_SPACE))
	//	{
	//		MessageBox(nullptr, "Something Happen", "Space Key is Pressed", MB_OK | MB_ICONEXCLAMATION);
	//	}*/

	//	//alt 키 wm_keydown에서 처리 x
	//	/*if (wnd.kbd.KeyIsPressed(VK_MENU))
	//	{
	//		MessageBox(nullptr, "Something Happen", "Space Key is Pressed", MB_OK | MB_ICONEXCLAMATION);
	//	}*/

	//	/*while (!wnd.mouse.IsEmpty())
	//	{
	//		const auto e = wnd.mouse.Read();
	//		if (e.GetType() == Mouse::Event::MouseType::Move)
	//		{
	//			std::ostringstream oss;
	//			oss << "MousePostition : (" << e.GetPosX() << " , " << e.GetPosY() << ")";
	//			wnd.SetTitle(oss.str());
	//		}
	//	}*/
	//	DoFrame();
	//}
	/*if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;*/
#pragma endregion

}

App::~App()
{
}

void App::DoFrame()
{
	/*const float t = timer.Peek();
	std::ostringstream oss;
	oss << "시간 경과ㅣ " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());*/

	//const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	auto dt = timer.Mark();
	//wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().ClearBuffer(0.07f, 0.0f, 0.12f);
	//wnd.Gfx().DrawTestTriangle(-timer.Peek(), 0.0f, 0.0f);
	//wnd.Gfx().DrawTestTriangle(timer.Peek(), (wnd.mouse.GetPosX() - 400.f) / 800.f, -(wnd.mouse.GetPosY() - 300.f) / 600.f);
	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}
