/*
* Controls all key press stuff
*/

#define KEYPRESSESMAIN

#include "KeyPresses.h"
struct Key
{
	bool Pressed = false;
	bool Released = false;
	bool Held = false;
};
Key AllKeys[349] = { 0 };
void ReleaseKeys()
{
	for (int i = 0; i < 349; i++)
	{
		AllKeys[i].Pressed = false;
		AllKeys[i].Released = false;
	}
}

namespace KeyPresses
{
	void Start()
	{

	}

	void Update()
	{
		ReleaseKeys();
	}

	void PressedKey(int Key)
	{
		AllKeys[Key].Pressed = true;
		AllKeys[Key].Held = true;
	}
	void ReleasedKey(int Key)
	{
		AllKeys[Key].Released = true;
		AllKeys[Key].Held = false;
	}
}

namespace Keys
{
	bool KeyHeld(int Key)
	{
		if (AllKeys[Key].Held == true)
			return true;
		return false;
	}
	bool KeyPressed(int Key)
	{
		if (AllKeys[Key].Pressed == true)
			return true;
		return false;
	}
	bool KeyReleased(int Key)
	{
		if (AllKeys[Key].Released == true)
			return true;
		return false;
	}
}