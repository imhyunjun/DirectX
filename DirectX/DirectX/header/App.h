#pragma once
#include "Window.h"
#include "PTimer.h"

class App
{
public:
	App();
	//������ ������, �޽��� ����
	int Go();
	~App();
	void DoFrame();

private:
	Window wnd;
	PTimer timer;
	std::vector<std::unique_ptr<class Box>> boxes;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	static constexpr size_t nDrawables = 180;
};