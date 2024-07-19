#pragma once

#ifdef TestingGround_MAIN

#include "IntializeShaders.h"
#include "ObjectClasses.h"
#include "KeyPresses.h"
#include "MathFunctions.h"
#include "Camera.h"
#include "ScriptBase.h"
#include "TextCreation.h"
#include "Mouse.h"
#include "Button.h"

#endif

namespace TestingGround
{
	void start();
	void update(GLFWwindow* window);
	void end();
}