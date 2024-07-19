#pragma once

namespace Keys
{
	bool keyHeld(int Key);
	bool keyPressed(int Key);
	bool keyReleased(int Key);
}
struct Key
{
	bool Pressed = false;
	bool Released = false;
	bool Held = false;
};
namespace KeyPresses
{
	extern Key AllKeys[349];
	void update();
	void pressedKey(int Key);
	void releasedKey(int Key);
}