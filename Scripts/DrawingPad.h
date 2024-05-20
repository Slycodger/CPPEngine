/*
* The DrawingPad header file
*/

#pragma once
#ifndef DRAWINGPAD
#define DRAWINGPAD

#ifdef DRAWINGPADMAIN

#include "GlStuff.h"
#include <iostream>
#include "IntializeShaders.h"
#include "ObjectClasses.h"
#include "KeyPresses.h"
#include "MathFunctions.h"
#include "AObjectDraw.h"
#include "Camera.h"
#include "GlobalVars.h"
#include "EngineBase.h"
#include "GUIText.h"
#include "Mouse.h"

#endif

namespace DrawingPad
{
	void Start();
	void Update(GLFWwindow* window);
	void End();
}

#endif