#pragma once
enum Key {

};
class KeyEvent
{
	int key_code;
	bool pressed;
public:
	KeyEvent() = default;
	int Key()const { return 0; }// key code is one of "Key"
	bool is_pressed() { return pressed; }
	bool is_released() { return !pressed; }
};
class MouseEvent
{
};

