#include "../header/Mouse.h"

bool Mouse::IsEmpty() const
{
	return buffer.empty();
}

void Mouse::Flush()
{
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int _x, int _y)
{
	x = _x;
	y = _y;
	buffer.push(Mouse::Event(Mouse::Event::MouseType::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::Leave, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int _x, int _y)
{
	leftIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int _x, int _y)
{
	leftIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int _x, int _y)
{
	rightIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int _x, int _y)
{
	rightIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::MouseType::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int _x, int _y)
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int _x, int _y)
{
	buffer.push(Mouse::Event(Mouse::Event::MouseType::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::OnWheelDelta(int _x, int _y, int _delta)
{
	wheelDeltaCarry += _delta;

	//120마다 이벤트 발생
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(_x, _y);
	}

	while (wheelDeltaCarry <= WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(_x, _y);
	}

}

std::pair<int, int> Mouse::GetPos() const
{
	return {x, y};
}

int Mouse::GetPosX() const
{
	return x;
}

int Mouse::GetPosY() const
{
	return y;
}

bool Mouse::IsInWindow() const
{
	return isInWindow;
}

bool Mouse::LeftIsPressed() const
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const
{
	return rightIsPressed;
}

Mouse::Event Mouse::Read()
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	return Mouse::Event();
}
