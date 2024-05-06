/*
* Keylogic such as Held, Released, or Pressed
*/

#pragma once
#ifndef KEYPRESSES
#define KEYPRESSES

namespace Keys
{
	bool KeyHeld(int Key);
	bool KeyPressed(int Key);
	bool KeyReleased(int Key);
}

namespace KeyPresses
{
	void Start();
	void Update();
	void PressedKey(int Key);
	void ReleasedKey(int Key);
}

#endif