/*
* Script
*/
#define DRAWINGPADMAIN
#include "DrawingPad.h"
void CreateSphere(int Define, float Scale, unsigned int& SVBO, unsigned int& SEBO);

static Model CubeModel;

namespace DrawingPad
{
	float Speed = 10;
	float RotSpeed = 20;
	List<Object*> Objects = 1000;
	Object* Floor = nullptr;
	Object* Cube = nullptr;
	Ray ray = Ray(Vector3(0,0,-1));
	
	void Start()
	{
		LoadModel(CubeModel, "Cube", "MYOBJ");
		Cube = new Object(CubeModel);
		Cube->Transform.Scale(0.02);
		for(int i = 0; i < 1000; i++)
		{
			Objects.AddToList(new Object(CubeModel));
			(*Objects[i])->Transform.T.Position = Vector3(((i % 20) * 2) -9, -10, (int)((i / 10) * 2) - 9.5);
		}
		Floor = new Object(CubeModel);
		Floor->Transform.T.Position = Vector3(0, -20, 0);
		Floor->Transform.Scale(50, 1, 50);
	}
	void Update(GLFWwindow* window)
	{
		ray.RayDirection = MainCamera.DirectionFacing();
		Cube->Transform.T.Position = MainCamera.Position + ray.RayDirection;

		if (Keys::KeyHeld(GLFW_KEY_O))
			Cube->Transform.Rotate(Vector3(45 * Time::deltaTime, 0, 0));
		if (Keys::KeyHeld(GLFW_KEY_P))
			Cube->Transform.Rotate(Vector3(-45 * Time::deltaTime, 0, 0));


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
		if(Keys::KeyPressed(GLFW_KEY_HOME))
		{
			MainCamera.Position = Vector3(0, 0, 0);
			MainCamera.SetRotation(Vector3(-90, 0, 0));
		}


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
		Objects.Delete();
		CubeModel.DeleteModel();
	}
}