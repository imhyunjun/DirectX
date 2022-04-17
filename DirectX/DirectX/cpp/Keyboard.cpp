#include "../header/Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char _keyCode) const noexcept
{
	return keyStates[_keyCode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keyBuffer.size() > 0u)
	{
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else
	{
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	keyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (charBuffer.size() > 0u)
	{
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
	return 0;
}

bool Keyboard::CharIsEmpty() noexcept
{
	return charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() noexcept
{
	return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char _keyCode) noexcept
{
	keyStates[_keyCode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::KeyType::Press, _keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char _keyCode) noexcept
{
	keyStates[_keyCode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::KeyType::Release, _keyCode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char _character) noexcept
{
	charBuffer.push(_character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept
{
	keyStates.reset();
}
