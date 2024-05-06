#pragma once
#ifndef LIGHTINGOBJECTS
#define LIGHTINGOBJECTS

#include "Vector.h"

struct LightObj
{
	Vector3 Position = 0;
	Vector4 Color = Vector4(1);
	unsigned int Texture = 0;

	LightObj() : Position(0), Color(1), Texture(0) {}
	void GenTexture()
	{
		glGenTextures(1, &Texture);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, ScreenSpace::Width, ScreenSpace::Height, 0, GL_RGBA, GL_UNSIGNED_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
};

#endif