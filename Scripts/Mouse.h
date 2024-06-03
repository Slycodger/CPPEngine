#pragma once
#ifndef MOUSE
#define MOUSE
#include "Vector.h"

struct MouseView
{
	enum Enumeration { Shown, Hidden };
};

struct Mouse
{
	Vector2 Pos, Movement;
	MouseView::Enumeration Visibility = MouseView::Shown;
};

extern Mouse ActiveMouse;

#endif