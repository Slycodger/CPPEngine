#pragma once

int startOpenGL();

#ifdef ApplicationBase_MAIN
#include "GlStuff.h"
#include "KeyPresses.h"
#include "TestingGround.h"
#include "AppObjHandler.h"
#include "TextCreation.h"
#include <Windows.h>
#include <chrono>
#include "Mouse.h"
#include "ListFolder.h"
#include "GlobalVars.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void windowMoveCallBack(GLFWwindow* window, int x, int y);
void scrollCallback(GLFWwindow* window, double x, double y);

#endif //ifdef ApplicationBase_MAIN