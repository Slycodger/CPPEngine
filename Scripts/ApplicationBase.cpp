/*
* The application window and function control
*/
#define ApplicationBase_MAIN
#include "ApplicationBase.h"
#ifdef _WINDLL
#include "DLLmain.h"
#endif

using namespace ScreenSpace;
using namespace Time;
using namespace App;
using namespace Shading;
using namespace Flags;

namespace ScreenSpace
{
	float Width = 1280;
	float Height = 720;
	int WindowXPos;
	int WindowYPos;
	float WidthToHeight = Width / Height;
}
namespace App
{
	bool Stop = false;
}
namespace Time
{
	double deltaTime = 0;
	bool Pause = false;
}
namespace Shading
{
	std::map<std::string, unsigned int> ShaderPrograms;
}

Shader shader;
Mouse ActiveMouse;


//Main drawing function
GLFWwindow* Window;
bool Started = false;

char CharExePath[MAX_PATH];
std::string ExePath;
std::string ExeName = "MySpecialEngine.exe";

int start()
{
	if (startOpenGL() < 0)
		return 0;

	int length = 0;
	if (length = GetModuleFileNameA(nullptr, CharExePath, MAX_PATH); length != MAX_PATH)
		for (size_t i = 0; i < length - ExeName.length(); i++)
			ExePath += CharExePath[i];

	std::map<std::string, std::string> NewShaders;

	{
		NewShaders.insert({ VERTFILE, "FractalVert" });
		NewShaders.insert({ FRAGFILE, "FractalFrag" });
		unsigned int TempShader;
		shader.createShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "FractalShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "BasicVert" });
		NewShaders.insert({ FRAGFILE, "BasicFrag" });
		unsigned int TempShader;
		shader.createShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "BasicTextureShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "WhiteSpaceVert" });
		NewShaders.insert({ FRAGFILE, "WhiteSpaceFrag" });
		unsigned int TempShader;
		shader.createShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "WhiteSpaceShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "BasicNoTextureVert" });
		NewShaders.insert({ FRAGFILE, "BasicNoTextureFrag" });
		unsigned int TempShader;
		shader.createShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "BasicNoTextureShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "TextSetupVert" });
		NewShaders.insert({ FRAGFILE, "TextSetupFrag" });
		unsigned int TempShader;
		shader.createShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "TextSetupShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "TextVert" });
		NewShaders.insert({ FRAGFILE, "TextFrag" });
		unsigned int TempShader;
		shader.createShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "TextShader", TempShader });
		NewShaders.clear();
	}


	AppObjHandler::start();
	TextCreation::start();
	TestingGround::start();
	ListFolder::start();

	glfwSwapBuffers(Window);
	glfwSetWindowPos(Window, 25, 25);
	glfwGetWindowPos(Window, &WindowXPos, &WindowYPos);
	return 1;
}

float CurrentFrame = 0;
float LastFrame = 0;
std::stringstream FPS;
int FramesPassed = 0;

int update()
{
	if (glfwWindowShouldClose(Window))
	{
		return 0;
	}
	CurrentFrame = glfwGetTime();
	deltaTime = CurrentFrame - LastFrame;
	LastFrame = CurrentFrame;
	if (FramesPassed > 75)
	{
		FPS << "OpenGL Window | FPS : " << (int)(1 / deltaTime);
		glfwSetWindowTitle(Window, FPS.str().c_str());
		FPS.str("");
		FramesPassed = 0;
	}

	if (!Pause)
		switch (ActiveMouse.Visibility)
		{
		case MouseView::Shown:
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		case MouseView::Hidden:
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			glfwSetCursorPos(Window, 0, 0);
			break;
		default:
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			break;
		}
	else
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	KeyPresses::update();

	//Keycallback
	glfwPollEvents();

	if (Stop)
		glfwSetWindowShouldClose(Window, 1);

	if (!Pause)
	{
		TestingGround::update(Window);
	}

	AppObjHandler::update();
	TextCreation::update();
	glFinish();
	glfwSwapBuffers(Window);

	ActiveMouse.Movement.x = 0;
	ActiveMouse.Movement.y = 0;
	ActiveMouse.Scrolling = { 0 };

	FramesPassed++;
	return 1;
}

void end()
{
	AppObjHandler::end();
	TestingGround::end();
	TextCreation::end();
	ModelLoading::deleteHeap();
	glfwSetWindowShouldClose(Window, 1);
	glfwTerminate();
}

int main()
{
	if (int i = start(); i != 1)
	{
		return i;
	}
	int Ret = 0;
	do
	{
		Ret = update();
	} while (Ret == 1);
	end();
	return Ret;
}

static int CALLBACK WinMain(_In_  HINSTANCE hInst, _In_opt_ HINSTANCE hInstPrev, _In_ PSTR cmdline, _In_ int cmdshow)
{
	main();
}

HWND window()
{
	return glfwGetWin32Window(Window);
}

//Gets called when a key is pressed
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if(action == GLFW_PRESS)
			KeyPresses::AllKeys[key].Pressed = true;
		KeyPresses::AllKeys[key].Held = true;
	}
	else if (action == GLFW_RELEASE)
	{
		KeyPresses::AllKeys[key].Released = true;
		KeyPresses::AllKeys[key].Held = false;
	}
}

//Mouse clicking callback
void mouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
		KeyPresses::pressedKey(button);
	 else if(action == GLFW_RELEASE)
		KeyPresses::releasedKey(button);
}

//Mouse moving callback
void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	Vector2 Move = Vector2(2 * xpos / Height, 2 * ypos / Height);
	Vector2 Pos = Vector2(2 * xpos / Width - 1, 1 - 2 * ypos / Height);
	ActiveMouse.Movement.x = Move.x;
	if (ActiveMouse.Visibility == MouseView::Hidden)
		ActiveMouse.Movement.y = -Move.y;
	else
		ActiveMouse.Movement.y = Move.y;
	ActiveMouse.Pos.x = Pos.x;
	ActiveMouse.Pos.y = Pos.y;
}

//Scrolling callback
void scrollCallback(GLFWwindow* window, double x, double y)
{
	ActiveMouse.Scrolling = {x, y};
}

//Window moving callback
void windowMoveCallBack(GLFWwindow *window, int x, int y)
{
	WindowXPos = x;
	WindowYPos = y;
}

//Starts up the openGL things
int startOpenGL()
{
	if (Started)
		return 0;
	Started = true;
	if (!glfwInit())
	{
		std::cout << "Failed to start GLFW\n";
		glfwTerminate();
		return -1;
	}
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	Window = glfwCreateWindow(Width, Height, "OpenGL Window", NULL, NULL);

	if (!Window)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -2;
	}
	glfwMakeContextCurrent(Window);

	if (!gladLoadGL())
	{
		std::cout << "Failed to start GLAD\n";
		glfwTerminate();
		return -3;
	}

	glfwSetKeyCallback(Window, keyPressCallback);
	glfwSetMouseButtonCallback(Window, mouseClickCallback);
	glfwSetCursorPosCallback(Window, mouseMoveCallback);
	glfwSetScrollCallback(Window, scrollCallback);
	glfwSetWindowPosCallback(Window, windowMoveCallBack);


	glEnable(GL_DEPTH_CLAMP);
	glEnable(GL_CULL_FACE);
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}


/*
* NOTE : 7/16/2024
* Figure out why these are here, there should be a better spot to put them
*/
Vector3 operator -(Vector3 const& obj)
{
	Vector3 ret;
	ret.x = -obj.x;
	ret.y = -obj.y;
	ret.z = -obj.z;
	return ret;
}
std::ostream& operator << (std::ostream& os, Vector2 const& obj)
{
	os << obj.x << ", " << obj.y;
	return os;
}
std::ostream& operator << (std::ostream &os, Vector3 const& obj)
{
	os << obj.x << ", " << obj.y << ", " << obj.z;
	return os;
}
std::ostream& operator << (std::ostream& os, Vector4 const& obj)
{
	os << obj.x << ", " << obj.y << ", " << obj.z << ", " << obj.w;
	return os;
}
bool operator == (Vector3 const& obj1, Vector3 const obj2)
{
	if (obj1.x != obj2.x)
		return false;
	if (obj1.y != obj2.y)
		return false;
	if (obj1.z != obj2.z)
		return false;
	return true;
}