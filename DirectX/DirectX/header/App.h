#pragma once
#include "Window.h"
#include "PTimer.h"

class App
{
public:
	App();
	//������ ������, �޽��� ����
	int Go();
	void DoFrame();

private:
	Window wnd;
	PTimer timer;
};