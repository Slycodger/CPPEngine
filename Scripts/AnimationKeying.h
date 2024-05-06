#pragma once
#ifndef ANIMATIONKEYING
#define ANIMATIONKEYING

#ifdef ANIMATIONKEYINGMAIN

#include "GlStuff.h"
#include "List.h"

struct AnimationKey
{
	List<void(*)(int)> Actions = 0;
};

struct Animation
{
	AnimationKey* Frames = nullptr;
	unsigned int FrameCount = 0;
	unsigned int CurrentFrame = 0;
	float FrameRate = 24;
	double TimeAtLastFrame = 0;
	double TimeBetweenFrames = 0;
};

#endif

void AdvanceAnimations();

#endif