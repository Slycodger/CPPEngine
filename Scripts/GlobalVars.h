/*
* Global variables meant to be externed
*/

#pragma once
#ifndef GLOBALVARS
#define GLOBALVARS
#include <map>
namespace ScreenSpace
{
	extern float yGridScale;
	extern float xGridScale;
	extern float Width;
	extern float Height;
	extern float gridHeight;
	extern int WindowXPos;
	extern int WindowYPos;
}
namespace App
{
	extern bool Stop;
}
namespace Time
{
	extern double deltaTime;
}
namespace Fails
{
	extern bool Break;
	extern int BreakCode;
}
namespace Shading
{
	extern std::map <std::string, unsigned int> ShaderPrograms;
}
#endif