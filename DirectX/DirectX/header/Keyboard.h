#pragma once
#include <queue>
#include <bitset>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class KeyType
		{
			Press, Release, Invalid
		};
	private:
		KeyType keyType;
		unsigned char code;
	public:
		Event() : keyType(KeyType::Invalid), code(0u) {}
		Event(KeyType _type, unsigned char _code) noexcept : keyType(_type), code(_code) {}
		bool IsPress() const noexcept { return keyType == KeyType::Press; }
		bool IsRealease() const noexcept { return keyType == KeyType::Release; }
		bool IsValid() const noexcept { return keyType != KeyType::Invalid; }
		unsigned char GetCode() const noexcept { return code; }
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//키 이벤트
	bool KeyIsPressed(unsigned char _keyCode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;

	//캐릭터(char0 이벤트
	char ReadChar() noexcept;
	bool CharIsEmpty() noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	//자동반복
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() noexcept;

private:
	void OnKeyPressed(unsigned char _keyCode) noexcept;
	void OnKeyReleased(unsigned char _keyCode) noexcept;
	void OnChar(char _character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& _buffer) noexcept;

private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;	//윈도우 키코드가 1바이트를 넘지 않음
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& _buffer) noexcept
{
	while (_buffer.size() > bufferSize)
	{
		_buffer.pop();
	}
}
