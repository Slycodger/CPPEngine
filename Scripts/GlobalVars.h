#pragma once

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
	extern bool Pause;
}
namespace Shading
{
	extern std::map <std::string, unsigned int> ShaderPrograms;
}
namespace Flags
{
	extern bool MouseMoveFlag;
}
extern Shader shader;