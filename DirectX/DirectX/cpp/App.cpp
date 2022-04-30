#include "../header/App.h"
#include <sstream>
#include <iomanip>

App::App() : wnd(800, 600, "HyunJun First Window")
{

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

void App::DoFrame()
{
	/*const float t = timer.Peek();
	std::ostringstream oss;
	oss << "�ð� ����� " << std::setprecision(1) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());*/

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle(timer.Peek());



	wnd.Gfx().EndFrame();
}
