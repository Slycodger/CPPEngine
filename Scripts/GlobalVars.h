/*
* Global variables meant to be externed
*/

#pragma once
#ifndef GLOBALVARS
#define GLOBALVARS
#include <map>
#include "IntializeShaders.h"
namespace ScreenSpace
{
	extern float Width;
	extern float Height;
	extern int WindowXPos;
	extern int WindowYPos;
	extern float WidthToHeight;
}
namespace App
{
	extern bool Stop;
}
namespace Time
{
	extern double deltaTime;
}
namespace Shading
{
	extern std::map <std::string, unsigned int> ShaderPrograms;
}
extern Shader shader;
#endif