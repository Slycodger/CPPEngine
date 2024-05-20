/*
* Script
*/
#define DRAWINGPADMAIN
#include "DrawingPad.h"

Model CubeModel;

namespace DrawingPad
{
	float Speed = 10;
	float RotSpeed = 60;
	List<Object*> Objects = 100;
	Object* Floor = nullptr;
	Object* Cube = nullptr;
	

	void Start()
	{
		LoadModel(CubeModel, "Cube", "MYOBJ");
		Cube = new Object(CubeModel);
		Cube->Transform.Scale(0.01);
		for(int i = 0; i < 100; i++)
		{
			Objects.AddToList(new Object*(new Object(CubeModel)));
			(*Objects[i])->Transform.T.Position = Vector3(((i % 10) * 5) -9, -10, (int)((i / 10) * 5) - 9.5);
		}
		Floor = new Object(CubeModel);
		Floor->Transform.T.Position = Vector3(0, -20, 0);
		Floor->Transform.Scale(50, 1, 50);
	}
	void Update(GLFWwindow* window)
	{
		Cube->Transform.T.Position = MainCamera.Position + MainCamera.DirectionFacing();

		if(Keys::KeyHeld(GLFW_KEY_W))
		{
			MainCamera.Position += MainCamera.DirectionFacing() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_A))
		{
			MainCamera.Position -= MainCamera.DirectionRight() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_S))
		{
			MainCamera.Position -= MainCamera.DirectionFacing() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_D))
		{
			MainCamera.Position += MainCamera.DirectionRight() * Speed * Time::deltaTime;
		}
		if (Keys::KeyHeld(GLFW_KEY_SPACE))
		{
			MainCamera.Translate(0, Speed * Time::deltaTime, 0);
		}
		if (Keys::KeyHeld(GLFW_KEY_LEFT_SHIFT))
		{
			MainCamera.Translate(0, -Speed * Time::deltaTime, 0);
		}
		if(Keys::KeyHeld(GLFW_KEY_PAGE_UP))
			MainCamera.ChangePerpMat(0, 15 * Time::deltaTime, 0, 0);
		if (Keys::KeyHeld(GLFW_KEY_PAGE_DOWN))
			MainCamera.ChangePerpMat(0, -15 * Time::deltaTime, 0, 0);

		if(Keys::KeyPressed(GLFW_KEY_RIGHT_SHIFT))
		{
			ActiveMouse.Visibility = MouseView::Shown;
		}
		if (Keys::KeyPressed(GLFW_KEY_RIGHT_CONTROL))
		{
			ActiveMouse.Visibility = MouseView::Hidden;
		}
		if(Keys::KeyPressed(GLFW_KEY_ESCAPE))
		{
			App::Stop = true;
		}
		if(ActiveMouse.Visibility == MouseView::Hidden)
			MainCamera.Rotate(-ActiveMouse.Movement.y * RotSpeed, -ActiveMouse.Movement.x * RotSpeed, 0);
		//std::cout << MainCamera.RotToVec() << "\n";
		if (MainCamera.Rotation().x > 90 && MainCamera.Rotation().x < 180)
			MainCamera.SetRotation(90, MainCamera.Rotation().y, MainCamera.Rotation().z);
		if (MainCamera.Rotation().x < 270 && MainCamera.Rotation().x > 180)
			MainCamera.SetRotation(270, MainCamera.Rotation().y, MainCamera.Rotation().z);
	}
	void End()
	{
		//Objects.Delete();
	}
}