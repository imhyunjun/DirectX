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
			//optional �� ���� ������ quit�̶� ��, ������ false
			return *ecode;
		}
		DoFrame();
	}

#pragma region OldThing
	////�޼��� ����
	//MSG msg;
	//BOOL gResult;
	//getmessage -> �޼����� �������� ����, WM_QUIT�� �߻��� ��쿡�� FALSE�� ����
	//getmessage ��ǲ�� ������ �۵� ���� -> Peekmessage ���
	//while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0)
	//{
	//	//���� Ű �Ǵ� �Է¿� ���� �޼��� ��ȯ
	//	//WM_KEYDOWN -> WM_CHAR
	//	TranslateMessage(&msg);

	//	//WndProc �Լ� ȣ��
	//	//WndProc�� ����ɶ����� ���
	//	//WndProc�� msg ����
	//	DispatchMessageW(&msg);

	//	/*if (wnd.kbd.KeyIsPressed(VK_SPACE))
	//	{
	//		MessageBox(nullptr, "Something Happen", "Space Key is Pressed", MB_OK | MB_ICONEXCLAMATION);
	//	}*/

	//	//alt Ű wm_keydown���� ó�� x
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
	oss << "�ð� ����� " << std::setprecision(1) << std::fixed << t << "s";
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
