#pragma once
#include <queue>
#include <Windows.h>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class MouseType {LPress, LRelease, RPress, RRelease, WheelUp, WheelDown, Move, Enter, Leave, InValid};
	private:
		MouseType mouseType;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event() : mouseType(MouseType::InValid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) {}
		Event(MouseType _type, const Mouse& _parent)
		{
			mouseType = _type;
			leftIsPressed = _parent.leftIsPressed;
			rightIsPressed = _parent.rightIsPressed;
			x = _parent.x;
			y = _parent.y;
		}
		bool IsValid() const{ return mouseType != MouseType::InValid; }
		MouseType GetType() const { return mouseType; }
		std::pair<int, int> GetPos() const { return { x, y }; }
		int GetPosX() const { return x; }
		int GetPosY() const { return y; }
		bool LeftIsPressed() const { return leftIsPressed; }
		bool RightIsPressed() const { return rightIsPressed; }
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool IsInWindow() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	Mouse::Event Read();
	bool IsEmpty() const;
	void Flush();

private:
	void OnMouseMove(int _x, int _y);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLeftPressed(int _x, int _y);
	void OnLeftReleased(int _x, int _y);
	void OnRightPressed(int _x, int _y);
	void OnRightReleased(int _x, int _y);
	void OnWheelUp(int _x, int _y);
	void OnWheelDown(int _x, int _y);
	void TrimBuffer();
	void OnWheelDelta(int _x, int _y, int _delta);

private:
	static constexpr unsigned int bufferSize = 16u;
	bool leftIsPressed;
	bool rightIsPressed;
	int x;
	int y;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};