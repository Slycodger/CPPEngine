/*
* The application window and function control
*/
#define APPLICATIONMAINMAIN
#include "ApplicationMain.h"


using namespace ScreenSpace;
using namespace Time;
using namespace Fails;
using namespace App;
using namespace Shading;

//Sets up the functions
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
void WindowMoveCallBack(GLFWwindow *window, int x, int y);

namespace ScreenSpace
{
	float Width = 1280;
	float Height = 720;
	int WindowXPos;
	int WindowYPos;
}
namespace App
{
	bool Stop = false;
}
namespace Time
{
	double deltaTime = 0;
}
namespace Fails
{
	bool Break;
	int BreakCode;
}
namespace Shading
{
	std::map<std::string, unsigned int> ShaderPrograms;
}

Mouse ActiveMouse;


//Main drawing function
GLFWwindow* Window;
bool Started = false;
int main()
{
	if (StartOpenGL() < 0)
		return -1;
	
	glfwSetKeyCallback(Window, KeyCallback);
	glfwSetMouseButtonCallback(Window, MouseClickCallback);
	glfwSetCursorPosCallback(Window, MouseMoveCallback);
	glfwSetWindowPosCallback(Window, WindowMoveCallBack);

	Shader myShader;
	std::map<std::string, std::string> NewShaders;

	{
		NewShaders.insert({ VERTFILE, "FractalVert"});
		NewShaders.insert({ FRAGFILE, "FractalFrag" });
		unsigned int TempShader;
		myShader.CreateShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "FractalShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "BasicVert" });
		NewShaders.insert({ FRAGFILE, "BasicFrag" });
		unsigned int TempShader;
		myShader.CreateShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "BasicTextureShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "WhiteSpaceVert"});
		NewShaders.insert({ FRAGFILE, "WhiteSpaceFrag"});
		unsigned int TempShader;
		myShader.CreateShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "WhiteSpaceShader", TempShader });
		NewShaders.clear();
	}
	{
		NewShaders.insert({ VERTFILE, "BasicNoTextureVert" });
		NewShaders.insert({ FRAGFILE, "BasicNoTextureFrag" });
		unsigned int TempShader;
		myShader.CreateShader(NewShaders, TempShader);
		ShaderPrograms.insert({ "BasicNoTextureShader", TempShader });
		NewShaders.clear();
	}


	KeyPresses::Start();
	DrawingPad::Start();
	AObjectDraw::Start(myShader);
	GUIText::Start(myShader);

	glfwSetWindowPos(Window, 25, 25);
	glfwGetWindowPos(Window, &WindowXPos, &WindowYPos);

	float CurrentFrame = 0;
	float LastFrame = 0;
	std::stringstream FPS;
	int FramesPassed = 0;

	while(!glfwWindowShouldClose(Window))
	{
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

		KeyPresses::Update();

		//Keycallback
		glfwPollEvents();

		if(Break)
		{
			glfwTerminate();
			return BreakCode;
		}
		if(Stop)
		{
			glfwSetWindowShouldClose(Window, 1);
		}

		AObjectDraw::Update(myShader);
		GUIText::Update(myShader);
		DrawingPad::Update(Window);
		glFinish();

		glfwSwapBuffers(Window);
		ActiveMouse.Movement.x = 0;
		ActiveMouse.Movement.y = 0;

		FramesPassed++;
	}
	AObjectDraw::End();
	DrawingPad::End();
	ModelLoading::DeleteHeap();
	glfwSetWindowShouldClose(Window, 1);
	glfwTerminate();
	return 0;
}

//Gets called when a key is pressed
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		KeyPresses::PressedKey(key);
	}else if (action == GLFW_RELEASE)
	{
		KeyPresses::ReleasedKey(key);
	}
}

//Gets called when a mouse button is pressed
void MouseClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		KeyPresses::PressedKey(button);
	}else if(action == GLFW_RELEASE)
	{
		KeyPresses::ReleasedKey(button);
	}
}

//Gets called when the mouse is moved
void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	ActiveMouse.Movement.x = xpos / width;
	ActiveMouse.Movement.y = ypos / height;
	ActiveMouse.Pos.x = xpos;
	ActiveMouse.Pos.y = ypos;
}

void WindowMoveCallBack(GLFWwindow *window, int x, int y)
{
	WindowXPos = x ;
	WindowYPos = y;
}

int StartOpenGL()
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glEnable(GL_BLEND);
	return 0;
}


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