#pragma once
#include "Window.h"
#include "PTimer.h"

class App
{
public:
	App();
	//마스터 프레임, 메시지 루프
	int Go();
	void DoFrame();

private:
	Window wnd;
	PTimer timer;
};