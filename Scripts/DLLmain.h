#pragma once

#ifdef MySpecialEngine_EXPORTS
#define api __declspec(dllexport)
#else
#define api __declspec(dllimport)
#endif

#include "pch.h"

extern "C" api int start();
extern "C" api int update();
extern "C" api void end();
extern "C" api HWND window();