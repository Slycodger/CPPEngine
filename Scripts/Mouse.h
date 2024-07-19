#pragma once

#include "Vector.h"

struct MouseView
{
	enum Enumeration { Shown, Hidden };
};

struct Mouse
{
	Vector2 Pos, Movement;
	Vector2 Scrolling;
	MouseView::Enumeration Visibility = MouseView::Shown;
};

extern Mouse ActiveMouse;