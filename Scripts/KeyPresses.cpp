/*
* Controls all key press stuff
*/

#include "KeyPresses.h"
using namespace KeyPresses;
namespace KeyPresses
{
	Key AllKeys[349];

	void releaseKeys()
	{
		for (int i = 0; i < 349; i++)
		{
			AllKeys[i].Pressed = false;
			AllKeys[i].Released = false;
		}
	}

	void update()
	{
		releaseKeys();
	}

	void pressedKey(int Key)
	{
		AllKeys[Key].Pressed = true;
		AllKeys[Key].Held = true;
	}
	void releasedKey(int Key)
	{
		AllKeys[Key].Released = true;
		AllKeys[Key].Held = false;
	}
}

namespace Keys
{
	bool keyHeld(int Key)
	{
		if (AllKeys[Key].Held == true)
			return true;
		return false;
	}
	bool keyPressed(int Key)
	{
		if (AllKeys[Key].Pressed == true)
			return true;
		return false;
	}
	bool keyReleased(int Key)
	{
		if (AllKeys[Key].Released == true)
			return true;
		return false;
	}
}